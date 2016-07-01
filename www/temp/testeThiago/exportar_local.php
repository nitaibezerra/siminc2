<?php

if ( !headers_sent() )
{
	//header( 'Content-Type: text/plain;' );
}
set_time_limit( 0 );
ini_set( 'display_errors', E_ALL );

// carrega as bibliotecas
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

function msg( $msg )
{
	echo $msg . "\n";
}

function erro( $erro )
{
	global $db;
	msg( 'erro! ' . $erro );
	msg( 'operação abortada' );
	$db->rollback();
	exit();
}

$db = new cls_banco();

mdg( 'fazer parte de exportação' );
