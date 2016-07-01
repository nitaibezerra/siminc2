<?php
// topo
include_once APPSAP . 'modulos/principal/fornecedor/partialsControl/topo.inc';

if($_GET['popup'] != 'S'){
	// monta cabeçalho do sistema
	include_once APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

$arMnuid = array( ABA_EDITAR_FORNECEDOR );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Fornecedor','');

// controller
extract($_POST);

// view
include_once APPSAP . 'modulos/principal/fornecedor/views/formPesquisa.inc';