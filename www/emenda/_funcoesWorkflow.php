<?php
/**
 * Caso o documento não estaja criado cria um novo
 *
 * @param string $ptrid
 * @return integer
 */
function criarDocumento( $ptrid ) {
	global $db;

	$docid = pegarDocid($ptrid);

	if( ! $docid ) {
		$obEmenda = new Emenda();
		$federal = $obEmenda->buscaEmendaDescentraliza( $ptrid );
		
		$boImpositivo = verificaOrcamentoImpositivo( $ptrid );
			
		if( $boImpositivo > 0 ){
			$tpdid = TPDID_EMENDA_IMPOSITIVO;
			$docdsc = "Cadastro de PTA (emendas orçamento impositivo) - n°" . $ptrid;
		} elseif( $federal == 'S' ){
			$tpdid = TPDID_DESCENTRALIZA;
		} else {
			// recupera o tipo do documento
			$tpdid = TPDID_EMENDAS;				
			// descrição do documento
			$docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
		}

		// descrição do documento
		$docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza o plano de trabalho
		$sql = "UPDATE
					emenda.planotrabalho
				SET 
					docid = ".$docid." 
				WHERE
					ptrid = ".$ptrid;

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;
}

/**
 * Pega o id do documento do plano de trabalho
 *
 * @param integer $ptrid
 * @return integer
 */
function pegarDocid( $ptrid ) {
	global $db;

	$sql = "SELECT
				docid
			FROM
				emenda.planotrabalho
			WHERE
			 	ptrid = " . (integer) $ptrid;

	return (integer) $db->pegaUm( $sql );
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $ptrid
 * @return integer
 */
function pegarEstadoAtual( $ptrid ) {
	global $db;

	$docid = pegarDocid( $ptrid );

	$sql = "select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;

	$estado = (integer) $db->pegaUm( $sql );

	return $estado;
}


/**
 * Verifica se o plano de trabalho pode ser enviado para analise.
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioPlanoTrabalho( $url = null, $ptrid = null){
	global $db;
	$url = !$url ? $_SESSION['favurl'] : $url;
	
		if( date('Y-m-d') >= date('Y').'04-01' ){
			return 'Prazo de envio encerrado de acordo com a Portaria Interministerial n° 40/2014';
		}

	if( strstr($url, "emenda.php?modulo=principal/validacaoPTA&acao=A") ){

		include_once APPRAIZ . 'emenda/classes/ValidacaoPlanoTrabalho.class.inc';
		include_once APPRAIZ . 'emenda/classes/Emenda.class.inc';
		$obj = new ValidacaoPlanoTrabalho($ptrid);
		$obj->validacaoPlanoTrabalho();

		$obEmenda = new Emenda();
		$federal = $obEmenda->buscaEmendaDescentraliza( $ptrid );
		if( empty($_SESSION['emenda']['federal']) ){
			$_SESSION['emenda']['federal'] = $federal;
		}

		$estadoAtual = pegarEstadoAtual( $ptrid );

		if(!$obj){
			return false;
		}elseif(!empty($obj->retorno["recursospta"])) {
			return false;
		}elseif(!empty($obj->retorno["iniciativas"])) {
			return false;
		}elseif(!empty($obj->retorno["cronograma"])) {
			return false;
		}elseif(!empty($obj->retorno["escolasbeneficiadas"]) && $federal == 'N' ) {
			return false;
		}if(!empty($obj->retorno["dadosadicionais"])) {
			return false;
		}elseif(!empty($obj->retorno["anexo"]) && $federal == 'S' && $estadoAtual != EM_REFORMULACAO_PROCESSO ) {
			return false;
		}elseif(!empty($obj->retorno["entidade"]) && $federal == 'S' ) {
			return false;
		}elseif($estadoAtual != EM_ELABORACAO && $estadoAtual != EM_REFORMULACAO_PROCESSO){
			return false;
		}else{
			return true;
		}
	}
	return false;
}


/**
 * Quando o pta e enviado para analise, roda essa função para enserir uma analise do tipo analise de dados
 *
 * @param integer $ptrid
 * @param integer $resid
 * @return boolean
 */
function insereAnaliseEmenda( $ptrid ){

	global $db;
	
	if( $_SESSION['exercicio'] >= '2012' ){
		
		$sql = "SELECT COALESCE(max(analote), null, 0) FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'D'";
		$analote = $db->pegaUm( $sql );
		
		$sql = "SELECT
				count(ana.anaid)
			FROM 
				emenda.analise ana 
			WHERE 
				ana.ptrid = {$ptrid} 
				AND ana.anatipo = 'D'
				and ana.analote = ".($analote ? $analote : '0');
				
		$boAnalise = $db->pegaUm( $sql );
		
		if( (int)$boAnalise == (int)0 ){
			$analote = !empty($analote) ? $analote + 1 : 1;
			
			$sql = "INSERT INTO emenda.analise( ptrid, anatipo, analote, anadatainclusao )
					VALUES ({$ptrid}, 'D', {$analote}, 'now')";
		
			$db->executar( $sql );
			$db->commit();
		}
	}

	return true;
}
function insereAnaliseTecnicaElaboracao( $ptrid ){
	global $db;
		
	$sql = "SELECT max(analote) FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'T'";
	$analote = $db->pegaUm( $sql );
	
	$arrAnalise = $db->pegaLinha("SELECT ana.uniid, usu.usuemail FROM emenda.analise ana
								inner join seguranca.usuario usu on usu.usucpf = ana.usucpf 
							WHERE ptrid = {$ptrid} AND anatipo = 'T' and analote = $analote");
	
	$analote = !empty($analote) ? $analote + 1 : 1;
	$sql = "INSERT INTO emenda.analise( ptrid, anatipo, uniid, analote, anadatainclusao )
			VALUES ({$ptrid}, 'T', ".($arrAnalise['uniid'] ? $arrAnalise['uniid'] : 'null').", {$analote}, 'now')";
	
	$db->executar( $sql );
	$db->commit();
	$ptrcod = pegaNumPTA( $ptrid );
	$strMensagem = "O plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi corrigido e retornado para Análise Técnica em ".date('d/m/Y h:i:s').".";
	$strAssunto = SIGLA_SISTEMA. ' - Emenda';
	$strEmailTo = $arrAnalise['usuemail'];
	enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
	
	return true;
}

function validaEnvioAnaliseMerito( $ptrid ){
	global $db;
	
	$sql = "SELECT
				count(ana.anaid)
			FROM 
				emenda.analise ana 
			WHERE 
				ana.ptrid = {$ptrid} 
				AND ana.anatipo = 'M' 
				--and ana.anadataconclusao is not null
				and ana.analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = 'M' 
						                    	and ptrid = ana.ptrid)";
	$boAnalise = $db->pegaUm( $sql );
	
	if( $boAnalise > 0 ) return true;
	
	return false;
}


/**
 * Verifica se é possivel movimentar o fluxos entre a analise de dados, de merito, tecnica.
 *
 * @param string $url
 * @param integer $ptrid
 * @param char $tipo
 * @return boolean
 */
function validaEnvioAnalise( $url = null, $ptrid = null , $tipo = 'T'){
	global $db;

	$url = !$url ? $_SESSION['favurl'] : $url;
	$estado = pegarEstadoAtual( $ptrid );
	if(!$ptrid) return false;
	
	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$sql = "SELECT ana.anasituacaoparecer FROM emenda.analise ana 
				WHERE 
					ana.ptrid = {$ptrid} 
					AND ana.anatipo = 'T' 
					AND ana.anadataconclusao IS NOT NULL 
					AND ana.anastatus <> 'I'
					and ana.analote = (SELECT max(analote) FROM emenda.analise WHERE anatipo = 'T' and anastatus = 'A' and ptrid = ana.ptrid)";

		$anaid = $db->carregarColuna( $sql );
		
		#verifica a situação do parecer E - Diligencia, D - Indeferido
		if( in_array( 'E', $anaid) || in_array( 'D', $anaid) ){
			return false;
		}
		if( !empty($anaid) ) return true;
	}

	if( strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") ){
		$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";			  
		$arDados = $db->pegaLinha($sql);

		if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ) return false;

		$sql = "select uniid from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T') $filtro";
			
		$unidades = $db->pegaUm($sql);
		if( empty($unidades) ) return false;
	}
	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$arTipos = pegaTipoReformulacao( $ptrid );

		$arCodigo = array();
		foreach ($arTipos as $v) {
			array_push( $arCodigo, $v['codigo'] );
		}
		
		# verfica se o codigo da reformulação é diferente 1 - Alteração das clausulas do convenio
		if( !in_array(1, $arCodigo) ){
			$existeExecucao = $db->pegaUm("select 1 from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A' and semid is null");
			if( !$existeExecucao ){
				return false;
			}
		}
	}
	if( strstr($url,"emenda.php?modulo=principal/analiseMeritoPTA&acao=A") ){
		$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
		#verifica se o responsavel do pta é a SESU
		//if( $resid != 1 && $_SESSION['exercicio'] == '2011' ) return false;
		$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
		if( $reenalisemerito == 'S' ){
			return false;
		} else {			
			return true;
		}
	}
	if( !strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") && ($estado == EM_ANALISE_TECNICA || $estado == EM_ANALISE_TECNICA_REFORMULACAO)){
		return false;
	}elseif( !strstr($url,"emenda.php?modulo=principal/analiseMeritoPTA&acao=A") && $estado == EM_ANALISE_DE_MERITO){
		return false;
	}elseif( !strstr($url,"emenda.php?modulo=principal/analiseDadosPTA&acao=A") && $estado == EM_ANALISE_DE_DADOS){
		return false;
	}elseif( !strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") && $estado == EM_ANALISE_DO_FNDE){
		return false;
	}else{
		return pegaConclusaoAnaliseDados( $ptrid, $tipo );
	}
}


/**
 * Verifica se a analise foi informada o parecer tecnico, isto é, se ela foi concluida.
 *
 * @param integer $ptrid
 * @param string $tipo
 * @param string $anastatus
 * @return boolean
 */
function pegaConclusaoAnaliseDados( $ptrid, $tipo = 'D' , $anastatus = "I"){
	global $db;
	
	$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
	if( $reenalisemerito == 'N' ) return false;
	
	$sql = "SELECT ana.anasituacaoparecer FROM emenda.analise ana
			WHERE ana.ptrid = {$ptrid} AND ana.anatipo = '{$tipo}' AND ana.anadataconclusao IS NOT NULL AND ana.anastatus <> '$anastatus'
            	and ana.analote = (SELECT max(analote) FROM emenda.analise WHERE anatipo = '{$tipo}' and anastatus = 'A' and ptrid = ana.ptrid)";

	$arDados = $db->carregarColuna($sql);
	$arDados = ( $arDados ? $arDados : array() );
	
	#verifica a situação do parecer E - Diligencia, D - Indeferido
	if( in_array( 'E', $arDados ) || in_array( 'D', $arDados ) ){
		return false;
	} else {
		if( empty($arDados) ) return false;
		return true;
	}
}


