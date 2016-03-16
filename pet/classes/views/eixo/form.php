<div class="row">

	<div class="col-lg-4">
		<div class="page-header">
			<h1><?= $this->titulo; ?></h1>
			<h4><?= $this->subtitulo; ?></h4>
		</div>

		<div id="div_form_eixo">
			<?php require_once('editar.php'); ?>
		</div>

		<div class="row" id="divListarEixo">
			<?php require_once('listar.php'); ?>
		</div>
	</div>

	<div class="col-lg-8" id="div_questao"></div>
</div>

<script type="text/javascript">
//	radioBtn('#radioBtn label');
	$(function () {
		//salvar
		$('#div_form_eixo').on('click', '#btGravarEixo', function () {
			$('#form-eixo').saveAjax({controller: 'eixo', action: 'salvar', retorno: true, displayErrorsInput: true, functionSucsess: 'atualizaGridEixo'});
		});

		//deletar
		$('body').on('click', '.btn_apagar_eixo', function () {
			$.deleteItem({controller: 'eixo', action: 'excluir', retorno: true, text: 'Deseja realmente excluir este Eixo?', id: $(this).data('id'), functionSucsess: 'atualizaGridEixo'});
		});

		//editar
		$('#divListarEixo').on('click', '.btn_editar_eixo', function () {
			$.post(window.location.href, {'controller': 'eixo', 'action': 'editar', 'id': $(this).data('id')}, function (html) {
				$('#div_form_eixo').html(html);
			});
		});

		//limpar
		$('#div_form_eixo').on('click', '#btNovoEixo', function () {
			$("#input_numeroeixo").val('');
			$("#input_nome").val('');
			$("#input_descricao").val('');
			$("#input_eixo_ideixo").val('');
		});

		//ir para a questao
		$('body').on('click', '.btn_selecionar_eixo', function () {
			$.post(window.location.href, {controller: 'questoes', action: 'index', id: $(this).data('id')}, function (data) {
				$('#div_questao').html(data);
			}, 'html');
		});
	});

	function atualizaGridEixo() {
		$("#input_numeroeixo").val('');
		$("#input_nome").val('');
		$("#input_descricao").val('');
		$("#input_eixo_ideixo").val('');
		$.post(window.location.href, {controller: 'eixo', action: 'listar'}, function (data) {
			$('#divListarEixo').html(data);
		});
	}
</script>