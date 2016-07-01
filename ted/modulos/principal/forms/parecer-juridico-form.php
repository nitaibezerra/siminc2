<form class="form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

    <section class="well">

        <?= $this->element->pcjid; ?>
        <?= $this->element->tcpid; ?>
        <?= $this->element->ungcod; ?>

        <div  class="form-group">
            <label class="control-label col-md-3" for="obsparecer">Observação:</label>
            <div class="col-md-9">
                <?= $this->element->obsparecer;?>
                <div id="counter-obsparecer" class=""></div>
            </div>
        </div>
        <hr>
        <div class="form-group">
            <div class="col-md-offset-2">
                <button type="button" class="btn btn-warning" name="cancel" id="cancel">
                    Cancelar
                </button>
                <button type="submit" class="btn btn-primary" name="submit" id="submit">
                    Gravar
                </button>
                <button type="submit" class="btn btn-success" name="submitcontinue" id="submitcontinue">
                    Gravar e Continuar
                </button>
            </div>
        </div>
    </section>

</form>

<form class="form-horizontal"
      enctype="multipart/form-data"
      name=""
      id=""
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

    <section class="well well-sm" id="anexo-form">
    </section>

    <section class="well" style="clear: both; padding-bottom: 0;">
        <section class="form-group">
            <div class="col-md-12">
                <button type="button" class="btn btn-success btn-sm" id="novo-anexo">
                    <span class="glyphicon glyphicon-save"></span> Novo Anexo
                </button>
                <button type="submit" id="btn-salvar-anexo" class="btn btn-primary btn-sm" name="save-anexo">
                    <span class="glyphicon glyphicon-floppy-disk"></span> Salvar
                </button>
            </div>
        </section>
    </section>

</form>