/**
 * Altera o status da analise de mérito para A - ativo
 * Envia e-mail para a unidade responsavel pela análise
 * Envia e-mail para o responsavel pela entidade do pta
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function alteraStatusAnaliseMerito( $url, $ptrid ){
	global $db;

	$sql = "UPDATE emenda.analise SET anastatus = 'A' WHERE ptrid = $ptrid and anatipo = 'M' and anadataconclusao is null";
	$db->executar($sql);
	
	if($db->commit() == "1"){
		//email de todos os cpfs autorizados da instituição, informando que o estado do workflow foi alterado.
		//if(enviaEmailMovimentacao($url, $ptrid)){
			//envia email para os responsavel das unidades
			//enviaEmailPerfilUnidade($url, $ptrid, 'M');
			return validaFluxoWorkflowReformulacao( $url, $ptrid );
		//}
		return false;
	}else{
		return false;
	}
}


/**
 * Envia o pta para correção
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function enviaAnaliseCorrecaoPTA( $url = null, $ptrid = null, $tipo = null ){
	global $db;

	if( (strstr($url, "emenda.php?modulo=principal/analiseMeritoPTA&acao=A") && pegarEstadoAtual( $ptrid ) == EM_ANALISE_DE_MERITO)
			|| (strstr($url, "emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") && pegarEstadoAtual( $ptrid ) == EM_ANALISE_TECNICA ) ){
		$sql = "SELECT
					ana.anasituacaoparecer
				FROM 
					emenda.analise ana 
				WHERE 
					ana.ptrid = {$ptrid} 
					AND ana.anatipo = '{$tipo}' 
					AND ana.anadataconclusao IS NOT NULL 
					AND ana.anastatus <> 'I'
					and ana.analote = (SELECT max(analote) FROM emenda.analise WHERE anatipo = '{$tipo}' and anastatus = 'A' and ptrid = ana.ptrid)";

		$anaid = $db->carregarColuna( $sql );
		
		#verifica a situação do parecer E - Diligencia, D - Indeferido
		if( in_array( 'E', $anaid) || in_array( 'D', $anaid) ){
			return true;
		}
		return false;
	}else{
		if( (strstr($url, "emenda.php?modulo=principal/analiseDadosPTA&acao=A") && pegarEstadoAtual( $ptrid ) == EM_ANALISE_DE_DADOS)){
			return true;
		} else {
			return validaEnvioAnalise( $url, $ptrid, $tipo);
		}
	}
}

/**
 * 
 */

function validaRetornoAnaliseDados($ptrid){
	global $db;
	if( $_SESSION['exercicio'] == '2011' ){
		$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	
		if( $resid == 1 ) return false;
		else return true;
	} else {
		return true;
	}
}

function validaRetornoAnaliseMeritoSESU($ptrid){
	global $db;
	
	$sql = "SELECT
				count(ana.anaid)
			FROM 
				emenda.analise ana 
			WHERE 
				ana.ptrid = {$ptrid} 
				AND ana.anatipo = 'M' 
				and ana.anadataconclusao is not null
				and ana.analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = 'M' 
						                    	and ptrid = ana.ptrid)";
	$boAnalise = $db->pegaUm( $sql );

	$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
	
	if( $reenalisemerito == 'S' && $boAnalise > 0 ){
		return true;
	} else {
		return false;
	}
}
function validaEnvioAnaliseDados($ptrid){
	global $db;
	
	if( $_SESSION['exercicio'] != '2011' ) return false;
	
	$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");

	if( $resid == 1 ){
		$sql = "SELECT
					ana.anaid
				FROM 
					emenda.analise ana 
				WHERE 
					ana.ptrid = {$ptrid} 
					AND ana.anatipo = 'M' 
					and ana.anadataconclusao is not null
					and ana.analote = (SELECT max(analote) 
							    				FROM emenda.analise 
							                    WHERE anatipo = 'M' 
							                    	and ptrid = ana.ptrid)";
		$anaid = $db->pegaUm( $sql );
	
		if( empty($anaid) ) return false;
		else return true;
	}
	else return false;
}

function validaRetornoAnaliseTecnicaSesu($ptrid){
	global $db;
	
	if( $_SESSION['exercicio'] != '2011' ) return false;
	
	$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	
	$sql = "SELECT ed.esdid, ed.esddsc, ac.aeddscrealizada, ac.aedid, ac.* 
		    FROM workflow.historicodocumento hd
		    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
		    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
		    WHERE hd.docid = {$docid} 
		    order by hd.hstid desc limit 1";
    
    $historicoDocumento = $db->pegaLinha($sql);
    $retorno = '';
    /*foreach($historicoDocumento as $chave => $documento){    	
        if($documento['esdid'] == EM_ANALISE_TECNICA){
            $esdiddestino = $historicoDocumento['esdid'];
            $aedid = $historicoDocumento['aedid'];
            break;
        }
    }*/
    $esdiddestino = $historicoDocumento['esdid'];
    if( $esdiddestino == EM_ANALISE_TECNICA ) return true;
    
    return false;
}


