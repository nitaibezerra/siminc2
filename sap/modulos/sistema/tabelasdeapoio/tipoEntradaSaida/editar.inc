<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
		
	//verifica se existe no banco um registro com a descrição informada no form
	$repetido = $oTipoEntradaSaida->verificaTipoRepetido($_POST);
		
	//caso não encontre registro com a descrição informada
	if(!$repetido){
		$resultado = $oTipoEntradaSaida->salvarTipoEntradaSaida($_POST);
		if($resultado[0]){
			direcionar('?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/'.$resultado[2].'&acao=A&tesid='.$resultado[1],'Operação realizada com sucesso!');
		}
        else if(!$resultado[0]){
        	direcionar('?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/'.$resultado[2].'&acao=A&tesid='.$resultado[1],'Operação falhou!');
        }
	}
	else{
		alerta('Já existe um Tipo de Entrada/Saída cadastrado com a descrição informada.');
		extract($_POST);
	}
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['tesid'])){
	$TipoEntradaSaida = $oTipoEntradaSaida->carregaTipoEntradaSaidaPorId($_GET['tesid']);
	extract($TipoEntradaSaida);
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '', array());

// monta o título da tela
monta_titulo('Tipos de Entrada/Sa&iacute;da','');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/views/formDefault.inc';
