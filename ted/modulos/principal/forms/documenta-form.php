<form class="well form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>" role="form">

    <?= $this->element->tcpid; ?>
	
	<input type="hidden" name="funcao" id="funcao" value="fndedocumenta" />
	
    <div class="form-group">
        <label class="control-label col-md-2" for="logindoc">Login:</label>
        <div class="col-md-4">
            <?= $this->element->logindoc; ?>
        </div>
    </div>

    <div class="form-group ">
        <label class="control-label col-md-2" for="senhadoc">Senha:</label>
        <div class="col-md-4">
            <?= $this->element->senhadoc; ?>
        </div>
    </div>

    <hr />

    <div class="form-group">
        <div class="col-md-offset-2">
            <button type="submit" class="btn btn-primary" name="submit" id="submit">Gerar nº processo FNDE</button>
        </div>
    </div>
</form>
