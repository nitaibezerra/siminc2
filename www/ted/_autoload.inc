<?php
//Adidiona o Zend no path
set_include_path(implode(PATH_SEPARATOR, array(
        APPRAIZ . 'ted/classes/',
        APPRAIZ . 'includes/classes/',
        APPRAIZ . 'ted/classes/htmlToDocx/phpword/',
        get_include_path()
    ))
);

include APPRAIZ. 'includes/classes/Modelo.class.inc';
include APPRAIZ. 'ted/classes/Ted/Connect.php';
include APPRAIZ. 'ted/classes/Ted/Model/Utils.php';

function ted_autoload($classe) {
    $classPath = str_replace('_', DIRECTORY_SEPARATOR, $classe);
    
    # Fix - Erro no servidor ao carregar classes do Ted e do Zend
    if(is_numeric(strpos($classPath, 'Ted'))){
        include_once APPRAIZ. 'ted/classes/'. "$classPath.php";
    } else {
        include_once "$classPath.php";
    }
}

spl_autoload_register('ted_autoload');
