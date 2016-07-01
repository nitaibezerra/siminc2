<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topoPopup.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){

	// atualiza
	if($oMaterial->atualizar($_POST))
		direcionar('?modulo=sistema/tabelasdeapoio/material/pesquisarMaterial&acao=A', 'Operação realizada com sucesso.');
	else
		alerta('Erro.');
			
	
}else if(!empty($_GET['matid'])){

	$dados = $oMaterial->pegarRegistro($_GET['matid']);
	extract($dados);
	
}

// monta o título da tela
monta_titulo('Material', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/formDefaultPopup.inc';
