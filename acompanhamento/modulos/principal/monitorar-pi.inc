<?php

    $urlModulo = '?modulo=principal/monitorar-ppa&acao=A';

    # Requisições
    include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-request.inc";

    # Cabeçalho
    include APPRAIZ . "includes/cabecalho.inc";
    
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        
        <?php
        
            # Requisições
            include_once APPRAIZ. "acompanhamento/modulos/principal/monitorar-pi-cad.inc";
        
        ?>

    </div>
</div>
