<div class="modal-dialog" style="width:80%;">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal">
                <span aria-hidden="true">&times;</span>
                <span class="sr-only">Close</span>
            </button>
            <h4 class="modal-title">Formulário de Previsão</h4>
        </div>
        <div class="modal-body">
            <form class="form-horizontal"
                  name="<?=$this->element->getName(); ?>"
                  id="<?=$this->element->getId(); ?>"
                  action="<?= $this->element->getAction(); ?>"
                  method="<?= $this->element->getMethod(); ?>"
                  role="form">
                    <?= $this->element->proid; ?>
                    <?= $this->element->tcpid; ?>
                    <?= $this->element->ptrid; ?>
                <div class="row">
                    <div class="form-group">
                        <label class="control-label col-md-2" for="ano">Ano:</label>
                        <div class="col-md-10">
                            <?= $this->element->proanoreferencia; ?>
                        </div>
                    </div>
                    <div id="fndeblocked" class="form-group">
                        <label class="control-label col-md-2" for="acao">Ação:</label>
                        <div class="col-md-10">
                            <p id="acao" class="form-control-static">Selecione um Programa de Trabalho</p>
                        </div>
                    </div>
                    <div class="form-group ">
                        <label class="control-label col-md-2" for="programaTrabalho">Programa de Trabalho:</label>
                        <div class="col-md-10">
                            <?= $this->element->programaTrabalho; ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="planoInterno">Plano Interno:</label>
                        <div class="col-md-10" id="plano">
                            <p id="pi" class="form-control-static">Selecione um Programa de Trabalho</p>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="descricao">Descrição da Ação Constante da LOA:</label>
                        <div class="col-md-10">
                            <p id="descricao" class="form-control-static">Selecione um Programa de Trabalho</p>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="naturezaDespesa">Natureza da Despesa:</label>
                        <div class="col-md-10">
                            <?= $this->element->ndpid;?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="valor">Valor(em R$ 1,00):</label>
                        <div class="col-md-10">
                            <?= $this->element->provalor; ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="mesliberacao">Mês da Liberação:</label>
                        <div class="col-md-10">
                            <?= $this->element->crdmesliberacao; ?>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="control-label col-md-2" for="crdmesexecucao">Prazo para o cumprimento do objeto:</label>
                        <div class="col-md-10">
                            <?= $this->element->crdmesexecucao; ?>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            <button type="button" class="btn btn-primary" id="btn-Salva-Previsao">Salvar</button>
        </div>
    </div>
</div>