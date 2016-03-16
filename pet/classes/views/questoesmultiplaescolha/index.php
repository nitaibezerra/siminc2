<br>
<div class="row">
	<div class="col-lg-12" id="div_form_multipla_escolha">
		<?php require_once('editar.php'); ?>
	</div>
</div>

<hr>

<div class="row" id="divListarMultiplaEscolha">
	<?php require_once('listar.php'); ?>
</div>

<script>
	$(function () {
		//limpar
		$('#div_form_multipla_escolha').on('click', '#btNovaQuestoesMultiplaEscolha', function () {
			$("#input_titulo, #text_descricao, #input_conceito1, #input_conceito2, #input_conceito3, #input_conceito4, #input_conceito5, #input_qmeid").val('');
		});

		//gravar
		$('#div_form_multipla_escolha').on('click', '#btGravarquestoesMultiplaEscolha', function () {
			$('#form-multipla-escolha').saveAjax({controller: 'questoesMultiplaEscolha', action: 'salvar', clearForm: false, retorno: true, displayErrorsInput: true, functionSucsess: 'atualizaGridMultiplaEscolha'});
		});

		//excluir
		$('body').on('click', '.btn_apagar_multipla_escolha', function () {
			$.deleteItem({controller: 'questoesMultiplaEscolha', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Questão?', id: $(this).data('id'), functionSucsess: 'atualizaGridMultiplaEscolha'});
		});

		//editar
		$('#divListarMultiplaEscolha').on('click', '.btn_editar_multipla_escolha', function () {
			$.post(window.location.href, {'controller': 'questoesMultiplaEscolha', 'action': 'editar', 'id': $(this).data('id')}, function (html) {
				$('#div_form_multipla_escolha').html(html);
			});
		});
	});

	function atualizaGridMultiplaEscolha() {
		var ideixo = $('#input_ideixo').val();
		$.post(window.location.href, {controller: 'questoesMultiplaEscolha', action: 'listar', ideixo:  ideixo }, function (data) {
			$("#input_titulo, #text_descricao, #input_conceito1, #input_conceito2, #input_conceito3, #input_conceito4, #input_conceito5, #input_qmeid").val('');
			$('#divListarMultiplaEscolha').html(data);
		});
	}
</script>