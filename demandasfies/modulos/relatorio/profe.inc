<?php include_once APPRAIZ . "demandasfies/modulos/relatorio/profeController.inc"; ?>

<div class="row">
	<div class="col-md-12">

		<div class="page-header">
			<h3 id="forms">Relatório PROFE</h3>
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

					<div class="form-group dmdstatusdecisao">
						<label for="dmdstatusdecisao" class="col-lg-4 col-md-4 control-label">Status da Decisão:</label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group " data-toggle="buttons">
								<label class="btn btn-default  <?= (empty($dmdstatusdecisao) ? 'active' : ''); ?> "> <input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao1" value=""> todos </label>
								<label class="btn btn-default  <?= ($dmdstatusdecisao == 'DF' ? 'active' : ''); ?> ">
									<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao1" value="DF" <?= ($dmdstatusdecisao == 'DF' ? 'checked="checked"' : ''); ?>> Deferida </label>
								<label class="btn btn-default   <?= ($dmdstatusdecisao == 'ID' ? 'active' : ''); ?>">
									<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao2" value="ID" <?= ($dmdstatusdecisao == 'ID' ? 'checked="checked"' : ''); ?>> Indeferida </label>
								<label class="btn btn-default   <?= ($dmdstatusdecisao == 'PD' ? 'active' : ''); ?>">
									<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao3" value="PD" <?= ($dmdstatusdecisao == 'PD' ? 'checked="checked"' : ''); ?>> Parcialmente Deferida </label>
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

					<div class="well">
						<div class="form-group">
							<label for="dmerelocorrencia" class="col-lg-4 col-md-4 control-label">Estado da Entrega</label>

							<div class="col-lg-5 col-md-5 ">
								<select id="pendecia" name="pendecia" class="form-control">
									<option <?= (empty($pendecia) ? 'selected' : '') ?> value="">Todos</option>
									<option <?= ($pendecia == ESD_ENTREGA_EM_ELABORACAO ? 'selected' : '') ?> value="<?= ESD_ENTREGA_EM_ELABORACAO ?>">Em Elaboração</option>
									<option <?= ($pendecia == ESD_ENTREGA_AGUARDANDO_APROVACAO ? 'selected' : '') ?> value="<?= ESD_ENTREGA_AGUARDANDO_APROVACAO ?>">Aguardando aprovação Núcleo Jurídico</option>
									<option <?= ($pendecia == ESD_ENTREGA_APROVADA ? 'selected' : '') ?> value="<?= ESD_ENTREGA_APROVADA ?>">Aprovada</option>
									<option <?= ($pendecia == ESD_ENTREGA_AGUARDANDO_APROVACAO_DIGEF ? 'selected' : '') ?> value="<?= ESD_ENTREGA_AGUARDANDO_APROVACAO_DIGEF ?>">Aguardando Aprovação Gerência DIGEF</option>
								</select>
							</div>
						</div>


						<fieldset <?= ($pendecia == ESD_ENTREGA_APROVADA ? '' : 'style="display: none"') ?> id="f_periodo">
							<legend>
								<small>Periodo de aprovação</small>
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
					</div>

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
		$dadosFiltrado = recuperarListagem($modelDemanda, $modelDemandaClassificacao);
        echo setBody($dadosFiltrado, $camposDaTabela);
		?>
	</div>
</div>

<script type="text/javascript">
	$(document).ready(function () {
		verAdvogados();
		verTipoEntrega();
		verEstado();

		$('.td_entregas').hide();

		$('#exibir_entrega').on('click', function () {
			$('.td_entregas').toggle();
		});

		$('#esdid').on('change', function () {
			verAdvogados();
			verTipoEntrega();
		});

		$('.cpf').mask('999.999.999-99');
		$('.cnpj').mask('99.999.999/9999-99');
		$('.data,#dt_fim, #dt_inicio').mask('99/99/9999');

		$('#dt_fim, #dt_inicio,.data ').datepicker();

		$('#pendecia').on('change', function () {
			verEstado();
		});


	});

	function verEstado() {
		if ($('#pendecia').val() == <?= ESD_ENTREGA_APROVADA ?>) {
			$('#f_periodo').show();
		} else {
			$('#f_periodo').hide();
			$('#dt_inicio').val('');
			$('#dt_fim').val('');
		}
	}


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