<?php

header( 'content-type: text/html; charset=UTF-8;' );
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

//Verifica se existe na máquina o expect
$urlSvn 			= $_POST['repositorioCasoDeUso'];
$repositorioProuni 	= strstr($urlSvn , 'dsv-svn-la-01.mec.gov.br');

if( $repositorioProuni === false ){

	$urlSvn = str_replace( "svn+ssh://", "", $urlSvn );
	$urlSvn = preg_replace( "/(\w+)?@?subversion.mec.gov.br/", "svnconsulta.mec.gov.br", $urlSvn );
	
	$urlArtefato   = $urlSvn;
	$urlArtefato   = str_replace( 'http://', '', $urlArtefato );
	$diretoriosUrl = explode( '/', $urlArtefato );
	
	foreach ( $diretoriosUrl as $index => $path ) {
	    if ( empty( $path ) ) {
	        unset( $diretoriosUrl[$index] );
	        continue;
	    }
	
	    $diretoriosUrl[$index] = rawurlencode( $path );
	}
	
	$urlFinal = 'http://' . implode( '/', $diretoriosUrl );
	
	$ch = curl_init();
	curl_setopt( $ch, CURLOPT_URL, $urlFinal );
	curl_setopt( $ch, CURLOPT_HEADER, true );
	curl_setopt( $ch, CURLOPT_NOBODY, true );
	curl_setopt( $ch, CURLOPT_CRLF, true );
	curl_setopt( $ch, CURLOPT_FAILONERROR, true );
	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
	
	if ( ! $resposta = curl_exec( $ch ) ) {
		$ce = curl_error($ch);
	}
	//$resposta = curl_exec( $ch );
	
	curl_close( $ch );

}else{

	$urlSvn = str_replace( "dsv-svn-la-01.mec.gov.br/prouni", "dsv-svn-la-01.mec.gov.br/public/prouni", $urlSvn );
	
	$urlArtefato   = $urlSvn;
	$urlArtefato   = str_replace( 'http://', '', $urlArtefato );
	$diretoriosUrl = explode( '/', $urlArtefato );
	
	foreach ( $diretoriosUrl as $index => $path ) {
		if ( empty( $path ) ) {
			unset( $diretoriosUrl[$index] );
			continue;
		}
	
		$diretoriosUrl[$index] = rawurlencode( $path );
	}
	
	$urlFinal = 'http://' . implode( '/', $diretoriosUrl );
	
	$ch = curl_init();
	curl_setopt( $ch, CURLOPT_URL, $urlFinal );
	curl_setopt( $ch, CURLOPT_HEADER, true );
	curl_setopt( $ch, CURLOPT_NOBODY, true );
	curl_setopt( $ch, CURLOPT_CRLF, true );
	curl_setopt( $ch, CURLOPT_FAILONERROR, true );
	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
	//curl_setopt( $ch, CURLOPT_USERPWD, "svn:svn123" );
	
	$resposta = curl_exec( $ch );
		
	curl_close( $ch );
}

// Monta retorno
if ( $resposta == false ) {

	$mensagem = "Repositório inválido";
	$mensagem = utf8_encode( $mensagem );
	$resposta = array(
			"STATUS"   => "ERROR",
			"MENSAGEM" => $mensagem
	);
} else {

	$mensagem = "O arquivo $resp encontra-se no SVN";
	$mensagem = utf8_encode( $mensagem );
	$resposta = array(
			"STATUS"   => "OK",
			"MENSAGEM" => $mensagem
	);
}

print simec_json_encode( $resposta );