<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
$foto = new FilesSimec("visitantefoto", array() ,"sca");

$foto->getDownloadArquivo($_GET['idArquivo']);

?>