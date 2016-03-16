<script language="javascript" src="/contratogestao/js/form_etapa_controle.js" charset="ISO-8859-1"></script>

<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title"><?php echo $this->titulo; ?></h4>
</div>

<div class="modal-body">
    <form id="form-etapa-controle" class="form-horizontal" action="">
        <fieldset>
            <input type="hidden" id="etapa" name="etapa" value="<?= $this->etapa; ?>">
            <div class="row">
                <div class="col-lg-12">
                    <div class="form-group">
                        <label for="funcao" class="col-lg-2 control-label">Selecione a função</label>
                        <div class="col-lg-2">
                            <select class="form-control" id="funcao" name="funcao">
                                <option value="">Selecione</option>
                                <option value="fisica" id="opt_fisica"><?= ucfirst($this->etapa); ?> (Pessoa Física)</option>
                                <option value="juridica" id="opt_juridica"><?= ucfirst($this->etapa); ?> (Pessoa Jurídica)</option>
                            </select>
                        </div>
                    </div>
                </div>
            </div>
        </fieldset>
        <div id="divTipoPessoa"></div>
    </form>
</div>
<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal"><span class="glyphicon glyphicon-remove"></span> Fechar</button>
    <button id="bt-salvar-etapa-controler" type="button" class="btn btn-success"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar</button>
</div>