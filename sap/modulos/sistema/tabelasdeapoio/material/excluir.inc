<?php
// topo
if(!empty($_GET['popup'])){
	include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topoPopup.inc';
}
else{
	include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topo.inc';	
}


// se passado o id para exclusao
if(!empty($_GET['matid'])){

	// exclui
	if($oMaterial->excluir($_GET['matid'])){
		if(!empty($_GET['popup'])){
			direcionar('?modulo=sistema/tabelasdeapoio/material/pesquisarMaterial&acao=A', 'Operação realizada com sucesso.');
		}
		else{
			direcionar('?modulo=sistema/tabelasdeapoio/material/pesquisar&acao=A', 'Operação realizada com sucesso.');	
		}
		
	}
	
}

