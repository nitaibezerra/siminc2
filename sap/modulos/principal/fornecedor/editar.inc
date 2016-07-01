<?php
// topo
include_once APPSAP . 'modulos/principal/fornecedor/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	$resultado = $oFornecedor->salvarFornecedor($_POST);
	if($resultado[0]){
		direcionar('?modulo=principal/fornecedor/'.$resultado[2].'&acao=A&forcpfcnpj='.$resultado[1],'Operação realizada com sucesso!');
	}
	else if(!$resultado[0]){
		direcionar('?modulo=principal/fornecedor/'.$resultado[2].'&acao=A&forcpfcnpj='.$resultado[1],'Operação falhou!');
	}
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['forcpfcnpj'])){
	$Fornecedor = $oFornecedor->carregaFornecedorPorId($_GET['forcpfcnpj']);
	extract($Fornecedor);
}

if($_GET['popup'] != 'S'){
	//monta cabeçalho do sistema
	include_once APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

$arMnuid = array();
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Fornecedor','');

// view
include_once APPSAP . 'modulos/principal/fornecedor/views/formDefault.inc';
