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
	<div class="col-lg-12">
		<?php require('_tbl_indicadores_metas_pne.php'); ?>
	</div>
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

<?php if (isset($_GET['impressao']) && $_GET['impressao'] == 1 ): ?>
	<script type="text/javascript">
		$(function () {
			window.print();
			$('.navbar, .rodape').hide();
			$('#top-shadow').remove();
		})
	</script>
<?php endif; ?>