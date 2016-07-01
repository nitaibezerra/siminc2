<?php
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/AcaoJudicial.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaMateria.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaClassificacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaPartesAcao.inc.php";
include_once APPRAIZ . "demandasfies/classes/DemandaArquivo.class.inc";
include_once APPRAIZ . "demandasfies/classes/Objeto.class.inc";
include_once APPRAIZ . "demandasfies/classes/Tribunal.class.inc";
include_once APPRAIZ . "demandasfies/classes/Classificacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/HistoricoIntervencao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaPartesAcao = new DemandaPartesAcao($_REQUEST['dpaid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);
$modelDemandaArquivo = new DemandaArquivo($_REQUEST['dmaid']);
$modelDemanda->dmdcumprimento = trim($modelDemanda->dmdcumprimento);

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$podeEditar = $modelDemanda->verificarPermissaoEdicao($estadoAtual['esdid'], $perfis);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch ($_REQUEST['action']) {
	case 'salvar':

		$modelDemanda->popularDadosObjeto();
		$modelDemanda->dmdvalorcausa = $modelDemanda->dmdvalorcausa ? desformata_valor($modelDemanda->dmdvalorcausa) : null;
//        $modelDemanda->dmdprazo = $modelDemanda->dmdprazo ? formata_data_sql($modelDemanda->dmdprazo) : null;
		$modelDemanda->dmddtentradaprofe = $modelDemanda->dmddtentradaprofe ? formata_data_sql($modelDemanda->dmddtentradaprofe) : null;
		$modelDemanda->dmdcnpjmantedora = $modelDemanda->dmdcnpjmantedora ? str_replace(array('/', '.', '-'), '', $modelDemanda->dmdcnpjmantedora) : null;
		$modelDemanda->dmdcnpjies = $modelDemanda->dmdcnpjies ? str_replace(array('/', '.', '-'), '', $modelDemanda->dmdcnpjies) : null;
//        $modelDemanda->objid = $modelDemanda->objid ? $modelDemanda->objid : null;
		$modelDemanda->trbid = $modelDemanda->trbid ? $modelDemanda->trbid : null;
		$modelDemanda->dmdcumprimento = $modelDemanda->dmdcumprimento ? $modelDemanda->dmdcumprimento : null;

		if ($modelDemanda->dmdid) {
			$modelDemanda->dmddtalteracao = date('Y-m-d H:i:s');
			$modelDemanda->usucpfalteracao = $_SESSION['usucpforigem'];
		} else {
			$modelDemanda->dmddtinclusao = date('Y-m-d H:i:s');
			$modelDemanda->usucpfinclusao = $_SESSION['usucpforigem'];
		}

		$camposNulos = array('dmdprazo', 'dmdprocesso', 'dmdobjeto', 'dmdresumo', 'dmduf', 'dmdtribunalorigem', 'dmdcompetencia', 'dmdvalorcausa', 'dmdnumeroprocessoadm', 'dmdcnpjmantedora', 'dmdtransitojulgado', 'dmdresumoautor', 'dmdquestionamentos', 'dmdliminar', 'dmdmulta', 'dmdjuizadoespecial', 'dmdchaveacesso', 'dmdvara', 'dmdcnpjies', 'objid', 'trbid', 'claiddti', 'acjid', 'decid', 'solid', 'dmddecisaorecorrivel', 'dmdmultadsc', 'dmdmateriatipo', 'dmdprazotecnico', 'dmdprazojudicial', 'dmdtransitojulgado', 'dmdfavoravel',);

		$sucesso = $modelDemanda->salvar(null, null, $camposNulos);
		$modelDemanda->commit();

		if ($modelDemanda->dmdid && (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis) || in_array(PFL_NUCLEO_JURIDICO, $perfis))) {
			$modelDemanda->vincularMaterias($_REQUEST['matid']);
			$modelDemanda->vincularClassificacao($_REQUEST['claid'], 'G');
			$modelDemanda->vincularClassificacao($_REQUEST['claidD'], 'D');

		}

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");

		die;
		
	case 'salvar_parte_acao':

		$modelDemandaPartesAcao->popularDadosObjeto();
		if ($modelDemandaPartesAcao->dpanome) {
			$modelDemandaPartesAcao->dpanome = utf8_decode($modelDemandaPartesAcao->dpanome);
		}
		if ($_POST['cpfcnpj'] == 'cpf') {
			$modelDemandaPartesAcao->dpacnpj = null;
		}
		if ($_POST['cpfcnpj'] == 'cnpj') {
			$modelDemandaPartesAcao->dpacpf = null;
		}

		if ($modelDemandaPartesAcao->dpacpf) {
			$modelDemandaPartesAcao->dpacpf = str_replace(array('.', '-'), '', $modelDemandaPartesAcao->dpacpf);
		}
		if ($modelDemandaPartesAcao->dpacnpj) {
			$modelDemandaPartesAcao->dpacnpj = str_replace(array('/', '.', '-'), '', $modelDemandaPartesAcao->dpacnpj);
		}
		if ($modelDemandaPartesAcao->dpaid) {
			$modelDemandaPartesAcao->dpadtalteracao = date('Y-m-d H:i:s');
			$modelDemandaPartesAcao->usucpfalteracao = $_SESSION['usucpforigem'];
		} else {
			$modelDemandaPartesAcao->dpadtinclusao = date('Y-m-d H:i:s');
			$modelDemandaPartesAcao->usucpfinclusao = $_SESSION['usucpforigem'];
		}

		$sucesso = $modelDemandaPartesAcao->salvar(null, null, array('dpacpf', 'dpacnpj', 'usucpfalteracao', 'dpadtalteracao', 'usucpfinativacao', 'dpadtinativacao', 'dpanome'));
		$modelDemandaPartesAcao->commit();
		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;
		die;
	case 'form_parte_acao_autor':
		$modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
		$modelDemandaPartesAcao->formularioAutor($podeEditar);
		die;
	case 'form_parte_acao_reu':
		$modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
		$modelDemandaPartesAcao->formularioReu($podeEditar);
		die;
	case 'salvar_observacao':
		$modelDemandaObservacao->popularDadosObjeto();

		if ($modelDemandaObservacao->dmotexto) {
			$modelDemandaObservacao->dmotexto = utf8_decode($modelDemandaObservacao->dmotexto);
		}

		if ($modelDemandaObservacao->dmoid) {
			$modelDemandaObservacao->dmodtalteracao = date('Y-m-d H:i:s');
			$modelDemandaObservacao->usucpfalteracao = $_SESSION['usucpforigem'];
		} else {
			$modelDemandaObservacao->dmodtinclusao = date('Y-m-d H:i:s');
			$modelDemandaObservacao->usucpfinclusao = $_SESSION['usucpforigem'];
		}
		$sucesso = $modelDemandaObservacao->salvar();

		$modelDemandaObservacao->commit();
		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		die;
	case 'form_observacao':
		$modelDemandaObservacao->popularDadosObjeto();
		$modelDemandaObservacao->formularioObservacoes();
		die;
	case 'salvar_arquivo':

		// -- Verifica se tem algum arquivo para salvar no servidor
		if ($_FILES['file']['size'] > 0) {

			$nomeArquivo = $_REQUEST['dmadsc'];

			$descricao = explode(".", $_FILES['file']['name']);
			$campos = array("dmdid" => "'" . $_REQUEST['dmdid'] . "'", "dmadsc" => "'" . utf8_decode($nomeArquivo) . "'", "dmadtinclusao" => "'" . date('Y-m-d H:i:s') . "'", "usucpfinclusao" => "'" . $_SESSION['usucpforigem'] . "'",);

			$file = new FilesSimec("demandaarquivo", $campos, "demandasfies");
			$arquivoSalvo = $file->setUpload($_FILES ['file']['name'], '', true);
			if ($arquivoSalvo) {
				echo "<script>alert('Arquivo cadastrado com sucesso!');</script>";
			}
		}
		die;
	case 'form_arquivo':
		$modelDemandaArquivo->popularDadosObjeto();
		$modelDemandaArquivo->formularioArquivos();
		die;
	case 'download_arquivo':
		$modelDemandaArquivo->popularDadosObjeto();
		$file = new FilesSimec();
		if ($modelDemandaArquivo->arqid) {
			ob_clean();
			$arquivo = $file->getDownloadArquivo($modelDemandaArquivo->arqid);
		}
		die;
	case 'inativar_autor':
		$modelDemandaPartesAcao->dpastatus = 'I';
		$modelDemandaPartesAcao->dpadtinativacao = date('Y-m-d H:i:s');
		$modelDemandaPartesAcao->usucpfinativacao = $_SESSION['usucpforigem'];
		$sucesso = $modelDemandaPartesAcao->salvar();
		$modelDemandaPartesAcao->commit();

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemandaPartesAcao->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'inativar_observacao':
		$modelDemandaObservacao->dmostatus = 'I';
		$modelDemandaObservacao->dmodtinativacao = date('Y-m-d H:i:s');
		$modelDemandaObservacao->usucpfinativacao = $_SESSION['usucpforigem'];
		$sucesso = $modelDemandaObservacao->salvar();
		$modelDemandaObservacao->commit();

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemandaObservacao->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'inativar_arquivo':
		$modelDemandaArquivo->dmastatus = 'I';
		$modelDemandaArquivo->dmadtinativacao = date('Y-m-d H:i:s');
		$modelDemandaArquivo->usucpfinativacao = $_SESSION['usucpforigem'];
		$sucesso = $modelDemandaArquivo->salvar();
		$modelDemandaArquivo->commit();

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemandaArquivo->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'getPessoaJuridica':
		require_once APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';
		$pessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");

		$pj = str_replace(array('/', '.', '-'), '', $_POST['cnpj']);
		$xml = $pessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
		$obj = (object)simplexml_load_string($xml);
		if (!$obj->PESSOA) {
			echo "CNPJ inexistente na base da Receita Federal.";
		}
		echo (string)$obj->PESSOA->no_empresarial_rf;
		die;

	case 'getPessoaFisica':
		$cpf = $_POST['cpf'];
		$dmdids = $modelDemandaPartesAcao->getDemandasVinculadasCpf($cpf);
		if( !empty($dmdids)  ){
			$dmdids_str = implode(', ', $dmdids);
			$data['msg'] = utf8_encode("CPF vinculado à demanda(s) {$dmdids_str}");
		}

		$cpf = str_replace(array('.', '-'), '', $_POST['cpf']);
		$usu_receita = recuperarUsuarioReceita($cpf);
		if ($usu_receita['usuarioexiste']) {
			$data['nome'] = utf8_encode($usu_receita['dados']['no_pessoa_rf']);
		}
		echo json_encode($data);
		die;

	case 'salvar-intervencao':

		$msg = $tipo = '';
		if ($estadoAtual['esdid'] && $modelDemanda->docid) {

			$sql = "SELECT aedid FROM workflow.acaoestadodoc
                    WHERE esdiddestino = " . ESD_DEMANDA_EM_INTERVENCAO . "
                    AND esdidorigem = {$estadoAtual['esdid']}";
			$aedid = $modelDemanda->pegaUm($sql);

			$sql = "select pfldsc
                    from seguranca.perfil
                    where sisid = " . SIS_DEMANDASFIES . "
                    and pflcod = {$_REQUEST['pflcod']}
                    order by pfldsc;";

			$pfldsc = $modelDemanda->pegaUm($sql);

			$cmddsc = "Solicitação de intervenção da área ({$pfldsc}): " . $_REQUEST['cmddsc'];
			$a = wf_alterarEstado($modelDemanda->docid, $aedid, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_EM_INTERVENCAO));

			$sql = "SELECT * FROM workflow.documento
                    WHERE docid = {$modelDemanda->docid}";
			$dados = $modelDemanda->pegaLinha($sql);

			$modeloHistorico = new HistoricoIntervencao();

			$sql = "update demandasfies.historicointervencao set hiistatus = 'I' where dmdid = {$modelDemanda->dmdid}";
			$modeloHistorico->executar($sql);

			$modeloHistorico->dmdid = $modelDemanda->dmdid;
			$modeloHistorico->docid = $modelDemanda->docid;
			$modeloHistorico->hstid = $dados['hstid'];
			$modeloHistorico->pflcod = $_REQUEST['pflcod'];
			$modeloHistorico->hiistatus = 'A';
			$modeloHistorico->esdidorigem = $estadoAtual['esdid'];
			$modeloHistorico->salvar();

			$modeloHistorico->commit();
			$msg = 'Operação realizada com sucesso!';
			$tipo = Simec_Helper_FlashMessage::SUCESSO;
		}


		$url .= '&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'salvar-retorno-intervencao':

		$msg = $tipo = '';
		if ($estadoAtual['esdid'] && $modelDemanda->docid) {

			$sql = "select * from demandasfies.historicointervencao h where dmdid = $modelDemanda->dmdid and hiistatus = 'A'";
			$dados = $db->pegaLinha($sql);

			$sql = "SELECT aedid FROM workflow.acaoestadodoc
                    WHERE esdiddestino = {$dados['esdidorigem']}
                    AND esdidorigem = " . ESD_DEMANDA_EM_INTERVENCAO;
			$aedid = $modelDemanda->pegaUm($sql);

			$cmddsc = "Demanda retornada à área demandante: " . $_REQUEST['cmddsc'];
			wf_alterarEstado($modelDemanda->docid, $aedid, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_EM_INTERVENCAO));

			$sql = "SELECT * FROM workflow.documento
                    WHERE docid = {$modelDemanda->docid}";
			$dados = $modelDemanda->pegaLinha($sql);

			$modeloHistorico = new HistoricoIntervencao();

			$sql = "update demandasfies.historicointervencao set hiistatus = 'I' where dmdid = {$modelDemanda->dmdid}";
			$modeloHistorico->executar($sql);
			$modeloHistorico->commit();
			$msg = 'Operação realizada com sucesso!';
			$tipo = Simec_Helper_FlashMessage::SUCESSO;
		}

		$url .= '&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'get_tribunal':
		ob_clean();
		echo $modelDemanda->getTribunais($_REQUEST['competencia']);
		die;
}

