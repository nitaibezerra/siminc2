<?php

/**
 * Gravando imagem capturada com plugin na sessao
 */

@session_start();

if($_REQUEST['acao'] == 'capturar')
    $_SESSION['imagemVisitante'] = base64_decode($_REQUEST["bindata"]);
else if($_REQUEST['acao'] == 'limpar')
    unset($_SESSION['imagemVisitante']);

?>