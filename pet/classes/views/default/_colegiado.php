<?php if ($this->view->identificacaoGrupo->getAttributeValue('idgid')): ?>
	<div class="row">
		<div class="col-lg-12">
			<?php if (!$this->somenteLeitura): ?>
				<div class="well" id="container-form-colegiado">
					<?php require_once('_form_colegiado.php'); ?>
				</div>
			<?php endif; ?>

			<fieldset>
				<legend>Lista de Colegiados</legend>
				<div id="container-listar-colegiado">
					<?= $this->colegiado->getListaColegiadoPorGrupo($this->idGrupo, $this->somenteLeitura); ?>
				</div>
			</fieldset>
		</div>
	</div>

	<?php if (!$this->somenteLeitura): ?>
		<script>
			$(function () {
				$("#container-form-colegiado").on("click", "#bt-colegiado", function () {
					$('#form-colegiado').saveAjax({ action: 'salvar', controller: 'colegiado', retorno: true, displayErrorsInput: true, clearForm: false, functionSucsess: 'atualizaGridColegiado'});
				});

				$('body').on('click', '.btn_Apagar', function () {
					$.deleteItem({controller: 'colegiado', action: 'excluir', retorno: true, text: 'Deseja realmente excluir este fator?', id: $(this).data('id'), functionSucsess: 'atualizaGridColegiado'});
				});

				$('#container-listar-colegiado').on('click', '.btn_Editar', function () {
					$.post(window.location.href, {'controller': 'colegiado', 'action': 'editar', 'id': $(this).data('id'), idGrupo: $('#grpid').val()}, function (html) {
						$('#container-form-colegiado').html(html);
					});
				});
			});
			function atualizaGridColegiado(data) {
				$.post(window.location.href, { controller: 'colegiado', action: 'listar', id: $('#grpid').val() }, function (html) {
					$('#container-listar-colegiado').hide().fadeIn().html(html);
					$('#nome').val('');
					$('#colid').val('');
				});
			}
		</script>
	<?php endif; ?>


<?php else: ?>
	<div role="alert" class="alert alert-danger">
		É obrigatório gravar uma descrição!
	</div>
<?php endif; ?>