/**
 * Deleta a analise de merito e envia e-mail para o responsavel pela entidade
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function deletaAnaliseMerito( $url = null, $ptrid = null, $tipo = null ){

	global $db;
	$sql = "delete from emenda.anexo where anaid in (select anaid from emenda.analise where ptrid = $ptrid AND anatipo = 'M' AND anadataconclusao is null AND uniid is not null)";
	$db->executar($sql);
	$sql = "delete from emenda.analisehistorico where anaid in (select anaid from emenda.analise where ptrid = $ptrid AND anatipo = 'M' AND anadataconclusao is null AND uniid is not null)";
	$db->executar($sql);	
	$sql = "DELETE FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'M' AND anadataconclusao is null AND uniid is not null";
	$db->executar( $sql );
	if($db->commit() == "1"){
		enviaEmailResponsavelEntidade($url, $ptrid, $tipo);
		return true;
	} else {
		return false;
	}
}


/**
 * Enter description here...
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function verificaEnvioInfGerais( $url, $ptrid ){
	global $db;
	
	if( $_SESSION['exercicio'] >= '2012'  ) return false;
	
	$resid = $db->pegaUm("SELECT resid, refid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = $resid");
	
	#desabilita essa ação para ano de exercicio igual a 2011
	//if( $_SESSION['exercicio'] != '2011' ) return false;
	if( $reenalisemerito == 'N' ) return true;
	$sql = "SELECT
					ana.anaid
				FROM 
					emenda.analise ana 
				WHERE 
					ana.ptrid = {$ptrid} 
					AND ana.anatipo = 'T'
					and ana.anastatus = 'A' 
					and ana.anadataconclusao is not null
					and ana.analote = (SELECT max(analote) 
							    				FROM emenda.analise 
							                    WHERE anatipo = 'T'
							                    	and anastatus = 'A' 
							                    	and ptrid = ana.ptrid)";
	$anaid = $db->pegaUm( $sql );
	
	if( empty($anaid) ) return false;
	else return true;
}


/**
 * Enviar e-mail informando a movimentação do fluxo
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function enviaEmailMovimentacao($url, $ptrid){
	global $db;

	if( strstr($url, 'emenda.php?modulo=principal/analiseDadosPTA&acao=A') ){
		$tipo = 'Análise de Dados';
		$para = 'Análise de Mérito';
	} else if( strstr($url, "emenda.php?modulo=principal/analiseMeritoPTA&acao=A") ){
		$tipo = 'Análise de Mérito';
		$para = 'Análise do FNDE';
	} else if( strstr($url, "emenda.php?modulo=principal/informacoesGerais&acao=A") ){
		$tipo = 'Análise do FNDE';
		$para = 'Análise Técnica';
	} else if( strstr($url, "emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$tipo = 'Análise Técnica';
		$para = 'Empenho';
	} else if( strstr($url, "emenda.php?modulo=principal/execucaoPTA&acao=A") ){
		$tipo = 'Empenho';
		$para = '';
	}

	$ptrcod = pegaNumPTA( $ptrid );

	$sql = "SELECT enbid FROM emenda.planotrabalho WHERE ptrid = $ptrid";
	$enbid = $db->pegaUm($sql);

	$strAssunto = SIGLA_SISTEMA. ' - Emenda';

	$sql = "SELECT
				usu.usuemail
			FROM 
				emenda.usuarioresponsabilidade usr
                    inner join seguranca.usuario usu
                    on usu.usucpf = usr.usucpf
			WHERE 
				usr.enbid = {$enbid}
				and usr.rpustatus = 'A'";

	$strEmailTo = $db->carregarColuna($sql);

	$strMensagem = "A situação de seu plano de trabalho nº $ptrcod/{$_SESSION['exercicio']} foi alterada de \"$tipo\" para \"$para\", em ".date('d/m/Y h:i:s').".";
	
	$retorno = enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
	if( $retorno && $tipo == 'Análise Técnica' ){
		$sql = "UPDATE
				  emenda.planotrabalho  
				SET 
				  ptrstatus = 'I'				 
				WHERE 
				  ptrid IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptrid = $ptrid and tt.ptrstatus = 'A')";
		$db->executar( $sql );
		$db->commit();

		enviaEmailSituacaoEmpenho($ptrid);
	}
	return true;
}


/**
 * Envia e-mail para o responsavel pela entidade
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function enviaEmailResponsavelEntidade($url, $ptrid, $tipo){
	global $db;

	if( strstr($url, "emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$sql = "UPDATE
				  	emenda.planotrabalho  
				SET 
				  	ptrestadoretorno = (SELECT
											esd.esdid
										FROM
											emenda.planotrabalho ptr
											inner join workflow.documento doc 
										    	on ptr.docid = doc.docid
											inner join workflow.estadodocumento esd 
										    	on esd.esdid = doc.esdid
										        and esd.esdstatus = 'A'
										WHERE
											ptr.ptrid = $ptrid
										    and ptr.ptrstatus = 'A')
				WHERE 
				  	ptrid = $ptrid";
			
		$db->executar($sql);
		$db->commit();
	}

	$sql = "SELECT DISTINCT
			    a.anasituacaoparecer
			FROM
				emenda.analise a
			WHERE
				a.ptrid = $ptrid
			    and a.anastatus = 'A'
			    and a.anatipo = '$tipo'
			    and a.analote = (SELECT max(analote) FROM emenda.analise 
			    					WHERE ptrid = a.ptrid
			                              and anatipo = '$tipo' 
			                              and anastatus = 'A')";

	$arDados = $db->carregarColuna($sql);

	if($arDados){
		foreach ($arDados as $valor) {
			enviaEmailEntidade($ptrid, $tipo, $valor);
		}
	}
	return true;
}


/**
 * Valida retorno para analise de merito
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoAnaliseMerito($url, $ptrid){
	global $db;
	
	$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
	
	if( $reenalisemerito == 'S' && $_SESSION['exercicio'] != '2012' ){
		return true;
	} else {
		return false;
	}
}


/**
 * Verifica se a ação esta liberada para o ano do exercicio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioPorAnoExercicio($url, $ptrid){
	global $db;

	if( strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") && $_SESSION['exercicio'] == '2009' ){
		return false;
	} else if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") && $_SESSION['exercicio'] == '2010' ){
		return false;
	} else if( strstr($url,"emenda.php?modulo=principal/execucaoPTA&acao=A") && $_SESSION['exercicio'] == '2009' ){
		return false;
	} else if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A") && $_SESSION['exercicio'] == '2010' ){
		return false;
	} else {
		return true;
	}
}

/**
 * Valida envio para analise tecnica
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function validaEnvioAnaliseTecnica( $url = null, $ptrid = null , $tipo = 'T' ){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
	
	//if( in_array(APOSTILAMENTO, $arRrefid) ) return false; #Reformulação de Apostilamento
	
	$ptridPai = pegaPaiPTA( $ptrid );
	#desabilita essa ação para ano de exercicio igual a 2011
	if( $_SESSION['exercicio'] == '2011' && empty($ptridPai) ) return false;
	//if($db->testa_superuser()) return true;

	$url = !$url ? $_SESSION['favurl'] : $url;

	if(!$ptrid){
		return false;
	}
	
	if( strstr( $url, "emenda.php?modulo=principal/validacaoPTA&acao=A" ) || strstr( $url, "emenda.php?modulo=principal/validacaoPTA&acao=A&popup=true") ){
		if( $_SESSION['exercicio'] == '2009' ) return false;

		if(!validaEnvioPlanoTrabalho( $url, $ptrid)) return false;

		$docid = pegarDocid( $ptrid );

		$sql = "select
					ed.esdid
				from workflow.historicodocumento hd
					inner join workflow.acaoestadodoc ac on
						ac.aedid = hd.aedid
					inner join workflow.estadodocumento ed on
						ed.esdid = ac.esdidorigem
				where
					hd.docid = $docid
		            and hd.hstid in ( select max(hstid) from workflow.historicodocumento where docid = $docid )";

		$estadoAnterior = $db->pegaUm( $sql );
		if( $estadoAnterior == EM_ANALISE_TECNICA ) return true;
		return false;
	}

	validaEnvioPorAnoExercicio( $url, $ptrid );
	
	$boPai = pegaPaiPTA( $ptrid );

	if( !empty($boPai) && !strstr( $url, "emenda.php?modulo=principal/informacoesGerais&acao=A" ) ) return false;

	$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";
	$arDados = $db->pegaLinha($sql);

	if( !strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") && pegarEstadoAtual( $ptrid ) == EM_ANALISE_DO_FNDE){
		return false;
	}else{
		
		$statusAna = ($_SESSION['exercicio'] == '2011' && empty($boPai)) ? " and anastatus = 'I' " : " and anastatus = 'A' ";
		$sql = "select anaid from emenda.analise where ptrid = $ptrid and anatipo = 'T' $statusAna";
		$anaid = $db->carregar($sql);
		
		if( !empty($anaid) ){
			if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ){
				return false;
			} else {
				$arTipos = pegaTipoReformulacao( $ptrid );
				$arCodigo = array();
				foreach ($arTipos as $v) {
					array_push( $arCodigo, $v['codigo'] );
				}
				# verfica se o codigo da reformulação é diferente 1 - Alteração das clausulas do convenio
				if( !in_array( 1, $arCodigo ) ){
					$existeExecucao = $db->pegaUm("select 1 from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A'");
					if( !$existeExecucao ){
						return false;
					} else
					return true;
				}
				return true;
			}
		}else{
			return false;
		}
	}
}


/**
 * Atualiza a analise tecnica para status = 'A'
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function selecionaUnidadeTecnicaPTA($url, $ptrid){
	global $db;

	$sql = "update emenda.analise set anastatus = 'A' where ptrid = {$ptrid} and anatipo = 'T' and anadataconclusao is null and uniid is not null";
	$db->executar( $sql );

	if($db->commit() == "1"){
		if( strstr( $url, 'emenda.php?modulo=principal/informacoesGerais&acao=A' ) || strstr( $url, 'emenda.php?modulo=principal/analiseDadosPTA&acao=A' ) ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&ptridAnalise=$ptrid';
				</script>";	
			return true;
		} else {
			//email de todos os cpfs autorizados da instituição, informando que o estado do workflow foi alterado.
//			enviaEmailMovimentacao($url, $ptrid);
			//envia email para os responsavel das unidades
			enviaEmailPerfilUnidade($url, $ptrid, 'T');
			return true;
		}
	} else {
		return false;
	}
}


/**
 * Verifica se pode movimentar o workflow para empenho
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function validaEnvioEmpenho( $url, $ptrid, $tipo = null){
	global $db;
	include_once APPRAIZ.'emenda/classes/Emenda.class.inc';
	
	$obEmenda = new Emenda();
	$federal = $obEmenda->buscaEmendaDescentraliza( $ptrid );
	
	if( $federal == 'N' ){
		return false;
	}
	
	$estadoAtual = pegarEstadoAtual( $ptrid );
	
	#desabilita essa ação para ano de exercicio igual a 2011
	if( $estadoAtual != EM_ANALISE_DO_FNDE || !strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") )
		if( $_SESSION['exercicio'] == '2011' ) return false;
		
	if( $_SESSION['exercicio'] == '2011' ){
		$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";
		$arDados = $db->pegaLinha($sql);
		
		$existeExecucao = $db->pegaUm("select 1 from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A' and semid is null");
		//if( !$existeExecucao ) return false;

		if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ) return false;
	}
	
	if( strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") ){
		
		$sql = "select
		            ed.esdid
				from workflow.historicodocumento hd
					inner join workflow.acaoestadodoc ac on
						ac.aedid = hd.aedid
					inner join workflow.estadodocumento ed on
						ed.esdid = ac.esdidorigem
					inner join seguranca.usuario us on
						us.usucpf = hd.usucpf
					left join workflow.comentariodocumento cd on
						cd.hstid = hd.hstid
				where
					hd.docid in (select docid from emenda.planotrabalho where ptrid = $ptrid)
				order by
					hd.htddata asc, hd.hstid asc";
		
		$historico = $db->carregarColuna( $sql );
		$historico = $historico ? $historico : array();
		
		if( !in_array( EM_EMPENHO, $historico ) ){
			return false; 
		}
		
		if($_SESSION['exercicio'] == '2009'){
			$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";			  
			$arDados = $db->pegaLinha($sql);

			if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ) return false;
		}

		$sql = "select uniid, anasituacaoparecer, anadataconclusao from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')";
		$arDados = $db->carregar($sql);
		
		$retorno = true;
		if( $_SESSION['exercicio'] == '2010'){
			if(is_array($arDados)){
				foreach ($arDados as $v) {
					if( (empty($v['anasituacaoparecer']) && empty($v['anadataconclusao']) ) || $v['anasituacaoparecer'] == 'D' ){
						$retorno = false;
					}
				}
			}
		}

		if( empty($arDados[0]['anadataconclusao']) || !$retorno ){
			return false;
		} else return true;
	}
	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		if($_SESSION['exercicio'] == '2009') return false;

		$sql = "select anasituacaoparecer from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')
					and anadataconclusao is not null";
			
		$arDados = $db->carregarColuna($sql);
		$arDados = ( $arDados ? $arDados : array() );
		
		#verifica a situação do parecer E - Diligencia
		if( in_array( 'E', $arDados ) || empty($arDados) ){
			return false;
		} else {
			return true;
		}
	}
	return false;
}


/**
 * Excluir analise tecnica 
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function excluirAnaliseTecnica($url = null, $ptrid = null, $tipo = null){
	global $db;
	$sql = "delete from emenda.anexo where anaid in (select anaid from emenda.analise where ptrid = $ptrid and anatipo = 'T' and anastatus = 'I')";
	$db->executar($sql);
	$sql = "delete from emenda.analisehistorico where anaid in (select anaid from emenda.analise where ptrid = $ptrid and anatipo = 'T' and anastatus = 'I')";
	$db->executar($sql);
	$sql = "delete from emenda.analise where ptrid = {$ptrid} and anatipo = 'T' and anastatus = 'I'";
	$db->executar($sql);
	if( $db->commit() == "1" ){
		enviaEmailResponsavelEntidade($url, $ptrid, $tipo);
		//email de todos os cpfs autorizados da instituição, informando que o estado do workflow foi alterado.
//		enviaEmailMovimentacao($url, $ptrid);
		//envia email para os responsaveis pelo empenho da analise
		enviaEmailSituacaoEmpenho($ptrid);
		return true;
	} else {
		return false;
	}
}


/**
 * Valida o retorno para Vinculação das Unidades Gestoras
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoUnidadeGestora($url, $ptrid){
	global $db;
	#desabilita essa ação para ano de exercicio igual a 2011
	if( $_SESSION['exercicio'] >= '2011' ) return false;
	return true;
}

function validaEnvioVinculacaoUnidadeGestora($ptrid){
	global $db;
	
	$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
	
	if( $reenalisemerito == 'S' ){
		$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";			  
		$arDados = $db->pegaLinha($sql);
	
		if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ) return false;
	
		$sql = "select uniid from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T') ";
			
		$unidades = $db->pegaUm($sql);
		if( empty($unidades) ) return false;
		
		return true;
	} else {
		return false;
	}
}

function validaEnvioAnaliseTecnica2012($url, $ptrid, $tipo = ''){
	global $db;
	
	if( $_SESSION['exercicio'] >= '2012' ){
		
		$resid = $db->pegaUm("SELECT resid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
		$reenalisemerito = $db->pegaUm("SELECT reenalisemerito FROM emenda.responsavelexercicio WHERE prsano = '{$_SESSION['exercicio']}' and resid = ".$resid);
		
		//if( $reenalisemerito == 'N' ){
			$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";			  
			$arDados = $db->pegaLinha($sql);
		
			if( !$arDados['ptrnumprocessoempenho'] || !$arDados['ptrnumdocumenta'] ) return false;
		
			$sql = "select uniid from emenda.analise
					where ptrid = $ptrid and anatipo = 'T'
						and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T') ";
				
			$unidades = $db->pegaUm($sql);
			if( empty($unidades) ) return false;
			
			return true;
		/*} else {
			return false;
		}*/
	} else {
		return validaEnvioAnaliseTecnica( $url, $ptrid, $tipo);
	}
}


