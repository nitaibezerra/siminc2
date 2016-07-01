<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){
	
	// verifica se registro ja exite
	if($oMotivoEstadoConservacao->existe($_POST)){
		
		alerta('Registro já existe.');
		extract($_POST);
		
	}else{
		
		// salva
		if($oMotivoEstadoConservacao->salvar($_POST))
			direcionar('?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/adicionar&acao=A', 'Operação realizada com sucesso.');
		else
			alerta('Erro.');
			
	}
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_MOTIVO_ESTADO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Motivos de Estados de Conserva&ccedil;&atilde;o', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/views/formDefault.inc';
