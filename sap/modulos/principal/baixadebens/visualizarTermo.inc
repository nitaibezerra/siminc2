<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topoRelatorioVisualizaBaixa.inc';

monta_titulo( 'Termo de Baixa de Bens', '' );

$sigla = $oBensBaixa->pegaSiglaTipoEntradaSaida($_GET['bebid']);

if($sigla == 'DOAC' || $sigla == 'VEND'){
	$arCabecalho = $oBensBaixa->cabecalhoVisualizaTermoPadrao($_GET['bebid']);
}
else if($sigla == 'SUBS'){
	$arCabecalho = $oBensBaixa->cabecalhoVisualizaTermoSubstituicao($_GET['bebid']);
}


extract($arCabecalho);

// view cabeçalho
if($sigla == 'DOAC' || $sigla == 'VEND'){
	include_once APPSAP . 'modulos/principal/baixadebens/views/cabecalhoVisualizaTermoPadrao.inc';	
}
else if($sigla == 'SUBS'){
	include_once APPSAP . 'modulos/principal/baixadebens/views/cabecalhoVisualizaTermoSubstituicao.inc';	
}

// view corpo
include_once APPSAP . 'modulos/principal/baixadebens/views/partialsViews/listaVisualizaTermo.inc';

?>


