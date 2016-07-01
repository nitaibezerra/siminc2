<?php

	include "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	$db = new cls_banco();
	define( 'DIR_SIAF_FILES', APPRAIZ . 'financeiro/arquivos/siafi/2006/02 - fev 06/17fev06/' );

	$p = DIR_SIAF_FILES . '*.ref';

	$a = glob( $p );
	dbg( $p );
	dbg( $a, 1 );
	
	$conteudo = trim( file_get_contents( DIR_SIAF_FILES . '2006/02 - fev 06/17fev06/mc021701.ref' ) );
	
	$campos_bruto = array();
	preg_match_all( '/(.*)[\s]{1,}([a-z]{1})\s(.*)/i', $conteudo, $campos_bruto, PREG_SET_ORDER );
	$campos = '';
	foreach ( $campos_bruto as $dados_campo )
	{
		$campos .= trim( $dados_campo[1] ) . ';' . trim( (integer) $dados_campo[3] ) . '#';
	}
	$campos = substr( $campos, 0, -1 );

	print $campos;
	
?>