<?php
// carrega as funções gerais
include_once "config.inc";
include_once "_constantes.php";
include_once '_funcoes.php';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$file = new FilesSimec();
$file->getDownloadArquivo($_REQUEST['arqid']);
?>