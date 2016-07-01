<?php
	$id = $_GET['id'];
	$sql = "SELECT
	ret.retdsc
	FROM
	contratos.ctretencaocontrato ret

	WHERE
	ret.retid = $id";
	
	$retencao = $db->pegaUm($sql);

	echo str_replace(";", ";<br><br>", $retencao);

?>
<br><br>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<input type="button" name="fechar" value="fechar" id="fechar" onclick="window.close();" align="center">