/**
 * Valida retorno para identificação do processo
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoIdentificacaoProcesso($url, $ptrid){
	global $db;
	#desabilita essa ação para ano de exercicio igual a 2011
	if( $_SESSION['exercicio'] == '2011' ) return false;
	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A")){
		if( $_SESSION['exercicio'] == '2009' ) return false;
		return true;
	}
	return false;
}


/**
 * Valida envio para pagamento
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function validaEnvioPagamento( $url, $ptrid, $tipo = null ){
	global $db;

	$sql = "SELECT 1 FROM emenda.execucaofinanceira WHERE ptrid = $ptrid and exfnumempenhooriginal is not null";
	$exfnumempenhooriginal = $db->pegaUm( $sql );

	if( empty( $exfnumempenhooriginal ) ){
		return false;
	} else {
		return validaEnvioAnalise( $url, $ptrid , $tipo);
	}
}


/**
 * Valida envio para a solicitação de reformulação do projeto
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioSolicitacaoReformulacao($url, $ptrid){
	global $db;
	//if( $_SESSION['exercicio'] == '2010' ) return false;

	$sql = "SELECT DISTINCT
				exf.exfid
			FROM
				emenda.execucaofinanceira exf
                left join emenda.ordembancaria orb
                	on orb.exfid = exf.exfid
                	and orb.orbnumsolicitacao is not null
			WHERE 
				exf.ptrid = $ptrid 
                and exf.exfstatus = 'A'
                and exf.semid = 4";

	$ordemBancaria = $db->carregar( $sql );

	if( empty($ordemBancaria) ){
		return false;
	}
	return true;
}


/**
 * Verifica qual o fluxo o sistema deve seguir e abre a pagina automatico do fluxo
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaFluxoWorkflowReformulacao( $url, $ptrid ){
	global $db;

	$estadoAtual = pegarEstadoAtual( $ptrid );
	$arPerfil    = pegaPerfilArray( $_SESSION['usucpf'] );
	
	if( strstr($url, "emenda.php?modulo=principal/validacaoPTA&acao=A") || strstr($url, "emenda.php?modulo=principal/validacaoPTA&acao=A&popup=true")){
		$ptridpai = pegaPaiPTA( $ptrid );
		
		if( !empty($ptridpai) ){
			$arTipos     = pegaTipoReformulacao( $ptrid );
			$refid		 = $db->pegaUm( "SELECT max(refid) FROM emenda.ptminreformulacao WHERE ptrid = ".$ptrid );
			$sql 	     = "";
	
			if( is_array( $arTipos ) ){
				foreach( $arTipos as $tipo ){
					# verfica se o codigo da reformulação é diferente 8 - Prorrogração de Ofício
					if( $tipo['codigo'] == 8 ) ativaPlanoTrabalhoFilho( $ptrid );
					$rftid = $db->pegaUm( "SELECT count(rftid) FROM emenda.reformulatipos WHERE refid = $refid and trefid = {$tipo['codigo']}" );
					if( $rftid == 0 ) $sql .= "INSERT INTO emenda.reformulatipos (trefid,refid) VALUES (".$tipo['codigo'].",".$refid.");";
				}
				
				if( $sql && $sql != '' ){
					$db->executar($sql);
					$db->commit();
				}
			}
		}
	}
	/**
	 * Fluxo do workflow para analise do PTA
	 */

	if( strstr($url,"emenda.php?modulo=principal/analiseDadosPTA&acao=A") ){
		if( $estadoAtual == EM_ANALISE_DE_MERITO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/analiseMeritoPTA&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
	}
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );

	/**
	 * Fluxo do workflow para reformulação
	 */

	if( $estadoAtual == EM_SOLICITACAO_REFORMULACAO ){
		echo "<script>
				window.opener.location = '/emenda/emenda.php?modulo=principal/reformulacao&acao=A&ptridAnalise=$ptrid';
				window.close();
			</script>";
		return true;
	}
	if( $estadoAtual == EM_REFORMULACAO_PROCESSO ){
			
		echo "<script>
				window.opener.location = '/emenda/emenda.php?modulo=principal/alteraDefinirRecursoPTA&acao=A&ptrid=".$ptrid."&emeid=0';
				window.close();
			 </script>";

		echo "<script>
				widow.location.href = emenda.php?modulo=principal/informacoesGerais&acao=A&ptridAnalise=$ptrid;
			 </script>";
		return true;
	}
	if( $estadoAtual == EM_PROCESSO_REFORMULADO ){
		echo "<script>
				window.opener.location = '/emenda/emenda.php?modulo=principal/reformulacao&acao=A&ptridAnalise=$ptrid';
				window.close();
			</script>";
		return true;
	}

	if( $estadoAtual == EM_IDENTIFICACAO_PROCESSO_REFORMULACAO ){
			
		if( in_array(APOSTILAMENTO, $arRrefid) ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/execucaoPTA&acao=A&ptridAnalise=$ptrid';
					window.close();
					window.opener.close();
				 </script>";
		} else {
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/informacoesGerais&acao=A&ptridAnalise=$ptrid';
					window.close();
					window.opener.close();
				 </script>";
		}
		return true;
	}

	if( strstr($url, "emenda.php?modulo=principal/validacaoPTA&acao=A&popup=true") && !in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ){
		if( $estadoAtual == EM_IDENTIFICACAO_PROCESSO_REFORMULACAO ){				
			echo "<script>
					window.opener.opener.location = '/emenda/emenda.php?modulo=principal/informacoesGerais&acao=A&ptridAnalise=$ptrid';
					window.close();
					window.opener.close();
				 </script>";
			return true;
		}
		if( $estadoAtual == EM_PUBLICACAO_REFORMULACAO ){
			echo "<script>
					window.opener.opener.location = '/emenda/emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=$ptrid';
					window.close();
					window.opener.close();
				</script>";
			return true;
		}
	} elseif( strstr($url, "emenda.php?modulo=principal/validacaoPTA&acao=A") && !in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ) {
		if( $estadoAtual == EM_IDENTIFICACAO_PROCESSO_REFORMULACAO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/informacoesGerais&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
		if( $estadoAtual == EM_PUBLICACAO_REFORMULACAO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
	}

	//if( strstr($url,"emenda.php?modulo=principal/informacoesGerais&acao=A") ){
		if( $estadoAtual == EM_ANALISE_TECNICA_REFORMULACAO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
	//}
	//if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		if( $estadoAtual == EM_EMPENHO_REFORMULACAO || $estadoAtual == EM_EMPENHO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/execucaoPTA&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
		if( $estadoAtual == EM_GERACAO_TERMO_ADITIVO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/minuta&acao=A&ptridAnalise=$ptrid&tipo=T';
					window.close();
				</script>";
			return true;
		}
	//}
	//if( strstr($url,"emenda.php?modulo=principal/execucaoPTA&acao=A") ){
		if( $estadoAtual == EM_GERACAO_TERMO_ADITIVO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/minuta&acao=A&ptridAnalise=$ptrid&tipo=T';
					window.close();
				</script>";
			return true;
		}
	//}
	//if( strstr($url,"emenda.php?modulo=principal/minuta&acao=A") ){
		if( $estadoAtual == EM_PUBLICACAO_REFORMULACAO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
		if( $estadoAtual == EM_ASSINATURA_REFORMULACAO || $estadoAtual == EM_ASSINATURA_CONVENENTE ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/assinaturasPTA&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
	//}
	//if( strstr($url,"emenda.php?modulo=principal/assinaturasPTA&acao=A") ){
		if( $estadoAtual == EM_PUBLICACAO_REFORMULACAO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
	//}
	//if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A") ){
		if( $estadoAtual == EM_LIBERACAO_RECURSO_REFORMULACAO ){
			echo "<script>
					window.opener.location = '/emenda/emenda.php?modulo=principal/analisepta/pagamento/solicitar&acao=A&ptridAnalise=$ptrid';
					window.close();
				</script>";
			return true;
		}
	//}
	return true;
}

/**
 * Verifica se habilita a ação para o exercicio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAcaoAno2009($url, $ptrid){
	if( $_SESSION['exercicio'] == '2009' ){
		return false;
	}else{
		return true;
	}
}

/**
 * Verifica se habilita a ação para o exercicio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAcaoAno2010($url, $ptrid){
	if( $_SESSION['exercicio'] == '2010' ){
		return false;
	}else{
		if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A")){
			return validaEnvioAnaliseTecnica( $url, $ptrid);
		} else {
			return true;
		}
	}
}

/**
 * Enter description here...
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function validaEnvioDiligenciaDocumentos($url, $ptrid, $tipo = 'T'){
	global $db;
	/*if(!validaEnvioAcaoAno2010($url, $ptrid)){
		return false;
	}*/
	$sql = "SELECT ana.anaid FROM emenda.analise ana
			WHERE ana.ptrid = $ptrid AND ana.anatipo = '$tipo' 
            	and ana.anadataconclusao is not null 
            	and ana.anastatus = 'A'
            	and ana.anasituacaoparecer = 'E'
            	and ana.analote = (SELECT max(analote) 
						    				FROM emenda.analise 
						                    WHERE anatipo = '$tipo'
						                    	and anastatus = 'A' 
						                    	and ptrid = ana.ptrid)";

	$anaid = $db->pegaUm( $sql );
	if( empty($anaid) ){
		return false;
	} else {
		return true;
	}
}

/**
 * Valida envio para indeferimento do pta
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioPTAIndeferido($url, $ptrid) {
	global $db;
	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$sql = "select uniid from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T') and anasituacaoparecer = 'D'";
			
		$unidades = $db->pegaUm($sql);
		if( empty($unidades) ){
			return false;
		} else
		return true;
	}
	return validaEnvioAcaoAno2009($url, $ptrid);
}

/**
 * Valida envio para cancelamento rap
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioCancelamentoRap($url, $ptrid){
	validaEnvioAcaoAno2009($url, $ptrid);
	if( pegarEstadoAtual( $ptrid ) != RECURSO_LIBERADO ){
		return false;
	}
	return true;
}

/**
 * Valida envio para correção do PTA
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioCorrecaoPTA($url, $ptrid){
	global $db;


	$sql = "select distinct
				count(pti.ptrid)

			from emenda.planotrabalho pt
				inner join emenda.ptiniciativa pti on pti.ptrid = pt.ptrid
				inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
				inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid
				inner join emenda.ptemendadetalheentidade ped on ped.pedid = per.pedid
				inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
				inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
				inner join emenda.autor aut on aut.autid = vede.autid
				inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
				inner join emenda.execucaofinanceira ef on pti.ptrid = ef.ptrid and ped.pedid = ef.pedid and ef.exfstatus = 'A'
				inner join emenda.situacaoempenho se on ef.semid = se.semid
				inner join monitora.pi_planointerno pli on pli.plicod = ef.plicod
                inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
                inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid and ptr.ptres = ef.ptres
				inner join public.esfera esf on fup.esfcod = esf.esfcod
			where 
				pti.ptrid = ".$ptrid;

	$empenho = $db->pegaUm( $sql );

	if( $empenho > 0 ) return false;
	if( $_SESSION['exercicio'] == '2009' ) return true;

	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$sql = "SELECT anasituacaoparecer FROM emenda.analise a
				WHERE a.ptrid = $ptrid and a.anatipo = 'T' and a.anastatus = 'A'
					and a.analote = (SELECT max(analote) 
										FROM emenda.analise 
									WHERE anatipo = 'T'
										and anastatus = 'A' 
										and ptrid = a.ptrid)";

		$arSituacao = $db->carregarColuna( $sql );
		#verifica a situação do parecer E - Diligencia, D - Indeferido
		if( in_array( 'D', $arSituacao ) || in_array( 'E', $arSituacao ) ){
			return true;
		}
	}
	return false;
}

/**
 * Função executada após o fluxo ser encaminhado para empenho
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function encaminharEmpenho( $url, $ptrid ){
	$boPai = pegaPaiPTA( $ptrid );
	if( /*strstr( $url, 'emenda.php?modulo=principal/analiseTecnicaPTA&acao=A' ) &&*/ !empty($boPai) ){
		ativaPlanoTrabalhoFilho( $ptrid );
	}
	return validaFluxoWorkflowReformulacao( $url, $ptrid );
}

/**
 * Verifica se existe numento de empenho
 *
 * @param string $ptrid
 * @return boolean
 */
function verificaPtrNumEmpenho($ptrid = null){
	global $db;
	
	if(!$ptrid){
		return false;
	}
	
	$arPtrnumprocessoempenho = $db->carregarColuna("SELECT exfnumempenhooriginal FROM emenda.execucaofinanceira WHERE ptrid = {$ptrid} and exfstatus = 'A'");
	
	foreach ($arPtrnumprocessoempenho as $v) {
		if(!empty($v)) return true;
	}	
	return false;
}

/**
 * Verifica se pode retornar para analise tecnica
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoAnaliseTecnica($url, $ptrid){
	global $db;
	
	$estadoAtual = pegarEstadoAtual( $ptrid );
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
	
	if( in_array(APOSTILAMENTO, $arRrefid) ) return false; #Reformulação de Apostilamento
	
	if( $estadoAtual == EM_EMPENHO && $_SESSION['exercicio'] == '2011' ) return false;
	if( strstr($url,"emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A")){
		if( $_SESSION['exercicio'] == '2010' ) return false;
		return true;
	}
	validaEnvioPorAnoExercicio($url, $ptrid);
	if( strstr( $url, "emenda.php?modulo=principal/execucaoPTA&acao=A" ) ){
		if(verificaPtrNumEmpenho($ptrid)){
			return false;
		} else {
			return true;
		}
	}
}
/**
 * Valida retorno da ação para analise tecnica, esta funcionalidade so funcionará para o exercicio 2011 
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoAnaliseTecnica2011($url, $ptrid){
	global $db;
	
	$estadoAtual = pegarEstadoAtual( $ptrid );
	
	if( $_SESSION['exercicio'] != '2011' ) return false;
	return true;	
}

/**
 * Verifica a possibilidade de enviar para minuta convenio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioMinuta($url, $ptrid){
	global $db;

	$sql = "SELECT mdoid FROM emenda.ptminutaconvenio where ptrid = $ptrid and pmcstatus = 'A'";
	$pmcid = $db->pegaUm( $sql );
	if( empty($pmcid) ) return false;
	
	include_once APPRAIZ . 'emenda/classes/MinutaConvenio.class.inc';

	$obConvenio = new MinutaConvenio($ptrid);
	$ptminutaconvenio = $obConvenio->carregaMinutaConvenio( $ptrid );

	if( $ptminutaconvenio['pmcid'] ){
		$sql = "SELECT
					oc.obcid as codigo
				FROM 
					emenda.objetoconvenio oc
				    inner join emenda.objetominutaconvenio omc on omc.obcid = oc.obcid
				WHERE
					omc.pmcid = ". $ptminutaconvenio['pmcid']."
				    and oc.obcstatus = 'A'";
		$obc = $db->pegaUm($sql);
	}else{
		return false;
	}

	if( empty($obc[0]) ){ //valida se tem algum objeto
		return false;
	}

	if( empty($ptminutaconvenio["pmctexto"]) ){ //valida se tem a minuta
		return false;
	}

	return true;
}

/**
 * Valida ano de exercicio para o convenio cancelado
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaAnoExercicioConvCancelado($url, $ptrid){
	global $db;

	if($db->testa_superuser()){
		return true;
	}
	if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A")){

		$sql = 'select esdid from workflow.estadodocumento where tpdid = 8 AND esdid = 245';
		$btn = $db->pegaUm( $sql );

		if( $_SESSION['exercicio'] == '2009' && CONVENIO_CANCELADO == $btn ){
			return true;
		}
	}
	return false;
}

/**
 * Verifica se existe data de assinatura
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function verificaDataAssinatura( $url, $ptrid){
	global $db;

	if(!$ptrid) return false;

	if( strstr( $url, 'emenda.php?modulo=principal/assinaturasPTA&acao=A' ) ){
		$sql = "SELECT ptrid FROM emenda.ptvigencia
				WHERE ptrid = $ptrid and vigstatus = 'A' 
					and vigdatafim is not null 
					and vigdias is not null 
					and vigdatainicio is not null";

		if($data = $db->pegaUm($sql)){
			return true;
		}else{
			return false;
		}
	}
}

/**
 * Verifica a possibilidade de enviar para pre-convenio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioPreConvenio($url, $ptrid){
	global $db;

	if( strstr( $url, "emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A" ) ){
		validaEnvioAcaoAno2009($url, $ptrid);

		$sql = "SELECT DISTINCT
				exf.exfid
			FROM
				emenda.execucaofinanceira exf
                inner join emenda.ordembancaria orb
                	on orb.exfid = exf.exfid
			WHERE 
				exf.ptrid = $ptrid 
                and exf.exfstatus = 'A'
                and orb.orbnumsolicitacao is not null
                and orb.spgcodigo = '2'";

		$ordemBancaria = $db->pegaUm( $sql );
		if( !empty($ordemBancaria) ){
			return false;
		}
		return true;
	}

	$boPai = pegaPaiPTA( $ptrid );

	if( !empty($boPai) ){
		return false;
	}

	if( strstr( $url, "emenda.php?modulo=principal/minutaConvenioDOU&acao=A" ) ){
		$sql = "SELECT pubid FROM
				  emenda.ptpublicacao where pmcid = (SELECT pmcid FROM emenda.ptminutaconvenio where ptrid = $ptrid and pmcstatus = 'A')
				  and pubstatus = 'A' and pubdatapublicacao is not null and pubnumportaria is not null and pubnumportaria is not null";
		$pubid = $db->pegaUm( $sql );
		if( empty( $pubid ) ){
			return false;
		}
	}
	return true;
}

function validoEnvioPublicado( $ptrid ){
	global $db;
	
	$sql = "SELECT pubid FROM
			  emenda.ptpublicacao where pmcid = (SELECT pmcid FROM emenda.ptminutaconvenio where ptrid = $ptrid and pmcstatus = 'A')
			  and pubstatus = 'A' and pubdatapublicacao is not null and pubnumportaria is not null and pubnumportaria is not null";
	$pubid = $db->pegaUm( $sql );
	if( empty( $pubid ) ){
		return false;
	} else {
		return true;
	}
}

/**
 * Verifica se habilita a ação encaminhar para recurso liberado
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioRecursoLiberado($url, $ptrid){
	global $db;

	$sql = "SELECT DISTINCT
				exf.exfid
			FROM
				emenda.execucaofinanceira exf
                inner join emenda.ordembancaria orb
                	on orb.exfid = exf.exfid
			WHERE 
				exf.ptrid = $ptrid 
                and exf.exfstatus = 'A'
                and orb.orbnumsolicitacao is not null
                and orb.spgcodigo = '2'";

	$ordemBancaria = $db->pegaUm( $sql );

	if( empty($ordemBancaria) ){
		return false;
	}
	return true;
}

/**
 *  Verifica se habilita a ação Encaminhar para Solicitação de Reformulação Indeferida
 *
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioReformulacaoIndeferida( $ptrid ){
	global $db;
	
	$refsituacaoanalise = $db->pegaUm("SELECT refsituacaoanalise FROM emenda.ptminreformulacao where ptrid = $ptrid");
	
	#verifica a situação da analise da reformulação D - Indeferido
	if( $refsituacaoanalise == 'D' ) return true;
	
	 return false;
}

/**
 * Cria ações possiveis a situação anterior da solicitação da reformulação
 *
 * @param integer $ptrid
 * @return boolean
 */
function enviaReformulacaoIndeferida( $ptrid ){
	global $db;
	$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	
	$sql = "SELECT ed.esdid, ed.esddsc, ac.aeddscrealizada, ac.aedid, ac.* 
		    FROM workflow.historicodocumento hd
		    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
		    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
		    WHERE hd.docid = {$docid} 
		    order by hd.hstid desc";
    
    $historicoDocumento = $db->carregar($sql);

    $retorno = '';
    foreach($historicoDocumento as $chave => $documento){    	
        if($documento['esdid'] == EM_SOLICITACAO_REFORMULACAO){
            $esdiddestino = $historicoDocumento[$chave+1]['esdid'];
            $aedid = $historicoDocumento[$chave+1]['aedid'];
            if( $esdiddestino != EM_SOLICITACAO_REFORMULACAO ) break; 
        }
    }
    require_once(APPRAIZ . 'includes/workflow.php');
    
    $atual = wf_pegarEstadoAtual( $docid );
    
    $sql = "SELECT count(aedid) FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdidorigem = ".$atual['esdid']." and esdiddestino =".$esdiddestino;
    $boAcao = $db->pegaUm($sql);
    if( $boAcao == 0 ) {
    	$sql = "SELECT aedid, aeddscrealizar, aeddscrealizada FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdiddestino = $esdiddestino limit 1";
    	$arAcoes = $db->pegaLinha( $sql );
    	
	    $sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, 
	  					aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa) 
				VALUES ({$atual['esdid']}, $esdiddestino, '{$arAcoes['aeddscrealizar']}', '{$arAcoes['aeddscrealizada']}',
	  					'$aedcondicao', '', '$aedposacao', true, '', false) RETURNING aedid";
	    
	    $aedid = $db->pegaUm( $sql );	
    }
    $sql = "SELECT a.aedid, a.aeddscrealizar, ed.esdid, ed.esddsc,
				a.aedobs, a.aedicone, a.aedcodicaonegativa
			FROM workflow.acaoestadodoc a
				inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
			WHERE
				esdidorigem = {$atual['esdid']} and
				aedstatus = 'A' and ed.esdid = $esdiddestino";
    $arEstados = $db->pegaLinha( $sql );
    
    $sql_excluirPerfil = "DELETE FROM workflow.estadodocumentoperfil WHERE aedid=".$arEstados['aedid'];
	$db->executar($sql_excluirPerfil);
	
    $sql_perfil = '';
    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".SUPER_USUARIO." , {$arEstados['aedid']} ); ";
    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ADMINISTRADOR_INST." , {$arEstados['aedid']}); ";
    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ADMINISTRADOR_REFORMULACAO." , {$arEstados['aedid']}); ";
    $sql_perfil.= "INSERT INTO workflow.estadodocumentoperfil (pflcod, aedid) VALUES( ".ANALISTA_TECNICO." , {$arEstados['aedid']}); ";
	
    $db->executar($sql_perfil);
    return $db->commit();
}

