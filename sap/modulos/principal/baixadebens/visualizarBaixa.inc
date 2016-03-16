<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topoRelatorioVisualizaBaixa.inc';

monta_titulo( 'Visualização de Baixa de Bens', '' );

$sigla = $oBensBaixa->pegaSiglaTipoEntradaSaida($_GET['bebid']);

if($sigla == 'DOAC' || $sigla == 'VEND'){
	$arCabecalho = $oBensBaixa->cabecalhoVisualizaBensBaixaPadrao($_GET['bebid']);
}
else if($sigla == 'SUBS'){
	$arCabecalho = $oBensBaixa->cabecalhoVisualizaBensBaixaSubstituicao($_GET['bebid']);
}


extract($arCabecalho);

$bebnumprocesso = substr($bebnumprocesso, 0,5).'.'.substr($bebnumprocesso, 5,6).'.'.substr($bebnumprocesso, 11,4).'-'.substr($bebnumprocesso, 15);

// view cabeçalho
if($sigla == 'DOAC' || $sigla == 'VEND'){
	include_once APPSAP . 'modulos/principal/baixadebens/views/cabecalhoVisualizaBaixaBensPadrao.inc';	
}
else if($sigla == 'SUBS'){
	include_once APPSAP . 'modulos/principal/baixadebens/views/cabecalhoVisualizaBaixaBensSubstituicao.inc';
}


// view corpo
include_once APPSAP . 'modulos/principal/baixadebens/views/partialsViews/listaVisualizaBaixaBens.inc';

?>
