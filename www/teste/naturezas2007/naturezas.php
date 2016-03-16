<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';

$db = new cls_banco();

pg_set_client_encoding( $db->link, 'UNICODE' );

header( 'Content-Type: text/plain; charset=utf-8' );

define( 'ANO', 2007 );

$sqlBase = "
select ndpstatus
from public.naturezadespesa
where ndpcod = '%s'
";

$handle = fopen ( "naturezas.csv", "r" );
$nomesCampo = fgetcsv( $handle, 1000, "," );

$n_linha = 2;
while ( ( $linha = fgetcsv( $handle, 1000, "," ) ) !== FALSE)
{
	$nf_linha = sprintf( "%04d", $n_linha );
    $dados = array_combine( $nomesCampo, $linha );
    if ( !$dados['COD_NATU'] )
    {
    	echo "\nlinha " . $nf_linha . " natureza com código inválido";
    	$n_linha++;
    	continue;
    }
    $sql = sprintf(
    	$sqlBase,
    		$dados['COD_NATU']
    );
    $dadosDB = $db->recuperar( $sql );
    if ( !$dadosDB )
    {
    	echo
    		"\nlinha " . $nf_linha . ":" . 
    		" natureza " . $dados['COD_NATU'] .
    		" nao existe na base de dados - " . $dados['TIT_CLASSIFICADOR'];
    }
    else if ( $dadosDB['ndpstatus'] != $dados['COD_SITUACAO'] )
    {
    	echo
    		"\nlinha " . $nf_linha . ":" . 
    		" natureza " . $dados['COD_NATU'] .
    		" com status diferente csv " . $dados['COD_SITUACAO'] . " | bd " .
    		$dadosDB['ndpstatus'];
    }
    $n_linha++;
}

fclose( $handle );






