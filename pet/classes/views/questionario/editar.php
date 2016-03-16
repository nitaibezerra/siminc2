<div class="page-header">
	<h3><?= $this->titulo ?></h3>
</div>

<form role="form" class="form-horizontal" method="post" id="form-questionario">
	<input type="hidden" name="queid" id="quest_queid" value="<?= $this->questionario->getAttributeValue('queid'); ?>">
	<fieldset>
		<div class="form-group">
			<label class="col-lg-4 control-label" for="titulo">Nome</label>

			<div class="col-lg-8">
				<input type="text" class="form-control" id="quest_titulo" name="titulo" maxlength="150" value="<?= $this->questionario->getAttributeValue('titulo'); ?>">
			</div>
		</div>

		<div class="form-group">
			<label class="col-lg-4 control-label" for="dataabertura">Data de Abertura</label>

			<div class="col-lg-4">
				<input type="text" class="form-control" id="quest_dataabertura" name="dataabertura" value="<?= $this->questionario->getAttributeValue('dataabertura'); ?>">
			</div>
		</div>

		<div class="form-group">
			<label class="col-lg-4 control-label" for="dataencerramento">Data de Encerramento</label>

			<div class="col-lg-4">
				<input type="text" class="form-control" id="quest_dataencerramento" name="dataencerramento" value="<?= $this->questionario->getAttributeValue('dataencerramento'); ?>">
			</div>
		</div>

		<div class="form-group text-right">
			<button type="button" class="btn btn-default btn-sm" id="btNovoQuestionario"><span class="glyphicon glyphicon-share-alt" aria-hidden="true"></span> Novo</button>
			<button type="button" class="btn btn-primary" id="btGravarQuestionario"><span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar Questionario</button>
		</div>
	</fieldset>
</form>


<script>
	$(function () {
		$("#quest_dataabertura, #quest_dataencerramento").mask('99/99/9999');

		$("#quest_dataabertura").datepicker({
			defaultDate: "+1w",
			changeMonth: true,
			numberOfMonths: 2,
			onClose: function (selectedDate) {
				$("#quest_dataencerramento").datepicker("option", "minDate", selectedDate);
			}
		});
		$("#quest_dataencerramento").datepicker({
			defaultDate: "+1w",
			changeMonth: true,
			numberOfMonths: 2,
			onClose: function (selectedDate) {
				$("#quest_dataabertura").datepicker("option", "maxDate", selectedDate);
			}
		});

		//limpar
		$('body').on('click', '#btNovoQuestionario', function () {
			$("#quest_titulo, #quest_dataabertura, #quest_dataencerramento, #quest_queid").val('');
		});
	});
</script>