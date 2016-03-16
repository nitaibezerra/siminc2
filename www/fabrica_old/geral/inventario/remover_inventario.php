<?php

header( 'content-type: text/html; charset=iso-8859-1;' );
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$status     = false;
$msg        = '';
$inventario = new InventarioRepositorio();
try {

    $coInventario = (int) $_REQUEST['co_inventario'];

    $inventario->setAtributos( array(
        'co_inventario' => $coInventario,
        'st_inventario' => 'I'
    ) );

    if ( !$inventario->alterar() ) {
        throw new Exception( 'Não foi possível excluir o inventário' );
    }

    $status = true;
    $msg    = 'Inventário removido com sucesso';
    $inventario->commit();
} catch ( Exception $e ) {
    $status = false;
    $msg    = utf8_encode( $e->getMessage() );
}

echo simec_json_encode( array(
    'status' => $status,
    'msg'    => utf8_encode( $msg )
) );
