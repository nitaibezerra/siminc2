<div class="panel panel-default">
	<div class="panel-body">
		<button type="button" class="btn btn-default btn-sm bt_voltar_lista_pto"><span class="glyphicon glyphicon-arrow-left" aria-hidden="true"></span> voltar</button>
		<a href="pto.php?modulo=relatorio/painelVersaoImpressao&acao=A&solid=<?= $this->solid; ?>&impressao=1" target="_blank" class="btn btn-primary btn-sm"><span class="glyphicon glyphicon-print" aria-hidden="true"></span> versão para impressão</a>
	</div>
</div>

<div class="row">
	<div class="col-lg-12">
		<h4 class="titulo_principal_solucao">PLANO TÁTICO OPERACIONAL (PTO)</h4>
	</div>
</div>

<div class="row">
	<div class="col-lg-12">
		<h5 class="titulo_solucao"><?= $this->dado['solucao']['solnumero'] ?> - <?= $this->dado['solucao']['soldsc'] ?></h5>
	</div>
</div>

<div class="row">
	<div class="col-lg-12">
		<?php require('_tbl_painel.php'); ?>
	</div>
</div>

<div class="row">
	<?php require('_tbl_indicadores_metas_pne.php'); ?>
</div>

<div class="row">
	<div class="col-lg-6">
		<h5 class="titulo_solucao">Indicadores do Projeto</h5>
		<?php require('_tbl_indicadores.php'); ?>
	</div>

	<div class="col-lg-6">
		<h5 class="titulo_solucao">Orçamento das Ações Vinculadas</h5>
		<?php require('_tbl_acoes.php'); ?>
	</div>
</div>
<br>

<div class="row">
	<div class="col-lg-12">
		<h5 class="titulo_solucao">Plano de Implementação</h5>
	</div>
</div>
<?php require('_etapa_atividade.php'); ?>

<script type="text/javascript">
	$(function () {
		$('.bt_voltar_lista_pto').on('click', function () {
			location.href = '/pto/pto.php?modulo=inicio&acao=C';
		});
	})
</script>