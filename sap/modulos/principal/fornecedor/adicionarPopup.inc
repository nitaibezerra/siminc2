<?php
// topo
include_once APPSAP . 'modulos/principal/fornecedor/partialsControl/topo.inc';

// monta o título da tela
monta_titulo('Fornecedor','');

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	
	//verifica se já existe registro com o documento informado
	$repetido = $oFornecedor->verificaUnico($_POST['forcpfcnpj']);
		
	//caso não encontre repetido, salva
	if(!$repetido){
		$resultado = $oFornecedor->salvarFornecedor($_POST);
	}
	else{
		alerta('Já existe um Fornecedor cadastrado com o documento informado.');
		extract($_POST);
	}
}


// view
include_once APPSAP . 'modulos/principal/fornecedor/views/formDefault.inc';

//fazendo este teste aqui pq o javascript é incluído no form
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	if($resultado[0]){
		executarScriptPai("retornaDadosFornecedor(\"".$_POST['forcpfcnpj']."\",\"".$_POST['forrazaosocial']."\")");
		alerta('Operação realizada com sucesso!');
		fecharPopup();
		die;
	}
	else if(!$resultado[0] && !$repetido){
		alerta('Operação Falhou!');
		die;
	}
}