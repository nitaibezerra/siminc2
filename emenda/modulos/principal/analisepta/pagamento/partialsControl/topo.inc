<?php
include_once APPRAIZ . 'includes/workflow.php';
// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_SESSION["emenda"]["ptridAnalise"] ) ? $_SESSION["emenda"]["ptridAnalise"] : $_SESSION["emenda"]["ptrid"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

validaSessionPTA( $ptrid );

$obPagamento = new PagamentoFNDE();
$obPagamento->setPta( $ptrid );
?>