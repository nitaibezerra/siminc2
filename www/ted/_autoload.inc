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
    include_once "$classPath.php";
}

spl_autoload_register('ted_autoload');
