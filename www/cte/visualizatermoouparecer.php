<?php
/************ INCLUDES ***********************/
require_once "config.inc";
include_once APPRAIZ."includes/classes_simec.inc";
include_once APPRAIZ."includes/funcoes.inc";
include_once APPRAIZ.'www/cte/_funcoes.php';

/************ CARREGA OBJETOS ****************/
$db = new cls_banco();

/************ MOSTRA TERMO OU PARECER ATUAL ********/
if($_REQUEST['documento'] == "termo"){
	if($_REQUEST['terid']){
		$terid = $_REQUEST['terid'];
	}else{
		$terid = NULL;
	}
	echo cte_visualizarTermo($_SESSION['inuid'],$terid);
	
}else if($_REQUEST['documento'] == "parecer"){
	if($_REQUEST['parid']){
		$parid = $_REQUEST['parid'];
	}else{
		$parid = NULL;
	}
	echo cte_visualizarParecer($_SESSION['inuid'],$parid);
}
?>