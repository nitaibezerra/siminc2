<div class="row">
	<div class="col-lg-6" id="div_form_questionario">
		<?php require_once('editar.php'); ?>
	</div>
</div>

<hr>

<div class="row" id="divListarQuestionario">
	<div class="col-lg-12">
		<div class="page-header">
			<h3>Lista de Questionários</small></h3>
		</div>
		<?= $this->questionario->getListaQuestionario(); ?>
	</div>
</div>

<script>
	$(function () {
		$('#div_form_questionario').on('click', '#btGravarQuestionario', function () {
			$('#form-questionario').saveAjax({controller: 'questionario', action: 'salvar', retorno: true, displayErrorsInput: true, functionSucsess: 'atualizaGridQuestionario'});
		});

		//editar
		$('body').on('click', '.btn_apagar_questionario', function () {
			$.deleteItem({controller: 'questionario', action: 'excluir', retorno: true, text: 'Deseja realmente excluir este Questionário?', id: $(this).data('id'), functionSucsess: 'atualizaGridQuestionario'});
		});

		//deletar
		$('#divListarQuestionario').on('click', '.btn_editar_questionario', function () {
			$.post(window.location.href, {'controller': 'questionario', 'action': 'editar', 'id': $(this).data('id')}, function (html) {
				$('#div_form_questionario').html(html);
			});
		});

		//ir para o eixo
		$('#divListarQuestionario').on('click', '.btn_selecionar', function () {
			window.location.href = '/pet/pet.php?modulo=principal/questionario/eixo&acao=A&id='+$(this).data('id');
		});
	});

	function atualizaGridQuestionario() {
		$.post(window.location.href, {controller: 'questionario', action: 'listar'}, function (data) {
			$('#divListarQuestionario').html(data);
		});
	}
</script>