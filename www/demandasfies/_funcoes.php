<?php
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaEntrega.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaPartesAcao.inc.php";

function alertlocation($dados)
{
	die("<script>
			" . (($dados['alert']) ? "alert('" . $dados['alert'] . "');" : "") . "
			" . (($dados['location']) ? "window.location='" . $dados['location'] . "';" : "") . "
			" . (($dados['javascript']) ? $dados['javascript'] : "") . "
		 </script>");
}

function pegarDocidDemanda($dmdid)
{
	global $db;
	$sql = "select docid from demandasfies.demanda where dmdid = {$dmdid}";
	$docid = $db->pegaUm($sql);
	if (!$docid) {
		$docid = wf_cadastrarDocumento(WF_TPDID_DEMANDASFIES_DEMANDA, "Demanda FIES {$dmdid}");

		$db->executar("UPDATE demandasfies.demanda SET docid = $docid where dmdid = {$dmdid}");
		$db->commit();
	}

	return $docid;
}

function pegarDocidEntrega($dmeid)
{
	global $db;
	$sql = "select docid from demandasfies.demandaentrega where dmeid = {$dmeid}";
	$docid = $db->pegaUm($sql);
	if (!$docid) {
		$docid = wf_cadastrarDocumento(WF_TPDID_DEMANDASFIES_ENTREGA, "Entrega Demanda FIES {$dmeid}");

		$db->executar("UPDATE demandasfies.demandaentrega SET docid = $docid where dmeid = {$dmeid}");
		$db->commit();
	}

	return $docid;
}

function wf_desenhaBarraNavegacao_demandasFIES($dmdid, $docid, $dados = array(), $esdid = null)
{
	global $db;
	wf_desenhaBarraNavegacao($docid, $dados);
}

function enviarEmailTramitacao($dmdid, $esdid = null)
{

	$modelDemanda = new Demanda($dmdid);
	$acjDesc = $modelDemanda->getTipoAcaoDescricao($modelDemanda->acjid);

	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
	if (empty($esdid)) {
		$esdid = $_REQUEST['esdid'] ? $_REQUEST['esdid'] : 0;
	}
	if (!empty($esdid)) {
		if ($_REQUEST['action'] == 'salvar-intervencao') {
			$esdid = ESD_DEMANDA_EM_INTERVENCAO;
		}

		$sql = "select esddsc from workflow.estadodocumento where esdid = {$esdid}";
		$esddsc = $modelDemanda->pegaUm($sql);

		$esddsc = $esddsc ? $esddsc : 'Retornado à área demandante';
		$paransEmail['remetente'] = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
		$paransEmail['assunto'] = "[Demandas FIES] Tramitação de Demanda (DE: '{$estadoAtual['esddsc']}' PARA: '{$esddsc}')";

		$paransEmail['mensagem'] = "<pre>Prezados,
A demanda <span style='color: red;'>{$modelDemanda->dmdid} </span> foi tramitada.

<b>Tipo de Ação:</b> {$acjDesc}

Passou do estado '<b style='color: red;'>{$estadoAtual['esddsc']}</b>' para : '<b style='color: #008000;'>{$esddsc}</b>'

Para maiores detalhes, favor entrar no SIMEC, módulo Demandas FIES em http://simec.mec.gov.br.

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";
		enviarEmailDemandasFies($esdid, $dmdid, $paransEmail);
	}
	return true;
}

function enviarEmailDemandasFies($esdid, $dmdid, array $paransEmail, array $destinatarios = array())
{
	global $db;
	$aedid = $_REQUEST['aedid'];
	$esdidorigem = null;

	if(!empty($aedid)){
		$sql = "select esdidorigem from workflow.acaoestadodoc where aedid = {$aedid}";
		$dados = $db->carregar($sql);
		if($dados){
			$esdidorigem = $dados[0]['esdidorigem'];
		}
	}
	$destinatarios_ = getDestinatioPorPerfil($esdid, $dmdid, $esdidorigem);

	$destinatarios_ = removeEmailLista($destinatarios_, array('antonio.neto@fnde.gov.br'));

	if ($destinatarios_) {
		$destinatarios = array_merge($destinatarios, $destinatarios_);
	}

	enviar_email($paransEmail['remetente'], $destinatarios, $paransEmail['assunto'], $paransEmail['mensagem']);
	return true;
}

function getDestinatioPorPerfil($esdid, $dmdid, $esdidorigem = null)
{
	$emails2 = array();
	switch ($esdid) {
		case ESD_DEMANDA_EM_CADASTRAMENTO:
		case ESD_DEMANDA_PROFE:
			$idPerfil = PFL_PROCURADOR_FEDERAL;
			break;
		case ESD_DEMANDA_NUCLEO_JURIDICO:
			$idPerfil = PFL_NUCLEO_JURIDICO;
			break;
		case ESD_DEMANDA_DTI_MEC:
			$idPerfil = PFL_DTI_MEC;
			break;
		case ESD_DEMANDA_4_NIVEL:
			$idPerfil = PFL_4_NIVEL;
			break;
		case ESD_DEMANDA_GESTOR_FIES:
			$idPerfil = PFL_GESTOR_FIES;
			break;
		case ESD_DEMANDA_4_NIVEL:
			$idPerfil = PFL_4_NIVEL;
			break;
		case ESD_GERENCIA_DIGEF:
			$idPerfil = PFL_GERENCIA_DIGEF;
			break;
//		case ESD_DEMANDA_CGSUP:
//			$idPerfil = PFL_CGSUP;
//			break;
//		case ESD_DEMANDA_ADVOGADO:
//			$idPerfil = PFL_ADVOGADO;
//			break;
//		case ESD_DEMANDA_ANALISTA_DTI_MEC:
//		case ESD_DEMANDA_DTI_MEC:
//			$idPerfil = PFL_DTI_MEC;
//			break;
	}

	if ($idPerfil) {
		$emails = getEmailsPorIdPerfil($idPerfil);
	} else {
		$emails = getEmailsPorIdDemanda($esdid, $dmdid);
	}


	if (
			(in_array( $esdidorigem , array(ESD_DEMANDA_GESTOR_FIES, ESD_DEMANDA_ADVOGADO, ESD_DEMANDA_4_NIVEL )) && in_array( $esdid , array(ESD_DEMANDA_DTI_MEC,
					ESD_DEMANDA_ANALISTA_DTI_MEC ) ) ) ||
			($esdidorigem == ESD_DEMANDA_DTI_MEC  && $esdid == ESD_DEMANDA_ANALISTA_DTI_MEC) ||
			( $esdidorigem == ESD_DEMANDA_ANALISTA_DTI_MEC  && in_array( $esdid , array(ESD_DEMANDA_ADVOGADO, ESD_DEMANDA_NUCLEO_JURIDICO, ESD_DEMANDA_4_NIVEL )))
	) {
		$emails_ = getEmailsPorIdPerfil(PFL_DTI_MEC);

		if( in_array( $esdid , array(ESD_DEMANDA_ANALISTA_DTI_MEC) ) ){
			$emails2 = getEmailsPorIdDemanda(ESD_DEMANDA_EXECUCAO_DTI_MEC, $dmdid);
		}
		$emails = array_unique(array_merge($emails, $emails_, $emails2), SORT_REGULAR);
	}
	$emails = array_unique($emails);
	return $emails;
}

function alinhaParaCentro($valor)
{
	$valor = " < p style = \"text-align: center !important;\">$valor</p>";
	return $valor;
}

function tratarDemandaJudicialLista($valor)
{
	$destaques = array('Mandado de Segurança', 'Ação Civil Pública (ACP)');
	if (in_array($valor, $destaques)) {
		return "<span style=\"color: red !important;\">$valor</span>";
	}
	return $valor;
}

function tratarCriticidadeLista($valor)
{
	switch ($valor) {
		case ('Baixa'):
			return "<span style=\"color: #008000 !important;\">$valor</span>";
		case ('Média'):
			return "<span style=\"color: darkorange !important;\">$valor</span>";
		case ('Alta'):
			return "<span style=\"color: red !important;\">$valor</span>";
	}

	return $valor;
}

function wf_verificar_bloqueio_tramitacao($dmdid, $esdidorigem = '', $esdiddestino = '')
{
	global $db;

	if($esdidorigem == ESD_DEMANDA_EM_CADASTRAMENTO){
		$demandaPartesAcao = new DemandaPartesAcao();
		if( $demandaPartesAcao->possuiAutorReuArquivo($dmdid) == false){
			return 'Não é possível tramitar esta demanda antes de vincular Autor, Réu e Arquivo';
		}
	}

	if (in_array($esdiddestino, array(ESD_DEMANDA_FINALIZADA, ESD_DEMANDA_PROFE))) {
		return true;
	}

	$perfis = pegaPerfilGeral();
	$perfis = $perfis ? $perfis : array();
	$valp = (in_array(PFL_SUPER_USUARIO, $perfis) OR in_array(PFL_PROCURADOR_FEDERAL, $perfis));

	if ($esdidorigem == ESD_DEMANDA_FINALIZADA && $valp) {
		return true;
	}
	if ($esdiddestino == ESD_DEMANDA_NUCLEO_JURIDICO && in_array(PFL_DISTRIBUIDOR, $perfis)) {
		return true;
	}

	$sql = "select * from demandasfies.responsavelarea where dmdid = {$dmdid} and esdid in ($esdidorigem, $esdiddestino) AND reastatus = 'A' ";
	$dados = $db->carregar($sql);
	$dados = $dados ? $dados : array();
	$responsaveis = array();
	foreach ($dados as $dado) {
		$responsaveis[$dado['esdid']] = $dado;
	}

	// O responsável e o prazo para o seu setor devem estar preenchidos
	if (empty($responsaveis[$esdidorigem]['usucpf']) || empty($responsaveis[$esdidorigem]['reaprazo'])) {
		return 'Não é possível tramitar esta demanda antes de vincular um RESPONSÁVEL e um PRAZO para a sua área.';
	}

	// O prazo para o próximo setor deve estar preenchido
	if (empty($responsaveis[$esdiddestino]['reaprazo'])) {
		return 'Não é possível tramitar esta demanda antes de vincular um PRAZO para a área demandada.';
	}

	// O prazo para o próximo setor deve estar preenchido
	if ((in_array($esdidorigem, array(ESD_DEMANDA_NUCLEO_JURIDICO, ESD_DEMANDA_DTI_MEC))) && ($esdiddestino != ESD_DEMANDA_4_NIVEL) && empty($responsaveis[$esdiddestino]['usucpf'])
	) {
		return 'Não é possível tramitar esta demanda antes de vincular um RESPONSÁVEL para a área demandada.';
	}

	return true;
}

function complementoDisabled($campo, $podeEditar, $perfis)
{
	global $db;

	$camposProfe = getCamposProfe();
	$camposNucleoJuridico = getCamposNucleoJuridico();
	$camposDTI = getCamposDTI();
	$campos4Nivel = getCampos4Nivel();

	if ($podeEditar && ($db->testa_superuser() || in_array(PFL_ADMINISTRADOR, $perfis) ||

			// Campos da PROFE
			(in_array(PFL_PROCURADOR_FEDERAL, $perfis) && !in_array($campo, $camposProfe['excecao'])) ||

			// Campos dos Advogados e Núcleo Jurídico
			(in_array(PFL_NUCLEO_JURIDICO, $perfis) && in_array($campo, $camposNucleoJuridico['permissao'])) || (in_array(PFL_ADVOGADO, $perfis) && in_array($campo, $camposNucleoJuridico['permissao'])) ||

			// Campos da DTI
			(in_array(PFL_DTI_MEC, $perfis) && in_array($campo, $camposDTI['permissao'])) || (in_array(PFL_ANALISTA_DTI_MEC, $perfis) && in_array($campo, $camposDTI['permissao'])) ||

			// Campos do 4º Nível
			(in_array(PFL_4_NIVEL, $perfis) && in_array($campo, $campos4Nivel['permissao'])))
	) {
		return '';
	}

	return 'disabled';
}

function getCamposProfe()
{
	return array('permissao' => array('*'), 'excecao' => array('dmdcnpjies', 'dmdcnpjmantedora', 'claiddti', 'claidD'),);
}

function getCamposNucleoJuridico()
{
	return array('permissao' => array('dmdcnpjies', 'dmdcnpjmantedora', 'claid', 'claidD', 'matid', 'objid'), 'excecao' => array(),);
}

function getCamposDTI()
{
	return array('permissao' => array('claiddti'), 'excecao' => array(),);
}

function getCampos4Nivel()
{
	return array('permissao' => array(), 'excecao' => array(),);
}

function getEmailsPorIdPerfil($idPerfil)
{
	global $db;
	$emails = array();
	if($idPerfil == PFL_PROCURADOR_FEDERAL){
		$emails = array('subsidiofies@fnde.gov.br');
	}else{
		$sql = "  SELECT usuemail
              FROM seguranca.perfil perfil
              INNER JOIN seguranca.perfilusuario per_usu ON per_usu.pflcod = perfil.pflcod
              INNER JOIN seguranca.usuario usuario ON usuario.usucpf = per_usu.usucpf
              WHERE perfil.pflcod = {$idPerfil} AND usuario.suscod= 'A' ";

		$dados = $db->carregar($sql);
		if (is_array($dados)) {
			foreach ($dados as $valor) {
				$emails[] = $valor['usuemail'];
			}
		}
	}
	return $emails;
}

function getEmailsPorIdDemanda($esdid, $dmdid)
{
	global $db;
	$emails = array();
	$sql = "
              SELECT distinct usuemail
                FROM demandasfies.responsavelarea responsavelarea
                INNER JOIN seguranca.usuario usuario ON usuario.usucpf =   responsavelarea.usucpf
                WHERE responsavelarea.esdid = {$esdid}
                AND responsavelarea.dmdid= {$dmdid}
                AND responsavelarea.reastatus = 'A' AND usuario.suscod= 'A'
           ";
	$dados = $db->carregar($sql);
	if (is_array($dados)) {
		foreach ($dados as $valor) {
			$emails[] = $valor['usuemail'];
		}
	}
	return $emails;
}

function exibirDemandasResposavel($valor, $dadosLinha, $idLinha, $parans)
{
	$usunome = $dadosLinha['usunome'];
	$pfldsc = $dadosLinha['pfldsc'];
	$usucpf = $dadosLinha['usucpf'];
	$campo = $parans['campo'];
	return $link = "<a href='#' data-toggle='modal' data-target='#modal_demanda_por_usuario' class='bt_$campo'
                        data-nome='{$usunome}'
                        data-area='{$pfldsc}'
                        data-campo='{$campo}'
                        data-cpf='{$usucpf}'>{$valor}</a> ";
}

function getAreas()
{
//    return array(
//        array('codigo' => PFL_PROCURADOR_FEDERAL, 'descricao' => 'Procuradoria Federal'),
//        array('codigo' => PFL_NUCLEO_JURIDICO, 'descricao' => 'Núcleo Jurídico (DIGEF)'),
//        array('codigo' => PFL_ADVOGADO, 'descricao' => 'Advogado (DIGEF)'),
//        array('codigo' => PFL_DTI_MEC, 'descricao' => 'DTI/MEC'),
//        array('codigo' => PFL_ANALISTA_DTI_MEC, 'descricao' => 'Analista DTI/MEC'),
//        array('codigo' => PFL_4_NIVEL, 'descricao' => '4º Nível'),
//        array('codigo' => PFL_GESTOR_FIES, 'descricao' => 'Gestor FIES'),
//    );
	return array(array('codigo' => ESD_DEMANDA_ADVOGADO, 'descricao' => 'Advogados'), array('codigo' => ESD_ENTREGA_AGUARDANDO_APROVACAO, 'descricao' => 'Aguardando Aprovação'), array('codigo' => ESD_DEMANDA_ANALISTA_DTI_MEC, 'descricao' => 'Analista DTI/MEC'), array('codigo' => ESD_ENTREGA_APROVADA, 'descricao' => 'Aprovada'), array('codigo' => ESD_DEMANDA_GESTOR_FIES, 'descricao' => 'Gestor FIES'), array('codigo' => ESD_DEMANDA_DTI_MEC, 'descricao' => 'DTI/MEC'), array('codigo' => ESD_DEMANDA_EM_CADASTRAMENTO, 'descricao' => 'Em Cadastramento'), array('codigo' => ESD_DEMANDA_EM_INTERVENCAO, 'descricao' => 'Em Intervenção'), array('codigo' => ESD_ENTREGA_EM_ELABORACAO, 'descricao' => 'Em Elaboração'), array('codigo' => ESD_DEMANDA_EXECUCAO_DTI_MEC, 'descricao' => 'Em execução (DTI/MEC)'), array('codigo' => ESD_DEMANDA_FINALIZADA, 'descricao' => 'Finalizada'), array('codigo' => ESD_DEMANDA_PROFE, 'descricao' => 'PROFE'), array('codigo' => ESD_DEMANDA_NUCLEO_JURIDICO, 'descricao' => 'CGFIN / Núcleo Jurídico'), array('codigo' => ESD_DEMANDA_4_NIVEL, 'descricao' => 'Suporte (4º Nível)')


	);
}


function posRealizaTramiteEntregaTotal($dmdid, $dmeid)
{
	$modelDemanda = new Demanda($dmdid);
	$modelDemandaEntrega = new DemandaEntrega($dmeid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
	$estadoAtualEntrega = wf_pegarEstadoAtual($modelDemandaEntrega->docid);

	if ($modelDemandaEntrega->dmerelocorrencia == 't' && $modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_PARCIAL) {
		$cmddsc = "Tramite automativo (ao tramitar demanda da Gerência DIGEF com entrega total e com relatórios de ocorrência) ";
		wf_alterarEstado($modelDemanda->docid, AC_EXECUCAO_DTI_MEC, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_EXECUCAO_DTI_MEC));
		enviarEmailTramitacaoEntrega($modelDemandaEntrega->dmeid, ESD_DEMANDA_DTI_MEC);
		enviarEmailTramitacao($dmdid, ESD_DEMANDA_ADVOGADO);
		enviarEmailTramitacao($dmdid, ESD_DEMANDA_NUCLEO_JURIDICO);
		enviarEmailTramitacao($dmdid, ESD_DEMANDA_PROFE);
	} elseif (($modelDemandaEntrega->dmerelocorrencia == 'f' || is_null($modelDemandaEntrega->dmerelocorrencia)) && $modelDemandaEntrega->dmetipo ==
		DemandaEntrega::K_TIPO_PARCIAL) {
		enviarEmailTramitacaoEntrega($modelDemandaEntrega->dmeid, ESD_DEMANDA_PROFE);
	} elseif ( $modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_TOTAL) {
		$cmddsc = "Tramite automativo (ao tramitar demanda da Gerência DIGEF com entrega total sem relatórios de ocorrência) ";

		wf_alterarEstado($modelDemanda->docid, AC_ENVIA_SUBSIDIO_PROFE, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_PROFE));
		enviarEmailTramitacaoEntrega($modelDemandaEntrega->dmeid, ESD_DEMANDA_PROFE);

		enviarEmailTramitacao($dmdid, ESD_DEMANDA_ADVOGADO);
		enviarEmailTramitacao($dmdid, ESD_DEMANDA_NUCLEO_JURIDICO);
		enviarEmailTramitacao($dmdid, ESD_DEMANDA_PROFE);
	}
	return true;
}

