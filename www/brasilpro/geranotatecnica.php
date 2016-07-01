<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/brasilpro/_funcoes.php';

$db = new cls_banco();

if(cte_emitirNotaTecnica( $_SESSION['inuid'] ) ){
	$db->commit();	
	echo '<script>alert("Nota Técnica foi gerada com sucesso.");window.opener.location.replace(window.opener.location);self.close();</script>';
}
else{
	echo '<script>alert("Ocorreu um problema ao gerar a nota Técnica");self.close();</script>';
}
?>
