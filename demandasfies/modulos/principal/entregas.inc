<?php
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaEntrega.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaArquivo.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$areas = getAreas();

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaEntrega = new DemandaEntrega($_REQUEST['dmeid']);
$modelDemandaArquivo = new DemandaArquivo($_REQUEST['dmaid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$estadoAtualEntrega = wf_pegarEstadoAtual($modelDemandaEntrega->docid);
if ($modelDemandaEntrega->dmeid && !$modelDemandaEntrega->docid) {
	$modelDemandaEntrega->docid = pegarDocidEntrega($modelDemandaEntrega->dmeid);
	$estadoAtualEntrega = wf_pegarEstadoAtual($modelDemandaEntrega->docid);
}

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$podeEditar = $modelDemanda->verificarPermissaoEdicao($estadoAtual['esdid'], $perfis);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch ($_REQUEST['action']) {
	case 'salvar':

		( !isset($_POST['dmerelocorrencia']) ? $_POST['dmerelocorrencia'] = 'f' : '');
		$modelDemandaEntrega->popularDadosObjeto();

		if ($modelDemandaEntrega->dmeid) {
			$modelDemandaEntrega->dmedtalteracao = date('Y-m-d H:i:s');
			$modelDemandaEntrega->usucpfalteracao = $_SESSION['usucpforigem'];
		} else {
			$modelDemandaEntrega->dmedtinclusao = date('Y-m-d H:i:s');
			$modelDemandaEntrega->usucpfinclusao = $_SESSION['usucpforigem'];
		}

		$sucesso = $modelDemandaEntrega->salvar();

		$modelDemandaEntrega->commit();
		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=entregas&dmeid=' . $modelDemandaEntrega->dmeid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");

		die;
	case 'salvar_observacao':
		$modelDemandaObservacao->popularDadosObjeto();

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

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=entregas&dmeid=' . $modelDemandaEntrega->dmeid;

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

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=entregas&dmeid=' . $modelDemandaEntrega->dmeid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'salvar_arquivo':

		// -- Verifica se tem algum arquivo para salvar no servidor
		if ($_FILES['file']['size'] > 0) {

			$nomeArquivo = $_REQUEST['dmadsc'];

			$descricao = explode(".", $_FILES['file']['name']);
			$campos = array("dmdid" => "'" . $_REQUEST['dmdid'] . "'", "dmatipo" => "'" . $_REQUEST['dmatipo'] . "'", "dmeid" => "'" . $_REQUEST['dmeid'] . "'", "dmadsc" => "'" . $nomeArquivo . "'", "dmadtinclusao" => "'" . date('Y-m-d H:i:s') . "'", "usucpfinclusao" => "'" . $_SESSION['usucpforigem'] . "'",);

			$file = new FilesSimec("demandaarquivo", $campos, "demandasfies");
			$sucesso = $file->setUpload($_FILES ['file']['name'], '', true);
		}

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=entregas&dmeid=' . $modelDemandaEntrega->dmeid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;

	case 'apagar_entrega':

		$dmeid = (int)$modelDemandaEntrega->dmeid;
		$where = " dmeid = {$dmeid}";

		try {
			$demandaArquivos = $modelDemandaArquivo->recuperarTodos(' * ', array($where));

			$modelDemandaArquivo->excluirVarios($where);
			$modelDemandaArquivo->commit();

			if (is_array($demandaArquivos)) {
				foreach ($demandaArquivos as $demandaArquivo) {
					$arqid = (int)$demandaArquivo['arqid'];
					if ($arqid) {
						$arquivo = new FilesSimec();
						$arquivo->setPulaTableEschema('true');
						$arquivo->setRemoveUpload($arqid);
					}
				}
			}
			$modelDemandaObservacao->excluirVarios($where);
			$modelDemandaEntrega->excluir();

			$modelDemandaObservacao->commit();
			$modelDemandaEntrega->commit();
			$sucesso = true;
		} catch (Exception $e) {
			$sucesso = false;
			$modelDemandaObservacao->rollback();
			$modelDemandaEntrega->rollback();
		}

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=entregas';

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
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

	if ($_REQUEST['formulario'] || $modelDemandaEntrega->dmeid) {
		?>

		<div class="row">
			<div class="row col-md-11">
				<div class="row">
					<div class="row col-md-12">

						<?php if (!$complementoDisabled){ ?>
						<form id="form-save" method="post" class="form-horizontal">
							<?php } ?>
							<input name="controller" type="hidden" value="instituicao"> <input name="action" type="hidden" value="salvar"> <input name="esdid" id="esdid" type="hidden" value="<?= $estadoAtual['esdid']; ?>">
							<input name="dmdid" id="dmdid" type="hidden" value="<?= $modelDemanda->dmdid; ?>"> <input name="dmeid" id="dmeid" type="hidden" value="<?= $modelDemandaEntrega->dmeid; ?>">

							<div class="well">
								<fieldset>
									<legend>Dados Gerais</legend>

									<?php if ($modelDemandaEntrega->dmeid) { ?>
										<div class="form-group">
											<label for="dmdid" class="col-lg-2 col-md-2 control-label">Cód. Entrega:</label>

											<div class="col-lg-10 col-md-10 " style="padding-top: 5px;">
												<span style="color: red;"><?= $modelDemandaEntrega->dmeid; ?></span>
											</div>
										</div>
									<?php } ?>

									<div class="form-group">
										<label for="dmetipo" class="col-lg-3 col-md-3 control-label">Tipo:</label>

										<div class="col-lg-9 col-md-9 ">
											<select class="form-control" id="dmetipo" name="dmetipo">
												<?= $modelDemandaEntrega->getTipos(); ?>
											</select>
										</div>
									</div>

									<div class="form-group">
										<label for="dmerelocorrencia" class="col-lg-3 col-md-3 control-label">Possui relatório de ocorrência?</label>

										<div class="col-lg-9 col-md-9 ">
											<div class="btn-group" data-toggle="buttons">
												<label class="btn btn-default <?= ($modelDemandaEntrega->dmerelocorrencia == 't' ? 'active' : ''); ?> "> <input class="relocorrencia" type="radio"
																																								name="dmerelocorrencia" id="dmerelocorrencia1" value="t" <?=
													($modelDemandaEntrega->dmerelocorrencia == 't' ? 'checked="checked"' : ''); ?>> Sim
												</label> <label class="btn btn-default <?= ($modelDemandaEntrega->dmerelocorrencia == 'f' ? 'active' : ''); ?> "> <input class="relocorrencia" type="radio"
																																										 name="dmerelocorrencia" id="dmerelocorrencia2" value="f" <?= ($modelDemandaEntrega->dmerelocorrencia == 'f' ? 'checked="checked"' : ''); ?>> Não
												</label>
											</div>
										</div>
									</div>

									<div class="form-group tipo-judicial div-multa">
										<label for="dmdmultadsc" class="col-lg-3 col-md-3 control-label">Descrição:</label>

										<div class="col-lg-9 col-md-9 ">
											<input id="dmedsc" name="dmedsc" type="text"
												   class="form-control" placeholder=""
												   value="<?= $modelDemandaEntrega->dmedsc; ?>">
										</div>
									</div>

								</fieldset>

								<div>
									<button title="Salvar" class="btn btn-success" type="submit">
										<span class="glyphicon glyphicon-thumbs-up"></span> Salvar
									</button>
									<a title="Voltar" class="btn btn-danger" href="/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?= $modelDemanda->dmdid; ?>&aba=entregas"><span class="glyphicon glyphicon-hand-left"></span> Voltar</a>
									<?php if ($modelDemandaEntrega->dmeid) { ?>
										<a title="Nova Entrega" class="btn btn-primary" href="/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?= $modelDemanda->dmdid; ?>&aba=entregas&formulario=1"><span class="glyphicon glyphicon-plus"></span> Nova Entrega</a>
									<?php } ?>
								</div>
							</div>
							<?php if (!$complementoDisabled){ ?>
						</form>
					<?php } ?>

					</div>

					<?php if ($modelDemandaEntrega->dmeid) { ?>

						<div class="row col-md-6">
							<?php
							$modelDemandaObservacao->dmdid = $modelDemanda->dmdid;
							$modelDemandaObservacao->dmeid = $modelDemandaEntrega->dmeid;
							$modelDemandaObservacao->formularioEntregas();
							?>
						</div>

						<div class="row col-md-6">
							<?php
							$modelDemandaArquivo->dmdid = $modelDemanda->dmdid;
							$modelDemandaArquivo->dmeid = $modelDemandaEntrega->dmeid;
							$modelDemandaArquivo->formularioEntregas();
							?>
						</div>
					<?php } ?>
				</div>
			</div>
			<div class="row col-md-1">
				<?php
				if ($modelDemandaEntrega->docid) {
					wf_desenhaBarraNavegacao($modelDemandaEntrega->docid, array('dmdid' => $modelDemanda->dmdid, 'dmeid' => $modelDemandaEntrega->dmeid));
				} ?>
			</div>


			<div class="clearfix"></div>

		</div>
	<?php } else { ?>

		<div class="row">
			<div class="row col-md-12">
				<div style="margin-bottom: 10px;">
					<a title="Voltar" class="btn btn-primary" href="/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?= $modelDemanda->dmdid; ?>&aba=entregas&formulario=1"><span class="glyphicon glyphicon-plus"></span> Nova Entrega</a>
				</div>

				<form id="form-save" method="post" class="form-horizontal">

					<div class="well">
						<input name="action" type="hidden" value="pesquisar">

						<div class="form-group">
							<label for="dmdid" class="col-lg-2 control-label">Área:</label>

							<div class="col-lg-3 ">
								<?php $area = $_REQUEST['area'];
								echo $db->monta_combo("area", $areas, 'S', "Selecione", "", "", "", " ", "N", "", "", $area, '', '', 'form-control chosen'); ?>
							</div>
						</div>
					</div>
					<div class="text-right">
						<button title="Pesquisar" class="btn btn-success" type="submit"><span
								class="glyphicon glyphicon-search"></span> Pequisar
						</button>
					</div>
					<br>
				</form>
				<div class="clearfix"></div>

				<?php $modelDemandaEntrega->getLista($modelDemanda); ?>
			</div>
		</div>


	<?php } ?>
</div>


<script src="/library/jquery/jquery.form.min.js" type="text/javascript"></script>

<script type="text/javascript">
	$(document).ready(function () {

		habilitaRelatorio();
		$('#dmetipo').on('change', function () {
			habilitaRelatorio();
		});

		$('.cnpj').mask('99.999.999/9999-99');
		$('.moeda').mask('000.000.000.000.000,00', {reverse: true});
		$('.data').mask('99/99/9999');
		$('.data').datepicker();

	});

	function habilitaRelatorio() {
		if ($('#dmetipo').val() == 'ET') {
			$('.relocorrencia').removeAttr('checked').closest('label').addClass('disabled').removeClass('active');
		} else {
			$('.relocorrencia').removeAttr('checked').closest('label').removeClass('disabled');
		}
	}
	function editarEntrega(dmeid, dmdid) {
		window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=' + dmdid + '&aba=entregas&dmeid=' + dmeid;
	}
	function apagarEntrega(dmeid) {
		if (confirm('Deseja realmente excluir o registro?')) {
			window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?= $modelDemanda->dmdid; ?>&action=apagar_entrega&aba=entregas&dmeid=' + dmeid;
		}
	}
</script>