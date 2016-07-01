<form class="form-horizontal" name="filtroTed" id="filtroTed" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">
	<?= $this->element->funcao; ?>
	<br />
    <div class="form-group">
    	<div class="col-md-12">
    		<center>
	    		<button type="button" class="btn btn-primary" onclick="gerarRelatorio('rel')" name="gerar" id="gerar">Gerar Relatório</button>
	    		<button type="button" class="btn btn-success" onclick="gerarRelatorio('xls')" name="gerarXls" id="gerarXls">Visualizar XLS</button>
    		</center>
    	</div>
    </div>
    
</form>