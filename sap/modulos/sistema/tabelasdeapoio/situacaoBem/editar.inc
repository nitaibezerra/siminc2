<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){

	// verifica se registro ja existe
	if($oSituacaoBem->existe($_POST)){
		
		alerta('Registro já existe.');
		extract($_POST);
		
	}else{
		
		// atualiza
		if($oSituacaoBem->atualizar($_POST))
			direcionar('?modulo=sistema/tabelasdeapoio/situacaoBem/editar&acao=A&sbmid='. $_POST['sbmid'], 'Operação realizada com sucesso.');
		else
			alerta('Erro.');
			
	}
	
}else if(!empty($_GET['sbmid'])){

	$dados = $oSituacaoBem->pegarRegistro($_GET['sbmid']);
	extract($dados);
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Situa&ccedil;&atilde;o do Bem', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/views/formDefault.inc';
