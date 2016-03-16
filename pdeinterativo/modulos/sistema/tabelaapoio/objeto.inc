<?php
	$schema = "pdeinterativo"; //informa o schema
	$table = "objetoapoio"; //informa a tabela
	
	$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
								);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>