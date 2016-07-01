<?php include_once APPRAIZ . "demandasfies/modulos/principal/demandasformulario_controller.inc"; ?>

<div id="modal-observacao" class="modal fade">
	<div class="modal-dialog">
		<div class="modal-content text-center">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
				<h4 class="modal-title" style="font-weight: bold;">Detalhe</h4>
			</div>
			<div class="modal-body text-left" id="conteudo_modal_observacao">

			</div>
			<div class="modal-footer">
				<button type="button" class="btn btn-danger" data-dismiss="modal"><span
						class="glyphicon glyphicon-remove"></span> Fechar
				</button>
			</div>
		</div>
		<!-- /.modal-content -->
	</div>
	<!-- /.modal-dialog -->
</div><!-- /.modal -->

<div id="modal-alterar-prazo" class="modal fade">
	<div class="modal-dialog">
		<div class="modal-content text-center">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
				<h4 class="modal-title" style="font-weight: bold;">Alteraçao de Prazo da Área</h4>
			</div>
			<div class="modal-body text-left" id="conteudo_modal_prazo">

			</div>
			<div class="modal-footer">
				<button title="Salvar" class="btn btn-success" type="button" id="tramitar-prazo">
					<span class="glyphicon glyphicon-thumbs-up"></span> Salvar
				</button>
				<button type="button" class="btn btn-danger" data-dismiss="modal">
					<span class="glyphicon glyphicon-remove"></span> Fechar
				</button>
			</div>
		</div>
		<!-- /.modal-content -->
	</div>
	<!-- /.modal-dialog -->
</div><!-- /.modal -->

<div id="modal-intervencao" class="modal fade">
	<div class="modal-dialog">
		<div class="modal-content text-center">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
				<h4 class="modal-title" style="font-weight: bold;">Solicitar intervenção da área</h4>
			</div>
			<div class="modal-body text-left">

				<form id="form-save-intervencao" method="post" class="form-horizontal">
					<input name="action" type="hidden" value="salvar-intervencao"> <input name="dmdid" id="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>"> <input name="esdidorigem" id="esdidorigem" type="hidden"
																																													value="<?php echo $estadoAtual['esdid']; ?>">

					<div class="form-group">
						<label for="dmdid" class="col-lg-4 col-md-4 control-label">Área:</label>

						<div class="col-lg-8 col-md-8 " style="padding-top: 5px;">
							<?php
							$sql = "select pflcod codigo, pfldsc as descricao
                                    from seguranca.perfil
                                    where sisid = " . SIS_DEMANDASFIES . "
                                    and pflcod not in (" . PFL_SUPER_USUARIO . ", " . PFL_ADMINISTRADOR . ", " . PFL_CONSULTA . ")
                                    order by pfldsc;";

							echo $db->monta_combo("pflcod", $sql, 'S', "Selecione", "", "", "", "200", "S", "pflcod_t", "");
							?>
						</div>
					</div>

					<div class="form-group">
						<label for="cmddsc" class="col-lg-4 col-md-4 control-label">Justificativa:</label>

						<div class="col-lg-8 col-md-8 ">
							<textarea class="form-control" rows="5" name="cmddsc" id="cmddsc_t"></textarea>
						</div>
					</div>

				</form>

			</div>
			<div class="modal-footer">
				<button title="Salvar" class="btn btn-success" type="button" id="tramitar-intervencao"><span
						class="glyphicon glyphicon-thumbs-up"></span> Salvar
				</button>
				<button type="button" class="btn btn-danger" data-dismiss="modal"><span
						class="glyphicon glyphicon-thumbs-down"></span> Cancelar
				</button>
			</div>
		</div>
		<!-- /.modal-content -->
	</div>
	<!-- /.modal-dialog -->
</div><!-- /.modal -->

<div id="modal-retorno-intervencao" class="modal fade">
	<div class="modal-dialog">
		<div class="modal-content text-center">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
				<h4 class="modal-title" style="font-weight: bold;">Retornar ao demandante</h4>
			</div>
			<div class="modal-body text-left">

				<form id="form-save-retorno-intervencao" method="post" class="form-horizontal">
					<input name="action" type="hidden" value="salvar-retorno-intervencao"> <input name="dmdid" id="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>">

					<div class="form-group">
						<label for="cmddsc" class="col-lg-4 col-md-4 control-label">Justificativa:</label>

						<div class="col-lg-8 col-md-8 ">
							<textarea class="form-control" rows="5" name="cmddsc" id="cmddsc_r"></textarea>
						</div>
					</div>

				</form>

			</div>
			<div class="modal-footer">
				<button title="Salvar" class="btn btn-success" type="button" id="tramitar-retorno-intervencao"><span
						class="glyphicon glyphicon-thumbs-up"></span> Salvar
				</button>
				<button type="button" class="btn btn-danger" data-dismiss="modal"><span
						class="glyphicon glyphicon-thumbs-down"></span> Cancelar
				</button>
			</div>
		</div>
		<!-- /.modal-content -->
	</div>
	<!-- /.modal-dialog -->
</div><!-- /.modal -->

<script type="text/javascript">
	$(function () {
		$('#tramitar-prazo').click(function () {

			if (!$('#reaprazo_alteracao').val()) {
				alert("O campo 'Novo Prazo' é obrigatório.");
				return false;
			}

			if (!$('#reajustificativa_alteracao').val()) {
				alert("O campo 'Justificativa' é obrigatório.");
				return false;
			}

			$('#form-save-prazo').submit();
		});

		$('#tramitar-intervencao').click(function () {

			if (!$('#pflcod_t').val()) {
				alert("O campo 'Área' é obrigatório.");
				return false;
			}

			if (!$('#cmddsc_t').val()) {
				alert("O campo 'Justificativa' é obrigatório.");
				return false;
			}

			$('#form-save-intervencao').submit();
		});

		$('#tramitar-retorno-intervencao').click(function () {

			if (!$('#cmddsc_r').val()) {
				alert("O campo 'Justificativa' é obrigatório.");
				return false;
			}

			$('#form-save-retorno-intervencao').submit();
		});
	});
	function dateDiferencaEmDias(a, b) {
		// Descartando timezone e horário de verão
		var utc1 = Date.UTC(a.getFullYear(), a.getMonth(), a.getDate());
		var utc2 = Date.UTC(b.getFullYear(), b.getMonth(), b.getDate());

		return Math.floor((utc2 - utc1) / ( 1000 * 60 * 60 * 24));
	}
</script>