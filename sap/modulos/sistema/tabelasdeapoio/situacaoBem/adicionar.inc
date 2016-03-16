<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){
	
	// verifica se registro ja exite
	if($oSituacaoBem->existe($_POST)){
		
		alerta('Registro já existe.');
		extract($_POST);
		
	}else{
		
		// salva
		if($oSituacaoBem->salvar($_POST))
			direcionar('?modulo=sistema/tabelasdeapoio/situacaoBem/adicionar&acao=A', 'Operação realizada com sucesso.');
		else
			alerta('Erro.');
			
	}
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// monta o título da tela
monta_titulo('Situa&ccedil;&atilde;o do Bem', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/views/formDefault.inc';
