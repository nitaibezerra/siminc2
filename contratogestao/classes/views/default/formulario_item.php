<script language="javascript" src="/contratogestao/js/form.js" charset="ISO-8859-1"></script>

<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title"><?= $this->titulo; ?></h4>
</div>

<div class="modal-body">
    <form id="form-gestao-contrato-item" class="form-horizontal" action="">
        <div class="row">
            <input name="conid" id="conid" type="hidden" value="<?= $this->contrato->getAttributeValue('conid'); ?>">
            <input name="hqcid" id="hqcid" type="hidden" value="<?= $this->contrato->getAttributeValue('hqcid'); ?>">

            <input name="hqcidpai" id="hqcidpai" type="hidden" value="<?= $this->hierarquiaContrato->getAttributeValue('hqcidpai'); ?>">
            <input name="hqcnivel" id="hqcnivel" type="hidden" value="<?= $this->hierarquiaContrato->getAttributeValue('hqcnivel'); ?>">

            <div class="col-lg-12">
                <div class="row">
                    <div class="col-lg-12">
                        <div class="form-group">
                            <label for="consigla" class="col-lg-3 control-label"><?= $this->contrato->getAttributeLabel('consigla'); ?></label>
                            <div class="col-lg-4">
                                <input id="consigla" name="consigla" type="text" class="form-control" placeholder="" maxlength="10"
                                       required="required" value="<?= $this->contrato->getAttributeValue('consigla'); ?>">
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <?php if ( (int)$this->hierarquiaContrato->getAttributeValue('hqcnivel') === 3):?>

                <div class="col-lg-12">
                    <div class="row">
                        <div class="col-lg-12">
                            <div class="form-group">
                                <label for="conaditivo" class="col-lg-3 control-label"><?= $this->contrato->getAttributeLabel('conaditivo'); ?></label>
                                <div class="col-lg-4">
                                    <input type="checkbox" id="conaditivo" name="conaditivo" <?= ( $this->contrato->getAttributeValue('conaditivo') == 't' ? 'checked= "checked"' : '' ); ?>> 
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            <?php endif ?>

            <div class="col-lg-12">
                <div class="row">
                    <div class="col-lg-12">
                        <div class="form-group">
                            <label for="condescricao" class="col-lg-3 control-label"><?= $this->contrato->getAttributeLabel('condescricao'); ?></label>
                            <div class="col-lg-9">
                                <textarea name="condescricao" class="form-control" id="condescricao" rows="4" maxlength="500"><?= $this->contrato->getAttributeValue('condescricao'); ?></textarea>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-lg-12">
                <div class="row">
                    <div class="col-lg-6">
                        <div class="form-group">
                            <label for="datainicial" class="col-lg-6 control-label"><?= $this->contrato->getAttributeLabel('datainicial'); ?></label>
                            <div class="col-lg-6">
                                <input id="datainicial" name="datainicial" type="text" class="form-control data data_inicio" maxlength="10" placeholder="dd/mm/aaaa" maxlength="20"
                                       value="<?= $this->contrato->getAttributeValue('datainicial'); ?>">
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">
                        <div class="form-group">
                            <label for="datafinal" class="col-lg-6 control-label"><?= $this->contrato->getAttributeLabel('datafinal'); ?></label>
                            <div class="col-lg-6">
                                <input id="datafinal" name="datafinal" type="text" class="form-control data data_fim" maxlength="10" placeholder="dd/mm/aaaa" required="required"
                                       value="<?= $this->contrato->getAttributeValue('datafinal'); ?>">
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </form>
</div>
<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal"><span class="glyphicon glyphicon-remove"></span> Fechar</button>
    <button id="bt-salvar-contrato-item" type="button" class="btn btn-success"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar</button>
</div>