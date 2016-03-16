<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){

	// verifica se registro ja existe
	if($oMotivoEstadoConservacao->existe($_POST)){
		
		alerta('Registro já existe.');
		extract($_POST);
		
	}else{
		
		// atualiza
		if($oMotivoEstadoConservacao->atualizar($_POST))
			direcionar('?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/editar&acao=A&mecid='. $_POST['mecid'], 'Operação realizada com sucesso.');
		else
			alerta('Erro.');
			
	}
	
}else if(!empty($_GET['mecid'])){

	$dados = $oMotivoEstadoConservacao->pegarRegistro($_GET['mecid']);
	extract($dados);
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Motivos de Estados de Conserva&ccedil;&atilde;o', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/views/formDefault.inc';
