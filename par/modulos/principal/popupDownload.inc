<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
$file = new FilesSimec();
$file->getDownloadArquivo( $_REQUEST['arqid']);
echo "<script>window.close();</script>";
?>