<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$caminho_atual = $_SERVER ['REQUEST_URI'];

if ($_REQUEST ['download'] == 'S') {
    $file = new FilesSimec();
    $arqid = $_REQUEST ['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo( $arqid );
    exit();
}

include APPRAIZ . "includes/cabecalho.inc";
?>
