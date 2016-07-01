<?php

//require_once( 'arquivos_requeridos_tarefas.inc' );

try
{
	error_reporting( E_ALL );
	$objTarefa = new Tarefa();
	$objTarefa = $objTarefa->getTarefaPeloId( "1033" );
	dbg( $objTarefa->getCodigoUnico() );
	dbg( $objTarefa->getCodigoEstruturado() );
	/*
	$objTarefa->setCodigoUnico( 12345 );
	$objTarefa->setContainerId( 2693 );
	$objTarefa->setDataFechada( false );
	*/
	$strDataFim = $objTarefa->getDataFim();
	dbg( $strDataFim );
	$objTarefa->setNovaDataFim( date( 'd/m/Y' , convertToTimestamp( $strDataFim ) + 24*60*60 ) );
	
	/*
	$objTarefa->setNovaDataInicio( '20/03/2007' );
	$objTarefa->setDescricao( 'as arveres somos nozes' );
	$objTarefa->setNome( 'as arveres' );
	$objTarefa->setPosicao( 1 );
	$objTarefa->setPredecessoraId( 2694 );
	$objTarefa->setProjetoId( 1 );
*/
	$objTarefa->validate();
	Tarefa::updateAllChangedEntities();
/*
	$objTarefa->update();
*/
}
catch( Exception $objError )
{
	// Ocorreu um Erro
	?>
		<script>
			<!-- chama o alert passando a mensagem de erro -->
			alert( "<?= $objError->getMessage() ?>" );
		</script>
	<?php
}
?>