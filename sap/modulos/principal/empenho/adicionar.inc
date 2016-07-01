<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	if($oEmpenho->verificaEmpenhoRepetido($_POST)){
		alerta('Já existe registro do Empenho informado.');
		extract($_POST);
	}
	else{
		$resultado = $oEmpenho->salvarEmpenho($_POST);
		if($resultado[0]){
			direcionar('?modulo=principal/empenho/'.$resultado[2].'&acao=A','Operação realizada com sucesso!');
		}
		else if(!$resultado[0]){
			direcionar('?modulo=principal/empenho/'.$resultado[2].'&acao=A','Operação falhou!');
		}
	}

}

if($_GET['popup'] != 'S'){
	// monta cabeçalho do sistema
	include_once APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

$arMnuid = array( ABA_EDITAR_EMPENHO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Nota de Empenho','');

// view
include_once APPSAP . 'modulos/principal/empenho/views/formDefault.inc';
