<div class="col-lg-12">
	<div class="page-header">
		<h4>Lista de Eixos</h4>
	</div>
	<?= $this->eixo->getListarEixo( $this->questionario->getAttributeValue('queid') ); ?>
</div>