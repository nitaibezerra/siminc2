<form class="form-horizontal" name="filtroTed" id="filtroTed" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">
	<?= $this->element->funcao; ?>
    <div class="form-group ">
        
        <label class="control-label col-md-2" for="ungcod">Unidade Gestora:</label>
        
        <div class="col-md-10">
            <?= $this->element->ungcod; ?>
        </div>
    </div>
    <hr />
    <div class="form-group">
    	<div class="col-md-offset-2">
    		<button type="button" class="btn btn-primary" onclick="gerarRelatorio('rel')" name="gerar" id="gerar">Gerar Relatório</button>
    		<button type="button" class="btn btn-success" onclick="gerarRelatorio('xls')" name="gerarXls" id="gerarXls">Visualizar XLS</button>
    	</div>
    </div>
    
</form>