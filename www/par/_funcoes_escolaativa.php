<?php 

function recuperarMuncodPorInuid( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			muncod
		from 
			par.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function recuperarEstufPorInuid( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			estuf
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return $db->pegaUm( $sql );
}

function verificaSessaoEscolaAtiva()
{
	if ( !$_SESSION['par']['inuid'] )
	{
		header( "Location: ?modulo=inicio&acao=A" );
		exit();
	}
}

function pegarItridEscolaAtiva( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			itrid
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function montarRelacionamentoEscolasAtivasPorEstuf( $estuf, $buscaNome, $buscaCod ){
	
	global $db;
	$sql = "select 
				'<input type=\"checkbox\" onclick=\"window.opener.adiciona_item( '|| ent.entid ||', \''|| ent.entnome ||'\', this.checked )\" name=\"entid[]\" id=\"entid_'|| ent.entid ||'\" value=\"'|| ent.entid ||'\" />' as checkbox,
				ent.entcodent as codigo,
				ent.entnome as descricao
            from entidade.entidade ent
            	inner join entidade.endereco d on ent.entid = d.entid
            	INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
            	left join territorios.municipio m on m.muncod = d.muncod
            where ent.entescolanova = false
			and fe.funid = 3 
			and ent.tpcid = 1 
			and m.estuf = '$estuf'";

	if ($buscaNome)
    {
    	$sql .= "and entnome ilike '%'||removeacento('{$buscaNome}')||'%'";
    }
    
    if ($buscaCod) 
    {
    	$sql .= "and ent.entcodent = '".trim($buscaCod)."'";
    } 
	   
    $sql .= "group by ent.entid, ent.entcodent, ent.entnome, m.mundescricao
             order by m.mundescricao, ent.entnome";            

	$resultado = $db->carregar( $sql );
	return $resultado ? $resultado : array();
	
}

function montarRelacionamentoEscolasAtivasPorMuncod( $muncod, $buscaNome, $buscaCod ){
	global $db;
	$sql = "select 
				'<input type=\"checkbox\" onclick=\"window.opener.adiciona_item( '|| ent.entid ||', \''|| ent.entnome ||'\', this.checked )\" name=\"entid[]\" id=\"entid_'|| ent.entid ||'\" value=\"'|| ent.entid ||'\" />' as checkbox,
				ent.entcodent as codigo,
				ent.entnome as descricao
            from entidade.entidade ent
				left join entidade.entidadedetalhe entd on ent.entcodent = entd.entcodent
					and(
						entdreg_infantil_creche = '1' or
						entdreg_infantil_preescola = '1' or
						entdreg_fund_8_anos        = '1' or
						entdreg_fund_9_anos        = '1'
					)
				inner join entidade.endereco ende on ent.entid = ende.entid
            where ent.entescolanova = false
			and ende.muncod = '$muncod'
            and ent.tpcid = 3
			and ent.entstatus = 'A'";			
            
    if ($buscaNome)
    {
    	$sql .= "and entnome ilike '%'||removeacento('{$buscaNome}')||'%'";
    } 
    if ($buscaCod) 
    {
    	$sql .= "and ent.entcodent = '".trim($buscaCod)."'";
    }    
    $sql .= "order by ent.entnome";

	$resultado = $db->carregar( $sql );
	
	return $resultado ? $resultado : array();
	
}

function recuperarEscolasPorMuncod( $muncod ){
	global $db;
	
	$sql = "select ent.entid as codigo, ent.entnome as descricao, ent.entcodent as inep
            from entidade.entidade ent
				left join entidade.entidadedetalhe entd on ent.entcodent = entd.entcodent
					and(
						entdreg_infantil_creche = '1' or
						entdreg_infantil_preescola = '1' or
						entdreg_fund_8_anos        = '1' or
						entdreg_fund_9_anos        = '1'
					)
				inner join entidade.endereco ende on ent.entid = ende.entid
            where (ent.entescolanova = false or ent.entescolanova is null)
			and ende.muncod = '$muncod'
            and ent.tpcid = 3
			and ent.entstatus = 'A'
            order by ent.entnome";

	$resultado = $db->carregar( $sql );
	
	return $resultado ? $resultado : array();
}

// INICIO FUNÇÕES DO WORKFLOW

function criaDocumentoEscolaAtiva( $esaid ) {
	
	global $db;
	
	if(empty($esaid)) return false;
	
	$docid = pegaDocidEscolaAtiva( $esaid );
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_ESCOLA_ATIVA;
		
		$docdsc = "Cadastramento Escola Ativa";
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		if($esaid) {
			$sql = "UPDATE par.escolaativa SET 
					 docid = ".$docid." 
					WHERE
					 esaid = ".$esaid;

			$db->executar( $sql );		
			$db->commit();
			return $docid;
		}else{
			return false;
		}
	}
	else {
		return $docid;
	}
}

function pegaDocidEscolaAtiva( $esaid ) {
	
	global $db;
	
	$esaid = (integer) $esaid;	
	
	$sql = "SELECT
			 docid
			FROM
			 par.escolaativa
			WHERE
			 esaid  = " . $esaid;
	
	return (integer) $db->pegaUm( $sql );
}

function pegaEstadoAtualEscolaAtiva( $docid ) {
	
	global $db; 
	
	if($docid) {
		$docid = (integer) $docid;
		 
		$sql = "
			select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estado = $db->pegaUm( $sql );
		 
		return $estado;
	} else {
		return false;
	}
}

// Funções validação e condição Workflow
/*
function verificarPreenchimentoEscolaAtiva( $esaid = null ){
	global $db; 
	dbg('ok',1);
	if(!$_SESSION['par']['adpid'] || !$_SESSION['par']['prgid'] || !$_SESSION['par']['inuid']){
		echo '<script type="text/javascript"> 
	    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
	    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
	    	  </script>';
	    die;
	}
	
	$esaid = $db->pegaUm("select esaid from par.escolaativa where inuid = ".$_SESSION['par']['inuid']);

	$obEscolaAtiva = new EscolaAtiva( $esaid );
	$retorno = $obEscolaAtiva->verificarPreenchimento();
	
	dbg($retorno);
	
	if(!$retorno){
		return 'Favor inserir o Secretário, inserir as escolas e todos os seus quantitativos e informar o N.º de Professores.';
	}
	else{
		return true;
	}
}
*/
function verificarAnaliseEscolaAtiva( $esaid ){

	return true;
}

?>