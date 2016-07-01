<div class="panel panel-default panel_binario">
	<div class="alert alert-info" style="margin-bottom: 0px;">
		<h4 class="panel-title">
			<span class="glyphicon glyphicon-list" aria-hidden="true"></span>
<!--			<a data-toggle="collapse" data-parent="#accordion" href="#collapseBinario" style="padding-left:30px;"></a>-->
			<?= $this->view->titulo . ' / '. $this->view->subtitulo; ?>
		</h4>
	</div>
	<div id="collapseBinario" class="panel-collapse collapse">
		<div class="panel-body">
			<div class="row">
				<div class="col-lg-12" id="div_form_questoes_binarias">
					<?php require_once('editar.php'); ?>
				</div>
			</div>

			<div class="row" id="divListarBinario">
				<?php require_once('listar.php'); ?>
			</div>

		</div>
	</div>
</div>

<script>
	$(function () {
		//gravar
		$('#div_form_questoes_binarias').on('click', '#btGravarQuestaoBinaria', function () {
			$('#form-categoria-questoes-binaria').saveAjax({controller: 'questoesBinarias', action: 'salvar', clearForm: false, retorno: true, displayErrorsInput: true, retorno:true, functionSucsess: 'atualizaGridQuestoesBinarias'});
		});

		//limpar
		$('#div_form_questoes_binarias').on('click', '#btNovaQuestaoBinaria', function () {
			$("#input_titulo").val('');
			$("#input_numeroquestao").val('');
			$("#ideixo").val('');
		});


		//excluir
		$('body').on('click', '.btn_apagar_binaria', function () {
			$.deleteItem({controller: 'questoesBinarias', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Questão?', id: $(this).data('id'), retorno:true, functionSucsess: 'atualizaGridQuestoesBinarias'});
		});

		//editar
		$('#divListarBinario').on('click', '.btn_editar_binaria', function () {
			$.post(window.location.href, {'controller': 'questoesBinarias', 'action': 'editar', 'id': $(this).data('id')}, function (html) {
				$('#div_form_questoes_binarias').html(html);
			});
		});
	});

	function atualizaGridQuestoesBinarias(retorno) {
		var cqbid = retorno.cqbid;
		$.post(window.location.href, {controller: 'questoesBinarias', action: 'listar', 'cqbid': cqbid}, function (data) {
			$("#input_titulo").val('');
			$("#input_numeroquestao").val('');
			$("#input_qubid").val('');
			$('#divListarBinario').html(data);
		});
	}
</script>