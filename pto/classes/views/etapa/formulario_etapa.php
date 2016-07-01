<br>
<form class="form-horizontal" method="post" id="form-etapa">
    <fieldset>
        <legend>Cadastrar Etapa(s)</legend>
        <div class="row">
            <input type="hidden" name="etpid" id="etpid" value="<?= $this->etapa->getAttributeValue('etpid'); ?>">

            <div class="form-group">
                <label class="col-lg-2 control-label" for="etpdsc">
                    <?= $this->etapa->getAttributeLabel('etpdsc'); ?>:
                    <span class="alert-danger" style="background-color: #FFF; font-size: 19px; ">*</span> </label>

                <div class="col-lg-6">
                    <input type="text" value="<?= $this->etapa->getAttributeValue('etpdsc'); ?>" placeholder=""
                           class="form-control" name="etpdsc" id="etpdsc">
                </div>
            </div>

            <div class="form-group">
                <label class="col-lg-2 control-label" for="acaid"> <?= $this->etapa->getAttributeLabel('acaid'); ?></label>

                <div class="col-lg-5">
                    <select class="form-control" name="acaid" id="acaid_etapa">
                        <?= $this->acaoSolucao->getOptionsAcaoBySolucao(); ?>
                    </select>
                </div>
            </div>

            <div class="form-group">
                <label class="col-lg-2 control-label" for="etpobs">
                    <?= $this->etapa->getAttributeLabel('etpobs'); ?>
                </label>

                <div class="col-lg-8">
                    <textarea rows="4" class="form-control" name="etpobs" id="etpobs"><?= $this->etapa->getAttributeValue('etpobs'); ?></textarea>
                </div>
            </div>

        </div>
        <?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
            <div class="text-right">
                <span class="help-block">Campos com <span style="font-size: 19px; ">*</span> são obrigatórios.</span>
                <button type="button" class="btn btn-success" title="Salvar" id="btn_salvar_etapa">
                    <span class="glyphicon glyphicon-ok"></span> Cadastrar
                </button>
                <button type="button" class="btn btn-default" title="Limpar" id="btn_limpar_etapa">
                    <span class="glyphicon glyphicon-repeat"></span> Limpar
                </button>
            </div>
        <?php endif; ?>
        <br>
    </fieldset>
</form>
<script type="text/javascript">
    $(function () {
        <?php if ($this->perfilUsuario->possuiAcessoEdicao()) : ?>
        /*** SALVA O FORMULARIO ***/
        $('#btn_salvar_etapa').on('click', function () {
            $('#form-etapa').saveAjax({action: 'salvar', controller: 'etapa', retorno: true, displayErrorsInput: true, functionSucsess: 'atualizaGridEtapa'});
        });

        $('#btn_limpar_etapa').on('click', function () {
            limparEtapa();
        });
        <?php endif; ?>
    });
</script>