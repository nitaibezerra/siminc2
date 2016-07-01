<?php 

//include_once "controleAcesso.inc";

define('ENTIDADES_FABRICA',    APPRAIZ . "fabrica/classes/");
define('REPOSITORIOS_FABRICA', APPRAIZ . "fabrica/classes/repositorios/");
define('SERVICO_FABRICA'  , APPRAIZ . "fabrica/classes/servico/");
define('UTIL_FABRICA'  , APPRAIZ . "fabrica/classes/util/");
define('VALIDATORS_FABRICA'  , APPRAIZ . "fabrica/classes/validators/");
define('COMPONENTES_FABRICA'  , APPRAIZ . "fabrica/classes/componentes/");

set_include_path(ENTIDADES_FABRICA. PATH_SEPARATOR .
				 UTIL_FABRICA . PATH_SEPARATOR . 
				 REPOSITORIOS_FABRICA . PATH_SEPARATOR . 
				 SERVICO_FABRICA . PATH_SEPARATOR . 
				 VALIDATORS_FABRICA . PATH_SEPARATOR . 
				 COMPONENTES_FABRICA . PATH_SEPARATOR . 
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