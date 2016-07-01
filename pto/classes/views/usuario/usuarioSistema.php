<div class="row" id="div_msg_vinc_usu" style="display:none">
	<div class="col-lg-12">
		<div class="alert alert-dismissable alert-success">
			<strong id="msg_vinc_usu">Usuário vinculado com sucesso! </strong>
		</div>
	</div>
</div>

<div class="modal fade vincular_usuario">
	<div class="modal-dialog">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
				<h4 class="modal-title">Deseja vincular este usuário ao sistema?</h4>
			</div>
			<div class="modal-body">
				<div class="alert alert-info" id="div_usuario" role="alert"></div>
				<button type="button" class="btn btn-success btn_vinc_usu" id="bt_vincular_usuario" data-usuid=""><span class="glyphicon glyphicon-thumbs-up" aria-hidden="true"></span> Sim</button>
				<button type="button" class="btn btn-danger btn_vinc_usu"><span class="glyphicon glyphicon-thumbs-down" aria-hidden="true"></span> Não</button>
			</div>
			<div class="modal-footer"></div>
		</div>
	</div>
</div>


<div class="row">
	<div class="col-lg-12">

	</div>
</div>

<div class="row">
	<div class="col-lg-12">
		<div class="well">
			<form id="form_pesquisar_usuario" class="form-horizontal">
				<fieldset>
					<legend>Pesquisar</legend>

					<p class="help-block">* É necessário preecher ao menos um dos campos abaixo:</p>

					<div class="form-group">
						<label for="cpf" class="col-lg-3 control-label">CPF:</label>

						<div class="col-lg-4">
							<input type="text" id="cpf" name="cpf" class="form-control">
						</div>
					</div>

					<div class="form-group">
						<label for="nome" class="col-lg-3 control-label"> Nome completo (ou parte do nome):</label>

						<div class="col-lg-4">
							<input type="text" id="nome" name="nome" class="form-control">
						</div>
					</div>

					<div class="text-right">
						<button id="btn_pesquisar_usuario" title="Pequisar" class="btn btn-success" type="button">
							<span class="glyphicon glyphicon-search"></span> Pequisar
						</button>
						<button id="btn_limpar_pesquisa" title="Limpar" class="btn btn-default" type="button">
							<span class="glyphicon glyphicon-repeat"></span> Limpar
						</button>
					</div>
				</fieldset>
			</form>
		</div>
	</div>
</div>

<div class="row">
	<div class="col-lg-12" id="div_listar_usuarios"></div>
</div>

<script type="text/javascript">
	$(function () {
		$('#cpf').mask('999.999.999-99');

		$('#btn_pesquisar_usuario').on('click', function (event) {

			if ( $('#cpf').val().length == 0 && $('#nome').val().length == 0 ){
				$('.form-group').addClass('has-error');
			}else{
				$.post(window.location.href, {'controller': 'usuario', 'action': 'listar', 'parans': $('#form_pesquisar_usuario').serialize()}, function (html) {
                	$('#div_listar_usuarios').html(html);
                	$('.form-group').removeClass('has-error');
            	}, 'html');
			}
		})

		$('.btn_vinc_usu').on('click', function (event) {
			$('.vincular_usuario').modal('hide');
		})

		$('#bt_vincular_usuario').on('click', function (event) {
			 $.post(window.location.href, {'controller': 'usuario', 'action': 'vincularUsuario', usuid: $(this).data('usuid')}, function (data) {
                $('#div_msg_vinc_usu').show().delay(3000).fadeOut();
                $('#msg_vinc_usu').html(data['msg'])
            }, 'json');
		});

	});

	function vincularSistema(cpf, nome){
			$('.vincular_usuario').modal('show');
			$('#div_usuario').html( cpf +' - ' + nome);
			$('#bt_vincular_usuario').attr('data-usuid', cpf );
	}


</script>


