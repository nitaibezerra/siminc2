<div class="panel panel-default">
	<div class="alert alert-success" style="margin-bottom: 0px;">
		<h4 class="panel-title">
			<a data-toggle="collapse" data-parent="#accordion" href="#collapseCategoria" style="text-decoration:underline; ">
				<span class="glyphicon glyphicon-th" aria-hidden="true"></span>
				<?= $this->titulo ?> / <?= $this->view->subtitulo ?>
			</a>
		</h4>
	</div>
	<div id="collapseCategoria" class="panel-collapse collapse in">
		<div class="panel-body bg-success">
			<div class="row">
				<div class="col-lg-12" id="div_form_categoria">
					<?php require_once('editar.php'); ?>
				</div>
			</div>

			<div class="row" id="divListarCategoria">
				<?php require_once('listar.php'); ?>
			</div>
		</div>
	</div>
</div>

<script>
	$(function () {
		//gravar
		$('#div_form_categoria').on('click', '#btGravarCategoria', function () {
			$('#form-categoria').saveAjax({controller: 'categoriaBinaria', action: 'salvar', clearForm: false, retorno: true, displayErrorsInput: true, functionSucsess: 'atualizaGridCategoria'});
		});

		//excluir
		$('body').on('click', '.btn_apagar_categoria', function () {
			$.deleteItem({controller: 'categoriaBinaria', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Categoria?', id: $(this).data('id'), functionSucsess: 'atualizaGridCategoria'});
		});

		//limpar
		$('#div_form_categoria').on('click', '#btNovaCategoria', function () {
			$("#input_nome_categoria").val('');
			$("#input_cqbid_categoria").val('');
		});


		//editar
		$('#divListarCategoria').on('click', '.btn_editar_categoria', function () {
			$.post(window.location.href, {'controller': 'categoriaBinaria', 'action': 'editar', 'id': $(this).data('id')}, function (html) {
				$('#div_form_categoria').html(html);
			});
		});

		//selecionar
		$('#divQuestaoBinaria').on('click', '.btn_selecionar_categoria', function () {
			$.post(window.location.href, {'controller': 'questoesBinarias', 'action': 'index', 'idCategoria': $(this).data('id')}, function (html) {
				$('#panelQuestaoBinaria').html(html);
				$('#collapseCategoria').collapse('hide');
				$('#collapseBinario').collapse('show');
			});

		});
	});

	function atualizaGridCategoria() {
		var ideixo = $('#categoria_binaria_ideixo').val();
		$.post(window.location.href, {controller: 'categoriaBinaria', action: 'listar', ideixo:  ideixo}, function (data) {
			$("#input_nome_categoria").val('');
			$("#input_cqbid_categoria").val('');
			$('#divListarCategoria').html(data);
		});
	}
</script>