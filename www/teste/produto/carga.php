<?php

/*

CREATE TABLE carga.produto (
	procod int4 NOT NULL,
	prodsc varchar(80),
	CONSTRAINT pk_produto PRIMARY KEY (procod)
) WITH OIDS;

CREATE TABLE carga.acao (
	prgano char(4),
	prgcod char(4),
	acacod char(4),
	saccod char(4),
	acasnrap bool,
	procod int4,
	CONSTRAINT pk_acao PRIMARY KEY (prgano, prgcod, acacod, saccod, acasnrap),
	CONSTRAINT fk_acao_produto FOREIGN KEY (procod) REFERENCES carga.produto (procod) MATCH SIMPLE
) WITH OIDS;

*/

set_time_limit( 0 );
header( "Content-Type: text/plain" );

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

# abre conexуo com o banco
$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
$db          = new cls_banco();

$sql = "delete from carga.acao";
$db->executar( $sql, false );

$sql = "delete from carga.produto";
$db->executar( $sql, false );

$produtos = simplexml_load_file( APPRAIZ . "arquivos/SIGPLAN/importacao/2007/apoio/CargaProduto.xml" );
$produto_duplicado = array();
foreach( $produtos as $produto ) {
	# aplica o filtro usado na importaчуo
	$produto = (array) $produto;
	$produto = array_combine( array_map( 'strtolower', array_keys( $produto ) ), array_values( $produto ) );
	$produto = array_map( 'utf8_decode', $produto );
	$produto = array_map( 'trim', $produto );
	$produto = array_map( 'mb_strtolower', $produto );
	
	# insere o registro
	$sql = sprintf(
		"insert into carga.produto ( procod, prodsc ) values ( %d, '%s' )",
		$produto['procod'],
		addslashes( $produto['prodsc'] )
	);
//	echo "\n{$sql}";
	$db->executar( $sql, false );
}

$acoes = simplexml_load_file( APPRAIZ . "arquivos/SIGPLAN/importacao/sigplan-2007-20080108.xml" );
foreach( $acoes->ArrayOfAcao[0] as $acao ) {
	# aplica o filtro usado na importaчуo
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
	if ( empty( $acao['procod'] ) ) {
		continue;
	}
	$sql = sprintf( "select count(procod) from carga.produto where procod = %d", $acao['procod'] );
	if ( $db->pegaUm( $sql ) == 0 ) {
		continue;
	}
	
	# insere o registro
	$sql = sprintf(
		"insert into carga.acao (
			prgano, prgcod, acacod, saccod, acasnrap, procod
		) values (
			'%s', '%s', '%s', '%s', '%s', %d
		)",
		$acao['prgano'],
		$acao['prgcod'],
		$acao['acacod'],
		$acao['saccod'],
		$acao['acasnrap'],
		$acao['procod']
	);
//	echo "\n{$sql}";
	$db->executar( $sql, false );
}

$db->commit();

?>