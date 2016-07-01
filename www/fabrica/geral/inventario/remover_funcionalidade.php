<?php

header( 'content-type: text/html; charset=iso-8859-1;' );
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$status             = false;
$msg                = '';
$funcionalidade     = new Funcionalidade();

try {
    
    $coFuncionalidade = (int) $_REQUEST['co_funcionalidade'];
    
    if( !$funcionalidade->excluir( $coFuncionalidade ) ) {
        throw new Exception('Não foi possível excluir a funcionalidade');
    }
    
    $status = true;
    $msg    = 'Funcionalidade removida com sucesso';
    $funcionalidade->commit();
} catch( Exception $e ) {
    $status = false;
    $msg    = utf8_encode( $e->getMessage() );
}

echo json_encode(array(
    'status'    => $status,
    'msg'       => $msg
));
