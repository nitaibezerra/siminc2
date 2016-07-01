<form role="form" class="form-horizontal" method="post" id="form-multipla-escolha">
	<input type="hidden" name="ideixo" id="input_ideixo" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('ideixo'); ?>">
	<input type="hidden" name="qmeid" id="input_qmeid" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('qmeid') ?>">
	<fieldset>
		<legend><?= $this->titulo ?> /
			<small><?= $this->view->subtitulo ?></small>
		</legend>

		<div class="form-group">
			<label class="col-lg-3 control-label" for="titulo">Título</label>

			<div class="col-lg-9">
				<input type="text" class="form-control" id="input_titulo" name="titulo" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('titulo') ?>">
			</div>
		</div>

		<div class="form-group">
			<label class="col-lg-3 control-label" for="descricao">Texto Explicativo</label>

			<div class="col-lg-9">
				<textarea class="form-control" name="descricao" id="text_descricao"><?= $this->questaoMultiplaEscolha->getAttributeValue('descricao') ?></textarea>
			</div>
		</div>

		<div class="well">
			<?php
			if (is_array($this->conceitos) && !empty($this->conceitos) ):
				foreach ($this->conceitos as $conceito):?>
					<div class="form-group">
						<label class="col-lg-2 control-label" for="conceito1">Opção <?= $conceito['ordem'] ?></label>

						<div class="col-lg-10">
							<input type="text" class="form-control" name="conceito[]" id="input_conceito1" value="<?= $conceito['texto'] ?>">
						</div>
					</div>
				<?php endforeach; ?>
			<?php else: ?>

				<div class="form-group">
					<label class="col-lg-2 control-label" for="conceito2">Opção 1</label>

					<div class="col-lg-10">
						<input type="text" class="form-control" name="conceito[]" id="input_conceito2" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('conceito2') ?>">
					</div>
				</div>

				<div class="form-group">
					<label class="col-lg-2 control-label" for="conceito2">Opção 2</label>

					<div class="col-lg-10">
						<input type="text" class="form-control" name="conceito[]" id="input_conceito2" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('conceito2') ?>">
					</div>
				</div>

				<div class="form-group">
					<label class="col-lg-2 control-label" for="conceito3">Opção 3</label>

					<div class="col-lg-10">
						<input type="text" class="form-control" name="conceito[]" id="input_conceito3" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('conceito3') ?>">
					</div>
				</div>

				<div class="form-group">
					<label class="col-lg-2 control-label" for="conceito4">Opção 4</label>

					<div class="col-lg-10">
						<input type="text" class="form-control" name="conceito[]" id="input_conceito4" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('conceito4') ?>">
					</div>
				</div>

				<div class="form-group">
					<label class="col-lg-2 control-label" for="conceito5">Opção 5</label>

					<div class="col-lg-10">
						<input type="text" class="form-control" name="conceito[]" id="input_conceito5" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('conceito5') ?>">
					</div>
				</div>

			<?php endif; ?>



		</div>

		<div class="text-right">
			<button type="button" class="btn btn-default btn-sm" id="btNovaQuestoesMultiplaEscolha"><span class="glyphicon glyphicon-share-alt" aria-hidden="true"></span> Novo</button>
			<button type="button" id="btGravarquestoesMultiplaEscolha" class="btn btn-primary"><span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar Questão de Multipla Escolha</button>
		</div>


	</fieldset>
</form>