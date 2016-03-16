<?php
$schema = "contratos"; //informa o schema
$table = "ctretencaocontrato"; //informa a tabela
$showStatus = false; //informa a tabela
$hideWarning = true; //esconde os alertas, erros e informações de pk, fk, mapeamento de tabelas, etc.
$arrAttr["retalicota"] = array( "mascara" => "###,##");
$arrAttr["retir"] = array( "mascara" => "###,##");
$arrAttr["retcsll"] = array( "mascara" => "###,##");
$arrAttr["retconfins"] = array( "mascara" => "###,##");
$arrAttr["retpasep"] = array( "mascara" => "###,##");
$arrPermission = array(
		"inserir", //Permite inserir registros na tabela
		"alterar", //Permite alterar registros da tabela
//		"excluir", //Permite excluir registros da tabela
		//								"comentar_tabela", //Permite adicionar / editar comentários sobre a tabela
//								"comentar_coluna", //Permite adicionar / editar comentários sobre a coluna da tabela
//								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
//								"campo_lista", //Permite definir quais os campos aparecerão na lista dos registros da tabela
//								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
);

include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>