<?php

include_once APPRAIZ . "www/academico/_funcoes.php";

$existe_portaria = academico_verificaportaria( $_REQUEST["prtid"] );

if( !$existe_portaria ){
	echo "<script>
			alert('Esta portaria não existe!');
			history.back(-1);
		  </script>";
	die;
}



?>