/**
 * Verifica se habilita a ação Encaminhar para aceitação da reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAceitacaoReformulacao($url, $ptrid){
	global $db;
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

	if( in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ){
		$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE ptrid = $ptrid and refstatus = 'A' and refsituacaoreformulacao = 'C'";
		$refid = $db->pegaUm( $sql );

		if( !empty($refid) ) return true;
		else false;
	} else {
		if( in_array( ANALISTA_FNDE, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil ) ){
			$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE refstatus = 'A' and refsituacaoreformulacao = 'C' and refsituacaoanalise = 'F' and ptrid = ".$ptrid;
			$refid = $db->pegaUm( $sql );

			if( !empty($refid) ) return true;
			else false;
		}
	}
	return false;
}

/**
 * Verifica se foi anexado algum documento
 *
 * @param string $url
 * @param integer $ptrid
 * @param integer $refid
 * @return boolean
 */
function encaminharAceitacaoReformulacao($url, $ptrid, $refid){
	global $db;

	$sql = "select anx.anxid
			from emenda.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
	 		where ptrid = {$ptrid} and anx.anxtipo = 'R' and refid = $refid";

	$anxid = $db->pegaUm($sql);

	if( empty($anxid) ){
		echo '<script type="text/javascript">
		    		alert("Não é possivel encaminhar para aceitação da reformulação, pois é necessário anexar o(s) documento(s).!");
    	  	  </script>';
		return false;
	}
	return true;
}

/**
 * Verifica se habilita a ação Encaminhar para a reformulação do Processo
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioReformulacao( $url, $ptrid){
	global $db;
	
	$arperfil = pegaPerfilArray( $_SESSION['usucpf'] );
	$estadoAtual = pegarEstadoAtual( $ptrid );

	$boReformulacao = pegaPaiPTA( $ptrid );

	$boReformulacao = ( !empty($boReformulacao) ? true : false );

	if( in_array( INSTITUICAO_BENEFICIADA, $arperfil ) ){
		return false;
	}
	$sql = "SELECT vigid FROM emenda.ptvigencia WHERE ptrid = $ptrid and vigstatus = 'A' and vigtipo = 'P' and vigdatainicio is not null";
	$boAssinatura = $db->pegaUm($sql);
	if( empty($boAssinatura) && !$boReformulacao ){
		return "Favor cadastrar as datas necessárias na aba Assinaturas!";
	}

	if( $estadoAtual == EM_ACEITACAO_REFORMULACAO || $estadoAtual == EM_SOLICITACAO_REFORMULACAO ){
		$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE refstatus = 'A' and refsituacaoreformulacao = 'C' and refsituacaoanalise = 'F' and ptrid = ".$ptrid;
		$refid = $db->pegaUm( $sql );

		if(	!empty($refid) ) return true;
		else return false;
	}
	$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE ptrid = $ptrid and refstatus = 'A' and refsituacao = 'A'";
	$refid = $db->pegaUm( $sql );

	if( empty($refid) ){
		return false;
	} else {
		return true;
	}
}

/**
 * Cria historico do pta
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function encaminhaParaReformulacaoPTA($url, $ptrid){
	global $db;

	$boPai = pegaPaiPTA( $ptrid );

	$sql = "SELECT edeid
				FROM emenda.v_emendadetalheentidade
				WHERE entid = (select enbid from emenda.planotrabalho where ptrid = {$ptrid})
                	AND emetipo = 'X'";
	$arEdeid = $db->carregar( $sql );
	if( is_array($arEdeid) ){
		foreach ($arEdeid as $v) {
			$sql = "UPDATE emenda.emendadetalheentidade SET ededisponivelpta = 'S' WHERE edeid = ".$v['edeid'];
			$db->executar( $sql );
		}
		$db->commit();
	}

	$ptridFilho = insereFilhosPTA( $ptrid, $boPai );
	if( !empty($ptridFilho) && is_numeric( $ptridFilho ) ){
		$_SESSION["emenda"]["ptridAnalise"] = $ptridFilho;
		enviaEmailEntidadeReformulacao( $url, $ptrid );
		return validaFluxoWorkflowReformulacao( $url, $ptridFilho );
	} else {
		return false;
	}
}

/**
 * Enviar e-mail para a entidade responsavel pela reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function enviaEmailEntidadeReformulacao($url, $ptrid){
	global $db;

	$strMensagem = '';

	$arDados = $db->pegaLinha("SELECT DISTINCT p.enbid, p.ptrcod, to_char(pmr.refdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as refdataalteracao,
									pmr.refdscanalise, ur.pflcod, pmr.refsituacaoanalise
								FROM emenda.planotrabalho p
									inner join emenda.ptminreformulacao pmr on pmr.ptrid = p.ptrid 
									inner join emenda.usuarioresponsabilidade ur on ur.usucpf = pmr.usucpfinclusao
								WHERE p.ptrid = $ptrid
									and pmr.refstatus = 'A'
								    and pmr.refsituacaoreformulacao = 'C'");

	# verifica se a situação da analise for aprovado e se a solicitação da reformulação foi feita pela instituição beneficiada.
	# se o pflcod for igual a 274, isto é, a reformulação foi solicitada pela instituição beneficiada.
	if( $arDados['pflcod'] == INSTITUICAO_BENEFICIADA && $arDados['refsituacaoanalise'] == 'F' ){
		$strMensagem = "Seu plano de trabalho nº ".$arDados['ptrcod']."/{$_SESSION['exercicio']} foi analisado em ".$arDados['refdataalteracao']." e teve parecer<br>de técnico \"Favorável/Aprovado\". Seu plano de trabalho está disponível para ser reformulado.";
	} else if( $arDados['refsituacaoanalise'] == 'E' ){
		$strMensagem = "Seu plano de trabalho nº ".$arDados['ptrcod']."/{$_SESSION['exercicio']} foi analisado em ".$arDados['refdataalteracao'].". A análise técnica teve <br>posicionamento \"Em diligência\". Segue(m) parecer(es) para atendimento. O prazo para <br>atendimento é de 15 dias corridos, contados do recebimento deste email.<br>".$arDados['refdscanalise'];
	}

	if( !empty($strMensagem) ){
		$sql = "SELECT
					usu.usuemail
				FROM 
					emenda.usuarioresponsabilidade usr
	                inner join seguranca.usuario usu on usu.usucpf = usr.usucpf
				WHERE 
					usr.enbid = ".$arDados['enbid']."
					and usr.rpustatus = 'A'";
			
		$strEmailTo = $db->carregarColuna($sql);
		$strAssunto = SIGLA_SISTEMA. " - Emenda";

		return enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo);
	}
	return true;
}

/**
 * Habilita se o estado anterior da solicitação for liberação de recurso
 *
 * @param integer $ptrid
 * @return boolean
 */
