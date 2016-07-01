<form class="well form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

    <legend class="text-center">Verificar efetivação da NC SIGEF</legend>

    <?= $this->element->tcpid; ?>
    <?= $this->element->funcao; ?>

    <div class="form-group">
        <label class="control-label col-md-3" for="sigefusername">Usuário do SIGEF:</label>
        <div class="col-md-4">
            <?= $this->element->sigefusername; ?>
        </div>
    </div>
    <div class="form-group">
        <label class="control-label col-md-3" for="sigefpassword">Senha do SIGEF:</label>
        <div class="col-md-4">
            <?= $this->element->sigefpassword; ?>
        </div>
    </div>

    <hr />

    <div class="form-group">
        <div class="col-md-offset-3">
            <button type="submit" class="btn btn-primary ncCheck" name="submit" id="submit">Verificar</button>
        </div>
    </div>
</form>
