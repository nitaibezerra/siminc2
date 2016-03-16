<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topoRelatorioVisualizaMovimentacao.inc';

//pega o tipo de movimentação
$tipoMov = $oTipoMov->pegaTipoMovimentacao($_GET['mvbid']);

//caso seja movimentação interna ou externa
if(strtoupper($tipoMov['tmvdsc']) == 'INTERNA' || strtoupper($tipoMov['tmvdsc']) == 'EXTERNA'){
	monta_titulo( 'Termo de Movimentação de Bens', '' );
	$arCabecalho = $oMovimentacao->cabecalhoVisualizaTermoGeral($_GET['mvbid']);
}
else{
	monta_titulo( 'Termo de Devolução de Bens', '' );
	$arCabecalho = $oMovimentacao->cabecalhoVisualizaTermoDevolucao($_GET['mvbid']);
}


extract($arCabecalho[0]);

//caso seja movimentação interna ou externa
if(strtoupper($tipoMov['tmvdsc']) == 'INTERNA' || strtoupper($tipoMov['tmvdsc']) == 'EXTERNA'){
	// view cabeçalho
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/cabecalhoVisualizaTermo.inc';
}
else{
	// view cabeçalho
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/cabecalhoVisualizaTermoDevolucao.inc';
}

if($mvbtipo == 'S'){
	// view corpo
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaVisualizaTermoSiape.inc';
}
else if($mvbtipo == 'L'){
	// view corpo
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaVisualizaTermoLocalidade.inc';
}
else if($mvbtipo == 'G'){
	// view corpo
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaVisualizaTermoGeral.inc';
}

?>