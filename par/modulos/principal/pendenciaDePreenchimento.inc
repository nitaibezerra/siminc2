<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php
ini_set("memory_limit","2000M");
set_time_limit(0);

$inuid 			= $_SESSION['par']['inuid'];
if( $_SESSION['par']['pendenciaBP'] ){
	$itrid 			= 3; 
} else {
	$itrid 			= $_SESSION['par']['itrid']; 
}
$tipoRetorno 	= 'HTML';

if( $inuid && $inuid != '' ){
	echo verificaPendenciaDeAnalise($itrid, $inuid, $tipoRetorno);
} else {
	echo "<script>alert('Falta o Inuid!');history.back(-1);</script>";
	exit;
}

//echo enviarParaAnalise($inuid);


?>

