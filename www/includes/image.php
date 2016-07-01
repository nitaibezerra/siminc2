<?php
/**
 * Página que cria e redimensiona imagem.
 * @author : Thiago Tasca Barbosa
 * @version 	: 1.0
 */

////// STRINGS //////
$altura		= $_REQUEST["altura"]		;
$largura	= $_REQUEST["largura"]		;
$cropaltura	= $_REQUEST["cropaltura"]	;
$foto		= $_REQUEST["image"]	;

////// ANTES DE TUDO //////
$imagem		= imagecreatefromjpeg	($foto);

$alturaori	= imagesy ($imagem)	;
$larguraori	= imagesx ($imagem)	;

////// TAMANHO //////
if($altura || $largura){
	if($altura){
		$largura = $altura * $larguraori / $alturaori;
	}
	if($largura){
		$altura = $largura * $alturaori / $larguraori;
	}
}
else{
	$altura		= $alturaori;
	$largura	= $larguraori;
}
if($cropaltura){
	$alturaori		= $larguraori*($cropaltura-2)/$largura;
	$altura			= $cropaltura;
}

////// ESCREVE A IMAGEM //////
header			("Content-type: image/jpeg");

$nova = imagecreatetruecolor ($largura,$altura);

imagealphablending($nova, true); 
imagecopyresampled($nova, $imagem, 0, 0, 1, 1, $largura+2, $altura+2, $larguraori, $alturaori);

$corLinha	= imagecolorallocate($nova,	181,181,157);
imageline($nova	,0+$i		,0+$i		,$largura+$i	, 0+$i 		,$corLinha);
imageline($nova	,0			,0			,0				, $altura	,$corLinha);
imageline($nova	,$largura-1	,0			,$largura-1		, $altura	,$corLinha);
imageline($nova	,0			,$altura-1	,$largura		, $altura-1	,$corLinha);

imagejpeg($nova);

////// DESTROY //////
imagedestroy ($nova)	;
imagedestroy ($imagem)	;
?>