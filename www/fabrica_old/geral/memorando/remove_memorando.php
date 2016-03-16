<?php
header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

include_once "config.inc";
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'fabrica/classes/autoload.inc';


$status  = false;
$retorno = '';

try {

    $memorandoRepositorio = new MemorandoRepositorio();
    if ( !$memorandoRepositorio->removeMemorando( $_POST['memo'] ) ) {
        throw new Exception( 'Não foi possível remover o memorando' );
    }

    $status  = true;
    $retorno = 'Memorando excluído com sucesso';
} catch ( Exception $e ) {
    $status  = false;
    $retorno = $e->getMessage();
}

print simec_json_encode( array(
            'status'  => $status,
            'retorno' => utf8_encode( $retorno )
        ) );