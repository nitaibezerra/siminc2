<?php
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	include "_constantes.php";
	include "_funcoes.php";
	$db = new cls_banco();

	$PstEstados			= $_REQUEST["value"];
	if(isset($PstEstados)){
		$sqlListaMicroregiao="	select mic.miccod as codigo, mes.estuf as estados, mic.micdsc as nome 
								from territorios.microregiao as mic
								inner join territorios.mesoregiao as mes
								on mes.mescod = mic.mescod
								where mes.estuf = '".$PstEstados."' order by nome";
		$microregioes = $db->carregar( $sqlListaMicroregiao );
	}
	
	echo "|--Todos--";
	foreach($microregioes as $microregiao){
		echo "#%",  $microregiao['codigo'], "|", $microregiao['nome'];
	}
?>