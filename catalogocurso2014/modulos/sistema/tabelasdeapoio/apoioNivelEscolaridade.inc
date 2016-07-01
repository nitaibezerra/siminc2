<?php
	$esconderCabecalho = true;
	
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';

	monta_abas();
	
	$schema = "catalogocurso2014"; //informa o schema
	$table = "nivelescolaridade"; //informa a tabela
	
	$arrPermission = array(
							"inserir", //Permite inserir registros na tabela
							"alterar", //Permite alterar registros da tabela
							"excluir", //Permite excluir registros da tabela
							);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>