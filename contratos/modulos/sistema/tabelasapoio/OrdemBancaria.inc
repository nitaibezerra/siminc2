<?php
	$schema = "evento"; //informa o schema
	$table = "ordembancaria"; //informa a tabela
	//$showStatus = true; //informa a tabela
	$hideWarning = true; //esconde os alertas, erros e informações de pk, fk, mapeamento de tabelas, etc.
	$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
//								"comentar_tabela", //Permite adicionar / editar comentários sobre a tabela
//								"comentar_coluna", //Permite adicionar / editar comentários sobre a coluna da tabela
//								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
//								"campo_lista", //Permite definir quais os campos aparecerão na lista dos registros da tabela
//								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
								);
	
	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>