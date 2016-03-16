<h4 class="modal-title"><?= $this->titulo; ?></h4>
<h5 class="modal-title"><?= $this->fator; ?></h5>
<br>
<div class="row well">
    <div class="col-lg-12">

        <form enctype="multipart/form-data" method="post" id="form-fator-avaliado-execucao" class="form-horizontal" action="">
            <div class="row">
                <div class="col-lg-12">
                    <input name="fatid" id="fatid" type="hidden" value="<?= $this->fatorAvaliado->getAttributeValue('fatid'); ?>">
                    <input name="acao" id="acao" type="hidden" value="">
                    <input name="recusado" id="recusado" type="hidden" value="">
                    <input name="recusado_retorno" id="recusado_retorno" type="hidden" value="">

                    <?php if ($this->comentarioDocumento->getAttributeValue('cmddsc')): ?>
                        <div class="form-group has-success">
                            <label for="cmddsc" class="col-lg-4 control-label">Descrição Anterior</label>
                            <div class="col-lg-5" style="color:#468847"><?= $this->comentarioDocumento->getAttributeValue('cmddsc'); ?></div>
                        </div>

                        <?php if ($this->fatorAvaliado->getAttributeValue('arqid')): ?>
                            <div class="form-group has-success" id="div_downloadFile">
                                <div class="col-lg-offset-4 col-lg-5">
                                    <a href="contratogestao.php?modulo=principal/download&acao=A&arqid=<?= $this->fatorAvaliado->getAttributeValue('arqid'); ?>"
                                       class="btn btn-default"><span class="glyphicon glyphicon-upload"></span> download do arquivo anterior</a>
                                </div>
                            </div>
                        <?php endif; ?>
                        <hr>
                    <?php endif; ?>

					
                    <div class="form-group <?= ($this->possuiErro ? 'has-error' : '' ) ?> div_certificador_e_validador">
                        <label for="cofid" class="col-lg-4 control-label"><?= $this->fatorAvaliado->getAttributeLabel('cofid'); ?></label>
                        <div class="col-lg-5">
                            <select name="cofid" class="form-control">
                                <?= $this->conformidade->getOptionsConfid(); ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group <?= ($this->possuiErro ? 'has-error' : '' ) ?> div_certificador_e_validador">
                        <label for="temid" class="col-lg-4 control-label"><?= $this->fatorAvaliado->getAttributeLabel('temid'); ?></label>
                        <div class="col-lg-5">
                            <select name="temid" class="form-control">
                                <?= $this->tempestividade->getOptionsTempestividade(); ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group <?= ($this->possuiErro ? 'has-error' : '' ) ?> div_certificador_e_validador">
                        <label for="satid" class="col-lg-4 control-label"><?= $this->fatorAvaliado->getAttributeLabel('satid'); ?></label>
                        <div class="col-lg-5">
                            <select name="satid" class="form-control">
                                <?= $this->satisfacao->getOptionsSatid(); ?>
                            </select>
                        </div>
                    </div>		

                    <div class="form-group <?= ($this->possuiErro ? 'has-error' : '' ) ?>" id="div_arqid">
                        <label for="arqid" class="col-lg-4 control-label"><?= $this->fatorAvaliado->getAttributeLabel('arqid'); ?></label>
                        <div class="col-lg-5">
                            <input id="arqid" name="arqid" type="file">
                            <?= ($this->possuiErro ? '<p class="help-block erro_input">Selecione um arquivo!</p>' : '' ) ?>
                        </div>
                    </div>

                    <div class="form-group" id="div_downloadFile" style="display:none;">
                        <div class="col-lg-offset-4 col-lg-5">
                            <a href="contratogestao.php?modulo=principal/download&acao=A&arqid=<?= $this->fatorAvaliado->getAttributeValue('arqid'); ?>"
                               class="btn btn-link"><span class="glyphicon glyphicon-upload"></span> download arquivo</a>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="cmddsc" class="col-lg-4 control-label"><?= $this->comentarioDocumento->getAttributeLabel('cmddsc'); ?></label>
                        <div class="col-lg-8">
                            <textarea rows="8" id="cmddsc" name="cmddsc" class="form-control"></textarea>
                        </div>
                    </div>


                </div>
            </div>

            <div class=" text-right">
                <button id="bt-salvar-fator-avaliado-execucao" type="submit" class="btn btn-primary"><span class="glyphicon glyphicon-floppy-disk"></span> Enviar</button>
                <button id="bt-recusar-fator-avaliado-execucao" type="button" class="btn btn-danger btn-sm" style="display: none;"><span class="glyphicon glyphicon-remove"></span> Recusar para Executor</button>
                <button id="bt-recusar-fator-avaliado-validacao" type="button" class="btn btn-danger btn-sm" style="display: none;"><span class="glyphicon glyphicon-remove-sign"></span> Recusar para Validador</button>
            </div>
        </form>
    </div>
</div>
<script>
<?php if (is_string($this->view->validadorObrigatorio)): ?>
        alert('<?= $this->view->validadorObrigatorio; ?>')
<?php endif; ?>
    $('#bt-recusar-fator-avaliado-execucao').on('click', function() {
        $('#recusado').val(1);
        $('#recusado_retorno').val('execucao');
        $('#form-fator-avaliado-execucao').submit();
    });

    $('#bt-recusar-fator-avaliado-validacao').on('click', function() {
        $('#recusado').val(1);
        $('#recusado_retorno').val('validacao');
        $('#form-fator-avaliado-execucao').submit();
    });
</script>