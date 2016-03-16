<form role="form" class="form-horizontal" method="post" id="form-eixo">
	<input type="hidden" name="ideixo" id="input_eixo_ideixo" value="<?= $this->eixo->getAttributeValue('ideixo'); ?>">

	<fieldset>

		<div class="form-group">
			<label class="col-lg-4 control-label" for="nome">Nome</label>

			<div class="col-lg-8">
				<input type="text" class="form-control" id="input_nome" name="nome" value="<?= $this->eixo->getAttributeValue('nome'); ?>">
			</div>
		</div>

		<div class="form-group">
			<label class="col-lg-4 control-label" for="descricao">Descrição</label>

			<div class="col-lg-8">
				<textarea class="form-control" name="descricao" id="input_descricao"><?= $this->eixo->getAttributeValue('descricao'); ?></textarea>
			</div>
		</div>

		<div class="form-group">
			<label for="tipo" class="col-lg-4 control-label">Tipo da Questão?</label>

			<div class="col-lg-8">
				<div class="radio">
					<input type="radio" name="tipo" value="B" checked="<?= ($this->eixo->getAttributeValue('tipo') == 'B' ? 'checked' : '') ?>"> Binária <br>
					<input type="radio" name="tipo" value="M" checked="<?= ($this->eixo->getAttributeValue('tipo') == 'M' ? 'checked' : '') ?>"> Multipla Escolha </label>
				</div>
			</div>
		</div>


		<div class="text-right">
			<button type="button" class="btn btn-default btn-sm" id="btNovoEixo"><span class="glyphicon glyphicon-share-alt" aria-hidden="true"></span> Novo</button>
			<button type="button" class="btn btn-primary text-right" id="btGravarEixo"><span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar Eixo</button>
		</div>

	</fieldset>
</form>
