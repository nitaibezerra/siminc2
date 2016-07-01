<section class="well">
    <form class="form-horizontal"
          enctype="multipart/form-data"
          name="<?= $this->element->getName(); ?>"
          id="<?= $this->element->getId(); ?>"
          action="<?= $this->element->getAction(); ?>"
          method="<?= $this->element->getMethod(); ?>"
          role="form">

        <?= $this->element->tcpid; ?>
        <hr>
        <div class="form-group">
            <div class="col-md-offset-2">
                <button type="button" class="btn btn-warning" name="cancel" id="cancel">Cancelar</button>
                <button type="submit" class="btn btn-primary" name="submit" id="submit">Gravar</button>
            </div>
        </div>
    </form>
</section>