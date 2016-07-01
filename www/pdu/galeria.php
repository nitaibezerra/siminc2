<?php
/**
 * Created by PhpStorm.
 * User: RuySilva
 * Date: 17/12/13
 * Time: 19:22
 */
//../../arquivos/scrum/postit/
error_reporting(E_ALL);
ini_set("display_errors", 1);
//include("file_with_errors.php");


date_default_timezone_set ('America/Sao_Paulo');


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
//include_once '_funcoes.php';
//include_once '_componentes.php';

//session_start();


$expires = 3600;
$cache_time = mktime(0,0,0,date('m'),date('d')+1,date('Y'));
header("Expires: " . date("D, d M Y H:i:s",$cache_time) . " GMT");
header("Cache-Control: max-age=$expires, must-revalidate");

//$_GET['file'] = 'postit/184';
$filePath = APPRAIZ . 'arquivos/pdu/' . $_GET['file'];
//var_dump($filePath);
//var_dump(file_exists($filePath));
//var_dump(is_file($filePath));
//exit;
//fopen($filePath , 'r+');
//$teste = file($filePath);

//var_dump($teste);


if(file_exists($filePath)){

//    var_dump(is_writable($filePath));
//    exit;
//    if(is_executable($filePath)){
        readfile($filePath);
//    } else {
//        echo 'Não tem permissão de pasta!';
//    }

//} else {
//    echo 'Não existe arquivo:' . $filePath;
}

