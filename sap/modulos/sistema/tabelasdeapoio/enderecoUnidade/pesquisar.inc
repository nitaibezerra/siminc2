<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/enderecoUnidade/partialsControl/topo.inc';

if($_GET['topo'] != 'N'){
	// monta cabeçalho do sistema
	include_once APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

// monta o título da tela
monta_titulo('Busca de Unidade', '');

// controller
extract($_POST);

if(!empty($_GET['uorco_uorg_lotacao_servidor'])){
	$result = $oEnderecoUnidade->carregaNomeSiglaUnidade($_GET['uorco_uorg_lotacao_servidor']);
	if(is_array($result[0]) && count($result[0]) >= 1){
		$uorsg = $result[0]['uorsg'];
		$uorno = $result[0]['uorno'];
		$uorco_uorg_lotacao_servidor = $_GET['uorco_uorg_lotacao_servidor'];
	}
}

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/enderecoUnidade/views/formPesquisa.inc';