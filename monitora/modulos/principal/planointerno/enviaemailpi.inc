<?php
	
 	flush();
 	
	include "planotrabalho/_funcoes.php";
	
	$sql = "SELECT plicod
	  		FROM monitora.planointerno
			where plisituacao <> 'P' limit 5";
	
	
	$dados = $db->carregar( $sql );
			
	foreach($dados as $d){
	
		$pi = "'".$d['plicod']."'";
		 		
		enviaEmailStatusPi($pi);
	
		echo $pi . ",";
		
		flush();
		//sleep(1);
	
	}

?>