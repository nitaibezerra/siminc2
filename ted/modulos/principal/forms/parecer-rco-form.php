<form class="form-horizontal"
      enctype="multipart/form-data"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

    <section class="well">
        <div class="form-group">
            <label class="control-label col-md-3" for="tcpobsrelatorio">Considerações sobre o objetivo:</label>
            <div class="col-md-9">
                <?= $this->element->tcpobsrelatorio; ?>
                <div id="counter-tcpobsrelatorio" class=""></div>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-3" for="anexo0">Anexar arquivo:</label>
            <div class="col-md-9">
                <input type="hidden" name="anexoCod[]" value="0">
                <input type="file" class="btn start" name="anexo_0" id="anexo0">
            </div>
        </div>
    </section>

    <div class="form-group">
        <div class="col-md-offset-2">
            <input type="reset" class="btn btn-warning" name="cancel_rco" id="cancel_rco" value="Cancelar">
            <input type="submit" class="btn btn-primary" name="enviar_rco" id="enviar_rco" value="Gravar">
        </div>
    </div>
</form>