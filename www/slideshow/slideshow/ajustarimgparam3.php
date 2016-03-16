<?
session_start();
$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");

if($_REQUEST['obrid']) {
	$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$_REQUEST['obrid']." AND aqostatus = 'A'", 
											   "tabela" => "obras.arquivosobra");
}

if($_REQUEST['preid']) {
	$_SESSION['imgparams'] = array("filtro" => "cnt.preid=".$_REQUEST['preid']."", 
											   "tabela" => "obras.preobrafotos");
}


header("location: index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=".$_REQUEST['_sisarquivo']);
exit;
?>