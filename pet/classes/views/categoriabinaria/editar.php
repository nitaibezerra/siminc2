<form role="form" class="form-horizontal" method="post" id="form-categoria">
	<input type="hidden" name="cqbid" id="input_cqbid_categoria" value="<?= $this->categoria->getAttributeValue('cqbid'); ?>">
	<input type="hidden" name="ideixo" id="categoria_binaria_ideixo" value="<?= $this->categoria->getAttributeValue('ideixo') ?>">
	<fieldset>
		<div class="form-group">
			<label class="col-lg-3 control-label" for="nome">Nome</label>

			<div class="col-lg-9">
				<input type="text" class="form-control" name="nome" id="input_nome_categoria" value="<?= $this->categoria->getAttributeValue('nome') ?>">
			</div>
		</div>

		<div class="text-right">
			<button type="button" class="btn btn-default btn-sm" id="btNovaCategoria"><span class="glyphicon glyphicon-share-alt" aria-hidden="true"></span> Novo</button>
			<button type="button" id="btGravarCategoria" class="btn btn-primary"><span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar Categoria</button>
		</div>
	</fieldset>
</form>