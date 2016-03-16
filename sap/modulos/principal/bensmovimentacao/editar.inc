<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST"){
	if ($_REQUEST['requisicao'] == 'salvar'){
		
		$resultado = $obBensBaixa->salvarBensBaixa($_POST);
		
	}
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET"){
	if(!empty($_REQUEST['bmvid'])){
		$bmvid = $_REQUEST['bmvid'];
	}
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );

// monta o título da tela
monta_titulo('Movimentação de Bens', '<b>Dados do Processo de Movimentação</b>');

// view
include_once APPSAP . 'modulos/principal/bensmovimentacao/views/formDefault.inc';


//fazendo este teste aqui pq o javascript é incluído no form
if($_SERVER['REQUEST_METHOD']=="POST"){
	if ($_REQUEST['requisicao'] == 'salvar'){
		if($resultado){
			executarScriptPai("retornaDadosBensMovimentacao(\"".$_POST."\")");
			fecharPopup();
		}
		else if(!$resultado){
			alerta('Operação Falhou!');
		}
		die;
	}
}