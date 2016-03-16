<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){

	// verifica se registro ja existe
	if($oTipoDocumento->existe($_POST)){
		
		alerta('Registro já existe.');
		extract($_POST);
		
	}else{
		
		// atualiza
		if($oTipoDocumento->atualizar($_POST))
			direcionar('?modulo=sistema/tabelasdeapoio/tipoDocumento/editar&acao=A&tdcid='. $_POST['tdcid'], 'Operação realizada com sucesso.');
		else
			alerta('Erro.');
			
	}
	
}else if(!empty($_GET['tdcid'])){

	$dados = $oTipoDocumento->pegarRegistro($_GET['tdcid']);
	extract($dados);
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Tipo de Documento', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/views/formDefault.inc';
