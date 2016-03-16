<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topoRelatorioVisualizaEntrada.inc';

monta_titulo( 'Visualização de Entrada de Bens', '' );

$sigla = $oBens->pegaSiglaTipoEntradaSaida($_GET['benid']);

if($sigla == 'COMP'){
	$arCabecalho = $oBens->cabecalhoVisualizaEntradaBensCompra($_GET['benid']);
}
else if($sigla == 'DOAC'){
	$arCabecalho = $oBens->cabecalhoVisualizaEntradaBensDoacao($_GET['benid']);
}
else if($sigla == 'SUBS'){
	$arCabecalho = $oBens->cabecalhoVisualizaEntradaBensSubstituicao($_GET['benid']);
}


extract($arCabecalho);

// view cabeçalho
if($sigla == 'COMP'){
	include_once APPSAP . 'modulos/principal/bens/views/cabecalhoVisualizaEntradaBensCompra.inc';	
}
else if($sigla == 'DOAC'){
	include_once APPSAP . 'modulos/principal/bens/views/cabecalhoVisualizaEntradaBensDoacao.inc';
}
else if($sigla == 'SUBS'){
	include_once APPSAP . 'modulos/principal/bens/views/cabecalhoVisualizaEntradaBensSubstituicao.inc';
}

// view corpo
include_once APPSAP . 'modulos/principal/bens/views/partialsViews/listaVisualizaEntradaBens.inc';

?>
