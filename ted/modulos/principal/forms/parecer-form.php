
<form class="form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

	<section class="well">
		<?= $this->element->tcpid; ?>
	    <?= $this->element->usucpfparecer; ?>
	    <?= $this->element->ptecid; ?>
	   	<div id="fndeblocked" class="form-group">
	    	<label class="control-label col-md-3" for="considentproponente">Considerações sobre a entidade proponente:</label>
	    	<div class="col-md-9">
	    		<?= $this->element->considentproponente;?>
                <div id="counter-considentproponente" class=""></div>
	    	</div>
	    </div>
	    <div class="form-group">
	        <label class="control-label col-md-3" for="considproposta">Considerações sobre a proposta:</label>        
	        <div class="col-md-9">
	            <?= $this->element->considproposta; ?>
                <div id="counter-considproposta" class=""></div>
		    </div>
		</div>	    
		<div class="form-group">
			<label class="control-label col-md-3" for="considobjeto">Considerações sobre o objeto:</label>	        
		    <div class="col-md-9">
		    	<?= $this->element->considobjeto; ?>
                <div id="counter-considobjeto" class=""></div>
		    </div>
		</div>
		<div class="form-group">
			<label class="control-label col-md-3" for="considobjetivo">Considerações sobre o objetivo:</label>        
		    <div class="col-md-9">
		    	<?= $this->element->considobjetivo; ?>
                <div id="counter-considobjetivo" class=""></div>
		    </div>
		</div>
		<div class="form-group">
			<label class="control-label col-md-3" for="considjustificativa">Considerações sobre a justificativa:</label>        
		    <div class="col-md-9">
		    	<?= $this->element->considjustificativa; ?>
                <div id="counter-considjustificativa" class=""></div>
		    </div>
		</div>
		<div class="form-group">        
			<label class="control-label col-md-3" for="considvalores">Considerações sobre os valores:</label>        
		    <div class="col-md-9">
		    	<?= $this->element->considvalores; ?>
                <div id="counter-considvalores" class=""></div>
		    </div>
		</div>
		<div class="form-group">        
			<label class="control-label col-md-3" for="considcabiveis">Outras considerações cabíveis:</label>        
		    <div class="col-md-9">
		    	<?= $this->element->considcabiveis; ?>
                <div id="counter-considcabiveis" class=""></div>
		    </div>
		</div>
		
		<div class="form-group" id="parecerTecnico">        
			<label class="control-label col-md-3" >Parecer Técnico elaborado por:</label>        
		    <div class="col-md-9">
		    	<p class="form-control-static"><?= $this->element->usunome; ?></p>
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
	    		<button type="submit" class="btn btn-primary" name="print" id="print">
                    <span class="glyphicon glyphicon-print"></span> Imprimir
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