include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
?>
<div class="col-lg-12">
	<div class="page-header">
		<h3 id="forms"><?= $modelDemanda->dmdid ? 'Demanda ' . $modelDemanda->dmdid : 'Nova Demanda'; ?></h3>
	</div>

	<?php

	// -- HTML das abas
	echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");

	echo $fm->getMensagens();

	?>

	<?php if ($modelDemanda->acjid == AcaoJudicial::K_MANDADO_SEGURANCA || $modelDemanda->acjid == AcaoJudicial::K_ACAO_CIVIL_PUBLICA) { ?>
		<div class="row col-md-12">
			<div style="background-color: yellow; text-align: center; color: red;" class="alert alert-warning" role="alert">
				<strong>ATENÇÃO</strong>: <?= $modelDemanda->acjid == AcaoJudicial::K_MANDADO_SEGURANCA ? 'MANDADO DE SEGURANÇA' : 'AÇÃO CIVIL PÚBLICA (ACP)'; ?>
			</div>
		</div>
	<?php } ?>


	<div class="row">
		<div class="row col-md-11">

			<div class="row">
				<div class="row col-md-7">

					<?php if ($podeEditar){ ?>
					<form id="form-save" method="post" class="form-horizontal">
						<?php } ?>
						<input name="controller" type="hidden" value="instituicao"> <input name="action" type="hidden" value="salvar">
						<input name="dmdid" id="dmdid" type="hidden" value="<?= $modelDemanda->dmdid; ?>">

						<div class="well">
							<fieldset class="form-horizontal">
								<legend>Dados Gerais</legend>

								<?php if ($modelDemanda->dmdid) { ?>
									<div class="form-group">
										<label for="dmdid" class="col-lg-4 col-md-4 control-label">Código:</label>

										<div class="col-lg-8 col-md-8 " style="padding-top: 5px;">
											<span style="color: red;"><?= $modelDemanda->dmdid; ?></span>
										</div>
									</div>
								<?php } ?>

								<div class="form-group">
									<label for="dmdtipo" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Tipo:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control chosen" id="dmdtipo" name="dmdtipo" <?= complementoDisabled('dmdtipo', $podeEditar, $perfis); ?>>
											<?= $modelDemanda->getTipos(); ?>
										</select>
									</div>
								</div>

								<div class="form-group">
									<label for="matid" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Matéria:</label>

									<div class="col-lg-8 col-md-8 ">
										<select data-placeholder="Selecione" class="form-control chosen campo-judicial" id="matid" name="matid[]" multiple <?= complementoDisabled('matid', $podeEditar, $perfis); ?>>
											<?= $modelDemanda->getMaterias(); ?>
										</select>
									</div>
								</div>

								<div class="form-group">
									<label for="acjid" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Tipo de Ação:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control chosen campo-judicial" id="acjid" name="acjid" <?= complementoDisabled('acjid', $podeEditar, $perfis); ?>>
											<?= $modelDemanda->getTipoAcao(); ?>
										</select>
									</div>
								</div>

								<div class="form-group">
									<label for="claid" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Objeto:</label>

									<div class="col-lg-8 col-md-8 ">
										<select data-placeholder="Selecione" class="form-control chosen campo-judicial" id="claid" name="claid[]" multiple <?= complementoDisabled('claid', $podeEditar, $perfis); ?>>
											<?php
											echo $modelDemanda->getClassificacao();?>
										</select>
									</div>
								</div>

								<div class="form-group">
									<label for="claid" class="col-lg-4 col-md-4 control-label">Classificação DIGEF:</label>

									<div class="col-lg-8 col-md-8 ">
										<select data-placeholder="Selecione" class="form-control chosen" id="claidD" name="claidD[]" multiple <?= complementoDisabled('claidD', $podeEditar, $perfis); ?>>
											<?php
											echo $modelDemanda->getClassificacao('D');?>
										</select>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdliminar" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Decisão:</label>

									<div class="col-lg-8 col-md-8 ">
										<div class="btn-group" data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmdliminar == 't' ? 'active' : ''); ?> <?= complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>">
												<input class="campo-decisao" type="radio" name="dmdliminar" id="dmdliminar1" value="t" <?= ($modelDemanda->dmdliminar == 't' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>> Sim
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdliminar == 'f' ? 'active' : ''); ?> <?= complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>">
												<input class="campo-decisao" type="radio" name="dmdliminar" id="dmdliminar2" value="f" <?= ($modelDemanda->dmdliminar == 'f' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>> Não
											</label>
										</div>
									</div>
								</div>

								<div class="form-group tipo-judicial tipo-decisao">
									<label for="dmddecisaofnde" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> FNDE:</label>

									<div class="col-lg-8 col-md-8 ">
										<div class="btn-group" data-toggle="buttons">
											<?php $dmddecisaofnde = $modelDemanda->dmddecisaofnde ?>
											<label class="btn btn-default <?= ( ($dmddecisaofnde == 't' || empty($dmddecisaofnde) ) ? 'active' : ''); ?> <?= complementoDisabled
											('dmddecisaofnde',
												$podeEditar, $perfis); ?>">
												<input class="campo-decisao" type="radio" name="dmddecisaofnde" id="dmddecisaofnde1" value="t" <?= ( ($dmddecisaofnde == 't' || empty($dmddecisaofnde) ) ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmddecisaofnde', $podeEditar, $perfis); ?>> Sim
											</label>
											<label class="btn btn-default <?= ($dmddecisaofnde == 'f' ? 'active' : ''); ?> <?= complementoDisabled('dmddecisaofnde', $podeEditar, $perfis); ?>">
												<input class="campo-decisao" type="radio" name="dmddecisaofnde" id="dmddecisaofnde" value="f" <?= ($dmddecisaofnde == 'f' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmddecisaofnde', $podeEditar, $perfis); ?>> Não
											</label>
										</div>
									</div>
								</div>


								<div class="form-group tipo-judicial tipo-decisao">
									<label for="claid" class="col-lg-4 col-md-4 control-label">Tipo Decisão:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control" id="decid" name="decid" <?= complementoDisabled('decid', $podeEditar, $perfis); ?>>
											<?php
											echo $modelDemanda->getDecisao();?>
										</select>
									</div>
								</div>

								<div class="form-group dmdstatusdecisao">
									<label for="dmdstatusdecisao" class="col-lg-4 col-md-4 control-label">Status da Decisão:</label>

									<div class="col-lg-8 col-md-8">
										<div class="btn-group " data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmdstatusdecisao == 'DF' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao1" value="DF" <?= ($modelDemanda->dmdstatusdecisao == 'DF' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>> Deferida
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdstatusdecisao == 'ID' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao2" value="ID" <?= ($modelDemanda->dmdstatusdecisao == 'ID' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>> Indeferida
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdstatusdecisao == 'PD' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao3" value="PD" <?= ($modelDemanda->dmdstatusdecisao == 'PD' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>> Parcialmente Deferida
											</label>
										</div>
									</div>
								</div>

								<div class="form-group tipo-judicial tipo-decisao">
									<label for="dmddecisaorecorrivel" class="col-lg-4 col-md-4 control-label">Recorrível:</label>

									<div class="col-lg-8 col-md-8 ">
										<div class="btn-group" data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmddecisaorecorrivel == 't' ? 'active' : ''); ?> <?= complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmddecisaorecorrivel" id="dmddecisaorecorrivel1" value="t" <?= ($modelDemanda->dmddecisaorecorrivel == 't' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>> Sim
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmddecisaorecorrivel == 'f' ? 'active' : ''); ?> <?= complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmddecisaorecorrivel" id="dmddecisaorecorrivel2" value="f" <?= ($modelDemanda->dmddecisaorecorrivel == 'f' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>> Não
											</label>
										</div>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdmulta" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Multa:</label>

									<div class="col-lg-8 col-md-8 ">
										<div class="btn-group" data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmdmulta == 't' ? 'active' : ''); ?> <?= complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>">
												<input class="campo-multa" type="radio" name="dmdmulta" id="dmdmulta1" value="t" <?= ($modelDemanda->dmdmulta == 't' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>> Sim
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdmulta == 'f' ? 'active' : ''); ?> <?= complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>">
												<input class="campo-multa" type="radio" name="dmdmulta" id="dmdmulta2" value="f" <?= ($modelDemanda->dmdmulta == 'f' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>> Não
											</label>
										</div>
									</div>
								</div>

								<div class="form-group tipo-judicial div-multa tipo-multa">
									<label for="dmdmultadsc" class="col-lg-4 col-md-4 control-label">Descrição Multa:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdmultadsc" name="dmdmultadsc" type="text"
											   class="form-control" placeholder=""
											   value="<?= $modelDemanda->dmdmultadsc; ?>" <?= $complementoDisabled; ?>  <?= complementoDisabled('dmdmultadsc', $podeEditar, $perfis); ?>>
									</div>
								</div>

								<?php if ($modelDemanda->testa_superuser() || in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis)) { ?>
									<div class="form-group">
										<label for="dmdprazojudicial" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Prazo Judicial (em dias):</label>

										<div class="col-lg-8 col-md-8 ">
											<input id="dmdprazojudicial" name="dmdprazojudicial" type="text" class="form-control campo-judicial inteiro"
												   value="<?= $modelDemanda->dmdprazojudicial; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdprazojudicial', $podeEditar, $perfis); ?>>
										</div>
									</div>
								<?php } ?>

								<?php if ($modelDemanda->dmdprazojudicial && $modelDemanda->dmdprazojudicial <= 2) { ?>
									<div class="form-group">
										<label for="dmdprazotecnico" class="col-lg-4 col-md-4 control-label">Prazo Técnico:</label>

										<div class="col-lg-8 col-md-8 ">
											<?php $dmdprazotecnico = $modelDemanda->dmdprazotecnico ? substr($modelDemanda->dmdprazotecnico, 8, 2) . '/' . substr($modelDemanda->dmdprazotecnico, 5, 2) . '/' . substr($modelDemanda->dmdprazotecnico, 0, 4) : ''; ?>
											<input id="dmdprazotecnico" name="dmdprazotecnico" type="text" class="data form-control"
												   placeholder=""
												   value="<?= $dmdprazotecnico; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdprazotecnico', $podeEditar, $perfis); ?>>
										</div>
									</div>
								<?php } ?>

								<div class="form-group">
									<label for="dmdvalorcausa" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Valor da Causa:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdvalorcausa" name="dmdvalorcausa" type="text" class="form-control moeda  campo-judicial"
											   placeholder=""
											   value="<?= $modelDemanda->dmdvalorcausa ? number_format($modelDemanda->dmdvalorcausa, 2, ',', '.') : ''; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdvalorcausa', $podeEditar, $perfis); ?>>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="solid" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Solicitação:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control campo-judicial" id="solid" name="solid" <?= complementoDisabled('solid', $podeEditar, $perfis); ?>>
											<?php
											echo $modelDemanda->getSolicitacao();?>
										</select>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdcompetencia" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Competência:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control campo-judicial" id="dmdcompetencia" name="dmdcompetencia" <?= complementoDisabled('dmdcompetencia', $podeEditar, $perfis); ?>>
											<option value="">Selecione</option>
											<?= $modelDemanda->getCompetencias(); ?>
										</select>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdjuizadoespecial" class="col-lg-4 col-md-4 control-label">Juizado Especial:</label>

									<div class="col-lg-8 col-md-8 ">
										<div class="btn-group" data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmdjuizadoespecial == 't' ? 'active' : ''); ?> <?= complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdjuizadoespecial" id="dmdjuizadoespecial1" value="t" <?= ($modelDemanda->dmdjuizadoespecial == 't' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>> Sim
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdjuizadoespecial == 'f' ? 'active' : ''); ?> <?= complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdjuizadoespecial" id="dmdjuizadoespecial2" value="f" <?= ($modelDemanda->dmdjuizadoespecial == 'f' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>> Não
											</label>
										</div>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="trbid" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Tribunal de Origem:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control campo-judicial" id="trbid" name="trbid" <?= complementoDisabled('trbid', $podeEditar, $perfis); ?>>
											<?= $modelDemanda->getTribunais($modelDemanda->dmdcompetencia); ?>
										</select>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdvara" class="col-lg-4 col-md-4 control-label">Vara:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdvara" name="dmdvara" type="text"
											   class="form-control" placeholder=""
											   value="<?= $modelDemanda->dmdvara; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdvara', $podeEditar, $perfis); ?>>
									</div>
								</div>

								<div class="form-group">
									<label for="dmduf" class="col-lg-4 col-md-4 control-label">UF:</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control chosen" id="dmduf" name="dmduf" <?= complementoDisabled('dmduf', $podeEditar, $perfis); ?>>
											<?= $modelDemanda->getComboUfs(); ?>
										</select>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdprocesso" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Nº Processo:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdprocesso" name="dmdprocesso" type="text" class="form-control campo-judicial"
											   placeholder=""
											   value="<?= $modelDemanda->dmdprocesso; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdprocesso', $podeEditar, $perfis); ?>>
									</div>
								</div>

								<div class="form-group">
									<label for="dmdnumeroprocessoadm" class="col-lg-4 col-md-4 control-label">Nº Processo Administrativo:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdnumeroprocessoadm" name="dmdnumeroprocessoadm" type="text"
											   class="form-control" placeholder=""
											   value="<?= $modelDemanda->dmdnumeroprocessoadm; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdnumeroprocessoadm', $podeEditar, $perfis); ?>>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdtransitojulgado" class="col-lg-4 col-md-4 control-label">Trânsito em Julgado:</label>

									<div class="col-lg-8 col-md-8">
										<div class="btn-group " data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmdtransitojulgado == 't' ? 'active' : ''); ?> <?= complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdtransitojulgado" id="dmdtransitojulgado1" value="t" <?= ($modelDemanda->dmdtransitojulgado == 't' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>> Sim
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdtransitojulgado == 'f' ? 'active' : ''); ?> <?= complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdtransitojulgado" id="dmdtransitojulgado2" value="f" <?= ($modelDemanda->dmdtransitojulgado == 'f' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>> Não
											</label>
										</div>
									</div>
								</div>

								<div class="form-group tipo-judicial">
									<label for="dmdfavoravel" class="col-lg-4 col-md-4 control-label">Status da Sentença:</label>

									<div class="col-lg-8 col-md-8">
										<div class="btn-group " data-toggle="buttons">
											<label class="btn btn-default <?= ($modelDemanda->dmdfavoravel == 'PR' ? 'active' : ''); ?> <?= complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdfavoravel" id="dmdfavoravel1" value="PR" <?= ($modelDemanda->dmdfavoravel == 'PR' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>> Procedente
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdfavoravel == 'IP' ? 'active' : ''); ?> <?= complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdfavoravel" id="dmdfavoravel2" value="IP" <?= ($modelDemanda->dmdfavoravel == 'IP' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>> Improcedente
											</label>
											<label class="btn btn-default <?= ($modelDemanda->dmdfavoravel == 'PP' ? 'active' : ''); ?> <?= complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdfavoravel" id="dmdfavoravel3" value="PP" <?= ($modelDemanda->dmdfavoravel == 'PP' ? 'checked="checked"' : ''); ?> <?= complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>> Parcialmente Procedente
											</label>
										</div>
									</div>
								</div>

								<div class="form-group">
									<label for="dmdcumprimento" class="col-lg-4 col-md-4 control-label">Cumprimento:</label>

									<div class="col-lg-8 col-md-8">
										<div class="btn-group " data-toggle="buttons">
											<label class="btn btn-default btn-sm <?= ($modelDemanda->dmdcumprimento == 'DC' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdcumprimento" id="dmdcumprimento1" value="DC" <?= ($modelDemanda->dmdcumprimento == 'DC' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>> Decisão Cumprida
											</label>
											<label class="btn btn-default btn-sm <?= ($modelDemanda->dmdcumprimento == 'DCP' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdcumprimento" id="dmdcumprimento2" value="DCP" <?= ($modelDemanda->dmdcumprimento == 'DCP' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>> Decisão Cumprida Parcialmente
											</label>
											<label class="btn btn-default btn-sm <?= ($modelDemanda->dmdcumprimento == 'DNC' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>">
												<input type="radio" name="dmdcumprimento" id="dmdcumprimento3" value="DNC" <?= ($modelDemanda->dmdcumprimento == 'DNC' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>> Decisão Não Cumprida
											</label>
										</div>
									</div>
								</div>

								<div class="form-group">
									<label for="dmdchaveacesso" class="col-lg-4 col-md-4 control-label">Chave de Acesso:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdchaveacesso" name="dmdchaveacesso" type="text"
											   class="form-control" placeholder=""
											   value="<?= $modelDemanda->dmdchaveacesso; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdchaveacesso', $podeEditar, $perfis); ?>>
									</div>
								</div>

								<div class="form-group">
									<label for="dmdcnpjies" class="col-lg-4 col-md-4 control-label">CNPJ Instituição Ensino:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdcnpjies" name="dmdcnpjies" type="text"
											   class="form-control cnpj cnpj_buscar" placeholder=""
											   value="<?= $modelDemanda->dmdcnpjies; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdcnpjies', $podeEditar, $perfis); ?>>
										<span id="span_dmdcnpjies" class="help-block"></span>
									</div>
								</div>

								<div class="form-group">
									<label for="dmdcnpjmantedora" class="col-lg-4 col-md-4 control-label">CNPJ Mantenedora:</label>

									<div class="col-lg-8 col-md-8 ">
										<input id="dmdcnpjmantedora" name="dmdcnpjmantedora" type="text"
											   class="form-control cnpj cnpj_buscar" placeholder=""
											   value="<?= $modelDemanda->dmdcnpjmantedora; ?>" <?= $complementoDisabled; ?> <?= complementoDisabled('dmdcnpjmantedora', $podeEditar, $perfis); ?>>
										<span id="span_dmdcnpjmantedora" class="help-block"></span>
									</div>
								</div>

								<div class="form-group">
									<label for="dmdtipo" class="col-lg-4 col-md-4 control-label">Classificação (DTI/MEC):</label>

									<div class="col-lg-8 col-md-8 ">
										<select class="form-control chosen" id="claiddti" name="claiddti" <?= complementoDisabled('claiddti', $podeEditar, $perfis); ?>>
											<?= $modelDemanda->getClassificacao('T'); ?>
										</select>
									</div>
								</div>

								<div class="form-group">
									<label for="dmddtentradaprofe" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Data de entrada na PROFE:</label>

									<div class="col-lg-6 col-md-6 ">
										<?php $dmddtentradaprofe = $modelDemanda->dmddtentradaprofe ? substr($modelDemanda->dmddtentradaprofe, 8, 2) . '/' . substr($modelDemanda->dmddtentradaprofe, 5, 2) . '/' . substr($modelDemanda->dmddtentradaprofe, 0, 4) : ''; ?>
										<input id="dmddtentradaprofe" name="dmddtentradaprofe" type="text" class="data form-control campo-judicial" placeholder=""
											   value="<?php echo $dmddtentradaprofe; ?>" <?php echo $complementoDisabled; ?>  <?php echo complementoDisabled('dmddtentradaprofe', $podeEditar, $perfis); ?>>
									</div>
								</div>

							</fieldset>
							<div>
								<?php if ($podeEditar) { ?>
									<button title="Salvar" class="btn btn-success" id="btn-salvar" type="button"><span
											class="glyphicon glyphicon-thumbs-up"></span> Salvar
									</button>
								<?php } ?>
								<a title="Voltar" class="btn btn-danger" href="/demandasfies/demandasfies.php?modulo=inicio&acao=C"><span
										class="glyphicon glyphicon-hand-left"></span> Voltar</a>
							</div>
						</div>
						<?php if ($podeEditar){ ?>
					</form>
				<?php } ?>
				</div>

				<div class="row col-md-5">

					<?php
					$podeEditarAutores = false;
					if ((in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) || (in_array(PFL_PROCURADOR_FEDERAL, $perfis) && $podeEditar)) {
						$podeEditarAutores = true;
					}

					?>

					<div id="div_listagem_autor" class="tipo-judicial">
						<?php if ($modelDemanda->dmdid) {
							$modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
							$modelDemandaPartesAcao->formularioAutor($podeEditarAutores);
						} ?>
					</div>

					<div id="div_listagem_reu" class="tipo-judicial">
						<?php if ($modelDemanda->dmdid) {
							$modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
							$modelDemandaPartesAcao->formularioReu($podeEditarAutores);
						} ?>
					</div>

					<div id="div_listagem_observacao">
						<?php if ($modelDemanda->dmdid) {
							$modelDemandaObservacao->dmdid = $modelDemanda->dmdid;
							$modelDemandaObservacao->formularioObservacoes($complementoDisabled);
						} ?>
					</div>

					<div id="div_listagem_arquivo">
						<?php if ($modelDemanda->dmdid) {
							$modelDemandaArquivo->dmdid = $modelDemanda->dmdid;
							$modelDemandaArquivo->formularioArquivos($complementoDisabled);
						} ?>
					</div>
				</div>
			</div>
		</div>
		<div class="row col-md-1">
			<?php wf_desenhaBarraNavegacao_demandasFIES($modelDemanda->dmdid, $modelDemanda->docid, array('dmdid' => $modelDemanda->dmdid), $estadoAtual['esdid']); ?>
		</div>
		<div class="clearfix"></div>

		<?php if ($modelDemanda->dmdid) { ?>
			<div class="col-md-12">
				<div class="row">
					<div class="col-md-12">
						<div class="well">
							<fieldset>
								<legend>Resumos</legend>
								<?php $modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_SINTESE_AUTOR, 'Síntese das alegações do autor'); ?>
								<?php $modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_DETERMINACAO_JUDICIAL, 'Síntese da determinação judicial'); ?>
								<?php $modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_QUESTIONAMENTOS_DIRIMIDOS, 'Questionamentos a serem dirimidos'); ?>
								<?php $modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_QUESTIONAMENTOS_AREAS, 'Questionamentos das Áreas'); ?>
							</fieldset>
						</div>
					</div>
				</div>
			</div>

			<div class="clearfix"></div>
		<?php } ?>

	</div>

