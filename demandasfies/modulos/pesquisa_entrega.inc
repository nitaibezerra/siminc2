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
$perfis = $_SESSION['perfilusuario'][$_SESSION['usucpf']];

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');

$demanda = new Demanda();
$modelDemandaEntrega = new DemandaEntrega();
$areas = getAreas();

$dt_inicio = ($_POST['dt_inicio']);
$dt_fim = ($_POST['dt_fim']);

if ($_POST['action'] == 'pesquisar') {
	$modelDemandaEntrega->popularDadosObjeto();
	$dmetipo = $_POST['dmetipo'];
	$pendecia = $_POST['pendecia'];
	$dmerelocorrencia = $modelDemandaEntrega->dmerelocorrencia;
}
?>

<div class="row">
	<div class="col-md-12">
		<div class="page-header">
			<h3 id="forms">Pesquisar Entregas</h3>
		</div>

		<div class="col-lg-offset-3 col-lg-6 col-md-6 ">
			<div class="well">
				<form id="form-pesquisar-entrega" method="post" class="form-horizontal">
					<input name="action" type="hidden" value="pesquisar">

					<div class="form-group tipo-judicial div-multa">
						<label for="dmdmultadsc" class="col-lg-5 col-md-5 control-label">Descrição:</label>

						<div class="col-lg-7 col-md-7">
							<input id="dmedsc" name="dmedsc" type="text" class="form-control" placeholder="" value="<?= $modelDemandaEntrega->dmedsc; ?>">
						</div>
					</div>

					<div class="form-group" id="div_tipo_entrega">
						<label for="dmetipo" class="col-lg-5 col-md-5 control-label"> Tipo Entrega: </label>

						<div class="col-lg-7 col-md-7 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($dmetipo == 'EP' ? 'active' : ''); ?>"> <input type="radio" name="dmetipo" id="dmetipo1" value="EP" <?= ($dmetipo == 'EP' ? 'checked' : ''); ?>>Parcial </label>
								<label class="btn btn-default <?= ($dmetipo == 'ET' ? 'active' : ''); ?>"> <input type="radio" name="dmetipo" id="dmetipo2" value="ET" <?= ($dmetipo == 'ET' ? 'checked' : ''); ?>>Total </label>
								<label class="btn btn-default <?= (empty($dmetipo) ? 'active' : ''); ?>"> <input type="radio" name="dmetipo" id="dmetipo3" value="" <?= (empty($dmetipo) ? 'checked' : ''); ?>> Ambos </label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmerelocorrencia" class="col-lg-5 col-md-5 control-label">Possui relatório de ocorrência?:</label>

						<div class="col-lg-4 col-md-4 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($dmerelocorrencia == 't' ? 'active' : ''); ?> ">
									<input class="campo-decisao" type="radio" name="dmerelocorrencia" id="dmerelocorrencia1" value="t" <?= ($dmerelocorrencia == 't' ? 'checked="checked"' : ''); ?>> Sim </label>
								<label class="btn btn-default <?= (($dmerelocorrencia == 'f') ? 'active' : ''); ?> ">
									<input class="campo-decisao" type="radio" name="dmerelocorrencia" id="dmerelocorrencia2" value="f" <?= (($dmerelocorrencia == 'f') ? 'checked="checked"' : ''); ?>> Não </label>
								<label class="btn btn-default <?= (empty($dmerelocorrencia) ? 'active' : ''); ?>"> <input type="radio" name="dmerelocorrencia" id="dmerelocorrencia3" value="" <?= (empty($dmerelocorrencia) ? 'checked' : ''); ?>> Ambos
								</label>

							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="acjid" class="col-lg-5 col-md-5 control-label">Tipo de Ação da Demanda:</label>

						<div class="col-lg-4 col-md-4 ">
							<select class="form-control chosen campo-judicial" id="acjid" name="acjid">
								<?php echo $modelDemanda->getTipoAcao(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="dmerelocorrencia" class="col-lg-5 col-md-5 control-label">Estado da Entrega</label>

						<div class="col-lg-7 col-md-7 ">
							<select id="pendecia" name="pendecia" class="form-control">
								<option <?= (empty($pendecia) ? 'selected' : '') ?> value="">Todos</option>
								<option <?= ( $pendecia == ESD_ENTREGA_EM_ELABORACAO ? 'selected' : '') ?> value="<?= ESD_ENTREGA_EM_ELABORACAO ?>">Em Elaboração</option>
								<option <?= ( $pendecia == ESD_ENTREGA_AGUARDANDO_APROVACAO  ? 'selected' : '') ?> value="<?= ESD_ENTREGA_AGUARDANDO_APROVACAO ?>">Aguardando aprovação Núcleo Jurídico</option>
								<option <?= ( $pendecia == ESD_ENTREGA_APROVADA   ? 'selected' : '') ?> value="<?= ESD_ENTREGA_APROVADA ?>">Aprovada</option>
								<option <?= ( $pendecia == ESD_ENTREGA_AGUARDANDO_APROVACAO_DIGEF  ? 'selected' : '') ?> value="<?= ESD_ENTREGA_AGUARDANDO_APROVACAO_DIGEF ?>">Aguardando Aprovação Gerência DIGEF</option>
							</select>
						</div>
					</div>

					<fieldset <?= ( $pendecia == ESD_ENTREGA_APROVADA  ? '' : 'style="display: none"') ?> id="f_periodo">
						<legend><small>Periodo de aprovação</small></legend>
						<div class="form-group">
							<label for="dt_inicio" class="col-lg-5 col-md-5 control-label">Data Inicio</label>

							<div class="col-lg-7 col-md-7">
								<input id="dt_inicio" name="dt_inicio" type="text" class="form-control" placeholder="" value="<?= $dt_inicio; ?>">
							</div>
						</div>

						<div class="form-group">
							<label for="dt_fim" class="col-lg-5 col-md-5 control-label">Data Fim</label>

							<div class="col-lg-7 col-md-7">
								<input id="dt_fim" name="dt_fim" type="text" class="form-control" placeholder="" value="<?= $dt_fim; ?>">
							</div>
						</div>
					</fieldset>

					<div class="text-right">
						<button title="Salvar" class="btn btn-success" type="submit">
							<span class="glyphicon glyphicon-search"></span> Pequisar
						</button>
					</div>
				</form>
			</div>
		</div>

		<div class="clearfix"></div>

		<?php $modelDemandaEntrega->getLista(); ?>
	</div>
</div>
<script type="text/javascript">
	function editarEntrega(dmeid, dmdid) {
		window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=' + dmdid + '&aba=entregas&dmeid=' + dmeid;
	}
	$(function(){
		$('#dt_inicio').datepicker();
		$('#dt_fim').datepicker();
		$('.navbar').css({zIndex: "1"});
		$('.navbar-fixed-top').css({zIndex: "3"});


		$('#pendecia').on('change',function(){
			if($(this).val()  == <?= ESD_ENTREGA_APROVADA ?> ){
				$('#f_periodo').show();
			}else{
				$('#f_periodo').hide();
				$('#dt_inicio').val('');
				$('#dt_fim').val('');
			}
		})
	})
</script>