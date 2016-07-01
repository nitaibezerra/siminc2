<?php
require_once( 'arquivos_requeridos_tarefas.inc' );

$intTarefaId = @$_REQUEST[ 'rsargs' ][ 1 ];
$objTarefa = new Tarefa();
$objTarefa = $objTarefa->getTarefaPeloId( $intTarefaId );
$arrTarefasQueContenho = $objTarefa->getArraydeTarefasqueContenho();
if ( sizeof( $arrTarefasQueContenho) > 0 ) 
{
	geraTabelaTarefas( $arrTarefasQueContenho , true );
}
?>
