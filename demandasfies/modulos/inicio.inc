<?
/*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Autor: Orion Teles de Mesquita
   Finalidade: permitir abrir a página inicial
*/

require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();
$tipodemanda = null;
if ($_POST && isset($_POST['tipodemanda'])) {
	$tipodemanda = $_POST['tipodemanda'];
}
?>
<div id="naoprinta">
	<?php
	$db->cria_aba($abacod_tela, $url, '');
	$modelDemanda = new Demanda($_REQUEST['dmdid']);
	?>

	<style type="text/css">
		p {
			margin-bottom: 0 !important;
		}

		.large {
			width: 100% !important;
		}

		/*general styles*/
		.printable {
			display: none;
		}

		/* print styles*/
		@media print {
			.printable {
				display: block;
			}

			.screen {
				display: none;
			}
		}
	</style>

	<script type="text/javascript">
		/*JS print click handler*/
		//get the modal box content and load it into the printable div
		function imp() {
			$('#naoprinta').addClass('notprint');
			$(".printable").html($(".modal-body").html());

			//fire the print method
			window.print();
			$('#naoprinta').removeClass('notprint');
		}

		function imprimirResumo(dmdid) {
			$('#naoprinta').addClass('notprint');
			$('#div-resumo').load('demandasfies.php?modulo=principal/demandasresumo&acao=A&dmdid=' + dmdid);
			$('#modalResumo').modal();
		}
	</script>


	<?php if (in_array(PFL_NUCLEO_JURIDICO, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)): ?>
		<div class="row">
			<div class="col-lg-offset-3 col-lg-6 col-md-6 ">
				<div class="well">
					<form class="form-horizontal" method="post" id="form-pesquisar-minhas-demandas">
						<input type="hidden" value="pesquisar" name="action">

						<div class="form-group">
							<label class="col-lg-5 col-md-5 control-label" for="dmerelocorrencia">Situação da Demanda:</label>

							<div class="col-lg-7 col-md-7 ">
								<div data-toggle="buttons" class="btn-group">
									<?php $esdIdsProfeCadastramento = ESD_DEMANDA_PROFE.','.ESD_DEMANDA_EM_CADASTRAMENTO; ?>
									<label class="btn btn-default <?php echo($tipodemanda == $esdIdsProfeCadastramento ? 'active' : ''); ?>">
										<input type="radio" value="<?php echo $esdIdsProfeCadastramento ?>" name="tipodemanda"> distribuição </label>
									<label class="btn btn-default <?php echo($tipodemanda == ESD_DEMANDA_ADVOGADO ? 'active' : ''); ?>">
										<input type="radio" value="<?php echo ESD_DEMANDA_ADVOGADO ?>" name="tipodemanda"> validação </label>
									<label class="btn btn-default <?php echo(empty($tipodemanda) ? 'active' : ''); ?>">
										<input type="radio" checked="" value="" name="tipodemanda"> Ambos </label>
								</div>
							</div>
						</div>

						<div class="text-right">
							<button type="submit" class="btn btn-success" title="Salvar">
								<span class="glyphicon glyphicon-search"></span> Pequisar
							</button>
						</div>
					</form>
				</div>
			</div>
		</div>
	<?php endif; ?>

	<div class="row">
		<div class="col-lg-12">
			<div class="well" style="font-size: 12px;">
				<?php $modelDemanda->listarDemandasPorPerfil($perfis); ?>
			</div>
		</div>
	</div>

</div> <!-- fecha <div id="naoprinta"> -->

<div class="screen">
	<div class="modal fade large" id="modalResumo" tabindex="-1" data-width="1000" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
		<div class="modal-dialog large">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
					<h4 class="modal-title">Resumo</h4>
				</div>
				<br>

				<div align="right">
					<button type="button" class="btn btn-default" onclick="imp()">Imprimir</button>
					<button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
					&nbsp;
				</div>
				<div class="modal-body" id="div-resumo">

				</div>
				<div class="modal-footer">
					<button type="button" class="btn btn-default" onclick="imp()">Imprimir</button>
					<button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
				</div>
			</div>
			<!-- /.modal-content -->
		</div>
		<!-- /.modal-dialog -->
	</div>
	<!-- /.modal -->
</div>

<div class="printable"></div>