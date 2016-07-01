<?php

$arrArgumentsRequest = (array) $_REQUEST[ 'rsargs' ];
$intIdObject		= loop_unxmlentities( @$arrArgumentsRequest[ 1 ] );

//error_reporting( E_ALL );

try
{
	$objTarefa = new Tarefa();
	$objTarefa = $objTarefa->getTarefaPeloId( $intIdObject );
	$arrMensagens = $objTarefa->verificaPossibilidadeDeRemover();
	foreach( $arrMensagens as &$strValue )
	{
		$strValue = xmlentities( $strValue );
	}
}
catch( Exception $objError )
{
	$arrMensagens = array( $objError->getMessage() );
}
?>