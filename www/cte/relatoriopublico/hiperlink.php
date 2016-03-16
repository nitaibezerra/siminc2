<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/Snoopy.class.php";
include "funcoes.php";


foreach($_REQUEST as $indice=>$dado ){
	if($indice == "page"){
		$link = $indice."=".$dado;
	}
	if($indice == "tipo"){
			$link .= "&".$indice."=".$dado;
	}
	if($indice == "codm"){
			$link .= "&".$indice."=".$dado;
	}
	
}

print "<script>top.window.scroll(0,0);</script>";
//exit();
$conexao = new Snoopy;
//$urlReferencia = "http://portal.mec.gov.br/ide/2008/hiperlink.php?".$link;
$conexao->fetch($urlReferencia);
$resultadoInd = $conexao->results;

$resultadoInd = str_replace("window.document.getElementById('foco').focus();",'',$resultadoInd);
$resultadoInd = str_replace('</html>','<script>top.window.scroll(0,0);</script></html>',$resultadoInd);
?>

<link rel="stylesheet" type="text/css" href="estilo.css"/>
<?=$resultadoInd; ?>
<script>top.window.scroll(0,0);</script>

