<script language="javascript" src="/contratogestao/js/form_fator_avaliado.js" charset="ISO-8859-1"></script>
<h4 class="modal-title"><?php echo $this->titulo; ?></h4>
<h5 class="modal-title"><?php echo $this->acao; ?></h5>
<div class="row">
    <div class="col-lg-12">
        <form id="form-fator-avaliado" class="form-horizontal" action="">
            <div class="row">
                <div class="col-lg-12">

                    <input name="fatid" id="fatid" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('fatid'); ?>">
                    <input name="conid" id="conid" type="hidden" value="<?php echo $_SESSION['conid']; ?>">

                    <input name="usucpfexecutor" id="usucpfexecutor" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('usucpfexecutor'); ?>">
                    <input name="usucpfvalidador" id="usucpfvalidador" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('usucpfvalidador'); ?>">
                    <input name="usucpfcertificador" id="usucpfcertificador" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('usucpfcertificador'); ?>">


                    <input name="entidexecutor" id="entidexecutor" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('entidexecutor'); ?>">
                    <input name="entidvalidador" id="entidvalidador" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('entidvalidador'); ?>">
                    <input name="entidcertificador" id="entidcertificador" type="hidden" value="<?php echo $this->fatorAvaliado->getAttributeValue('entidcertificador'); ?>">

                    <div class="form-group">
                        <label for="fatdsc" class="col-lg-3 control-label"><?php echo $this->fatorAvaliado->getAttributeLabel('fatdsc'); ?></label>
                        <div class="col-lg-5">
                            <input id="fatdsc" name="fatdsc" type="text" class="form-control" placeholder="" maxlength="500"
                                   required="required" value="<?php echo $this->fatorAvaliado->getAttributeValue('fatdsc'); ?>">
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="fatprazo" class="col-lg-3 control-label"><?php echo $this->fatorAvaliado->getAttributeLabel('fatprazo'); ?></label>
                        <div class="col-lg-2">
                            <input id="fatprazo" name="fatprazo" type="text" class="form-control" maxlength="10"
                                   placeholder="dd/mm/aaaa"
                                   value="<?php echo $this->fatorAvaliado->getAttributeValue('fatprazo'); ?>">
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="fatvalordesembolso" class="col-lg-3 control-label"><?php echo $this->fatorAvaliado->getAttributeLabel('fatvalordesembolso'); ?></label>
                        <div class="col-lg-2">
                            <div class="input-group">
                                <div class="input-group-addon">R$ </div>
                                <input id="fatvalordesembolso" name="fatvalordesembolso" type="text" class="form-control" 
                                       maxlength="10" value="<?php echo $this->fatorAvaliado->getAttributeValue('fatvalordesembolso'); ?>">
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="etapas_de_controle" class="col-lg-3 control-label">Etapas de Controle</label>
                        <div class="btn-group bts_etapas_de_controle customizado" data-toggle="buttons">
                            <label class="btn btn-default <?= ( empty($this->executor) ? '' : 'active' ); ?>">
                                <input type="checkbox" data-etapa="executor" id="etapas_de_controle_execucao" name="etapas_de_controle" <?= ( empty($this->executor) ? '' : 'checked= "checked"' ); ?>>Execução 
                            </label>
                            <label class="btn btn-default <?= ( empty($this->validador) ? '' : 'active' ); ?> <?= ( empty($this->executor) ? 'disabled' : '' ); ?> ">
                                <input type="checkbox" data-etapa="validador" id="etapas_de_controle_validacao" name="etapas_de_controle" <?= ( empty($this->validador) ? '' : 'checked= "checked"' ); ?>>Validação
                            </label>
                            <label class="btn btn-default <?= ( empty($this->certificador) ? '' : 'active' ); ?> <?= ( empty($this->validador) ? 'disabled' : '' ); ?>">
                                <input type="checkbox" data-etapa="certificador" id="etapas_de_controle_certificacao" name="etapas_de_controle" <?= ( empty($this->certificador) ? '' : 'checked= "checked"' ); ?>>Certificação
                            </label>
                        </div>
                    </div>

                </div>
            </div>
            <div class="row">
                <div class="col-lg-offset-3 col-lg-9">
                    <div class="form-group" style="<?= ( empty($this->executor) ? 'display: none;' : '' ); ?>" id="executor">
                        <button type="button" data-etapa="executor" class="btn btn-default bt_etapas_de_controle"><span class="glyphicon glyphicon-wrench"></span> Executor</button>
                        <span id="span_nome_executor" style="<?= (empty($this->executor) ? 'display: none;' : '' ); ?>">
                            <span id="nome_executor"><?= $this->executor ?></span>
                            <button type="button" data-etapa="executor" class="btn btn-default bt_remover_pessoa" title="desvincular executor"><span class="glyphicon glyphicon-trash"></span></button>
                        </span>
                    </div>

                    <div class="form-group" style="<?= (empty($this->validador) ? 'display: none;' : '' ); ?>" id="validador">
                        <button type="button" data-etapa="validador" class="btn btn-default bt_etapas_de_controle"><span class="glyphicon glyphicon-ok"></span>  Validador</button> 
                        <span id="span_nome_validador" style="<?= (empty($this->validador) ? 'display: none;' : '' ); ?>">
                            <span id="nome_validador"><?= $this->validador ?></span>
                            <button type="button" data-etapa="validador" class="btn btn-default bt_remover_pessoa" title="desvincular executor"><span class="glyphicon glyphicon-trash"></span></button>
                        </span>
                    </div>

                    <div class="form-group" style="<?= (empty($this->certificador) ? 'display: none;' : '' ); ?>" id="certificador">
                        <button type="button" data-etapa="certificador" class="btn btn-default bt_etapas_de_controle"><span class="glyphicon glyphicon-certificate"></span> Certificador</button>
                        <span id="span_nome_certificador" style="<?= (empty($this->certificador) ? 'display: none;' : '' ); ?>">
                            <span id="nome_certificador"><?= $this->certificador ?></span>
                            <button type="button" data-etapa="certificador" class="btn btn-default bt_remover_pessoa" title="desvincular executor"><span class="glyphicon glyphicon-trash"></span></button>
                        </span>
                    </div>
                </div>
            </div>
        </form>
        <?php if ( $this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) or is_null( $this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) ) ) : ?>
            <hr>
            <div class=" text-right">
                <button id="bt-salvar-fator-avaliado" type="button" class="btn btn-success"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar</button>
                <button id="bt_reset_fator_avaliado" type="reset" class="btn btn-primary"><span class="glyphicon glyphicon-asterisk"></span> Novo</button>
            </div>
        <?php endif; ?>
    </div>
</div>
