<?php
$file           = $_GET['file'];
$parts          = split("/", $file);
$filename       = $parts[count($parts)-1];

//Enviando para o usuario (download) da forma correta
header('Content-type: application/pdf');
header('Content-Disposition: inline; filename="' . $filename . '"');
header('Content-Transfer-Encoding: binary');
header('Content-Length: ' . filesize($file));
header('Accept-Ranges: bytes');
readfile( $file );