<?php

header( "content-Type: text/plain" );

$arquivo = fopen( "ideb.csv", "r" );
while ( $registro = fgetcsv( $arquivo, null, ";" ) ) {
	$registro[0] = trim( $registro[0] );
	$registro[1] = trim( $registro[1] );
	print( "\nupdate territorios.municipio set muncodinep = '{$registro[1]}' where muncod = '{$registro[0]}';" );
}

?>