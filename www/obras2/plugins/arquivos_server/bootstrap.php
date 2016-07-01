<?php
error_reporting(0);
ini_set('display_errors', 'Off');


// if( ($_SERVER['HTTP_HOST'] == 'localhost' ) || ($_SERVER['HTTP_HOST'] == 'websis04' ) ){
//     define("APPRAIZ", "c:\\xampp\\htdocs\\fgvprojetos\\web\\arquivos_server\\");
//     define("ARQUIVOS_RAIZ", APPRAIZ . "arquivos");
// }else{
    define("APPRAIZ", "/var/www/simec/simec/www/fgvprojetos/web/arquivos_server/");
    define("ARQUIVOS_RAIZ", APPRAIZ . "arquivos");
// }

date_default_timezone_set('America/Sao_Paulo');    
    
define('DS', DIRECTORY_SEPARATOR);

// Set error reporting
//ini_set('display_errors', 'On');
//ini_set('html_errors', 'On');

function validaPath($path){
    $raiz    = realpath(ARQUIVOS_RAIZ);
    $raizQtd = strlen($raiz);

    $path    = substr($path, 0, $raizQtd);
    return $path === $raiz;
}

// Arruma a codificação dos itens para a tela
function arrumaCodificacaoItens($arFilhos){
    $filhos = array();

    foreach($arFilhos as $k => $filho){
        $filhos[$k] = arrumaCodificacaoItem($filho);
    }

    return $filhos;
}

// Código para arrumar a codificação de um único item
function arrumaCodificacaoItem($filho){
    $filhoCodificado = $filho;

    if(isset($filhoCodificado['data']['title']))
        $filhoCodificado['data']['title'] = utf8_encode($filhoCodificado['data']['title']);

    if(isset($filhoCodificado['attr']['id']))
        $filhoCodificado['attr']['id'] = utf8_encode($filhoCodificado['attr']['id']);

    if(isset($filhoCodificado['attr']['path']))
        $filhoCodificado['attr']['path'] = utf8_encode($filhoCodificado['attr']['path']);

    if(isset($filhoCodificado['attr']['href']))
        $filhoCodificado['attr']['href'] = utf8_encode($filhoCodificado['attr']['href']);

    return $filhoCodificado;
}

function codifica($array, $type = 'encode'){
    if($array){
        foreach($array as $k => $value){
            if(!is_array($value)){
                if($type == 'encode'){
                    $array[$k] = utf8_encode($value);
                }elseif($type == 'decode'){
                    $array[$k] = utf8_decode($value);
                }
            }
        }
    }

    return $array;
}

// Arruma a codificação
$_REQUEST = codifica($_REQUEST, 'decode');