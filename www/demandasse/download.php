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

include_once APPRAIZ . "includes/classes/file.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

/**
 * Classe que gera graficos.
 */
include_once APPRAIZ . "includes/library/simec/Grafico.php";

/**
 * Classe de listagem.
 */
include_once APPRAIZ . "includes/library/simec/Crud/Listing.php";

/**
 * Classe para carregar as classes em mvc.
 */
include_once '_autoload.php';

//$controller = new Documentoarquivo();
//$controller->downloadAction();

//global $db;

$db = new cls_banco();

$arqid = $db->pegaUm("SELECT arqid
               FROM demandasse.demandaarquivo dma
               WHERE dmastatus = 'A'
               AND dmaid = {$_GET['dmaid']}");
$file = new FilesSimec();
$file->getDownloadArquivo((int) $arqid);