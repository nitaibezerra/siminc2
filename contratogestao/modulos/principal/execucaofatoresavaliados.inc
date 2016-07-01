<?php require(APPRAIZ . "includes/cabecalho.inc"); ?>
<?php $controllerExecucaoFatorAvaliado = new Controller_ExecucaoFatorAvaliado(); ?>

<div class="container-fluid" style="padding: 0 10px ">
    <div class="page-header">
        <h3>Execução Fator Avaliado</h3>
    </div>
    <?php $controllerExecucaoFatorAvaliado->indexAction(); ?>
</div>