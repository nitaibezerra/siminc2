<?php
include_once("../modulos/provaCarlos/dao/profdao.php");
include_once("../modulos/provaCarlos/ctrl/profvo.php");

class professorCTRL extends professorDao {
	function confirmaInsercao(Professor $professor){
		if(parent::insereProf($professor)){
			echo "Professor Inserido com sucesso";
		}
	}
}
?>