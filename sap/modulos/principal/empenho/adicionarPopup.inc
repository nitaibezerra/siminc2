<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

// monta o título da tela
monta_titulo('Nota de Empenho','');

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	
	if($oEmpenho->verificaEmpenhoRepetido($_POST)){
		alerta('Já existe registro do Empenho informado.');
		extract($_POST);
	}
	else{
		$resultado = $oEmpenho->salvarEmpenho($_POST);	
	}
	
	
}

// view
include_once APPSAP . 'modulos/principal/empenho/views/formDefault.inc';

//fazendo este teste aqui pq o javascript é incluído no form
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	if($resultado[0]){
		executarScriptPai("retornaDadosEmpenho(".$_POST['empnumero'].",\"".$_POST['empdata']."\",\"".$_POST['empvalorper']."\",".$resultado[1].")");
		alerta('Operação realizada com sucesso!');
		fecharPopup();
		die;
	}
	else if(!$resultado[0]){
		alerta('Operação Falhou!');
		die;
	}
}