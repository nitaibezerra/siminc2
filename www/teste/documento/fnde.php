<?php

// с! учуo щ`eьђ:u"u

header( "content-Type: text/plain" );

$arquivo = fopen( "fnde.csv", "r" );
while ( $registro = fgetcsv( $arquivo, null, ";" ) ) {
	$registro[0] = trim( $registro[0] );
	$registro[3] = trim( $registro[3] );
	$registro[6] = trim( $registro[6] );
	print( "\nupdate territorios.municipio set muncodcompleto = '{$registro[3]}', munprocesso = '{$registro[6]}' where muncod = '$registro[2]';" );
}

?>