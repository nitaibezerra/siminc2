<script language="javascript" src="/contratogestao/js/form.js"></script>

<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title"><?php echo $this->titulo; ?></h4>
</div>

<div class="modal-body">
    <form id="form-gestao-contrato" class="form-horizontal" action="">
        <div class="row">
            <input name="conid" id="conid" type="hidden" value="<?php echo $this->contrato->entity['conid']['value']; ?>">
            <input name="hqcid" id="hqcid" type="hidden" value="<?php echo $this->contrato->entity['hqcid']['value']; ?>">

            <input name="hqcidpai" id="hqcidpai" type="hidden" value="<?php echo $this->hierarquiaContrato->entity['hqcidpai']['value']; ?>">
            <input name="hqcnivel" id="hqcnivel" type="hidden" value="<?php echo $this->hierarquiaContrato->entity['hqcnivel']['value']; ?>">


            <div class="col-lg-6">
                <fieldset>
                    <div class="row">
                        <div class="col-lg-5">
                            <div class="form-group">
                                <label for="consigla" class="col-lg-5 control-label"><?php echo $this->contrato->entity['consigla']['label']; ?></label>
                                <div class="col-lg-6">
                                    <input id="consigla" name="consigla" type="text" class="form-control" placeholder="" maxlength="10"
                                           required="required" value="<?php echo $this->contrato->entity['consigla']['value'] ?>">
                                </div>
                            </div>
                        </div>
                        <div class="col-lg-7">
                            <div class="form-group">
                                <label for="conprocesso" class="col-lg-3 control-label"><?php echo $this->contrato->entity['conprocesso']['label']; ?></label>
                                <div class="col-lg-9">
                                    <input id="cmpfonefax" name="conprocesso" type="text" class="form-control" maxlength="50"
                                           placeholder="" value="<?php echo $this->contrato->entity['conprocesso']['value'] ?>">
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="row">
                            <div class="col-lg-6">
                                <div class="form-group">
                                    <label for="datainicial" class="col-lg-4 control-label"><?php echo $this->contrato->entity['datainicial']['label']; ?></label>
                                    <div class="col-lg-6">
                                        <input id="datainicial" name="datainicial" type="text" class="form-control data data_inicio" maxlength="10"
                                               placeholder="dd/mm/aaaa" maxlength="20"
                                               value="<?php echo $this->contrato->entity['datainicial']['value'] ?>">
                                    </div>
                                </div>
                            </div>
                            <div class="col-lg-6">
                                <div class="form-group">
                                    <label for="datafinal" class="col-lg-4 control-label"><?php echo $this->contrato->entity['datafinal']['label']; ?></label>
                                    <div class="col-lg-6">
                                        <input id="datafinal" name="datafinal" type="text" class="form-control data data_fim" maxlength="10"
                                               placeholder="dd/mm/aaaa" required="required"
                                               value="<?php echo $this->contrato->entity['datafinal']['value'] ?>">
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-lg-12">
                            <div class="form-group">
                                <label for="concontratada" class="col-lg-2 control-label"><?php echo $this->contrato->entity['concontratada']['label']; ?></label>
                                <div class="col-lg-10">
                                    <input id="concontratada" name="concontratada" type="text" maxlength="100"
                                           class="form-control" placeholder="" required="required"
                                           value="<?php echo $this->contrato->entity['concontratada']['value'] ?>">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="connumerocontrato" class="col-lg-3 control-label"><?php echo $this->contrato->entity['connumerocontrato']['label']; ?></label>
                                <div class="col-lg-9">
                                    <input id="connumerocontrato" name="connumerocontrato" type="text" maxlength="50"
                                           class="form-control" placeholder=""
                                           value="<?php echo $this->contrato->entity['connumerocontrato']['value'] ?>">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="connumeroaditivo" class="col-lg-3 control-label"><?php echo $this->contrato->entity['connumeroaditivo']['label']; ?></label>
                                <div class="col-lg-9">
                                    <input id="connumeroaditivo" name="connumeroaditivo" type="text" maxlength="50"
                                           class="form-control" placeholder=""
                                           value="<?php echo $this->contrato->entity['connumeroaditivo']['value'] ?>">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="conarearesponsavel" class="col-lg-3 control-label"><?php echo $this->contrato->entity['conarearesponsavel']['label']; ?></label>
                                <div class="col-lg-9">
                                    <input id="conarearesponsavel" name="conarearesponsavel" type="text" maxlength="50"
                                           class="form-control" placeholder=""
                                           value="<?php echo $this->contrato->entity['conarearesponsavel']['value'] ?>">
                                </div>
                            </div>
                        </div>
                    </div>
                </fieldset>
            </div>
            <div class="col-lg-6">
                <fieldset>
                    <div class="row">
                        <div class="col-lg-12">
                            <div class="form-group">
                                <label for="condescricao" class="col-lg-2 control-label"><?php echo $this->contrato->entity['condescricao']['label']; ?></label>
                                <div class="col-lg-10">
                                    <textarea name="condescricao" class="form-control" id="condescricao" rows="4" maxlength="500"><?php echo $this->contrato->entity['condescricao']['value'] ?></textarea>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="conobjetivo" class="col-lg-2 control-label"><?php echo $this->contrato->entity['conobjetivo']['label']; ?></label>
                                <div class="col-lg-10">
                                    <textarea name="conobjetivo" class="form-control" id="conobjetivo" rows="4" maxlength="500"><?php echo $this->contrato->entity['conobjetivo']['value'] ?></textarea>
                                </div>
                            </div>
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>
    </form>
</div>
<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal"><span class="glyphicon glyphicon-remove"></span> Fechar</button>
    <button id="bt-salvar-contrato" type="button" class="btn btn-success"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar</button>
</div>