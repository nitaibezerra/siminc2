<br>
<div class="row">
    <div class="col-lg-11">
        <form class="form-horizontal" method="post" id="form-atividade">
            <fieldset>
                <legend>Atividade selecionada</legend>
                <input type="hidden" name="atvid" id="atvid" value="<?= $this->atividade->getAttributeValue('atvid'); ?>">
                <input type="hidden" name="docid" id="docid" value="<?= $this->atividade->getAttributeValue('docid'); ?>">
                <input type="hidden" name="usucpf" class="usucpf_atividade" value="<?= $this->atividade->getAttributeValue('usucpf'); ?>">

                <div class="form-group">
                    <label class="col-lg-2 control-label" for="atvdsc"><?= $this->atividade->getAttributeLabel('atvdsc'); ?> </label>

                    <div class="col-lg-10">
                        <input type="text" value="<?= $this->atividade->getAttributeValue('atvdsc'); ?>" placeholder="" class="form-control" name="atvdsc" id="atvdsc">
                    </div>
                </div>

                <div class="form-group">
                    <label class="col-lg-2 control-label" for="atvprazo"><?= $this->atividade->getAttributeLabel('atvprazo'); ?> </label>

                    <div class="col-lg-5">
                        <input type="date" value="<?= $this->atividade->getAttributeValue('atvprazo'); ?>" placeholder="" class="form-control" name="atvprazo" id="atvprazo">
                    </div>
                </div>

                <div class="form-group">
                    <div class="col-lg-offset-2 col-lg-10">
                        <span id="span_nome_executor" style="<?= (empty($this->executor) ? 'display: none;' : ''); ?>">
                            <span id="nome_executor"><?= $this->executor ?> </span>
                        </span>
                    </div>
                </div>
                <br>
            </fieldset>
        </form>
    </div>
    <div class="col-lg-1">
        <?php
        if ($this->atividade->getAttributeValue('docid')) {
            wf_desenhaBarraNavegacao($this->atividade->getAttributeValue('docid'), array());
        }
        ?>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        $('#atvprazo').datepicker();
        $("#atvprazo").mask("99/99/9999");
        $("#usucpf").mask("999.999.999-99");

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
        $('.bt_remover_executor').on('click', function() {
            $('.usucpf_atividade').val('');
            $('#nome_executor').html('');
            $('#span_nome_executor').hide();
        });

        $('#btn_limpar_atividade').on('click', function() {
            $('#nome_executor').html('');
            $('#span_nome_executor').hide();
            $('#atvid').val('');
            $('#docid').val('');
            $('.usucpf_atividade').val('');
            $('#form-atividade').get(0).reset();
        });

    });
</script>