<?php

	//include APPRAIZ .'includes/envia_email_sis_geral.inc';
	
	if($_REQUEST['enviar_email'] == 'S'){
		echo "<meta HTTP-EQUIV='refresh' CONTENT='0; URL=pdeinterativo.php?modulo=sistema/geral/envia_email_sis&acao=A'>";
		include APPRAIZ .'seguranca/www/scripts_exec/envio_email_walace.php';
		$_POST['enviar_email'] = 'S';
	}else{
		include APPRAIZ .'seguranca/www/scripts_exec/envio_email_walace.php';
	}
	
?>