function enviarParaNucleoJuridico($dmdid, $dmeid)
{
	$modelDemanda = new Demanda($dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
	$modelDemandaEntrega = new DemandaEntrega($dmeid);

	$perfis = pegaPerfilGeral();
	$perfis = $perfis ? $perfis : array();

	$condicao = ($modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_TOTAL && in_array(PFL_ADVOGADO, $perfis)) || ($modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_PARCIAL && $modelDemandaEntrega->dmerelocorrencia == 'f' && in_array(PFL_ADVOGADO, $perfis));
	return $condicao;
}

function enviarParaGerenciaDigef($dmdid, $dmeid)
{
	$modelDemanda = new Demanda($dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
	$modelDemandaEntrega = new DemandaEntrega($dmeid);

	$perfis = pegaPerfilGeral();
	$perfis = $perfis ? $perfis : array();

	$condicao = ($modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_TOTAL && in_array(PFL_ADVOGADO, $perfis) && $estadoAtual['esdid'] == ESD_DEMANDA_ADVOGADO);

	return $condicao;
}

function posRealizaTramite($dmdid, $dmeid)
{
	$perfis = pegaPerfilGeral();
	$perfis = $perfis ? $perfis : array();

	$modelDemanda = new Demanda($dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
	$modelDemandaEntrega = new DemandaEntrega($dmeid);

	$condicaoNucleoJuridico = false;
	$condicaoNucleoJuridico2 = false;
	$condicaoAdvogado = false;

	if (isset($estadoAtual['esdid'])) {
		$condicaoNucleoJuridico = in_array(PFL_NUCLEO_JURIDICO, $perfis) && $modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_PARCIAL && $modelDemandaEntrega->dmerelocorrencia == 't' && $estadoAtual['esdid'] == ESD_DEMANDA_NUCLEO_JURIDICO;
		$condicaoNucleoJuridico2 = in_array(PFL_NUCLEO_JURIDICO, $perfis) && $modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_TOTAL && $estadoAtual['esdid'] == ESD_DEMANDA_NUCLEO_JURIDICO;

		$condicaoAdvogado = in_array(PFL_ADVOGADO, $perfis) && $estadoAtual['esdid'] == ESD_DEMANDA_ADVOGADO && $modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_PARCIAL && $modelDemandaEntrega->dmerelocorrencia == 't';
		$condicaoAdvogado2 = in_array(PFL_ADVOGADO, $perfis) && $modelDemandaEntrega->dmetipo == DemandaEntrega::K_TIPO_TOTAL && $estadoAtual['esdid'] == ESD_DEMANDA_ADVOGADO;
	}


	if ($condicaoNucleoJuridico || $condicaoNucleoJuridico2 || $condicaoAdvogado) {
		$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = " . ESD_GERENCIA_DIGEF . " AND esdidorigem = {$estadoAtual['esdid']}";
		$aedid = $modelDemanda->pegaUm($sql);
		$cmddsc = "Tramite automativo (ao tramitar demanda para Gerencia DIGEF) ";
		wf_alterarEstado($modelDemanda->docid, $aedid, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_GERENCIA_DIGEF));
		enviarEmailTramitacao($modelDemanda->dmdid, ESD_GERENCIA_DIGEF);
	}

	if ($condicaoAdvogado2) {
		$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = " . ESD_DEMANDA_NUCLEO_JURIDICO . " AND esdidorigem = {$estadoAtual['esdid']}";
		$aedid = $modelDemanda->pegaUm($sql);
		$cmddsc = "Tramite automativo (ao tramitar demanda para Nucleo Juridico) ";
		wf_alterarEstado($modelDemanda->docid, $aedid, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_NUCLEO_JURIDICO));
		enviarEmailTramitacao($modelDemanda->dmdid, ESD_DEMANDA_NUCLEO_JURIDICO);
	}
	return true;
}

function validaTramitaçãoProfe($dmdid)
{
	$modelDemanda = new Demanda($dmdid);
	$arrayHistoricoEsdid = wf_pegarHistoricoEsdid($modelDemanda->docid);

	if(in_array(ESD_DEMANDA_ADVOGADO, $arrayHistoricoEsdid)){
		return false;
	}
	return true;
}


function enviarEmailTramitacaoEntrega($dmeid, $esdid)
{
	$modelDemandaEntrega = new DemandaEntrega($dmeid);
	$modelDemanda = new Demanda($modelDemandaEntrega->dmdid);
	$acjDesc = $modelDemanda->getTipoAcaoDescricao($modelDemanda->acjid);

	$paransEmail['remetente'] = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
	$paransEmail['assunto'] = "[Demandas FIES] Entrega {$modelDemandaEntrega->dmeid} Aprovada";
	$paransEmail['mensagem'] = "<pre>Prezados,
A entrega <span style='color: red;'>{$modelDemandaEntrega->dmeid} - {$modelDemandaEntrega->dmedsc}</span> foi aprovada.

<b>Tipo de Ação:</b> {$acjDesc}

Pertencente a demanda <span style='color: red;'>{$modelDemandaEntrega->dmdid}</span>.

Para maiores detalhes, favor entrar no SIMEC, módulo Demandas FIES em http://simec.mec.gov.br.

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";
	enviarEmailDemandasFies($esdid, $modelDemandaEntrega->dmdid, $paransEmail);
	return true;
}


function wf_pegarHistoricoEsdid($docid)
{
	$docid = (int)$docid;
	$sql = "
		SELECT ed.esdid
			FROM workflow.historicodocumento hd
				INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
		WHERE
			hd.docid = {$docid}
		ORDER BY hd.htddata ASC
	";
	$modelDemanda = new Demanda();
	$dados = $modelDemanda->carregar($sql);
	$esdids = array();
	if($dados){
		foreach($dados as $valor){
			$esdids[] = $valor['esdid'];
		}
	}
	return $esdids;
}

function removeEmailLista($destinatarios_, $emails){
	foreach ($destinatarios_ as $key => $email_destinario){
		foreach($emails as $email){
			if( $email_destinario == trim($email)){
				unset ($destinatarios_[$key]);
			}
		}
	}
	return $destinatarios_;
}
?>
