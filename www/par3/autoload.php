<?php
define ( 'CLASSES_GERAL', APPRAIZ . "par3/classes/" );
define ( 'CLASSES_CONTROLE', APPRAIZ . 'par3/classes/controller/' );
define ( 'CLASSES_HELPER', APPRAIZ . 'par3/classes/helper/' );
define ( 'CLASSES_MODELO', APPRAIZ . 'par3/classes/model/' );
define ( 'CLASSES_VISAO', APPRAIZ . 'includes/classes/view/' );
define ( 'CLASSES_HTML', APPRAIZ . 'includes/classes/html/' );

set_include_path ( CLASSES_GERAL . PATH_SEPARATOR . 
				   CLASSES_CONTROLE . PATH_SEPARATOR . 
				   CLASSES_MODELO . PATH_SEPARATOR . 
				   CLASSES_VISAO . PATH_SEPARATOR .
				   CLASSES_HELPER . PATH_SEPARATOR .
				   CLASSES_HTML . PATH_SEPARATOR . get_include_path () );

function __autoload($class) {
	if (PHP_OS != "WINNT") { // Se "nгo for Windows"
		$separaDiretorio = ":";
		$include_path = get_include_path ();
		$include_path_tokens = explode ( $separaDiretorio, $include_path );
	} else { // Se for Windows
		$raiz = strtolower ( substr ( APPRAIZ, 0, 2 ) );
		$separaDiretorio = ";$raiz";
		$include_path = get_include_path ();
		$include_path = str_replace ( '.;', $raiz, strtolower ( $include_path ) );
		$include_path = str_replace ( '/', '\\', $include_path );
		$include_path_tokens = explode ( $separaDiretorio, $include_path );
		$include_path_tokens = str_replace ( "//", "/", $include_path_tokens );
		$include_path_tokens [0] = str_replace ( $raiz, '', $include_path_tokens [0] );
	}

	foreach ( $include_path_tokens as $prefix ) {
        // Recupera a ъltima posiзгo do array, substituindo o array_pop para parar o erro de parвmetro por referкncia
        $aClasse = explode('_', $class);
        
		$file = $aClasse[(count($aClasse)-1)];

		$path[0] = strtolower($prefix) . $file . '.class.inc';
		$path[1] = strtolower($prefix) . $file . '.php';
		$path[2] = strtolower($prefix) . $class . '.class.inc';
		$path[3] = strtolower($prefix) . $class . '.php';

		foreach ( $path as $thisPath ) {
			if (file_exists ( $thisPath )) {
				require_once $thisPath;
				return;
			}
		}
	}
}?>