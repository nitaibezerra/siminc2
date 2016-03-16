<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	
	//verifica se existe no banco um registro com a descrição informada no form
	$repetido = $oEstadoConservacao->verificaEstadoRepetido($_POST);
		
	//caso não encontre registro com a descrição informada
	if(!$repetido){
		$resultado = $oEstadoConservacao->salvarEstadoConservacao($_POST);
		if($resultado[0]){
			direcionar('?modulo=sistema/tabelasdeapoio/estadoConservacao/'.$resultado[2].'&acao=A&ecoid='.$resultado[1],'Operação realizada com sucesso!');
		}
		else if(!$resultado[0]){
			direcionar('?modulo=sistema/tabelasdeapoio/estadoConservacao/'.$resultado[2].'&acao=A&ecoid='.$resultado[1],'Operação falhou!');
		}
	}
	else{
		alerta('Já existe um Estado de Conservação cadastrado com a descrição informada.');
		extract($_POST);
	}
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['ecoid'])){
	$EstadoConservacao = $oEstadoConservacao->carregaEstadoConservacaoPorId($_GET['ecoid']);
	extract($EstadoConservacao);
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Estado de Conserva&ccedil;&atilde;o','');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/views/formDefault.inc';
