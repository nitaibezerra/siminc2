<section class="modal fade" id="nc" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <section class="modal-dialog" style="width:30%;">
        <section class="modal-content">
            <section class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span>
                    <span class="sr-only">Close</span>
                </button>
                <h4 class="modal-title" id="myModalLabel">Envio para Pagamento</h4>
            </section>
            <form class="form-horizontal" role="form" method="post" action="<?$this->element->getAction(); ?>">
                <?= $this->element->tcpid; ?>
                <?= $this->element->ptrid; ?>
                <section class="modal-body">
                    <section class="form-group">
                        <label class="control-label col-md-4" for="tr">Número de Transferência:</label>
                        <section class="col-md-8">
                            <input type="text" maxlength="10" id="tr" name="transferencia" class="form-control"/>
                        </section>
                    </section>
                    <section class="form-group">
                        <label class="control-label col-md-4" for="nnc">Número NC:</label>
                        <section class="col-md-8">
                            <input type="text" maxlength="10" id="nnc" name="numeronc" class="form-control"/>
                        </section>
                    </section>
                </section>
                <section class="modal-footer">
                    <button type="reset" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    <button type="submit" name="submit" class="btn btn-primary">Salvar Alterações</button>
                </section>
            </form>
        </section>
    </section>
</section>