<?php
/***
* File : captcha.php
* Description : Cria uma imagem captcha e guarda o texto numa variбvel session
* Autor : Kiran Paul V.J. aka kiranvj aka human
* Licenзa : Freeware
* Ъltima atualizaзгo : 22-Aug-2007
*/

// Definir o header como image/png para indicar que esta pбgina contйm dados
// do tipo image->PNG
header("Content-type: image/png");


include_once "config.inc";

// Criar um novo recurso de imagem a partir de um arquivo
$imagemCaptcha = imagecreatefrompng(APPRAIZ . "www/imagens/captcha.png")
or die("Nгo foi possнvel inicializar uma nova imagem");

//Carregar uma nova fonte
$fonteCaptcha = imageloadfont(APPRAIZ . "www/fontes/anonymous.gdf");

// Criar o texto para o captcha
$textoCaptcha = substr(md5(uniqid('')),-9,4);

// Guardar o texto numa variбvel session
$_SESSION['session_textoCaptcha'] = $textoCaptcha;

// Indicar a cor para o texto
$corCaptcha = imagecolorallocate($imagemCaptcha,0,0,0);

// Escrever a string na cor escolhida
imagestring($imagemCaptcha,$fonteCaptcha,15,5,$textoCaptcha,$corCaptcha);

// Mostrar a imagem captha no formato PNG.
// Outros formatos podem ser usados com imagejpeg, imagegif, imagewbmp, etc.
imagepng($imagemCaptcha);

// Liberar memуria
imagedestroy($imagemCaptcha);
?>