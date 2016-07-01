<div class="row">
	<div class="col-lg-12" id="div_msg_pto" style="display:none">
		<div class="alert alert-dismissable alert-success">
			<strong>Sucesso! </strong><span id="msg_retorno"></span>
		</div>
	</div>
</div>

<br>

<div class="bs-example bs-example-tabs">

	<ul role="tablist" class="nav nav-tabs notprint" id="tab_solucao">
		<li class="active"><a data-toggle="tab" role="tab" href="pto.php?modulo=inicio&acao=A#cadastro_atividade" id="aba_pesquisa">Pesquisar</a></li>
		<li><a data-toggle="tab" role="tab" href="#cadastro_solucao" id="aba_cadastro_solucao">Cadastro Projeto</a></li>
		<li class="disabled"><a data-toggle="tab" role="tab" href="#cadastro_etapa" id="aba_cadastro_etapa">Cadastro Etapa</a></li>
		<li class="disabled"><a data-toggle="tab" role="tab" href="#cadastro_atividade" id="aba_cadastro_atividade">Cadastro Atividade</a></li>
		<li class="disabled"><a data-toggle="tab" role="tab" href="#anexar_boletim" id="aba_anexar_boletim">Anexar Boletim</a></li>
	</ul>

	<div class="tab-content" id="myTabContent">
		<div id="conteudo_principal" class="tab-pane fade active in">
			<div class="row">
				<div class="col-lg-12">
					<?php require_once('formulario_pesquisa.php'); ?>

<!--					<div class="row notprint">-->
<!--						<div class="col-lg-12">-->
<!--							<button type="button" class="btn btn-link btn-xs" id="exibir_todos">-->
<!--								<span class="glyphicon glyphicon-resize-full"></span> expandir todas as etapas-->
<!--							</button>-->
<!--							|-->
<!--							<button type="button" class="btn btn-link btn-xs" id="esconder_todos">-->
<!--								<span class="glyphicon glyphicon-resize-small"></span> esconder todas as etapas-->
<!--							</button>-->
<!--						</div>-->
<!--					</div>-->

					<hr>

					<div class="row">
						<div class="col-lg-12" id="div_listar">
							<?php require_once('listar.php'); ?>
						</div>
					</div>
				</div>
			</div>
		</div>

		<div id="cadastro_solucao" class="tab-pane fade">
			<div class="row">
				<div class="col-lg-12" id="div_cadastro_solucao"></div>
			</div>
		</div>

		<div id="cadastro_etapa" class="tab-pane fade">
			<div class="row">
				<div class="col-lg-12" id="div_cadastro_etapa"></div>
			</div>
		</div>

		<div id="cadastro_atividade" class="tab-pane fade">
			<div class="row">
				<div class="col-lg-12" id="div_cadastro_atividade"></div>
			</div>
		</div>

		<div id="anexar_boletim" class="tab-pane fade">
			<div class="row">
				<div class="col-lg-12" id="div_anexar_boletim"></div>
			</div>
		</div>
	</div>
</div>

<script type="text/javascript">
	$(function () {
		$('#aba_pesquisa').on('click', function () {
			location.href = '/pto/pto.php?modulo=inicio&acao=C';
		});

		$('#aba_cadastro_solucao').on('click', function () {
			cadastrarSolucao();
		});

		$('#aba_cadastro_etapa').on('click', function (event) {
			if (!$(this).closest('li').hasClass('disabled')) {
				cadastrarEtapa();
			} else {
				event.stopPropagation();
			}
		});

		$('#aba_cadastro_atividade').on('click', function (event) {
			if (!$(this).closest('li').hasClass('disabled')) {
				cadastrarAtividade();
			} else {
				event.stopPropagation();
			}
		});

		$('#aba_anexar_boletim').on('click', function (event) {
			if (!$(this).closest('li').hasClass('disabled')) {
				boletim();
			} else {
				event.stopPropagation();
			}
		});

//		/** BOTOES DE ESCONDER E EXIBIR TODOS */
//		$('#exibir_todos').on('click', function () {
//			$('#table_solucao').find(".btn_visualizar_etapa").trigger('click');
//		});
//
//
//		$('#esconder_todos').click(function (e) {
//			$('#table_solucao').find("a[onclick^='fechar_visualizacao_etapa']").trigger('click');
//		});
	})
</script>