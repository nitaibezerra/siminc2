<?php

header( "Content-Type: application/pdf" );
header( "Content-Disposition: attachment; filename=\"roteiro_elaboracao.pdf\"");

$fp = fopen( dirname( __FILE__ ) . "/roteiro_relatorio.pdf", "r" );
while ( !feof( $fp ) )
{
	echo fread( $fp, 100 );
}
fclose( $fp );

?>