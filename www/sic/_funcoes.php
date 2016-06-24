<?php 

// INICIO FUNÇÕES DO WORKFLOW

function criaDocumento( $slcid ) {
	
	global $db;
	
	if(empty($slcid)) return false;
	
	$docid = pegaDocid( $slcid );
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_SIC;
		
		$docdsc = "Cadastramento sistema de informação ao cidadão";
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		if($slcid) {
			$sql = "UPDATE sic.solicitacao SET 
					 docid = ".$docid." 
					WHERE
					 slcid = ".$slcid;

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

function pegaDocid( $slcid ) {
	
	global $db;
	
	$slcid = (integer) $slcid;	
	
	$sql = "SELECT
			 docid
			FROM
			 sic.solicitacao
			WHERE
			 slcid  = " . $slcid;
	
	return (integer) $db->pegaUm( $sql );
}

function pegaEstadoAtual( $docid ) {
	
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

// INICIO FUNÇÕES DE PERFIL

function checkPerfil( $pflcods ){

	global $db;

// 	if ($db->testa_superuser()) {

// 		return true;
		
// 	}else{
		if ( is_array( $pflcods ) )
		{
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		}
		else
		{
			$pflcods = array( (integer) $pflcods );
		}
		if ( count( $pflcods ) == 0 )
		{
			return false;
		}
		$sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		
		return $db->pegaUm( $sql ) ;
// 	}
}

function verificaPodeFinalizarPedido()
{
	global $db;

	if(!$_SESSION['sic']['slcid']) return false;
	
	$sql = "select
 					
			case when slcdtinclusao is not null then
				case when slcprorrogado = 't' then			
					case when 
							((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date)-current_date < 0 							  
						then 0
					else
							((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date)-current_date
						end
				else
					case when 
							((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+21 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+22 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+23 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+24 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+25 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date)-current_date < 0 
						then 0
					else
							((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+21 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+22 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+23 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+24 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+25 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date)-current_date
						 end
				end
			else
				0 end as dias
		from
			sic.solicitacao sc
		left join 
			entidade.entidade ent on ent.entid = sc.entid
		left join 
			entidade.endereco ede on ede.entid = sc.entid
		left join 
			territorios.municipio mun on mun.muncod = ede.muncod
		left join 
			workflow.documento doc on doc.docid = sc.docid
		left join 
			workflow.estadodocumento esd on esd.esdid = doc.esdid		
		where
			sc.slcid = {$_SESSION['sic']['slcid']}";

	$dias = $db->pegaUm($sql);
	
	//if($dias >= 1){
	//	return false;
	//}
	
	$sql = "select 
				usucpfresponsavel, 
				slcresposta,
				slcexistenoportaldomec,
				slcpublicar 
			from 
				sic.solicitacao 
			where 
				slcid = {$_SESSION['sic']['slcid']}";
	
	$dados = $db->pegaLinha($sql);
	
	$cpf 	  = $dados['usucpfresponsavel'];
	$resposta = trim($dados['slcresposta']);
	
	/*
	if($dados['slcexistenoportaldomec'] == 'f' || $dados['slcexistenoportaldomec'] == ''){		
		return false;
	}else{		
		if(empty($resposta)){			
			return false;
		}
	}*/
	
	if($cpf == $_SESSION['usucpf'] || checkPerfil(array(SIC_PERFIL_ADMINISTRADOR))|| checkPerfil(array(SIC_PERFIL_SUPER_USUARIO))){
		return true;
	}
	
	return false;
}

function enviaEmailParaAreaResponsavel( $slcid )
{
	global $db;
	
	$arEmail[] = EMAIL_PRINCIPAL_SIC;
	
	$sql = "select secemail, slcnumsic, slcpergunta from sic.solicitacao sc
			inner join sic.secretaria se on se.secid = sc.secid
			where slcstatus = 'A'
  			  and secstatus = 'A'
  			  and sc.slcid = {$slcid}";
	
	$solicitacao = $db->pegaLinha($sql);
	
	if($emailSecretaria){
		$arEmail[] = $solicitacao['secemail']; 
	}

	$sql = "select distinct
				usuemail 
			from sic.solicitacao sc
			inner join sic.usuarioresponsabilidade ur on sc.secid = ur.secid
			inner join seguranca.usuario us on us.usucpf = ur.usucpf
			where ur.rpustatus = 'A'
			and us.suscod = 'A'
			and sc.slcid = {$slcid}";
	
	$responsaveis = $db->carregar($sql);
	
	if($responsaveis){
		foreach($responsaveis as $dados){
			$arEmail[] = $dados['usuemail'];
		}
	}
	
	$remetente 	= ''; 
	$assunto	= 'SIC: Foi cadastrado uma solicitação à sua secretaria';
	 
	$conteudo	= ' 
					<p>Prezados,</p>
					
					<p>Existe uma solicitação nº '.$solicitacao['slcnumsic'].' pertencente a sua secretaria, 
					favor entrar no <a href="http://simec.mec.gov.br" target="_blank">simec</a> para devidas providências.</p>
					
					<p><b>Pergunta:</b>&nbsp;'.$solicitacao['slcpergunta'].'</p>
				  ';
	 
	$cc			= array($_SESSION['email_sistema']);
	$cco		= ''; 
	$arquivos 	= array();
			
	enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );	
	
	return true;
}

/**
 * Envia e-mail notificando usuários reponsaveis que uma solicitação tramitou para 
 * a situação de Análise NAI
 * 
 * @global object $db Objeto de conexão do banco
 * @param integer $slcid Codigo da solicitação
 * @return boolean Retorna verdadeiro caso o metodo não apresente erro
 */
function enviarEmailAnaliseNAI( $slcid )
{
    global $db;
    $arEmail = array();
    $arEmail[] = EMAIL_PRINCIPAL_SIC;
    
    $sql = "
        SELECT
            ent.entnome,
            secemail,
            slcnumsic,
            slcpergunta,
            us.usunome,
            us.usuemail,
                 case when slcdtinclusao is not null and slcprorrogado = 't' 
						then						
						to_char((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date, 'dd/MM/yyyy')							
					 when slcdtinclusao is not null and (slcprorrogado = 'f' or slcprorrogado is null) 
					 	then 
					 		to_char((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+21 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+22 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+23 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+24 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+25 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date, 'dd/MM/yyyy')
				else to_char(slcdtinclusao, 'dd/MM/yyyy') end as data_resposta
        FROM
            sic.solicitacao sc
            JOIN sic.secretaria se on se.secid = sc.secid
            JOIN seguranca.usuario us ON sc.usucpfinclusao = us.usucpf
            LEFT JOIN entidade.entidade ent ON sc.entid = ent.entid
        WHERE
            slcstatus = 'A'
            AND secstatus = 'A'
            AND sc.slcid = {$slcid}
    ";
    $solicitacao = $db->pegaLinha($sql);

    $responsaveis = $db->carregar(montarConsultaUsuariosEmailResponsaveis());

    if($responsaveis){
        foreach($responsaveis as $dados){
            $arEmail[] = $dados['usuemail'];
        }
    }
    
    $remetente = '';
    $assunto = 'SIC: Foi enviado uma solicitação à análise NAI';
    $conteudo = ' 
        <p>Prezados,</p>
        <p>Solicitação de nº '.$solicitacao['slcnumsic'].', prazo de atendimento '.$solicitacao['data_resposta'].', do solicitante '.$solicitacao['entnome']. 'foi enviada para Análise pelo NAI.</p>
    ';
    $arEmail[] = $solicitacao['usuemail'];
    $cco = '';
    $arquivos = array();
    enviar_email($remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos);

    return true;
}

/**
 * Monta SQL para buscar usuarios responsaveis que receberão e-mail ao tramitar 
 * para análise NAI
 * 
 * @return string SQL da consulta a ser realizada
 */
function montarConsultaUsuariosEmailResponsaveis(){
    $sql = "
        SELECT
            us.usunome,
            us.usuemail
        FROM
            sic.usuarioenvioemail use
            JOIN seguranca.usuario us ON use.usucpf = us.usucpf
        WHERE
            use.ueestatus = 'A'
    ";
    return $sql;
}

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function devolverParaEmcadastramento( $slcid )
{
	global $db;
	
	$sql = "update sic.solicitacao set usucpfresponsavel = null where slcid = {$slcid}";
	$db->executar($sql);
	$db->commit();
	return true;
}

function cancelarPedido( $slcid )
{
	global $db;
	
	if(!$slcid){
		return false;
	}
	
	$sql = "update sic.solicitacao set slcstatus = 'I' where slcid = {$slcid}";
	
	$db->executar($sql);
	if($db->commit()){
		return true;
	}
	return false;
}

function enviaEmailProrrogacao( $slcid )
{
	global $db;
		
	$sql = "select 
				se.secid,
				secemail, 
				slcnumsic, 
				slcpergunta 
			from 
				sic.solicitacao sc
			inner join 
				sic.secretaria se on se.secid = sc.secid
			where 
				sc.slcid = {$slcid}";
	
	$solicitacao = $db->pegaLinha($sql);	
	
	$arEmail = array(EMAIL_PRINCIPAL_SIC, $solicitacao['secemail'], $_SESSION['usuemail']);
	
	$remetente 	= ''; 
	$assunto	= 'SIC: Prorrogação de solicitação';
	 
	$conteudo	= ' 
					<p>Prezados,</p>
					
					<p>Foi prorrogado a solicitação nº '.$solicitacao['slcnumsic'].' pertencente a sua secretaria.</p>
					
					<p><b>Pergunta:</b>&nbsp;'.$solicitacao['slcpergunta'].'</p>
				  ';
	 
	$cc			= array($_SESSION['email_sistema']);
	$cco		= ''; 
	$arquivos 	= array();
	
	
	enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );
}

function verificaRespostaNAI()
{
	global $db;
	
	$sql = "select 
				slcexistenoportaldomec 
			from 
				sic.solicitacao 
			where 
				slcid = {$_SESSION['sic']['slcid']}";
	
	$rs = $db->pegaUm($sql);
		
	if($rs == 'f' || $rs == "'f'"){
		return true;
	}
	return false;
}

function verificaPeenchimentoRecurso()
{	
	global $db, $esdid;
	
	$sql = "select 
				slc.slcpergunta1instancia,
				slc.slcresposta1instancia,
				slc.slcpergunta2instancia,
				slc.slcresposta2instancia,
				doc.esdid
			from 
				sic.solicitacao slc
			left join
				workflow.documento doc on doc.docid = slc.docid 
			where 
				slc.slcid = {$_SESSION['sic']['slcid']}";
	
	
	$rs = $db->pegaLinha($sql);
	
	$esdid = $esdid ? $esdid : $rs['esdid']; 
	
	if($esdid == WF_ESDID_RECURSO_1_INSTANCIA && $rs['slcpergunta1instancia']){
		return true;
	}else 
	if($esdid == WF_ESDID_RECURSO_2_INSTANCIA && $rs['slcpergunta2instancia']){
		return true;
	}else 
	if($esdid == WF_ESDID_ANALISE_1_RECURSO && $rs['slcresposta1instancia']){
		return true;	
	}else
	if($_REQUEST['esdid'] == WF_ESDID_ANALISE_1_RECURSO && $rs['slcpergunta1instancia']){
		return true;
	}else	 
	if($esdid == WF_ESDID_ANALISE_2_RECURSO && $rs['slcresposta2instancia']){
		return true;
	}else 
	if($_REQUEST['esdid'] == WF_ESDID_ANALISE_2_RECURSO && $rs['slcpergunta2instancia']){
		return true;
	}else if(in_array($_REQUEST['esdid'],array(WF_ESDID_FINALIZADO_1_INSTANCIA,WF_ESDID_FINALIZADO_2_INSTANCIA)) && ($rs['slcpergunta1instancia'] && $rs['slcresposta1instancia'])){
		return true;
	}	
	return false;
}

function subistituiCaracteres($string) {
	$palavra = strtr ( $string, "ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ", "SOZsozYYuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy" );
	$palavranova = str_replace ( "_", " ", $palavra );
	$pattern = '|[^a-zA-Z0-9\-]|';
	$palavranova = preg_replace ( $pattern, ' ', $palavranova );
	$string = str_replace ( ' ', '', $palavranova );
	$string = str_replace ( '---', '', $string );
	$string = str_replace ( '--', '', $string );
	$string = str_replace ( '-', '', $string );
	return $string;
}
?>