<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topo.inc';

// se requisição for para salvar
if($_POST['requisicao'] == 'salvar'){
	
	// salva
	if($oMaterial->salvar($_POST))
		direcionar('?modulo=sistema/tabelasdeapoio/material/adicionar&acao=A', 'Operação realizada com sucesso.');
	else
		alerta('Erro.');
			
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_MATERIAL );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Material', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/formDefault.inc';