</div>


<script src="/library/jquery/jquery.form.min.js" type="text/javascript"></script>

<script type="text/javascript">
	$(document).ready(function () {
		$('body').on('change', '.cnpj_buscar', function () {
			$campo = $(this);
			if ($campo.val()) {
				var cnpj = str_replace(['.', ',', '/', '-'], [''], $campo.val());

				$.ajax({
					type: "POST",
					url: window.location.href,
					data: {action: 'getPessoaJuridica', cnpj: cnpj },
					async: false,
					success: function (data) {
						$('#span_' + $campo.attr('id')).html(data);

						if (!data) {
							alert('CNPJ não encontrado');
							$campo.val('');
						}
					}
				});
			}
		});

		$('.cnpj_buscar').change();

		$('#dmdcompetencia').change(function () {
			$('#trbid').load('demandasfies.php?modulo=principal/demandasformulario&acao=A&action=get_tribunal&competencia=' + $(this).val());
		});

		$('#tramitar-intervencao').click(function () {

			if (!$('#pflcod_t').val()) {
				alert("O campo 'Área' é obrigatório.");
				return false;
			}

			if (!$('#cmddsc_t').val()) {
				alert("O campo 'Justificativa' é obrigatório.");
				return false;
			}

			$('#form-save-intervencao').submit();
		});

		$('#tramitar-retorno-intervencao').click(function () {

			if (!$('#cmddsc_r').val()) {
				alert("O campo 'Justificativa' é obrigatório.");
				return false;
			}

			$('#form-save-retorno-intervencao').submit();
		});

		$('#dmdtipo').change(function () {
			if ($(this).val() == 1) {
				$('.tipo-judicial').show();
			} else {
				$('.tipo-judicial').hide();
			}
		}).change();

		exibircamposDecisao();
		$('.campo-decisao').change(function () {
			exibircamposDecisao();
		});

		exibircamposMulta();
		$('.campo-multa').change(function () {
			exibircamposMulta();
		});

		$('#btn-salvar').click(function () {

			var envia = true;

			$('.campo-judicial').each(function (index, obj) {
				$obj = $(obj);

				console.log($obj.val(), $obj);

				if (!$obj.val()) {
					alert("O campo [" + $('label[for=' + $obj.attr("id") + ']').html().replace(':', '') + "] é obrigatório!");
					envia = false;
					return false;
				}
			});

			if (!envia) {
				return false;
			}
			$('#form-save').submit();
		});
	});

	function exibircamposDecisao() {
		if ($('.campo-decisao:checked').val() == 't') {
			$('.tipo-decisao').show();
		} else {
			$('.tipo-decisao').hide();
		}
	}

	function exibircamposMulta() {
		if ($('.campo-multa:checked').val() == 't') {
			$('.tipo-multa').show();
		} else {
			$('.tipo-multa').hide();
		}
	}
</script>