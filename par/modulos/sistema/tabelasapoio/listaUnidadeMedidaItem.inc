<?php
	$titulo = "Unidade de Medida do Item de Composiзгo"; //Monta o tнtulo da pбgina
	$schema = "par"; //informa o schema
	$table = "unidademedidadetalhamentoitem"; //informa a tabela
	$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
//								"comentar_tabela", //Permite adicionar / editar comentбrios sobre a tabela
//								"comentar_coluna", //Permite adicionar / editar comentбrios sobre a coluna da tabela
//								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
//								"campo_lista", //Permite definir quais os campos aparecerгo na lista dos registros da tabela
//								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
								);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>