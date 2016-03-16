
<form class="form-horizontal" enctype="multipart/form-data" name="" id="" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">
	<?= $this->element->tcpid; ?>
	<section class="well well-sm" id="anexo-form">
		
	</section>		      		
<!-- 	<section class="well"> -->
<!-- 		<section class="form-group"> -->
<!-- 			<label class="control-label col-md-2" for="br">Projeto básico ou termo de referência:</label> -->
<!-- 			<section class="col-md-4"> -->
<!-- 				<input type="file" class="btn btn-link" id="br" name="pbtr" /> -->
<!-- 			</section> -->
<!-- 			<section class="col-md-6"> -->
<!-- 				<textarea rows="2" cols="" class="form-control" placeholder="Descrição"></textarea> -->
<!-- 			</section> -->
<!-- 		</section> -->
<!-- 	</section> -->
	<section class="well" style="clear: both; padding-bottom: 0;">
		<section class="form-group">
			<div class="col-md-12">
				<button type="button" class="btn btn-success btn-sm" id="novo-anexo"><span class="glyphicon glyphicon-plus"></span> Demais Anexos</button>
				<button type="submit" id="btn-salvar-anexo" class="btn btn-primary " name="save-anexo"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar</button>
			</div>			
		</section>
	</section>
	
</form>