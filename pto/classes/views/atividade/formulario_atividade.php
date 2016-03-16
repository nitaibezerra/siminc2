<br>
<div class="row">
    <div class="col-lg-12">
        <form class="form-horizontal" method="post" id="form-atividade">
            <fieldset>
                <legend>Cadastrar Atividade(s)</legend>
                <input type="hidden" name="atvid" id="atvid" value="<?= $this->atividade->getAttributeValue('atvid'); ?>">
                <input type="hidden" name="docid" id="docid" value="<?= $this->atividade->getAttributeValue('docid'); ?>">
                <input type="hidden" name="usucpf" class="usucpf_atividade" value="<?= $this->atividade->getAttributeValue('usucpf'); ?>">

                <div class="form-group">
                    <label class="col-lg-2 control-label" for="atvdsc">
                        <?= $this->atividade->getAttributeLabel('atvdsc'); ?>
                        <span class="alert-danger" style="background-color: #FFF; font-size: 19px; ">*</span>
                    </label>

                    <div class="col-lg-6">
                        <input type="text" value="<?= $this->atividade->getAttributeValue('atvdsc'); ?>" placeholder="" class="form-control" name="atvdsc" id="atvdsc">
                    </div>
                </div>

                <div class="form-group">
                    <label class="col-lg-2 control-label" for="atvprazo"><?= $this->atividade->getAttributeLabel('atvprazo'); ?> </label>

                    <div class="col-lg-2">
                        <input type="date" value="<?= $this->atividade->getAttributeValue('atvprazo'); ?>" placeholder="" class="form-control" name="atvprazo" id="atvprazo">
                    </div>
                </div>

				<div class="form-group">
					<label class="col-lg-2 control-label" for="atvprazo"><?= $this->atividade->getAttributeLabel('atvcritico'); ?> </label>

					<div class="col-lg-2">
						<div class="checkbox">
							<label>
								<input type="checkbox" value="t" name="atvcritico" id="atvcritico" <?= ($this->atividade->getAttributeValue('atvcritico') == 't' ? 'checked' :''); ?>>
							</label>
						</div>
					</div>
				</div>

                <div class="form-group">
                    <label class="col-lg-2 control-label" for="atvobs">
                        <?= $this->atividade->getAttributeLabel('atvobs'); ?>
                    </label>

                    <div class="col-lg-8">
                        <textarea rows="4" class="form-control" name="atvobs" id="atvobs"><?= $this->atividade->getAttributeValue('atvobs'); ?></textarea>
                    </div>
                </div>

                <div class="form-group">
                    <div class="col-lg-offset-2 col-lg-10">
                        <?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
                            <button type="button" class="btn btn-default" id="bt_adicionar_executor">
                                <span class="glyphicon glyphicon-wrench"></span> Adicionar Executor
                            </button>
                        <?php endif; ?>
                        <span id="span_nome_executor" style="<?= (empty($this->executor) ? 'display: none;' : ''); ?>">
                            <span id="nome_executor"><?= $this->executor ?> </span>
                            <?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
                                <button type="button" data-etapa="executor" class="btn btn-default bt_remover_executor" title="desvincular executor">
                                    <span class="glyphicon glyphicon-trash"></span>
                                </button>
                            <?php endif; ?>
                        </span>
                    </div>
                </div>

                <?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
                    <div class="text-right">
                        <span class="help-block">Campos com <span style="font-size: 19px; ">*</span> são obrigatórios.</span>
                        <button type="button" class="btn btn-success" title="Salvar" id="btn_salvar_atividade">
                            <span class="glyphicon glyphicon-ok"></span> Cadastrar
                        </button>
                        <button type="reset" class="btn btn-default" title="Limpar" id="btn_limpar_atividade">
                            <span class="glyphicon glyphicon-repeat"></span> Limpar
                        </button>
                    </div>
                <?php endif; ?>
                <br>
            </fieldset>
        </form>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        $('#atvprazo').datepicker();
        $("#atvprazo").mask("99/99/9999");
        $("#usucpf").mask("999.999.999-99");

        <?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
        /*** SALVA O FORMULARIO ***/
        $('#btn_salvar_atividade').on('click', function () {
            $('#form-atividade').saveAjax({action: 'salvar', controller: 'atividade', retorno: true, displayErrorsInput: true, functionSucsess: 'atualizaGridAtividade' });
        });

        /*** ABRE O FORMULARIO ETAPAS DE CONTROLE ***/
        $('#bt_adicionar_executor').on('click', function () {
            $.post(window.location.href, {controller: 'atividade', action: 'adicionarExecutor' }, function (data) {
                $('#formulario_executor').html(data);
                $('#dialogo_formulario_executor').modal('show');
            });
        });

        /*** EXCLUIR PESSOA SELECIONADA DO FATOR AVALIADO***/
        $('.bt_remover_executor').on('click', function () {
            $('.usucpf_atividade').val('');
            $('#nome_executor').html('');
            $('#span_nome_executor').hide();
        });

        $('#btn_limpar_atividade').on('click', function () {
			cadastrarAtividade();
        });
        <?php endif; ?>

    });
</script>