function mostraEstadoAnteriorLiberacaoRecurso($ptrid){
	$retorno  = mostraEstadoAnterior( $ptrid );
	
	if( $retorno == EM_LIBERACAO_RECURSO) return true;
	
	return false;
}

/**
 * Habilita se o estado anterior da solicitação for analise tecnica
 *
 * @param integer $ptrid
 * @return boolean
 */
function mostraEstadoAnteriorAnaliseTecnica($ptrid){
	$retorno  = mostraEstadoAnterior( $ptrid );
	
	if( $retorno == EM_ANALISE_TECNICA) return true;
	
	return false;
}

/**
 * Habilita se o estado anterior da solicitação for recurso liberado
 *
 * @param integer $ptrid
 * @return boolean
 */
function mostraEstadoAnteriorRecursoLiberado($ptrid){
	$retorno  = mostraEstadoAnterior( $ptrid );
	
	if( $retorno == RECURSO_LIBERADO) return true;
	
	return false;
}

/**
 * Habilita se o estado anterior da solicitação for pre-convenio
 *
 * @param integer $ptrid
 * @return boolean
 */
function mostraEstadoAnteriorPreConvenio($ptrid){
	$retorno  = mostraEstadoAnterior( $ptrid );
	
	if( $retorno == EM_PRE_CONVENIO) return true;
	
	return false;
}

#Funções referente as Ações que está antes da solicitação da reformulação
function validaEnvioAnaliseTecnicaSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );	
	if( $retorno == EM_ANALISE_TECNICA) return true;
	else return mostraEstadoAnteriorAnaliseTecnica($ptrid);
}
function validaEnvioPreConvenioSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );	
	if( $retorno == EM_PRE_CONVENIO) return true;	
	else return mostraEstadoAnteriorPreConvenio($ptrid);
}
function validaEnvioLiberacaoRecursoSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );
	if( $retorno == EM_LIBERACAO_RECURSO) return true;	
	else return mostraEstadoAnteriorLiberacaoRecurso( $ptrid );
}
function validaEnvioRecursoLiberadoSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );
	if( $retorno == RECURSO_LIBERADO) return true;	
	else return mostraEstadoAnteriorRecursoLiberado( $ptrid );
}
#fim da funções

/**
 * Habilita se o estado anterior da solicitação for em geração de termo aditivo
 *
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioGeraTermoAdtivoSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );
	if( $retorno == EM_GERACAO_TERMO_ADITIVO) return true;	
	return false;
}

/**
 * Habilita se o estado anterior da solicitação for em termo aditivo publicado
 *
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioTermoAdtivoPublicadoSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );
	if( $retorno == EM_TERMO_ADITIVO_PUBLICADO) return true;	
	return false;
}

/**
 * Habilita se o estado anterior da solicitação for em aceitação da reformulação
 *
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAprovacaoReformulacaoSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );
	if( $retorno == EM_ACEITACAO_REFORMULACAO) return true;	
	return false;
}

/**
 * Habilita se o estado anterior da solicitação for em processo reformulado
 *
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioProcessoReformuladoSolicitacao($ptrid){
	$retorno  = verificaAcaoAnteriorSolicitacao('', $ptrid );
	if( $retorno == EM_PROCESSO_REFORMULADO) return true;	
	return false;
}

/**
 * Verifica ação anterior a solicitação de reformulação
 *
 * @param integer $ptrid
 * @return boolean
 */
function verificaAcaoAnteriorSolicitacao($url, $ptrid){
	global $db;
	
	$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	
	$sql = "SELECT ed.esdid, ed.esddsc, ac.aedid 
		    FROM workflow.historicodocumento hd
		    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
		    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
		    WHERE hd.docid = {$docid} 
		    order by hd.hstid desc";
    
    $historicoDocumento = $db->pegaLinha($sql);
    $esdiddestino = $historicoDocumento['esdid'];
    
    return $esdiddestino;
}

/**
 * Verifica se a reformulação e do tipo reformulação de oficio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioReformulacaoOficio($url = null, $ptrid = null ){
	global $db;

	#verifica o tipo de reformulação realizada, com isso muda o fluxo do workflow.
	$arTipos = pegaTipoReformulacao( $ptrid );

	if(!is_array($arTipos)){
		return false;
	}

	$refprorrogacaooficio = $db->pegaUm("select refprorrogacaooficio from emenda.ptminreformulacao where ptrid = $ptrid");
	
	$boDados = $db->pegaUm("select count(vigid) from emenda.ptvigencia where ptrid = $ptrid and vigdatafim is not null");

	if( !empty($refprorrogacaooficio) && $refprorrogacaooficio == 'S' && (int)$boDados > 0 ) return true;
	return false;
}

/**
 * Atualiza a situação do plano de trabalho
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function encaminhaPublicacaoReformulacao( $url, $ptrid ){
	global $db;

	$pta = $db->pegaLinha("SELECT ptridpai, refid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	$refid = $pta['refid'];
	$ptridpai = $pta['ptridpai'];
	
	if( empty($refid) ) $refid = $db->pegaUm("select refid from emenda.ptminreformulacao WHERE ptrid = $ptrid");

	insereTiposReformulacao( $ptrid, $refid );

	$sql = "UPDATE emenda.ptminreformulacao SET refsituacao = 'T', refsituacaoreformulacao = 'F', refprorrogacaooficio = 'S' WHERE refstatus ='A' and refid = $refid";
	$db->executar( $sql );

	if( $ptridpai ){
		$sql = "UPDATE emenda.planotrabalho SET ptrstatus = 'I', usucpfalteracao = '".$_SESSION['usucpf']."', ptrdataalteracao = now() WHERE ptrid = $ptridpai;";
		$sql .= "UPDATE emenda.planotrabalho SET ptrsituacao = 'A', usucpfalteracao = '".$_SESSION['usucpf']."', ptrdataalteracao = now() WHERE ptrid = $ptrid";
		$db->executar( $sql );
	}
	$db->commit();

	return validaFluxoWorkflowReformulacao($url, $ptrid );
}

/**
 * Valida envio para reformulação do PTA e verifica se é um reformulação de oficio
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioReformulacaoPTA( $url = null, $ptrid = null){
	global $db;
	$url = !$url ? $_SESSION['favurl'] : $url;
		#verifica o tipo de reformulação realizada, com isso muda o fluxo do workflow.
	$arTipos = pegaTipoReformulacao( $ptrid );
	
	$ptridPai = pegaPaiPTA($ptrid);
	
	$arrTipoRef = verificaTiposReformulacao( $ptrid, 'codigo' );
	
	if( in_array(RENDIMENTO_DE_APLICACAO, $arrTipoRef) ){
		$sql = "select
					count(p.perid)
				from 
					emenda.ptaespecificacaorendimento p 
				where 
					p.perstatus = 'A'
				    and p.ptiid in (select ptiid from emenda.ptiniciativa where ptrid = $ptrid)";
		$boEnviaTecnica = $db->pegaUm($sql);
		if( $boEnviaTecnica > 0 ) return true;
		else return false;
	}
	
	#Apostilamento
	if( in_array( APOSTILAMENTO, $arrTipoRef ) ){
		$sql = "select
					p.prddata,
				    p.prdtipo
				from 
					emenda.ptparceladesembolso p 
				where 
					ptrid = $ptridPai
					and prdminuta = 'P'
				order by p.prdtipo, p.prdid";
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$boEnvia = false;
		foreach ($arrDados as $v) {
			$sql = "select
					count(prdid)
				from 
					emenda.ptparceladesembolso p 
				where 
					ptrid = $ptrid
					and prddata = '{$v['prddata']}'
					and prdtipo = '{$v['prdtipo']}'
					and prdminuta = 'P'";
			
			$botem = $db->pegaUm($sql);
			if( $botem > 0 ){
				$boEnvia = true;
			}
		}
		
		if( $boEnvia == true ){
			return true;
		} else {
			return false;
		}
	}
	
	if(empty($arTipos[0]) ) return 'O plano de trabalho não foi reformulado.';

	$refprorrogacaooficio = $db->pegaUm("select refprorrogacaooficio from emenda.ptminreformulacao where ptrid = $ptrid");

	if( !empty($refprorrogacaooficio) && $refprorrogacaooficio == 'S' ) return 'Reformulação de Oficio.';
	
	if( strstr($url, "emenda.php?modulo=principal/reformulacaoVigenciaPTA&acao=A")){
		return true;
	}
	return validaEnvioPlanoTrabalho( $url, $ptrid);
}

function validaEnvioEmpenhoRefor( $ptrid ){
	global $db;

	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
	
	if( in_array(APOSTILAMENTO, $arRrefid) ){
		$sql = "select count(e.exfid) from emenda.execucaofinanceira e 
					inner join emenda.planotrabalho p on p.ptrid = e.ptrid
				where 
					e.ptrid = $ptrid 
				    and e.exfvalor is not null
				    and e.plicod is not null
				    and e.ptres is not null
				    and p.ptrnumprocessoempenho is not null
    				and p.ptrnumdocumenta is not null";
		$boExe = $db->pegaUm($sql);
			
		if( $boExe > 0 ){
			return true;
		}
	} else {	
		return false;
	}
}

function verificaTiposReformulacao( $ptrid, $coluna = '', $boAtual = true ){
	global $db;
	
	if( $boAtual ) $filtro = " and pr.refsituacaoreformulacao = 'C' ";
	$sql = "SELECT tr.trefid as codigo, tr.trefnome as descricao
				FROM emenda.ptminreformulacao pr
					inner join emenda.reformulatipos rt on rt.refid = pr.refid
				    inner join emenda.tiporeformulacao tr on tr.trefid = rt.trefid and tr.trefstatus = 'A'
				WHERE pr.ptrid = $ptrid
					 $filtro
				    and pr.refstatus = 'A'";
	
	if( $coluna ) $arRrefid = $db->carregarColuna($sql, $coluna);
	else $arRrefid = $db->carregar($sql);
	$arRrefid = $arRrefid ? $arRrefid : array();
	
	return $arRrefid;
}

/**
 * Verifica se habilita a ação Encaminhar para processo reformulado
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioProcessoReformulado($url, $ptrid){
	global $db;
	//if($db->testa_superuser()) return true;
	#verifica o tipo de reformulação realizada, com isso muda o fluxo do workflow.
	$arTipos = pegaTipoReformulacao( $ptrid );
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo', false );
	
	if( in_array(APOSTILAMENTO, $arRrefid) ) return true; #Reformulação de Apostilamento

	$arCodigo = array();
	foreach ($arTipos as $v) {
		array_push( $arCodigo, $v['codigo'] );
	}
	
	# Tipos de Reformulação:
	# 1 - Alteração das clausulas do convenio
	# 2 - Alteração de Valor
	# 3 - Prorrogação da vigência
	# 4 - Reformulação
	# 5 - Suplementação dos recursos
	# 8 - Prorrogração de Ofício
	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		if( ( in_array( 2, $arCodigo ) || in_array( 5, $arCodigo ) || in_array( 3, $arCodigo ) || in_array( 1, $arCodigo )  ) ){
			return false;
		}
		if( in_array( 4, $arCodigo ) || in_array(RENDIMENTO_DE_APLICACAO, $arRrefid) ){
			return validaEnvioAnalise( $url, $ptrid , $tipo);
		} else {
			return false;
		}
	}
	if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A") ){
		$sql = "SELECT pubid FROM emenda.ptpublicacao
  				WHERE pmcid = (select pmcid from emenda.ptminutaconvenio where ptrid = $ptrid and pmcstatus = 'A') and pubstatus = 'A' and pubdatapublicacao is not null";

		if( in_array( 2, $arCodigo ) ){
			return false;
		} else if( in_array( 3, $arCodigo ) || in_array( 1, $arCodigo ) || in_array( 8, $arCodigo ) ){
			$pubid = $db->pegaUm( $sql );
			if( empty($pubid) ) return false;
			else return true;
		}
		return false;
	}
	//if( strstr($url,"emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A") ){
		return validaEnvioRecursoLiberado($url, $ptrid);
	//}
	return false;
}

/**
 * Encaminhar para processo reformulado, atualiza a situação da reformulação para aprovada
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function encaminharProcessoReformulado( $url, $ptrid ){
	global $db;

	//if( strstr( $url, 'emenda.php?modulo=principal/analiseTecnicaPTA&acao=A' ) ){
		ativaPlanoTrabalhoFilho( $ptrid );
	//}

	/*if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A") || strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ||
	strstr($url,"emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A") ){*/
		$sql = "UPDATE emenda.ptminreformulacao SET refsituacaoreformulacao = 'F' WHERE ptrid = $ptrid and refstatus = 'A' and refsituacaoreformulacao = 'C'";
		$db->executar( $sql );
		$db->commit();
	//}
	return validaFluxoWorkflowReformulacao( $url, $ptrid );
}

