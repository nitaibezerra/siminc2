<form class="form-horizontal"
      name="<?=$this->element->getName();?>"
      id="<?=$this->element->getId();?>"
      action="<?=$this->element->getAction();?>"
      method="<?=$this->element->getMethod();?>"
      role="form">

    <?= $this->element->vigid; ?>
    <?= $this->element->tcpid; ?>

    <div class="form-group ">
        <label class="control-label col-md-2" for="dtexecucao">Inicio da Vigência:</label>
        <div class="col-md-10" id="dtexecucao"></div>
    </div>

    <div class="form-group">
        <label class="control-label col-md-2" for="vigdata">Fim da Vigência:</label>
        <div class="col-md-10">
            <?= $this->element->vigdata; ?>
        </div>
    </div>

    <div class="form-group ">
        <label class="control-label col-md-2" for="vigjustificativa">Justificativa:</label>
        <div class="col-md-10">
            <?= $this->element->vigjustificativa; ?>
        </div>
    </div>
</form>
