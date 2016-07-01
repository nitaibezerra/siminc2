<br>
<form class="form-horizontal" method="post" id="form-solucao">
	<fieldset>
		<div class="row">

			<input type="hidden" value="<?= $this->solucao->getAttributeValue('solid'); ?>" name="solid" id="solid"> <input type="hidden" value="pesquisar" name="action">

			<div class="form-group">
				<label class="col-lg-1 control-label" for="solnumero">
					<?= $this->solucao->getAttributeLabel('solnumero'); ?>
				</label>

				<div class="col-lg-3">
					<input type="text" value="<?= $this->solucao->getAttributeValue('solnumero'); ?>" class="form-control" name="solnumero" id="solnumero">
				</div>
			</div>

			<div class="form-group">
				<label class="col-lg-1 control-label" for="soldsc">
					<?= $this->solucao->getAttributeLabel('soldsc'); ?>
					<span class="alert-danger" style="background-color: #FFF; font-size: 19px; ">*</span> </label>

				<div class="col-lg-6">
					<input type="text" value="<?= $this->solucao->getAttributeValue('soldsc'); ?>" placeholder="" class="form-control" name="soldsc" id="soldsc">
				</div>
			</div>

			<div class="form-group">
				<label class="col-lg-1 control-label" for="solapelido">
					<?= $this->solucao->getAttributeLabel('solapelido'); ?>
				</label>

				<div class="col-lg-5">
					<input type="text" value="<?= $this->solucao->getAttributeValue('solapelido'); ?>" placeholder="" class="form-control" name="solapelido" id="solapelido">
				</div>
			</div>

			<div class="form-group">
				<label class="col-lg-1 control-label" for="solprazo">
					<?= $this->solucao->getAttributeLabel('solprazo'); ?>
				</label>

				<div class="col-lg-2">
					<input type="text" value="<?= $this->solucao->getAttributeValue('solprazo'); ?>" placeholder="" class="form-control" name="solprazo" id="solprazo">
				</div>
			</div>

			<div class="form-group">
				<label class="col-lg-1 control-label" for="solobs">
					<?= $this->solucao->getAttributeLabel('solobs'); ?>
				</label>

				<div class="col-lg-10">
					<textarea rows="4" class="form-control" name="solobs" id="solobs"><?= $this->solucao->getAttributeValue('solobs'); ?></textarea>
				</div>
			</div>

			<div class="form-group well">
				<h4>
					<label for="temid"><?= $this->tema->getAttributeLabel('temdsc'); ?></label>
					<span class="alert-danger" style="background-color: #FFF; font-size: 19px; ">*</span>
				</h4>

				<div class="col-lg-12">
					<select class="form-control" name="temid[]" id="temid" multiple="multiple">
						<?= $this->tema->getOptionsTema(); ?>
					</select>
				</div>
			</div>

			<!--	Ações Estratégicas -->
			<div class="form-group well div_acoes_estrategica">
				<?php require_once('formularioAcoesEstrategica.php'); ?>
			</div>

			<!--	Dispositivos PNE -->
			<div class="form-group well div_dispositivo_pne" id="div_metaSolucao">
				<?php require_once('formularioMetaSolucao.php'); ?>
			</div>
			<br>

			<div class="form-group" well <?= ($this->solucao->getAttributeValue('solmetajustificativa') ? '' : 'style="display: none;"') ?> id="div_justificativa">
				<label class="col-lg-2 control-label" for="solmetajustificativa">Justificativa <span class="alert-danger" style="background-color: #FFF; font-size: 19px; ">*</span></label>

				<div class="col-lg-10">
					<textarea class="form-control" id="solmetajustificativa" name="solmetajustificativa"><?= $this->solucao->getAttributeValue('solmetajustificativa'); ?></textarea>
				</div>
			</div>

            <!--	Artigos -->
            <?php
                $mpneids = $this->metaSolucao->getAttributeValue('mpneid');
                $displayArtigo = (array_search( Model_Metasolucao::CORPO_LEI_ID, $mpneids) === false ? ' display: none;' : '');
            ?>
            <div class="form-group well div_artigo" style=" <?php echo $displayArtigo; ?>">
                <?php require_once('formularioArtigo.php'); ?>
            </div>

			<!--	Estratégia -->
			<div class="form-group well div_estrategia">
				<?php require_once('formularioEstrategia.php'); ?>
			</div>

			<!--	Objetivos Estratégicos -->
			<div class="form-group well div_objetivo_estrategico">
				<?php require_once('formularioObjetivoEstrategico.php'); ?>
			</div>

			<!--	Iniciativas -->
			<div class="form-group well div_iniciativa">
				<?php require_once('formularioIniciativa.php'); ?>
			</div>

			<!--	Indicador -->
			<div class="form-group well div_indicador">
				<?php require_once('formulario_indicador.php'); ?>
			</div>
			<br>

			<div class="form-group well">
				<h4><label for="resid_se">Responsável SE</label></h4>

				<div class="col-lg-12">
					<select class="form-control" name="resid_se[]" id="resid_se" multiple="multiple">
						<?= $this->responsavelSolucaoSe->getOptionsResponsavelSe(); ?>
					</select>
				</div>
			</div>
			<br>

			<div class="form-group well">
				<h4><label for="resid_se_au">Responsável Secretaria/Autarquia</label></h4>

				<div class="col-lg-12">
					<select class="form-control" name="resid_se_au[]" id="resid_se_au" multiple="multiple">
						<?= $this->responsavelSolucaoSeAut->getOptionsResponsavelSe(); ?>
					</select>
				</div>
			</div>

			<div class="form-group well">
				<h4><label for="secid">Áreas Envolvidas</label></h4>

				<div class="col-lg-12">
					<select class="form-control" name="secid[]" id="secid" multiple="multiple">
						<?= $this->secretaria->getOptionsSecretaria(); ?>
					</select>
				</div>
			</div>

		</div>

		<?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
			<div class="text-right">
				<span class="help-block">Campos com <span style="font-size: 19px; ">*</span> são obrigatórios.</span>
				<button type="button" class="btn btn-success" title="Salvar" id="btn_salvar_solucao">
					<span class="glyphicon glyphicon-ok"></span> Cadastrar
				</button>
			</div>
		<?php endif; ?>

		<br>
	</fieldset>
