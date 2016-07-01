<?php include_once APPRAIZ . "demandasfies/modulos/relatorio/vencidasAvencerController.inc"; ?>
<div class="row">
	<div class="col-md-12">

		<div class="page-header">
			<h3 id="forms">Relatório Demandas Vencidas / à Vencer</h3>
		</div>

		<form id="form-save" method="post" class="form-horizontal">
			<input name="action" type="hidden" value="pesquisar">

			<div>
				<legend>Filtros</legend>

				<div class="col-md-8 ">

					<div class="form-group">
						<label for="dmdtipo" class="col-lg-4 col-md-4 control-label">Tipo:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen" id="dmdtipo" name="dmdtipo">
								<?= $modelDemanda->getTipos(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="claid" class="col-lg-4 col-md-4 control-label">Objeto:</label>

						<div class="col-lg-8 col-md-8 ">
							<select data-placeholder="Selecione" class="form-control chosen campo-judicial" id="claid" name="claid[]" multiple>
								<?= $modelDemanda->getClassificacao(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdstatusdecisao" class="col-lg-4 col-md-4 control-label">Status da Decisão:</label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group " data-toggle="buttons">
								<label class="btn btn-default  <?= (empty($dmdstatusdecisao) ? 'active' : ''); ?> ">
									<input type="radio" name="dmdstatusdecisao" value="">
									todos
								</label>

								<label class="btn btn-default  <?= ($dmdstatusdecisao == 'DF' ? 'active' : ''); ?> ">
									<input type="radio" name="dmdstatusdecisao" value="DF" <?= ($dmdstatusdecisao == 'DF' ? 'checked="checked"' : ''); ?>>
									Deferida
								</label>

								<label class="btn btn-default   <?= ($dmdstatusdecisao == 'ID' ? 'active' : ''); ?>">
									<input type="radio" name="dmdstatusdecisao" value="ID" <?= ($dmdstatusdecisao == 'ID' ? 'checked="checked"' : ''); ?>>
									Indeferida
								</label>

								<label class="btn btn-default   <?= ($dmdstatusdecisao == 'PD' ? 'active' : ''); ?>">
									<input type="radio" name="dmdstatusdecisao" value="PD" <?= ($dmdstatusdecisao == 'PD' ? 'checked="checked"' : ''); ?>>
									Parcialmente Deferida
								</label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdmulta" class="col-lg-4 col-md-4 control-label"> Multa </label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdmulta == 't' ? 'active' : ''); ?>"> <input type="radio" name="dmdmulta" id="dmdmulta1" value="t" <?= ($demanda->dmdmulta == 't' ? 'checked' : ''); ?> >Sim </label>
								<label class="btn btn-default <?= ($modelDemanda->dmdmulta == 'f' ? 'active' : ''); ?>"> <input type="radio" name="dmdmulta" id="dmdmulta2" value="f" <?= ($demanda->dmdmulta == 'f' ? 'checked' : ''); ?> >Não </label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdid" class="col-lg-4 ol-md-4 control-label">Situação:</label>

						<div class="col-lg-8 col-md-8">
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
								WHERE perf.pflcod = {$pfl_adv} AND ususis.sisid = {$sisid} ORDER BY usu.usunome
							";
							echo $db->monta_combo("usucpf", $sql, 'S', "Selecione", "", "", "", " ", "N", "usucpf", "", '', '', '', 'form-control chosen');
							?>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="trbid" class="col-lg-4 col-md-4 control-label">Tribunal de Origem:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control campo-judicial" id="trbid" name="trbid">
								<?= $modelDemanda->getTribunais(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="dmduf" class="col-lg-4 col-md-4 control-label">UF:</label>

						<div class="col-lg-5 col-md-5 ">
							<select class="form-control chosen" id="dmduf" name="dmduf">
								<?= $modelDemanda->getComboUfs(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdcumprimento" class="col-lg-4 col-md-4 control-label">Cumprimento:</label>

						<div class="col-lg-6 col-md-6">
							<select class="form-control chosen" id="dmdcumprimento" name="dmdcumprimento">
								<?= $modelDemanda->getComboComprimento(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="tiporelatorio" class="col-lg-4 col-md-4 control-label">Tipo Relatório:</label>

						<div class="col-lg-8 col-md-8">
							<select name="tiporelatorio" class="form-control tiporelatorio">
								<option <?= (empty($tiporelatorio) ? 'selected' : '') ?> value="">Todos</option>
								<option <?= ($tiporelatorio == 'V' ? 'selected' : '') ?> value="V">Vencidas</option>
								<option <?= ($tiporelatorio == 'AV' ? 'selected' : '') ?> value="AV">à vencer</option>
							</select>
						</div>
					</div>

					<fieldset <?= ($tiporelatorio == 'ID' ? '' : 'style="display: none"') ?> id="f_periodo">
						<legend>
							<small>Periodo</small>
						</legend>
						<div class="form-group">
							<label for="dt_inicio" class="col-lg-4 col-md-4 control-label">Data Inicio</label>

							<div class="col-lg-5 col-md-5">
								<input id="dt_inicio" name="dt_inicio" type="text" class="form-control" placeholder="" value="<?= $dt_inicio; ?>">
							</div>
						</div>

						<div class="form-group">
							<label for="dt_fim" class="col-lg-4 col-md-4 control-label">Data Fim</label>

							<div class="col-lg-5 col-md-5">
								<input id="dt_fim" name="dt_fim" type="text" class="form-control" placeholder="" value="<?= $dt_fim; ?>">
							</div>
						</div>
					</fieldset>

					<hr>
					<div class="text-right">
						<button title="Salvar" class="btn btn-success" type="submit">
							<span class="glyphicon glyphicon-search"></span> Pequisar
						</button>
						<button name="btn_relatorio" value="relatorio" class="btn btn-info" type="submit" title="gerar relatorio PDF">
							<span class="glyphicon glyphicon-book"></span> Gerar Relatório PDF
						</button>
					</div>

				</div>
			</div>
		</form>
	</div>
</div>

<hr>

<div class="row">
	<div class="col-md-12">
		<div class="text-left">
			<button id="exibir_entrega" title="Exibir Todas Entregas" class="btn btn-default btn-sm" type="button">
				<span class="glyphicon glyphicon-resize-full"></span> exibir / esconder entregas
			</button>
		</div>
	</div>
</div>


<br>

<div class="row">
	<div class="col-md-12" style="font-size: 11px;">
		<?php
		$tabelaArvore = new HTML_Table('relaorio_html_demandas_fies', 'table table-bordered tabela-listagem tabela-demanda');
		$dados = recuperarListagem($modelDemanda, $modelDemandaClassificacao);
		setHeader($camposDaTabela, $tabelaArvore);
		setBody($dados, $camposDaTabela, $tabelaArvore);
		echo $tabelaArvore->display();
		?>
	</div>
</div>

<script type="text/javascript">
	$(document).ready(function () {
		verEstado();
		$('.td_entregas').hide();
		$('#exibir_entrega').on('click', function () {
			$('.td_entregas').toggle();
		});

		$('.data,#dt_fim, #dt_inicio').mask('99/99/9999');
		$('#dt_fim, #dt_inicio,.data ').datepicker();

		$('.tiporelatorio').on('change', function () {
			verEstado();
		});
	});

	function verEstado() {
		if ($('.tiporelatorio').val() == 'AV') {
			$('#f_periodo').show();
		} else {
			$('#f_periodo').hide();
			$('#dt_inicio').val('');
			$('#dt_fim').val('');
		}
	}
</script>

<style type="text/css">
	.form-group {
		margin-bottom: 7px;
	}

	.tabela-demanda td, {
		background-color: #f4f4f4;
	}

	.tabela-entrega {
		margin-left: 15px;;
		width: 99%;
	}

	.tabela-entrega td {
		background-color: #e3eaea;
	}

	.th_entrega {
		background-color: #cecece;
	}

	.th_demanda {
		background-color: #bebebe;
	}
</style>
<script type="text/javascript">
	$(document).ready(function () {
		verAdvogados();

		$('#esdid').on('change', function () {
			verAdvogados();
		});
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
</script>