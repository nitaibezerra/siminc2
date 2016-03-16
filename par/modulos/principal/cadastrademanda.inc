<?php
	
	global $db;
	#Se o sisid do sistema estiver nessa tabela ele mostra o cadastro da demanda, senão não mostra.
	$sql = "SELECT sidid FROM demandas.sididassociasisid WHERE sisid = ".$_SESSION['sisid'];
	$sidid = $db->pegaUm( $sql );

	if($sidid){
		$_SESSION['sidid'] = $sidid;
		include APPRAIZ .'includes/cadDemanda.php';
	}
?>