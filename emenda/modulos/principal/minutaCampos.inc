<?php
$titulo = "Tipos de Macros"; //Monta o tэtulo da pсgina
$schema = "emenda"; //informa o schema
$table = "minutacampos"; //informa a tabela
//$hideWarning = true; //esconde os alertas, erros e informaчѕes de pk, fk, mapeamento de tabelas, etc.
//$showPK = true; //exibe o campo de PK (readonly)
//$showStatus = true; //exibe o combo de status (A,I e H)
$arrOrdemColunas = array("miccampo","mictabela", "miccoluna", "micstatus", "micschema", "micdescricao"); //Ordena a exibiчуo das colunas na tela, pode ser agrupado.

//$ptsid = 8;

//$arrWhere = array("propostatiposubacao.frmid = 1","frmdsc like '%A%'"); //Array de filtros p/ a lista.

/*
 * Ex: $arrOrdemColunas = array( "Parte 1" => array(
 * 													"coluna1",
 * 													"coluna2",
 * 													"coluna3") , 
 * 								 "coluna4", 
 * 								 "Parte 2" => array(
 * 													"coluna5",
 * 													"coluna6",
 * 													"coluna7"));
 */
$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
//								"comentar_tabela", //Permite adicionar / editar comentсrios sobre a tabela
//								"comentar_coluna", //Permite adicionar / editar comentсrios sobre a coluna da tabela
//								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
//								"campo_lista", //Permite definir quais os campos aparecerуo na lista dos registros da tabela
//								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
								);

include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>