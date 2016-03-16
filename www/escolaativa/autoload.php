<?php

define('CLASSES_GERAL',    APPRAIZ . "/includes/classes/");
define('CLASSES_MODELO'  , APPRAIZ . 'escolaativa/classes/');
define('CLASSES_VISAO'  , APPRAIZ . 'includes/classes/view/');
define('CLASSES_HTML'  , APPRAIZ . 'includes/classes/html/');

set_include_path(CLASSES_GERAL. PATH_SEPARATOR .
				 CLASSES_MODELO . PATH_SEPARATOR . 
				 CLASSES_VISAO . PATH_SEPARATOR . 
				 CLASSES_HTML . PATH_SEPARATOR . 
				 get_include_path() );

function __autoload($class) {
    if(PHP_OS != "WINNT") { // Se "não for Windows"
    	$separaDiretorio = ":";
	    $include_path = get_include_path();
	    $include_path_tokens = explode($separaDiretorio, $include_path);
	} else { // Se for Windows
    	$separaDiretorio = ";c:";
	    $include_path = get_include_path();
	    $include_path_tokens = explode($separaDiretorio, $include_path);
	    $include_path_tokens = str_replace("//", "/", $include_path_tokens);
    	$include_path_tokens = explode(";", $include_path_tokens[0]);
	}

    foreach($include_path_tokens as $prefix){
            $path[0] = $prefix . $class . '.class.inc';
            $path[1] = $prefix . $class . '.php';
     
     	foreach($path as $thisPath){
        	if(file_exists($thisPath)){
            	require_once $thisPath;
                return;
            }
		}
    }
}
