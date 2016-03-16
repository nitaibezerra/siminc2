<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topoRelatorioVisualizaEntrada.inc';

monta_titulo( 'Termo de Entrada de Bens', '' );

$sigla = $oBens->pegaSiglaTipoEntradaSaida($_GET['benid']);

if($sigla == 'COMP'){
	$arCabecalho = $oBens->cabecalhoVisualizaTermoCompra($_GET['benid']);
}
else if($sigla == 'DOAC'){
	$arCabecalho = $oBens->cabecalhoVisualizaTermoDoacao($_GET['benid']);
}
else if($sigla == 'SUBS'){
	/*$dadosTermoBaixa = $oTermo->carregaTermoBaixa($_GET['bebid']);
	if(is_array($dadosTermoBaixa)){
		extract($dadosTermoBaixa[0]);
	}*/
	
	$dadosTermoBaixa = $oBens->pegaTermoBaixaDaEntradaTipoSubstituicao($_GET['benid']);
	if(is_array($dadosTermoBaixa)){
		extract($dadosTermoBaixa);	
	}
	
	
	$arCabecalho = $oBens->cabecalhoVisualizaTermoSubstituicao($_GET['benid']);
}


extract($arCabecalho);

// view cabeçalho
if($sigla == 'COMP'){
	include_once APPSAP . 'modulos/principal/bens/views/cabecalhoVisualizaTermoCompra.inc';	
}
else if($sigla == 'DOAC'){
	include_once APPSAP . 'modulos/principal/bens/views/cabecalhoVisualizaTermoDoacao.inc';	
}
else if($sigla == 'SUBS'){
	include_once APPSAP . 'modulos/principal/bens/views/cabecalhoVisualizaTermoSubstituicao.inc';	
}

// view corpo
include_once APPSAP . 'modulos/principal/bens/views/partialsViews/listaVisualizaTermo.inc';

?>


