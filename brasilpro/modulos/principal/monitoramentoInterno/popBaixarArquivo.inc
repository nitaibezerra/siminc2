<?php
	ob_clean();
	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	$file 		= new FilesSimec(NULL, NULL, 'cte');
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die();
?>