/**
 * Verifica se habilita a ação Encaminhar para empenho da reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function validaEnvioEmpenhoReformulacao( $url, $ptrid, $tipo = ''){
	global $db;	
	
	//if($db->testa_superuser()) return true;

	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		#verifica o tipo de reformulação realizada, com isso muda o fluxo do workflow.
		$arTipos = pegaTipoReformulacao( $ptrid );
		
		$arCodigo = array();
		foreach ($arTipos as $v) {
			array_push( $arCodigo, $v['codigo'] );
		}
		# Tipos de Reformulação:
		# 1 - Alteração das clausulas do convenio
		# 2 - Alteração de Valor
		# 3 - Prorrogação da vigência
		# 5 - Suplementação dos recursos
		# 9 - Apostilamento
		if( in_array( 2, $arCodigo ) || in_array( 5, $arCodigo ) || in_array( 9, $arCodigo ) ){
			return validaEnvioAnalise( $url, $ptrid , $tipo);
		} else if( in_array( 1, $arCodigo ) || in_array( 3, $arCodigo ) ){
			return false;
		} else {
			return false;
		}
	}
}

/**
 * Bloqueia analise de merito para o exercicio 2011
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function bloqueiaAnaliseMerito( $url, $ptrid, $tipo ){
	#desabilita essa ação para ano de exercicio igual a 2011
	if( $_SESSION['exercicio'] == '2011' ) return false;
	validaEnvioAnalise( $url, $ptrid, $tipo);
}

/**
 * Verifica se habilita a ação Encaminhar para correção da reformulação do processo
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioCorrecaoReformulacao($url, $ptrid){
	global $db;

	if( strstr($url,"emenda.php?modulo=principal/analiseTecnicaPTA&acao=A") ){
		$sql = "SELECT anasituacaoparecer FROM emenda.analise a
				WHERE a.ptrid = $ptrid and a.anatipo = 'T' and a.anastatus = 'A'
					and a.analote = (SELECT max(analote) 
										FROM emenda.analise 
									WHERE anatipo = 'T'
										and anastatus = 'A' 
										and ptrid = a.ptrid)";
		
		#verifica a situação do parecer E - Diligencia
		$situacao = $db->pegaUm( $sql );
		if( $situacao == 'E' ){
			return true;
		}
	}
	return false;
}

/**
 * Verifica se habilita a ação Encaminhar para a geração do termo aditivo
 *
 * @param string $url
 * @param integer $ptrid
 * @param string $tipo
 * @return boolean
 */
function validaEnvioTermoAditivo( $url, $ptrid, $tipo = ''){
	global $db;
	
	//if($db->testa_superuser()) return true;

	if( strstr($url,"emenda.php?modulo=principal/execucaoPTA&acao=A") ){
		$sql = "SELECT exfid FROM emenda.execucaofinanceira WHERE semid <> 4 AND ptrid = $ptrid";
		$EmpenhoEfetivado = $db->pegaUm( $sql );
		
		if( empty($EmpenhoEfetivado) ){
			return true;
		} else {
			return false;
		}
	} else {
		#verifica o tipo de reformulação realizada, com isso muda o fluxo do workflow.
		$arTipos = pegaTipoReformulacao( $ptrid );

		$arCodigo = array();
		foreach ($arTipos as $v) {
			array_push( $arCodigo, $v['codigo'] );
		}
		# Tipos de Reformulação:
		# 1 - Alteração das clausulas do convenio
		# 2 - Alteração de Valor
		# 3 - Prorrogação da vigência
		# 5 - Suplementação dos recursos
		# 9 - Apostilamento
		if( in_array( 2, $arCodigo ) || in_array( 5, $arCodigo ) || in_array( 9, $arCodigo ) ){
			return false;
		} else if( in_array( 1, $arCodigo ) || in_array( 3, $arCodigo ) ){
			return validaEnvioAnalise( $url, $ptrid , $tipo);
		} else {
			return false;
		}
	}
}

/**
 * Encaminhar para termo aditivo
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function encaminharTermoAditivo( $url, $ptrid ){
	if( strstr( $url, 'emenda.php?modulo=principal/analiseTecnicaPTA&acao=A' ) ){
		ativaPlanoTrabalhoFilho( $ptrid );
	}
	return validaFluxoWorkflowReformulacao( $url, $ptrid );
}

/**
 * Verifica se habilita a ação Encaminhar para assinatura do convenente
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAssinaturaConvenente($url, $ptrid){
	global $db;
	if($db->testa_superuser()) return true;
	$boPai = pegaPaiPTA( $ptrid );

	if( strstr($url,"emenda.php?modulo=principal/minuta&acao=A") && empty($boPai) ){
		$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE ptrid = $ptrid and refstatus = 'A' and mdoid IS NOT NULL";
		$refid = $db->pegaUm( $sql );

		if( empty( $refid ) ) return false;
		else return true;
	} else {
		return false;
	}
}

/**
 * Verifica se habilita a ação Encaminhar para análise jurídica da reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAnaliseJuridica($url, $ptrid){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo', false );	
	if( in_array(APOSTILAMENTO, $arRrefid) ) return false; #Reformulação de Apostilamento
	
	if($db->testa_superuser()) return true;
	
	if( strstr($url,"emenda.php?modulo=principal/minuta&acao=A") ){
		$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE ptrid = $ptrid and refstatus = 'A' and mdoid IS NOT NULL";
		$refid = $db->pegaUm( $sql );

		if( empty( $refid ) ) return false;
		else return true;
	}
	return false;
}

function validaEnvioAssinaturaReformulacao($url, $ptrid){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo', false );	
	if( in_array(APOSTILAMENTO, $arRrefid) ) return false; #Reformulação de Apostilamento

	return true;
}

function validaEnvioPagamentoRefor($ptrid){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );	
	if( in_array(APOSTILAMENTO, $arRrefid) ) {
		return true; #Reformulação de Apostilamento
	} else {	
		return false;
	}
}

function validaRetornoDouRefor($ptrid){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );	
	if( in_array(APOSTILAMENTO, $arRrefid) ) {
		return false; #Reformulação de Apostilamento
	} else {	
		return true;
	}
}

function validaRetornoAnaliseJuridicaRefor($ptrid){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
	if( in_array(APOSTILAMENTO, $arRrefid) ){
		return false; #Reformulação de Apostilamento
	} else {
		return true;
	}
}

function validaEnvioPublicacaoRefor($ptrid){
	global $db;
	
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );
	if( in_array(APOSTILAMENTO, $arRrefid) ){
		return false; #Reformulação de Apostilamento
	} else {
		return true;
	}
	
}

/**
 * Verifica se habilita a ação Encaminhar para liberação de recurso da reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioLiberacaoRecursoReformulacao( $url, $ptrid ){
	global $db;

	if($db->testa_superuser()) return true;

	#verifica o tipo de reformulação realizada, com isso muda o fluxo do workflow.
	$arTipos = pegaTipoReformulacao( $ptrid );

	$arCodigo = array();
	foreach ($arTipos as $v) {
		array_push( $arCodigo, $v['codigo'] );
	}

	if( strstr($url,"emenda.php?modulo=principal/minutaConvenioDOU&acao=A") ){
		if( !in_array( 1, $arCodigo ) || !in_array( 3, $arCodigo ) ){
			$sql = "SELECT pubid FROM emenda.ptpublicacao
  				WHERE pmcid = (select pmcid from emenda.ptminutaconvenio where ptrid = $ptrid and pmcstatus = 'A') 
  					and pubstatus = 'A' and pubdatapublicacao is not null";

			$pubid = $db->pegaUm( $sql );
			if( empty($pubid) ) return false;
			else return true;
		} else {
			return false;
		}
	}
	return true;
}


/**
 * Verifica se habilita a ação Encaminhar para publicação da reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioPublicacaoDOU( $url, $ptrid ){
	$boPai = pegaPaiPTA( $ptrid );

	if( !empty($boPai) && strstr( $url, 'emenda.php?modulo=principal/minuta&acao=A' ) ){
		return false;
	} elseif( !strstr( $url, 'emenda.php?modulo=principal/assinaturasPTA&acao=A' ) ){
		return false;
	} else {
		return true;
	}
}


/**
 * Verifica se habilita a ação Retornar para em solicitação da reformulação
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoSolicitacaoReformulacao($url, $ptrid){
	global $db;

	$sql = "SELECT refid FROM emenda.ptminreformulacao WHERE refstatus = 'A' and refsituacaoreformulacao = 'C' and refsituacaoanalise = 'E' and ptrid = ".$ptrid;
	$refid = $db->pegaUm( $sql );

	if(	!empty($refid) )
	return true;
	else
	return false;
}

/**
 * Valida envio para analise tecnica em 2011
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioAnaliseTecnica2011($url, $ptrid){
	global $db;
	if( $_SESSION['exercicio'] >= '2012') return false;
	
	if( $_SESSION['exercicio'] != '2011' || !strstr( $url, 'emenda.php?modulo=principal/analiseDadosPTA&acao=A' ) ) return false;
	
	$sql = "SELECT anaid FROM emenda.analise where ptrid = $ptrid 
			AND analote = (SELECT max(analote) FROM emenda.analise WHERE anatipo = 'T' and ptrid = $ptrid)";
	$anaid = $db->carregar($sql);
	if( !empty($anaid) ) return true;
	return false;
}

/**
 * valida envio para informações gerais
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaEnvioInfoGerais($url, $ptrid){
	global $db;
	
	if( $_SESSION['exercicio'] != '2011' ) return false;
	
	$sql = "SELECT anaid FROM emenda.analise 
			WHERE ptrid = $ptrid 
			AND anadataconclusao IS NOT NULL 
			AND anastatus = 'A'
			AND analote = (SELECT max(analote) FROM emenda.analise WHERE anatipo = 'T' and anastatus = 'A' and ptrid = $ptrid)";
	$anaid = $db->carregar($sql);
	
	if( !empty($anaid) ) return true;
	return false;
}

/**
 * valida retorno para informações gerais
 *
 * @param string $url
 * @param integer $ptrid
 * @return boolean
 */
