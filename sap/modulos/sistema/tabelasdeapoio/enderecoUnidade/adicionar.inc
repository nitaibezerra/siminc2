<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/enderecoUnidade/partialsControl/topo.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){
	
	// verifica se registro ja exite
	if($oEnderecoUnidade->existe($_POST)){
		
		alerta('Registro já existe.');

		$uorco_uorg_lotacao_servidor = $_POST['uorco_uorg_lotacao_servidor'];
		$uorsg = $_POST['uorsg'];
		$uorno = $_POST['uorno'];
		
	}else{
		
		// salva
		$resultado = $oEnderecoUnidade->salvar($_POST);
		
		//caso tenha gravado com sucesso e a chamada NÃO TENHA sido da entrada de bens
		if($resultado !== false && empty($_POST['viaEntradaBens'])){
			direcionar('?modulo=sistema/tabelasdeapoio/enderecoUnidade/adicionar&acao=A&uorco_uorg_lotacao_servidor='.$_POST['uorco_uorg_lotacao_servidor'].'&uorsg='.$_POST['uorsg'].'&uorno='.$_POST['uorno'], 'Operação realizada com sucesso.');
		}
		//caso tenha gravado com sucesso e a chamada TENHA sido da entrada de bens
		else if($resultado !== false && $_POST['viaEntradaBens'] == 'S'){
			direcionar('?modulo=sistema/tabelasdeapoio/enderecoUnidade/adicionar&acao=A&uendid='.$resultado, 'Operação realizada com sucesso.');
		}
		else{
			alerta('Erro.');
		}
			
	}
	
}
else{
	//pega os dados necessários vindos da tela anterior(pesquisa de unidade/endereço)
	$uorco_uorg_lotacao_servidor = $_GET['uorco_uorg_lotacao_servidor'];
	$uorsg = $_GET['uorsg'];
	$uorno = $_GET['uorno'];
}

if($_GET['topo'] != 'N'){
	// monta cabeçalho do sistema
	include_once APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

// monta o título da tela
monta_titulo('Endere&ccedil;os de Unidade', '');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/enderecoUnidade/views/formDefault.inc';
