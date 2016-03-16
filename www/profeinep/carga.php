<?
	global $db;
	$sql = "update profeinep.processoprofeinep set prcstatus='A' where prcnumsidoc='23000014795201019'";
	$resposta = $db->executar($sql);
	echo 'resultado = ' .$resposta;
?>



