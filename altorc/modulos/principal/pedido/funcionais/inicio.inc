<?php
/**
 * Arquivo da aba funcionais.
 * $Id: inicio.inc 77947 2014-03-26 17:40:04Z maykelbraz $
 */
?>
<div class="row col-md-12">
    <div class="well col-md-12">
        <?php require dirname(__FILE__) . '/formFuncionais.inc'; ?>
    </div>
    <?php
    $fm = new Simec_Helper_FlashMessage('altorc/pedido');
    echo $fm->getMensagens();
    ?>
    <div class="col-md-12">
        <?php require dirname(__FILE__) . '/listarFuncionais.inc'; ?>
    </div>
</div>