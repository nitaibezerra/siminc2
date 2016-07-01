<?php
	$titulo = "Delegar Competência"; //Monta o título da página
	$schema = "seguranca"; //informa o schema
	$table = "delegacao_competencia"; //informa a tabela
	$hideWarning = true; //esconde os alertas, erros e informações de pk, fk, mapeamento de tabelas, etc.
	//$showPK = true; //exibe o campo de PK (readonly)
	//$showStatus = true; //exibe o combo de status (A,I e H)
	$arrOrdemColunas = array("usucpf_delegado","dcpdata_ini","dcpdata_fim"); //Ordena a exibição das colunas na tela, pode ser agrupado.
	/*
	$arrOrdemColunas = array(
							"Dados Pessoais" => array(
														"fdpcpf",
														"fdpnome",
														"fdpdatanascimento",
														"fdpnaturalidade",
														"eciid",
														"fdpconjuge"),
							"Contatos" => array(
													"fdptelefoneres",
													"fdpcelular"),
							"Endereço" => array(
													"fdpcep",
													"fdpenderecoresidencial",
													"fdpbairro",
													"estuf",
													"muncod"), 
							"Filiação" => array(
													"fdpnomepai",
													"fdpnomemae"), 
							"Dados Bancários" => array(
														"fdpbanco",
														"fdpnomebanco",
														"fdpagencia",
														"fdpcontacorrente")
						);
	*/
	//$ptsid = 8;
	
	$arrAttr["usucpf_delegado"] = array("obrigatório" => true);
							  
	//$arrAttr["muncod"] = array( "vinculo" => "estuf" );
	
	$arrAttr["usucpf"] = array( "mascara" => "###.###.###-##", //máscara java script
								"validacao" => "validaCPF" ); //validação em PHP e JS (Obs: a função em PHP e JS devem ter o nome da validação.)
	
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
								"comentar_tabela", //Permite adicionar / editar comentários sobre a tabela
								"comentar_coluna", //Permite adicionar / editar comentários sobre a coluna da tabela
								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
								"campo_lista", //Permite definir quais os campos aparecerão na lista dos registros da tabela
//								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
								);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>