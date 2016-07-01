<?
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
//Chamada de programa
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaPartesAcao.inc.php";
include_once APPRAIZ . "demandasfies/classes/DemandaEntrega.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaArquivo.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaPartesAcao = new DemandaPartesAcao($_REQUEST['dpaid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);
$modelDemandaArquivo = new DemandaArquivo($_REQUEST['dmaid']);

$perfis = $_SESSION['perfilusuario'][$_SESSION['usucpf']];


//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');

$demanda = new Demanda();
$demandaPartesAcao = new DemandaPartesAcao();

if ($_POST['action'] == 'pesquisar') {
	$demanda->popularDadosObjeto();
	$demandaPartesAcao->popularDadosObjeto();
	$esdid_entrega = $_POST['esdid_entrega'];
	$area = $_POST['area'];
	$usucpf = $_POST['usucpf'];
}
$esdid = $_REQUEST['esdid'];

$areas = getAreas();

?>
<style type="text/css">
	.form-group {
		margin-bottom: 7px;
	}

	p {
		margin-bottom: 0 !important;
	}

	.marcado {
		background-color: #C1FFC1 !important
	}

	.remover {
		display: none;
	}
</style>

<div class="row">
	<div class="col-md-12">
		<div class="page-header">
			<h3 id="forms">Pesquisa de Demandas</h3>
		</div>

		<form id="form-save" method="post" class="form-horizontal">
			<div class="col-md-6 ">
				<div class="well">
					<input name="action" type="hidden" value="pesquisar">

					<div class="form-group">
						<label for="dmdid" class="col-lg-4 col-md-4 control-label">Código:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdid" name="dmdid" type="text" class="form-control" placeholder="" value="<?php echo $demanda->dmdid; ?>">
						</div>
					</div>

					<div class="form-group">
						<label for="dmdid" class="col-lg-4 col-md-4 control-label">Situação:</label>

						<div class="col-lg-8 col-md-8 ">
							<?php
							$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 201 order by esdordem;";
							echo $db->monta_combo("esdid", $sql, 'S', "Selecione", "", "", "", " ", "N", "esdid", "", '', '', '', 'form-control chosen');
							?>
						</div>
					</div>

					<div class="form-group" id="div_advogado">
						<label for="dmdid" class="col-lg-4 col-md-4 control-label">Advogados:</label>

						<div class="col-lg-8 col-md-8 ">
							<?php
							$sisid = SIS_DEMANDASFIES;
							$pfl_adv = PFL_ADVOGADO;
							$sql = "
							SELECT usu.usucpf as codigo, usu.usunome as descricao FROM seguranca.usuario AS usu
								INNER JOIN seguranca.usuario_sistema AS ususis ON ususis.usucpf = usu.usucpf
								INNER JOIN seguranca.perfilusuario AS perf ON perf.usucpf = usu.usucpf
								WHERE perf.pflcod = {$pfl_adv} AND ususis.sisid = {$sisid}  ORDER BY usu.usunome
							";
							echo $db->monta_combo("usucpf", $sql, 'S', "Selecione", "", "", "", " ", "N", "usucpf", "", '', '', '', 'form-control chosen');
							?>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdid" class="col-lg-4 col-md-4 control-label">Área:</label>

						<div class="col-lg-8 col-md-8 ">
							<?php echo $db->monta_combo("area", $areas, 'S', "Selecione", "", "", "", " ", "N", "", "", '', '', '', 'form-control chosen'); ?>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdmandatoseguranca" class="col-lg-4 col-md-4 control-label"> Mandado de Segurança: </label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default "> <input type="radio" name="dmdmandatoseguranca" id="dmdmandatoseguranca1" value="t">Sim </label> <label class="btn btn-default">
									<input type="radio" name="dmdmandatoseguranca" id="dmdmandatoseguranca2" value="f">Não </label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdliminar" class="col-lg-4 col-md-4 control-label"> Decisão </label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($demanda->dmdliminar == 't' ? 'active' : ''); ?>"> <input type="radio" name="dmdliminar" id="dmdliminar1" value="t" <?= ($demanda->dmdliminar == 't' ? 'checked' : ''); ?> >Sim
								</label> <label class="btn btn-default <?= ($demanda->dmdliminar == 'f' ? 'active' : ''); ?>"> <input type="radio" name="dmdliminar" id="dmdliminar2" value="f" <?= ($demanda->dmdliminar == 'f' ? 'checked' : ''); ?> >Não
								</label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="esdid_entrega" class="col-lg-4 col-md-4 control-label"> Entregas Aguardando Aprovação </label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($esdid_entrega == 't' ? 'active' : ''); ?>"> <input type="radio" name="esdid_entrega" id="esdid_entrega1" value="t" <?= ($esdid_entrega == 't' ? 'checked' : ''); ?> >Sim </label>
								<label class="btn btn-default <?= ($esdid_entrega == 'f' ? 'active' : ''); ?>"> <input type="radio" name="esdid_entrega" id="esdid_entrega2" value="f" <?= ($esdid_entrega == 'f' ? 'checked' : ''); ?> >Não </label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdmulta" class="col-lg-4 col-md-4 control-label"> Multa </label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($demanda->dmdmulta == 't' ? 'active' : ''); ?>"> <input type="radio" name="dmdmulta" id="dmdmulta1" value="t" <?= ($demanda->dmdmulta == 't' ? 'checked' : ''); ?> >Sim </label>
								<label class="btn btn-default <?= ($demanda->dmdmulta == 'f' ? 'active' : ''); ?>"> <input type="radio" name="dmdmulta" id="dmdmulta2" value="f" <?= ($demanda->dmdmulta == 'f' ? 'checked' : ''); ?> >Não </label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdprazo" class="col-lg-4 col-md-4 control-label">Data de Prazo:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdprazo" name="dmdprazo" type="text" class="data form-control" placeholder="" value="<?= $demanda->dmdprazo; ?>">
						</div>
					</div>

					<div class="form-group">
						<label for="dmdprocesso" class="col-lg-4 col-md-4 control-label">Nº Processo:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdprocesso" name="dmdprocesso" type="text" class="form-control" placeholder="" value="<?= $demanda->dmdprocesso; ?>">
						</div>
					</div>

				</div>
			</div>

			<div class="col-md-6">
				<div class="well">

					<fieldset>
						<legend>Partes (autor/réu)</legend>

						<div class="form-group">
							<label id="dpacpfcnpj_autor_label" class="col-lg-4 col-md-4 control-label">CPF:</label>

							<div class="col-lg-8 col-md-8 ">
								<input id="dpacpf_autor" name="dpacpf" type="text" class="form-control cpf" placeholder="" value="<?= $demandaPartesAcao->dpacpf; ?>">
							</div>
						</div>

						<div class="form-group">
							<label id="dpacpfcnpj_autor_label" class="col-lg-4 col-md-4 control-label">CNPJ:</label>

							<div class="col-lg-8 col-md-8 ">
								<input id="dpacnpj_autor" name="dpacnpj" type="text" class="form-control cnpj" placeholder="" value="<?= $demandaPartesAcao->dpacnpj; ?>">
							</div>
						</div>

						<div class="form-group">
							<label for="dmddsc" class="col-lg-4 col-md-4 control-label">Nome:</label>

							<div class="col-lg-8 col-md-8 ">
								<input id="dpanome_autor" name="dpanome" type="text" class="form-control" placeholder="" value="<?= $demandaPartesAcao->dpanome; ?>">
							</div>
						</div>

					</fieldset>
				</div>
			</div>
			<div class="text-right">
				<button title="Salvar" class="btn btn-success" type="submit"><span
						class="glyphicon glyphicon-search"></span> Pequisar
				</button>
			</div>
			<br>
		</form>

		<div class="clearfix"></div>

		<div style="font-size: 12px;">
			<?php $modelDemanda->recuperarListagem(); ?>
		</div>

	</div>
	<script type="text/javascript">
		$(document).ready(function () {
			verAdvogados();
			verTipoEntrega();

			$('#esdid').on('change', function () {
				verAdvogados();
				verTipoEntrega();
			});

			$('.cpf').mask('999.999.999-99');
			$('.cnpj').mask('99.999.999/9999-99');
			$('.moeda').mask('000.000.000.000.000,00', {reverse: true});
			$('.data').mask('99/99/9999');
			$('.data').datepicker();
		});


		function verAdvogados() {
			if ($('#esdid').val() == <?= ESD_DEMANDA_ADVOGADO ?>) {
				$("#div_advogado").show();
				$("#usucpf_chosen").width(461);
			} else {
				$("#usucpf").val('');
				$("#div_advogado").hide();
			}
		}

		function verTipoEntrega() {
			if ($('#esdid').val() == <?= ESD_DEMANDA_NUCLEO_JURIDICO ?> || $('#esdid').val() == <?= ESD_DEMANDA_ADVOGADO ?> || $('#esdid').val() == <?= ESD_GERENCIA_DIGEF ?>) {
				$("#div_tipo_entrega").show();
			} else {
				$("radio[name='dmetipo']").prop('checked', false);
				$("#div_tipo_entrega").hide();
			}
		}
	</script>
</div>