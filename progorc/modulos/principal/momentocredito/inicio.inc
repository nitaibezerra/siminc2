<?php
/**
 * Gestão de momentos de crédito.
 * $Id: inicio.inc 76511 2014-03-05 21:03:27Z maykelbraz $
 */

/**
 * Cabeçalho simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

/**
 * Helper de exibição de alertas entre requisições.
 * @see Simec_Helper_FlashMessage
 */
require APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
?>
<style type="text/css">
</style>
<script type="text/javascript" language="javascript">
$(document).ready(function(){});
</script>
<div class="row col-md-12">
    <div class="page-header">
        <h4>Momentos de crédito</h4>
    </div>
    <br />
    <div class="well col-md-12">
        <?php require dirname(__FILE__) . '/formMomentoCredito.inc'; ?>
    </div>
    <?php
    $fm = new Simec_Helper_FlashMessage('progorc::momentocredito');
    ?>
    <div class="col-md-12">
        <?php require dirname(__FILE__) . '/listarMomentoCredito.inc'; ?>
    </div>
</div>