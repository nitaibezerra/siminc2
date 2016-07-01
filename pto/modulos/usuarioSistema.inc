<?php require(APPRAIZ . "includes/cabecalho.inc"); ?>
<?php $controllerInstituicao = new Controller_Usuario(); ?>

<div class="container-fluid" style="padding: 0 10px ">
    <div class="page-header">
        <h3>Planos Táticos Operacionais - PTO</h3>
    </div>

    <?php $controllerInstituicao->usuarioSistemaAction(); ?>
</div>