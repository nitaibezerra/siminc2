<div class="modal-dialog" style="width:90%;">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal">
                <span aria-hidden="true">&times;</span>
                <span class="sr-only">Close</span>
            </button>
            <h4 class="modal-title">Crédito a Remanejar</h4>
        </div>
        <div class="modal-body">

            <div class="row">
                <div class="col-md-12" id="table-log"></div>
            </div>
            <div class="row">
                <div class="col-md-12">&nbsp;</div>
            </div>

            <form class="form-horizontal"
                  name="<?=$this->element->getName(); ?>"
                  id="<?=$this->element->getId(); ?>"
                  action="<?= $this->element->getAction(); ?>"
                  method="<?= $this->element->getMethod(); ?>"
                  role="form">
                <?= $this->element->_proid_; ?>
                <?= $this->element->_provalor_; ?>
                <div class="row">
                    <div class="form-group">
                        <label class="control-label col-md-2" for="nc_devolucao">NC de devolução:</label>
                        <div class="col-md-10">
                            <?= $this->element->nc_devolucao; ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="codncsiafi">Valor:</label>
                        <div class="col-md-10">
                            <?= $this->element->valor_remanejar; ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="codncsiafi">Observação:</label>
                        <div class="col-md-10">
                            <?= $this->element->observacao; ?>
                        </div>
                    </div>
                </div>
            </form>

            <div class="row">
                <div class="col-md-12" id="register-log"></div>
            </div>
            <div class="row">
                <div class="col-md-12">&nbsp;</div>
            </div>

        </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-default hide-modal-remanejar" data-dismiss="modal">Fechar</button>
            <button type="button" class="btn btn-primary" id="btn-Salva-remanejar">Salvar</button>
        </div>
    </div>
</div>