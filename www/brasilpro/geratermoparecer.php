<?php
require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/brasilpro/_funcoes.php';
$db = new cls_banco();

if(cte_emitirDocumentos($_SESSION['inuid']))
{
	$db->commit();	
	echo '<script>alert("Parecer foi gerado.");window.opener.location.replace(window.opener.location);self.close();</script>';
}else{
	echo '<script>alert("Ocorreu um problema ao gerar o Parecer");self.close();</script>';
}
/*
if(cte_emitirDocumentos($_SESSION['inuid']))
{
	$db->commit();	
	echo '<script>alert("Termo e Parecer foram gerados.");self.close();</script>';
}else{
	echo '<script>alert("Ocorreu um problema ao gerar o termo e o Parecer");self.close();</script>';
}
*/
?>
