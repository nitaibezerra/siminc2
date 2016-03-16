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
			direcionar('?modulo=sistema/tabelasdeapoio/estadoConservacao/'.$resultado[2].'&acao=A','Operação realizada com sucesso!');
		}
        else if(!$resultado[0]){
        	direcionar('?modulo=sistema/tabelasdeapoio/estadoConservacao/'.$resultado[2].'&acao=A','Operação falhou!');
        }
	}
	else{
		alerta('Já existe um Estado de Conservação cadastrado com a descrição informada.');
		extract($_POST);
	}
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_ESTADO_CONSERVACAO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Estado de Conserva&ccedil;&atilde;o','');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/views/formDefault.inc';
