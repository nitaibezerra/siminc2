<?php
// topo
include_once APPSAP . 'modulos/principal/fornecedor/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	
	//verifica se já existe registro com o documento informado
	$repetido = $oFornecedor->verificaUnico($_POST['forcpfcnpj']);
		
	//caso não encontre repetido, salva
	if(!$repetido){
		$resultado = $oFornecedor->salvarFornecedor($_POST);
		
		if($resultado[0]){
			direcionar('?modulo=principal/fornecedor/'.$resultado[2].'&acao=A','Operação realizada com sucesso!');
		}
		else if(!$resultado[0]){
			direcionar('?modulo=principal/fornecedor/'.$resultado[2].'&acao=A','Operação falhou!');
		 }
	}
	else{
		alerta('Já existe um Fornecedor cadastrado com o documento informado.');
		extract($_POST);
	}
}

if($_GET['popup'] != 'S'){
	//monta cabeçalho do sistema
	include_once APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

$arMnuid = array( ABA_EDITAR_FORNECEDOR );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Fornecedor','');

// view
include_once APPSAP . 'modulos/principal/fornecedor/views/formDefault.inc';
