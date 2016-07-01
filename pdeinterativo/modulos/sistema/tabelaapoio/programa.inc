<?php
	$schema = "pdeinterativo"; //informa o schema
	$table = "programa"; //informa a tabela
		
	$arrDados[0] = array("codigo" => "S", "descricao" => "Sim");
	$arrDados[1] = array("codigo" => "N", "descricao" => "Não");
	$arrColuna["prorepassarec"] = $db->monta_combo("prorepassarec",$arrDados,"S","","","","","","S","",true,$prorepassarec);
	
	$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
								);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>