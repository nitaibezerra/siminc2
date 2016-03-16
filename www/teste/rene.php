<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );

$booleano = array(
	'acasnmedireta',
	'acasnmedesc',
	'acasnmelincred',
	'acasnmetanaocumulativa',
	'acasnrap',
	'acasnfiscalseguridade',
	'acasninvestatais',
	'acasnoutrasfontes'
);

function xmlentities( $string ) {
    return str_replace ( array ( '&', '"', "'", '<', '>' ), array ( '&amp;' , '&quot;', '&apos;' , '&lt;' , '&gt;' ), $string );
}

# abre conexão com o banco
$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
$db          = new cls_banco();

# captura os dados do sigplan
$documento = simplexml_load_file( APPRAIZ . "arquivos/SIGPLAN/importacao/sigplan-2007-20080108.xml" );
//$documento = simplexml_load_file( "rene_CargaProgramacao.xml" );

# cria documento de relatório
$relatorio = new DOMDocument( '1.0', 'utf-8' );
$tag_relatorio = new DOMElement( 'relatorio' );
$relatorio->appendChild( $tag_relatorio );
$tag_relatorio->setAttribute( 'data', date( 'Y-m-d H:i:s.0T' ) );

foreach ( $documento->ArrayOfAcao[0] as $acao ) {
	
	# formata os dados capturados do sigplan
	$acao = (array) $acao;
	$acao = array_combine( array_map( 'strtolower', array_keys( $acao ) ), array_values( $acao ) );
	$acao = array_map( 'utf8_decode', $acao );
	$acao = array_map( 'trim', $acao );
	$acao = array_map( 'addslashes', $acao );
	foreach ( $acao as $campo => $valor ) {
		if ( in_array( $campo, $booleano ) ) {
			$acao[$campo] = $acao[$campo] == 'true' ? 't' : 'f';
		}
	}
	
	# captura os dados do simec
	$sql = sprintf(
		"select * from monitora.acao where prgano = '%s' and prgcod = '%s' and acacod = '%s' and saccod = '%s'",
		$acao['prgano'],
		$acao['prgcod'],
		$acao['acacod'],
		$acao['saccod']
	);
	$registro = $db->pegaLinha( $sql );
	if ( !$registro ) {
		$diferenca = array_keys( $acao );
	} else {
		# compara os dados obtidos no sigplan com aqueles capturados no simec
		$diferenca = array();
		foreach ( $acao as $campo => $valor ) {
			if ( !array_key_exists( $campo, $registro ) ) {
				continue;
			}
			$acao[$campo] = preg_replace( "(\r\n|\n|\r|\t)", "", $acao[$campo] ); 
			$registro[$campo] = preg_replace( "(\r\n|\n|\r|\t)", "", $registro[$campo] );
			# identifica os campos onde há diferença
			if ( $acao[$campo] != $registro[$campo] ) {
				array_push( $diferenca, $campo );
			}
		}
	}
	
	if ( count( $diferenca ) > 0 ) {
		$tag_acao = new DOMElement( "acao" );
		$tag_relatorio->appendChild( $tag_acao );
		
		$tag_acao->setAttribute( "prgano", xmlentities( utf8_encode( $acao["prgano"] ) ) );
		$tag_acao->setAttribute( "prgcod", xmlentities( utf8_encode( $acao["prgcod"] ) ) );
		$tag_acao->setAttribute( "acacod", xmlentities( utf8_encode( $acao["acacod"] ) ) );
		$tag_acao->setAttribute( "saccod", xmlentities( utf8_encode( $acao["saccod"] ) ) );
		
		$tag_simec = new DOMElement( "simec" );
		$tag_acao->appendChild( $tag_simec );
		
		$tag_sigplan = new DOMElement( "sigplan" );
		$tag_acao->appendChild( $tag_sigplan );
		
		foreach ( $diferenca as $campo ) {
			$tag_simec->appendChild( new DOMElement( $campo, xmlentities( utf8_encode( $registro[$campo] ) ) ) );
			$tag_sigplan->appendChild( new DOMElement( $campo, xmlentities( utf8_encode( $acao[$campo] ) ) ) );
		}
	}
}

//dump( $relatorio );
dump( $relatorio->save( "rene_Relatorio.xml" ) );




dump( null, true );
?>