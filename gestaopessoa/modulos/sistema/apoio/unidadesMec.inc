<?php
	$titulo = "Unidades Mec"; //Monta o título da página
	$schema = "academico"; //informa o schema
	$table = "unidademec"; //informa a tabela
	//$hideWarning = true; //esconde os alertas, erros e informações de pk, fk, mapeamento de tabelas, etc.
	$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
								);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>