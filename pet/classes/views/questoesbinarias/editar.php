<form role="form" class="form-horizontal" method="post" id="form-categoria-questoes-binaria">
	<input type="hidden" id="input_qubid" name="qubid" value="<?= $this->questaoBinaria->getAttributeValue('qubid'); ?>">
	<input type="hidden" name="cqbid" value="<?= $this->questaoBinaria->getAttributeValue('cqbid'); ?>">

	<fieldset>
		<div class="form-group">
			<label class="col-lg-4 control-label" for="titulo">Título</label>

			<div class="col-lg-8">
				<input type="text" class="form-control" id="input_titulo" name="titulo" value="<?= $this->questaoBinaria->getAttributeValue('titulo'); ?>">
			</div>
		</div>

		<div class="text-right">
			<button type="button" class="btn btn-default btn-sm" id="btNovaQuestaoBinaria"><span class="glyphicon glyphicon-share-alt" aria-hidden="true"></span> Novo</button>
			<button type="button" class="btn btn-primary" id="btGravarQuestaoBinaria"><span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar Questão Binária</button>
		</div>

	</fieldset>
</form>