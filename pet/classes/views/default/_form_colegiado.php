<form role="form" class="form-horizontal" method="post" id="form-colegiado">
    <input name="idgid" id="idgid" type="hidden" value="<?= $this->view->identificacaoGrupo->getAttributeValue('idgid'); ?>">
    <fieldset>
        <legend>Cadastrar Colegiado</legend>

        <div class="form-group">
            <div class="col-lg-10">
                <label for="nome" class="col-lg-2 control-label">Nome</label>
                <div class="col-lg-10">
                    <input type="text" class="form-control" id="nome" name="nome" placeholder="Nome do colegiado">
                </div>
            </div>
        </div>

        <div class="form-group">
            <div class="col-lg-offset-2 col-lg-10 text-right">
                <button type="button" class="btn btn-primary" id="bt-colegiado">
                    <span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Cadastrar Colegiado
                </button>
            </div>
        </div>

    </fieldset>
</form>