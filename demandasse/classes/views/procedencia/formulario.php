<div class="modal-dialog">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h3 class="modal-title">
                Salvar procedência
                <!--                <small>na Instituição -->
                <?php //echo $_SESSION['instituicao']['intdscrazaosocial'] ?><!--</small>-->
            </h3>
            <!--            <h4 class="modal-title"></h4>-->
        </div>
        <div class="modal-body">
            <div class="row">
                <div class="col-md-12">
                    <form id="form_save" method="post" class="form-horizontal">
                        <div class="col-md-12">
                            <div class="well">
                                <input name="controller" type="hidden" value="procedencia">
                                <input name="action" type="hidden" value="salvar">
                                <input name="prcid" type="hidden" value="<?php echo $this->entity['prcid']['value'] ?>">

                                <div class="form-group">
                                    <label for="prcsigla" class="col-lg-4 col-md-4 control-label">Sigla:</label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <input id="prcsigla" name="prcsigla" type="text" class="form-control"
                                               placeholder="" value="<?php echo $this->entity['prcsigla']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="prcdsc" class="col-lg-4 col-md-4 control-label">Descrição:</label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <input id="prcdsc" name="prcdsc" type="text" class="form-control" placeholder=""
                                               value="<?php echo $this->entity['prcdsc']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="prcresponsavel"
                                           class="col-lg-4 col-md-4 control-label">Responsável:</label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <input id="prcresponsavel" name="prcresponsavel" type="text"
                                               class="form-control" placeholder=""
                                               value="<?php echo $this->entity['prcresponsavel']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="prcremailesponsavel" class="col-lg-4 col-md-4 control-label">E-mail do
                                        responsável:</label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <input id="prcremailesponsavel" name="prcremailesponsavel" type="text"
                                               class="form-control" placeholder=""
                                               value="<?php echo $this->entity['prcremailesponsavel']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="prcremailinstitucional" class="col-lg-4 col-md-4 control-label">E-mail
                                        Institucional:</label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <input id="prcremailinstitucional" name="prcremailinstitucional" type="text"
                                               class="form-control" placeholder=""
                                               value="<?php echo $this->entity['prcremailinstitucional']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="prcstatus" class="col-lg-4 col-md-4 control-label">
                                        Status:
                                    </label>

                                    <div class="col-lg-8 col-md-8 ">
                                        <div class="btn-group" data-toggle="buttons">
                                            <label
                                                class="btn btn-default <?php if ($this->entity['prcstatus']['value'] == 'A' || empty($this->entity['prcstatus']['value'])) echo 'active' ?>">
                                                <input id="prcstatus" name="prcstatus" type="radio"
                                                       value="A" <?php if ($this->entity['prcstatus']['value'] == 'A' || empty($this->entity['prcstatus']['value'])) echo 'checked="checked"' ?>>
                                                Ativo
                                            </label>
                                            <label
                                                class="btn btn-default <?php if ($this->entity['prcstatus']['value'] == 'I') echo 'active' ?>">
                                                <input id="prcstatus" name="prcstatus" type="radio"
                                                       value="I" <?php if ($this->entity['prcstatus']['value'] == 'I') echo 'checked="checked"' ?>>
                                                Inativo
                                            </label>
                                        </div>
                                    </div>
                                </div>

                            </div>
                        </div>
                    </form>

                    <div class="clearfix"></div>

                    <!--        --><?php //$modelDemanda->recuperarListagem(); ?>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button id="bt-salvar" type="button" class="btn btn-success">Salvar</button>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript">
    $('#bt-salvar').click(function () {        
        $('#form_save').saveAjax({clearForm: true, functionSucsess: 'fecharModal'});
    });
</script>