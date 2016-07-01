<?php

define('CLASSES_GERAL',    APPRAIZ . "includes/classes/");
define('CLASSES_CONTROLE', APPRAIZ . 'pes/classes/controllers/');
define('CLASSES_MODELO'  , APPRAIZ . 'pes/classes/models/');
define('CLASSES_VISAO'  , APPRAIZ . 'pes/classes/views/');
//define('CLASSES_VISAO'  , APPRAIZ . 'includes/classes/view/');
//define('CLASSES_HTML'  , APPRAIZ . 'includes/classes/html/');

set_include_path(
                    CLASSES_GERAL. PATH_SEPARATOR .
                    CLASSES_CONTROLE . PATH_SEPARATOR . 
                    CLASSES_MODELO . PATH_SEPARATOR . 
                    CLASSES_VISAO . PATH_SEPARATOR . 
//                    CLASSES_HTML . PATH_SEPARATOR . 
                    get_include_path() 
                );

function __autoload($class) {
    
    require_once APPRAIZ . "pes/classes/abstracts/Controller.php"; 
    require_once APPRAIZ . "pes/classes/abstracts/Model.php"; 
    require_once APPRAIZ . "pes/classes/abstracts/View.php";

    if (PHP_OS != "WINNT")
    { // Se "não for Windows"
        $separaDiretorio = ":";
        $include_path = get_include_path();
        $include_path_tokens = explode($separaDiretorio, $include_path);
    } else
    { // Se for Windows
        $separaDiretorio = ";c:";
        $include_path = get_include_path();

        $include_path = str_replace('.;', 'c:', strtolower($include_path));
        $include_path = str_replace('/', '\\', $include_path);

        $include_path_tokens = explode($separaDiretorio, $include_path);
        $include_path_tokens = str_replace("//", "/", $include_path_tokens);
//        $include_path_tokens[0] = explode(";", $include_path_tokens[0]);

        $include_path_tokens[0] = str_replace('c:', '', $include_path_tokens[0]);

//        ver(PHP_OS, $include_path_tokens, $include_path,d);

    }

    foreach ($include_path_tokens as $prefix) {
//        $file = pathinfo($prefix, PATHINFO_BASENAME); //end(explode('/',$prefix));
//        $file = ucfirst(substr($file, 0, -1));


        $file = array_pop(explode('_', $class));
//
        $pathModule = $prefix . $file . '.php';
        if (file_exists($pathModule))
            require_once $pathModule;

        $path[0] = $prefix . $class . '.class.inc';
        $path[1] = $prefix . $class . '.php';

        foreach ($path as $thisPath) {
            if (file_exists($thisPath))
            {
                require_once $thisPath;
                return;
            }
        }

    }
}
