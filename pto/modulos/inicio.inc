<?php require(APPRAIZ . "includes/cabecalho.inc"); ?>
<?php $controllerInstituicao = new Controller_Default(); ?>

<div class="container-fluid" style="padding: 0 10px ">
    <div class="page-header">
        <h3>Planos Táticos Operacionais - PTO</h3>
    </div>

    <div class="row well" style="display: none;">
        <div class="col-lg-12">
            <div class="" role="alert" id="div_solucao_selecionada"></div>
            <br>
            <div class="" role="alert" id="div_etapa_selecionada"></div>
        </div>
    </div>
    <?php $controllerInstituicao->indexAction(); ?>
</div>
