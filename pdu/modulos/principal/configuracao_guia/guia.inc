<?php include APPRAIZ . "includes/cabecalho.inc"; ?>

<div id="container_arvore">
    <?php 
        $controllerGuia = new Controller_Guia();
        $controllerGuia->arvoreAction();
    ?>
</div>