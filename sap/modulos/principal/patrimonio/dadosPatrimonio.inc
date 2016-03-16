<?php
// topo
include_once APPSAP . 'modulos/principal/patrimonio/partialsControl/topo.inc';

if( !empty($_GET['rgpid']) ){
	$arDados = $obPatrimonio->carregaRGPPorRGPId($_GET['rgpid']);
	
	
	
	//pegando responsável e endereço atuais do rgp
	$dadosResponsavel = $obRgp->pegarResponsavel($_GET['rgpid']);
	$nu_matricula_siape = $dadosResponsavel['nu_matricula_siape'];
	$no_servidor = $dadosResponsavel['no_servidor'];
	
	$dadosEndereco = $obRgp->pegarEnderecoUnidade($_GET['rgpid']);
	$uorco_uorg_lotacao_servidor = $dadosEndereco['uorco_uorg_lotacao_servidor'];
	$uorno = $dadosEndereco['uorno'];
	$enduf = $dadosEndereco['enduf'];
	$endcid = $dadosEndereco['endcid'];
	$endcep = $dadosEndereco['endcep'];

	if(!empty($dadosEndereco['endnum'])){
		$endereco = $dadosEndereco['endlog'].', '.$dadosEndereco['endnum'].', '.$dadosEndereco['endcom'];
	}
	else{
		$endereco = $dadosEndereco['endlog'].', '.$dadosEndereco['endcom'];
	}
	
	$enadescricao = $dadosEndereco['enadescricao'];
	$easdescricao = $dadosEndereco['easdescricao'];
	
	
	
	
	
	extract( $arDados );
	
	//formata os dados
	$bebdata = (!empty( $bebdata ) ? formata_data( $bebdata ) : '');
	$bendtentrada = (!empty( $bendtentrada ) ? formata_data( $bendtentrada ) : '');
	if(!empty($bennumproc)){
		$bennumproc = substr($bennumproc, 0,5).'.'.substr($bennumproc, 5,6).'.'.substr($bennumproc, 11,4).'-'.substr($bennumproc, 15);
	}
	if(!empty($bebnumprocesso)){
		$bebnumprocesso = substr($bebnumprocesso, 0,5).'.'.substr($bebnumprocesso, 5,6).'.'.substr($bebnumprocesso, 11,4).'-'.substr($bebnumprocesso, 15);
	}
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// monta o título da tela
monta_titulo('Dados do Patrim&ocirc;nio', '');

// view
include_once APPSAP . 'modulos/principal/patrimonio/views/formDadosPatrimonio.inc';

