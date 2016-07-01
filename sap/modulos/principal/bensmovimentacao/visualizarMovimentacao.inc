<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topoRelatorioVisualizaMovimentacao.inc';

//pega o tipo de movimentação
//$tipoMov = $oTipoMov->pegaTipoMovimentacao($_GET['mvbid']);
$tipoMov = $oTipoMov->carregaTipoMovimentacaoPorId($_GET['tmvid']);



//caso seja movimentação interna ou externa
if(strtoupper($tipoMov['tmvdsc']) == 'INTERNA' || strtoupper($tipoMov['tmvdsc']) == 'EXTERNA'){
	monta_titulo( 'Visualização de Movimentação de Bens', '' );
	//$arCabecalho = $oMovimentacao->cabecalhoVisualizaMovimentacaoGeral($_GET['mvbid']);
	
	$arCabecalho = $oMovimentacao->pegaDadosMovimentacao($_GET['mvbid']);
	if(is_array($arCabecalho) && count($arCabecalho) >= 1){
		$mvbdata = $arCabecalho['mvbdata'];
	}
	else{
		$mvbdata = date('d/m/Y');
	}
	
	$mvbobservacao = $_GET['mvbobservacao'];
	$mvbtipo = $_GET['mvbtipo'];
	
	if($_GET['mvbtipo'] == 'L'){
		$tipo = $tipoMov['tmvdsc'].' por Localidade';
		$uornoantigo = $_GET['unidadeantes'];
		$endantigo = $_GET['enderecoantes'];
		$uornoatual = $_GET['unidadeagora'];
		$endatual = $_GET['enderecoagora'];
	}
	else if($_GET['mvbtipo'] == 'S'){
		$tipo = $tipoMov['tmvdsc'].' por Siape';
		$no_servidorantigo = $_GET['respantes'];
		$nu_matricula_siapeantigo = $_GET['matriculaantes'];
		$no_servidoratual = $_GET['respagora'];
		$nu_matricula_siapeatual = $_GET['matriculaagora'];
	}
	else if($_GET['mvbtipo'] == 'G'){
		$tipo = $tipoMov['tmvdsc'].' Geral';
		$uornoatual = $_GET['unidade'];
		$end = $_GET['end'];
		$no_servidoratual = $_GET['resp'];
		$nu_matricula_siapeatual = $_GET['matricula'];
	}
	
}
else{
	monta_titulo( 'Visualização de Devolução de Bens', '' );
	//$arCabecalho = $oMovimentacao->cabecalhoVisualizaMovimentacaoDevolucao($_GET['mvbid']);
	
	$arCabecalho = $oMovimentacao->pegaDadosMovimentacao($_GET['mvbid']);
	if(is_array($arCabecalho) && count($arCabecalho) >= 1){
		$mvbdata = $arCabecalho['mvbdata'];
	}
	else{
		$mvbdata = date('d/m/Y');
	}
	
	$tipo = $tipoMov['tmvdsc'].' Geral';
	$no_servidor = $_GET['resp'];
	$nu_matricula_siape = $_GET['matricula'];
	$mvbobservacao = $_GET['mvbobservacao'];
	$mvbtipo = $_GET['mvbtipo'];
	
	$matricularespdevolucao = $_GET['matriculadevolucao'];
	$nomerespdevolucao = $_GET['respdevolucao'];
	$matricularesprecebimento = $_GET['matricularecebimento'];
	$nomeresprecebimento = $_GET['resprecebimento'];
	
}

//extract($arCabecalho[0]);

//caso seja movimentação interna ou externa
if(strtoupper($tipoMov['tmvdsc']) == 'INTERNA' || strtoupper($tipoMov['tmvdsc']) == 'EXTERNA'){
	// view cabeçalho
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/cabecalhoVisualizaMovimentacao.inc';
}
else{
	// view cabeçalho
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/cabecalhoVisualizaMovimentacaoDevolucao.inc';
}


if($mvbtipo == 'S'){
	// view corpo
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaVisualizaMovimentacaoBensSiape.inc';
}
else if($mvbtipo == 'L'){
	// view corpo
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaVisualizaMovimentacaoBensLocalidade.inc';
}
else if($mvbtipo == 'G'){
	// view corpo
	include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaVisualizaMovimentacaoBensGeral.inc';
}


?>