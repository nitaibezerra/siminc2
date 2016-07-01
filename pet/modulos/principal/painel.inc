<?php
include APPRAIZ . "includes/cabecalho_bootstrap.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/funcoesspo.php";

$controller = new Controller_Default();
?>

<div class="container-fluid" style="padding: 0 10px ">
    <?php $controller->indexAction(); ?>
</div>