<?php
ini_set( 'default_charset', 'utf-8');
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/Snoopy.class.php";
include "funcoes.php";



$municod = $_REQUEST["municod"];

$conexao = new Snoopy;
$urlReferencia = "http://portal.mec.gov.br/ide/2008/gerarTabela.php?municipio=".$municod;
$conexao->fetch($urlReferencia);
$resultadoInd = $conexao->results;

$resultadoInd = str_replace('<img alt="Nova Consulta" src="/novo/img/nova_consulta.gif">','',$resultadoInd);
$resultadoInd = str_replace('<img width="80" height="20" alt="Imprimir" src="/novo/img/imprimir.png">','',$resultadoInd);
$resultadoInd = str_replace('<img align="left" width="380" height="57" alt="Indicadores" src="/novo/img/logo.gif">','',$resultadoInd);
$resultadoInd = str_replace('<a href="hiperlink.php','<a target="_blank" href="http://portal.mec.gov.br/ide/2008/hiperlink.php',$resultadoInd);
?>
<link rel="stylesheet" type="text/css" href="estilo.css"/>
<div>
<?=$resultadoInd; ?>
</div>


