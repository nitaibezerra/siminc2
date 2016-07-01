<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';


$sidid = (int) $_POST['sidid'];

if( !empty($sidid) )
{
	$agrupadorFuncionalidade = new AgrupadorFuncionalidade( );
	$resultado = $agrupadorFuncionalidade->listarArupadoresPorSistema( $sidid );


}
?>