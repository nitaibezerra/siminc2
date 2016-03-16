<?php 
if ( $_SESSION['exercicio'] >= 2012 ){
	include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/acao/monitoraacao_2012.inc");
}else{
	include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/acao/monitoraacao_old.inc");
}
?>