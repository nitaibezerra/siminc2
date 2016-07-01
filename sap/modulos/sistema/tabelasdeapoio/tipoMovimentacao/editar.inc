<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoMovimentacao/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
			
	//verifica se existe no banco um registro com a descrição informada no form
	$repetido = $oTipoMovimentacao->verificaTipoRepetido($_POST);
		
	//caso não encontre registro com a descrição informada
	if(!$repetido){
		$resultado = $oTipoMovimentacao->salvarTipoMovimentacao($_POST);
		if($resultado[0]){
			direcionar('?modulo=sistema/tabelasdeapoio/tipoMovimentacao/'.$resultado[2].'&acao=A&tmvid='.$resultado[1],'Operação realizada com sucesso!');
		}
        else if(!$resultado[0]){
        	direcionar('?modulo=sistema/tabelasdeapoio/tipoMovimentacao/'.$resultado[2].'&acao=A&tmvid='.$resultado[1],'Operação falhou!');
        }
	}
	else{
		alerta('Já existe um Tipo de Movimentação cadastrado com a descrição informada.');
		extract($_POST);
	}
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['tmvid'])){
	$TipoMovimentacao = $oTipoMovimentacao->carregaTipoMovimentacaoPorId($_GET['tmvid']);
	extract($TipoMovimentacao);
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Tipo de Movimenta&ccedil;&atilde;o','');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoMovimentacao/views/formDefault.inc';
