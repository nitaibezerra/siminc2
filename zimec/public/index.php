<?php

define( 'd', 'die' );
function ver(){

    $parar = false;

    $caminho = array_shift(debug_backtrace());
    echo '
        <div style="background-color: #ddd; color: #000; font-size: 14px;">
        <div>
            <label>
                Linha   --> <label class="valorCaminho">'.$caminho['line']    .'</label><br />
                Caminho --> <label class="valorCaminho">'.$caminho['file']    .'</label><br />
            </label>
        </div>';

    echo '<div><pre>';


    foreach( $caminho['args'] as $indice => $valor ){

        if(  $valor == 'die' ) $parar = true;
        echo '<label style="color:red; font-weight: bold;">Argumento '.( $indice + 1 ).'</label><br />';
        print_r( $valor );
        echo '<br /><br />';

    }

    echo '</div></pre></div>';


    // Verifica se é para dar o die no final
    if( $parar ){ die; }
}

// Define path to application directory
defined('APPLICATION_PATH')
    || define('APPLICATION_PATH', realpath(dirname(__FILE__) . '/../application'));

// Define application environment
defined('APPLICATION_ENV')
    || define('APPLICATION_ENV', (getenv('APPLICATION_ENV') ? getenv('APPLICATION_ENV') : 'simec_espelho_producao'));

// Ensure library/ is on include_path
set_include_path(implode(PATH_SEPARATOR, array(
    realpath(APPLICATION_PATH . '/../library'),
    get_include_path(),
)));

/** Zend_Application */
require_once 'Zend/Application.php';

// Create application, bootstrap, and run
$application = new Zend_Application(
    APPLICATION_ENV,
    APPLICATION_PATH . '/configs/application.ini'
);
$application->bootstrap()
            ->run();