function validaRetornoInforGerais2011($url, $ptrid){
	global $db;
	
	if( $_SESSION['exercicio'] != '2011' ) return false;
	return true;
}

function validaEnvioPrestacaoContas( $ptrid ){
	$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );	
	if( in_array(APOSTILAMENTO, $arRrefid) ){
		return false; #Reformulação de Apostilamento
	} else {
		return true;
	}
}

function validaEnvioArquivado($ptrid){
	global $db;
	
	$sql = "SELECT
				ve.emdliberado, ef.semid
			FROM
				emenda.ptemendadetalheentidade pte
			    inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid
			    left join emenda.execucaofinanceira ef on ef.ptrid = pte.ptrid and pte.pedid = ef.pedid
			WHERE
				pte.ptrid = $ptrid
			    and ve.edestatus = 'A'";
	$liberado = $db->pegaLinha( $sql );
	
	if( $liberado['emdliberado'] == 'N' && empty($liberado['semid']) ){
		return true;
	} else {
		return false;
	}
	
}

function insereParecerAnaliseJuridicaTecnica( $ptrid ){
	global $db;
		
	$sql = "SELECT max(analote) FROM emenda.analise WHERE ptrid = {$ptrid} AND anatipo = 'T'";
	$analote = $db->pegaUm( $sql );
	
	$arrAnalise = $db->pegaLinha("SELECT ana.uniid, usu.usuemail FROM emenda.analise ana
									inner join seguranca.usuario usu on usu.usucpf = ana.usucpf 
								WHERE ptrid = {$ptrid} AND anatipo = 'T' and analote = $analote");
	
	$analote = !empty($analote) ? $analote + 1 : 1;
	$sql = "INSERT INTO emenda.analise( ptrid, anatipo, uniid, analote, anadatainclusao )
			VALUES ({$ptrid}, 'T', {$arrAnalise['uniid']}, {$analote}, 'now')";
	
	$db->executar( $sql );
	return $db->commit();
}

function validaEnvioTipoReformulacoes( $url, $ptrid ){
	$boRef = new WorkflowReformulacao($url, $ptrid);
}

function validaEnvioOrcamentoImpositivo( $ptrid ){
	global $db;
	include_once APPRAIZ.'/emenda/classes/Emenda.class.inc';
	
	$obEmenda = new Emenda();
	$federal = $obEmenda->buscaEmendaDescentraliza( $ptrid );
	
	if( $federal == 'N' ){
		return true;
	} else {
		return false;
	}
}

function enviaOrcamentoImpositivo($ptrid){
	global $db;
	
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/orcamentoImpositivo&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	
	return true;
}

function validaEnvioAnaliseImpositivo($ptrid){
	global $db;
	
	include_once APPRAIZ . 'emenda/classes/ValidacaoPlanoTrabalho.class.inc';
	include_once APPRAIZ . 'emenda/classes/Emenda.class.inc';
	
	//$boLiberarEmenda = true;
	//$dataFimLibera = $_SESSION['exercicio']."-03-22";
	//$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);		
	//if( !$boLiberarEmenda ) return 'Impositivo';
	
	$obj = new ValidacaoPlanoTrabalho($ptrid);
	$obj->validacaoPlanoTrabalho();
	
	$obEmenda = new Emenda();
	$federal = $obEmenda->buscaEmendaDescentraliza( $ptrid );
	
	$impositiva = $obEmenda->buscaEmendaDescentraliza( $ptrid );
		
	$estadoAtual = pegarEstadoAtual( $ptrid );
	
	//if($estadoAtual == EM_ELABORACAO_IMPOSITIVO){
		if( date('Y-m-d') >= date('Y').'04-01' ){
			return 'Prazo de envio encerrado de acordo com a Portaria Interministerial n° 40/2014';
		}
	///}

	if(!$obj){
		return false;
	}elseif(!empty($obj->retorno["recursospta"])) {
		return 'Pendências Recursos';
	}elseif(!empty($obj->retorno["iniciativas"])) {
		return 'É necessário informar os dados do Benefiário';
	}elseif(!empty($obj->retorno["cronograma"])) {
		return 'Pendências Cronograma de Execução e Desembolso';
	}elseif(!empty($obj->retorno["escolasbeneficiadas"]) && $federal == 'N' ) {
		return 'Pendências Cadastro de Escolas Beneficiadas';
	}if(!empty($obj->retorno["dadosadicionais"])) {
		return 'Pendências em Dados Adicionais';
	}elseif(!empty($obj->retorno["anexo"]) && $federal == 'S'  ) {
		return false;
	}elseif(!empty($obj->retorno["entidade"]) && $federal == 'S' ) {
		return false;
	}elseif($estadoAtual != EM_ELABORACAO_IMPOSITIVO ){
		return false;
	}else{
		return true;
	}
}

function validaEnvioAnaliseTecnicaImpositivo($ptrid){
	global $db;
	
	$boLiberarEmenda = true;
	/*$dataFimLibera = ($_SESSION['exercicio'])."-04-19";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);*/		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {
		
		$existeExecucao = $db->pegaUm("select count(exfid) from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A'");
		
		if( $existeExecucao < 1 ) return 'Informe os dados da Programação Orçamentária.';
		
		$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";			  
		$arDados = $db->pegaLinha($sql);
	
		if( !$arDados['ptrnumprocessoempenho'] ) return 'Nº do Processo Empenho não pode ser vazio.';
		if( !$arDados['ptrnumdocumenta'] ) return 'Nº do Documento não pode ser vazio.';
	
		$sql = "select count(uniid) from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T') ";
			
		$unidades = $db->pegaUm($sql);
		if( $unidades < 1 ) return 'Informe quais unidades que realizarão a Análise Técnica do PTA.';
		
		return true;
	}
}

function validaEnvioAnaliseMeritoTecnicaImpositivo($ptrid){
	global $db;
	
	$boLiberarEmenda = true;
	/*$dataFimLibera = ($_SESSION['exercicio'])."-04-19";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);*/		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {
		
		$existeAnalise = $db->pegaUm("select count(anaid) from emenda.analise WHERE ptrid = $ptrid and anatipo = 'M' and anadataconclusao is not null");
		
		if( $existeAnalise < 1 ) return 'Informe o parecer da Análise Mérito.';
		
		return true;
	}
}

function validaEnvioUnidadeGestoraImpositivo($ptrid){
	global $db;
	
	/*$boLiberarEmenda = true;
	$dataFimLibera = ($_SESSION['exercicio'])."-04-19";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {*/
		$existeExecucao = 1;
		//$existeExecucao = $db->pegaUm("select count(exfid) from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A'");
		
		if( $existeExecucao < 1 ) return 'Informe os dados da Programação Orçamentária.';
		
		$sql = "SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = $ptrid";			  
		$arDados = $db->pegaLinha($sql);
	
		if( !$arDados['ptrnumprocessoempenho'] ) return 'Nº do Processo Empenho não pode ser vazio.';
		if( !$arDados['ptrnumdocumenta'] ) return 'Nº do Documento não pode ser vazio.';
	
		$sql = "select count(uniid) from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T') ";
			
		$unidades = $db->pegaUm($sql);
		if( $unidades < 1 ) return 'Informe quais unidades que realizarão a Análise Técnica do PTA.';
		
		return true;
	//}
}

function validaEnvioAnaliseMeritoImpositivo($ptrid){
	global $db;
	
	/*$boLiberarEmenda = true;
	$dataFimLibera = ($_SESSION['exercicio'])."-04-19";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {*/
		
		$existeAnalise = $db->pegaUm("select count(anaid) from emenda.analise WHERE ptrid = $ptrid and anatipo = 'M' and anadataconclusao is null");
		
		if( $existeAnalise < 1 ) return 'Informe quais unidades que realizarão a Análise Mérito.';
		
		return true;
	//}
}

function validaEnvioDiligenciaImpositivo($ptrid){
	global $db;
	
	/*$boLiberarEmenda = true;
	$dataFimLibera = ($_SESSION['exercicio'])."-04-18";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {*/
		
		$sql = "select anasituacaoparecer from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')
					and anadataconclusao is not null";
			
		$arDados = $db->carregarColuna($sql);
		$arDados = ( $arDados ? $arDados : array() );
		
		#verifica a situação do parecer E - Diligencia
		if( empty($arDados) ){
			return 'Informe o parecer da Análise Técnica.';
		} else {
			if( in_array( 'E', $arDados ) ){
				return true;
			} 
			return false;
		}
	//}
}

function validaEnvioSiconvImpositivo($ptrid){
	global $db;
	
	/*$boLiberarEmenda = true;
	$dataFimLibera = ($_SESSION['exercicio'])."-04-18";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {*/
		
		$sql = "select anasituacaoparecer from emenda.analise
				where ptrid = $ptrid and anatipo = 'T'
					and anastatus = 'A' and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')
					and anadataconclusao is not null";
			
		$arDados = $db->carregarColuna($sql);
		$arDados = ( $arDados ? $arDados : array() );
		
		#verifica a situação do parecer E - Diligencia
		//if( in_array( 'E', $arDados ) || empty($arDados) ){
		if( empty($arDados) ){
			return 'Informe o parecer da Análise Técnica.';
		} else {
			if( in_array( 'F', $arDados ) ){
				return true;
			} 
			return false;
		}
	//}
}

function validaEnvioEmpenhoImpositivo($ptrid){
	global $db;
	
	/*$boLiberarEmenda = true;
	$dataFimLibera = ($_SESSION['exercicio']+1)."-04-24";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);		
	if( !$boLiberarEmenda ){
		return 'Impositivo';
	} else {*/
		$ptrpropostasiconv = $db->pegaUm("select ptrpropostasiconv from emenda.planotrabalho where ptrid = $ptrid");
		if( empty($ptrpropostasiconv) ){
			return 'Proposta SICONV não enviada';
		}
		return true;
		
	//}
}

function fluxoWorkflowUnidadeGestora($ptrid){
	insereAnaliseEmenda( $ptrid );
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/analiseDadosPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowAnaliseTecnica($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowAnaliseMerito($ptrid){
	alteraStatusAnaliseMerito('', $ptrid );
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/analiseMeritoPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowDiligencia($ptrid){
	global $db;
	enviaEmailResponsavelEntidade($url, $ptrid, $tipo);
	
	$db->executar("UPDATE emenda.planotrabalho SET ptrdatadiligencia = now() WHERE ptrid = $ptrid");
	$db->commit();
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowSiconv($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/propostaSiconv&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowPublicado($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowEmpenho($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/execucaoPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowMinutaConvenio($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/minuta&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowAssinatura($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/assinaturasPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowCriarConvenioPTA($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/criarConvenioPTA&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function fluxoWorkflowPagamento($ptrid){
	echo "<script>
			window.opener.location = '/emenda/emenda.php?modulo=principal/analisepta/pagamento/solicitar&acao=A&ptridAnalise=$ptrid';
			window.close();
		</script>";
	return true;
}

function envioElaboracaoAnalise($ptrid){
	global $db;
	
	$sql = "select ed.esdid
			from workflow.historicodocumento hd
				inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
				inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
			where
				hd.docid = (select docid from emenda.planotrabalho where ptrid = $ptrid)
			order by
	        	hd.htddata desc limit 1";
	$esdid = $db->pegaUm($sql);
	
	if( $esdid == EM_ANALISE_TECNICA_IMPOSITIVO || $esdid == EM_ANALISE_TECNICA || $esdid == EM_ANALISE_TECNICA_REFORMULACAO ){
		return true;
	} else {
		return false;
	}
}

function encaminharAnalisePublicado($ptrid){
	global $db;
	
	$sql = "select ed.esdid
			from workflow.historicodocumento hd
				inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
				inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
			where
				hd.docid = (select docid from emenda.planotrabalho where ptrid = $ptrid)
			order by
	        	hd.htddata desc limit 1";
	$esdid = $db->pegaUm($sql);
	
	if( $esdid == EM_PUBLICADO ){
		return true;
	} else {
		return false;
	}
}
?>