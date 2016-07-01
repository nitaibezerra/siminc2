<?
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";

include APPRAIZ . "includes/cabecalho.inc";
?>
<div id="naoprinta">
	<?php
	$db->cria_aba($abacod_tela, $url, '');

	$modelDemanda = new Demanda($_REQUEST['dmdid']);

	$perfis = pegaPerfilGeral();
	$perfis = $perfis ? $perfis : array();
	?>
</div>

<div class="row">
	<div class="col-lg-12">
		<div class="panel panel-default">
			<div class="panel-heading">
				<h3 class="panel-title">Demandas por Situação</h3>
			</div>
			<div class="panel-body">
				<div class="row">
					<div class="col-lg-6">

						<div class="panel panel-default">
							<div class="panel-body">
								<?php $modelDemanda->exibirGraficoPorSituacao(); ?>
							</div>
						</div>

						<?php if (in_array(PFL_NUCLEO_JURIDICO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)): ?>
							<div class="panel panel-default">
								<div class="panel-body">
									<?php $modelDemanda->getDadosGraficoEmDistribuicaoValidacao(); ?>
								</div>
							</div>
						<?php endif; ?>
					</div>
					<div class="col-lg-6">
						<?php $modelDemanda->listarQuantidadePorSituacao(); ?>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

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

