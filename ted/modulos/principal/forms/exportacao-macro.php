<form class="form-horizontal" name="filtroTed" id="filtroTed" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">

    <div class="form-group ">
        
        <label class="control-label col-md-2" for="lotid">Número do lote:</label>
        
        <div class="col-md-10">
            <?= $this->element->lotid; ?>
        </div>
    </div>
    <div class="form-group">        
        <label class="control-label col-md-2" for="lotdsc">Número do Termo:</label>
        
        <div class="col-md-10">
            <?= $this->element->lotdsc; ?>
        </div>
    </div>
    <div class="form-group">        
        <label class="control-label col-md-2" for="lotdata">Data:</label>
        
        <div class="col-md-10">
            <?= $this->element->lotdata; ?>
        </div>
    </div>
    <div class="form-group">        
        <label class="control-label col-md-2" for="usucpf">CPF:</label>        
        <div class="col-md-10">
            <?= $this->element->usucpf; ?>
        </div>
    </div>
    <div class="form-group">        
		<label class="control-label col-md-2" for="usunome">Nome:</label>        
        <div class="col-md-10">
            <?= $this->element->usunome; ?>
        </div>
    </div>
    <hr />
    <div class="form-group">
    	<div class="col-md-offset-2">
    		<button type="submit" class="btn btn-primary" name="search" id="search">Pesquisar</button>
    		<button type="reset" class="btn btn-warning" id="clear">Limpar</button>	
    		<button type="button" class="btn btn-success" onclick="novoLote()" name="novo" id="novo">Novo Lote</button>
    	</div>
    </div>
    
</form>
<script>
	$('.campoData').datepicker();
	$('.campoData').mask('99/99/9999');
	$('.campoCpf').mask('999.999.999-99');
</script>