</form>

<script type="text/javascript">
	$(function () {
		$('#solprazo').datepicker();
		$("#solprazo").mask("99/99/9999");

		$('#temid, #resid_se, #resid_se_au, #secid').multiSelect({
			keepOrder: true,
			selectableHeader: "<h5 style='margin: 7px 0 5px 0; font-weight: bold'>SELECIONAR</h5><div class='input-group'><input type='text' class='form-control input-sm selectableSearch' autocomplete='off' placeholder='pesquisar itens'><div class='input-group-addon'><span class='glyphicon glyphicon-search'></span></div></div>",
			selectionHeader: "<h5 style='margin: 5px 0 5px 0; font-weight: bold'>SELECIONADOS</h5><div class='input-group'><input type='text' class='form-control input-sm selectionSearch' autocomplete='off' placeholder='pesquisar itens selecionados'><div class='input-group-addon'><span class='glyphicon glyphicon-search'></span></div></div>",
			afterInit: function (ms) {
				if ($('#solmetajustificativa').val().length > 0) {
					$('#mpneid option :first').attr('selected', 'selected');
					$('#mpneid').multiSelect('select', ['nenhuma']);
				}

				var that = this,
					$selectableSearch = that.$selectableUl.prev().children('input'),
					$selectionSearch = that.$selectionUl.prev().children('input'),
					selectableSearchString = '#' + that.$container.attr('id') + ' .ms-elem-selectable:not(.ms-selected)',
					selectionSearchString = '#' + that.$container.attr('id') + ' .ms-elem-selection.ms-selected';

				that.qs1 = $selectableSearch.quicksearch(selectableSearchString).on('keydown', function (e) {
					if (e.which === 40) {
						that.$selectableUl.focus();
						return false;
					}
				});
				that.qs2 = $selectionSearch.quicksearch(selectionSearchString).on('keydown', function (e) {
					if (e.which == 40) {
						that.$selectionUl.focus();
						return false;
					}
				});
			}, afterSelect: function (value) {

                if(this.$element.attr('id') == 'temid'){
                    filtroTema(value, 'hide');
                }

				return false;

			}, afterDeselect: function (value) {
                if(this.$element.attr('id') == 'temid'){
                    filtroTema(value, 'show');
                }
			}
		});

		var spans = $('.ms-list > li > span');
		$.each(spans, function (key, value) {
			var str = $(this).text().substring(0, 90);
			if ($(this).text().length > 90) {
				str += ' ... ';
			}
			$(this).text(str);
		});

		<?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
		/*** SALVA O FORMULARIO SOLUCAO ***/
		$('#btn_salvar_solucao').on('click', function () {
			$('#form-solucao').saveAjax({action: 'salvar', controller: 'default', retorno: true, displayErrorsInput: true, functionSucsess: 'aposInserirSolucao'});
		});
		<?php endif; ?>

        regrasAoCarregar();
	});





</script>