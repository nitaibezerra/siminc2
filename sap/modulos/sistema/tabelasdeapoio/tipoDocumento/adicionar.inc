<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){
	
	// verifica se registro ja exite
	if($oTipoDocumento->existe($_POST)){
		
		alerta('Registro já existe.');
		extract($_POST);
		
	}else{
		
		// salva
		if($oTipoDocumento->salvar($_POST))
			direcionar('?modulo=sistema/tabelasdeapoio/tipoDocumento/adicionar&acao=A', 'Operação realizada com sucesso.');
		else
			alerta('Erro.');
			
	}
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_TIPO_DOCUMENTO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Tipo de Documento', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/views/formDefault.inc';
