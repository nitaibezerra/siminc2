<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){

	// atualiza
	if($oMaterial->atualizar($_POST))
		direcionar('?modulo=sistema/tabelasdeapoio/material/editar&acao=A&matid='. $_POST['matid'], 'Operação realizada com sucesso.');
	else
		alerta('Erro.');
	
}else if(!empty($_GET['matid'])){

	$dados = $oMaterial->pegarRegistro($_GET['matid']);
	extract($dados);
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Material', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/formDefault.inc';
