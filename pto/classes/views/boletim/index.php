<div class="row">
	<input type="hidden" name="tituloSolucao" id="tituloSolucao" value="<?= $this->tituloSolucao; ?>">

	<div class="col-lg-12" id="div_form_boletim">

		<div class="well">
			<form enctype="multipart/form-data" class="form-horizontal" method="post" id="form-boletim">

				<input type="hidden" value="<?= $this->solucao->getAttributeValue('solid'); ?>" name="solid" id="solid_anexo_boletim"> <input type="hidden" value="" id="arqid" name="arqid"> <input type="hidden" value="boletim" name="controller">
				<input type="hidden" value="salvar" name="action">

				<fieldset>
					<legend>Anexos Boletim</legend>

					<div class="form-group">
						<label class="col-lg-2 col-md-2 control-label" for="file_boletim">Arquivo</label>

						<div class="col-lg-7 col-md-7 ">
							<input type="file" title="Selecionar arquivo" id="file_boletim" name="file_boletim" class="btn btn-primary start">
						</div>
					</div>

					<div class="form-group">
						<label class="col-lg-2 control-label" for="anxdesc">
							<?= $this->boletim->getAttributeLabel('anxdesc'); ?>
						</label>

						<div class="col-lg-6 col-md-6">
							<input type="text" value="<?= $this->boletim->getAttributeValue('anxdesc'); ?>" class="form-control" name="anxdesc" id="anxdesc">
						</div>
					</div>

				</fieldset>

				<div>
					<button type="button" class="btn btn-success" id="btn-salvar-arquivo-boletim" title="Salvar">
						<span class="glyphicon glyphicon-thumbs-up"></span> Salvar
					</button>
				</div>

			</form>
		</div>
	</div>
</div>
<hr>
<fieldset>
	<legend> Lista de Anexos Boletins</legend>
	<div class="row">
		<div class="col-lg-12" id="div_listar_boletim">
			<?php require_once('listar.php'); ?>
		</div>
	</div>
</fieldset>

<script type="text/javascript">
	$(function () {

		$("#btn-salvar-arquivo-boletim").click(function () {
			var formData = new FormData($('#form-boletim')[0]);
			$.ajax({
				url: window.location.href,
				type: 'POST',
				dataType: 'json',
				data: formData,
				cache: false,
				contentType: false,
				processData: false
			}).done(function (data) {
				completeHandler(data);
			});
		});

		function completeHandler(result) {
			var form = $('#form-boletim');

			form.find('.has-error').removeClass('has-error');
			form.find('.erro_input').remove();

			if (result['status'] == true) {
				$('#form-boletim').trigger("reset");
				var html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-success"><strong>Sucesso! </strong>' + result['msg'] + '<a class="alert-link" href="#"></a></div></div>';
				atualizaGridBoletim();
			} else {
				var html = '';


				$('.has-error').removeClass('has-error');
				form.find('.erro_input').remove();

				$(result['result']).each(function () {

					if (this.name) {

						element = form.find('#' + this.name);
						label = form.find('label[for=' + this.name + ']').text();

						if (!label) {
							label = element.closest('.form-group').children('label').text();
						}

						html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
						element.closest('.form-group').addClass('has-error');
						if (!($("#block_error_" + this.name).length > 0)) {
							element.parent().append('<p class="help-block erro_input">' + this.msg + '</p>');
						}
					}
					else if (this.msg) {
						html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
					}
				});

				if (html === '') {
					html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + result['msg'] + '</div></div>'
				}

			}
			$('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
		}
	});
</script>