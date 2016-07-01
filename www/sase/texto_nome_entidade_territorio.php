<?php

// PADRAO
require_once '../../global/config.inc'; // carrega as funções gerais
if( !IS_PRODUCAO ) $_SESSION['baselogin'] = "simec_espelho_producao";
// CPF do administrador de sistemas
if(!$_SESSION['usucpf']){
    $_SESSION['usucpforigem'] = '';
    $auxusucpf = '';
    $auxusucpforigem = '';
}else{
    $auxusucpf = $_SESSION['usucpf'];
    $auxusucpforigem = $_SESSION['usucpforigem'];
}
// --
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
$db = new cls_banco();
// --

// The text to draw
if( $_GET['muncod'] )
    $sql = " select mundescricao from territorios.municipio where muncod = '{$_GET['muncod']}' ";
else if( $_GET['estuf'] )
    $sql = " select estuf from territorios.estado where estuf = '{$_GET['estuf']}' ";
$text = $db->pegaUm( $sql );
// --

// header('Content-Type: image/png'); // Set the content-type
$im = imagecreatetruecolor(400, 30); // Create the image
imagealphablending($im, false);
// Create some colors
$transparent = imagecolorallocatealpha($im, 0, 0, 0, 127);
$black = imagecolorallocate($im, 0, 0, 0);
imagefilledrectangle($im, 0, 0, 399, 29, $transparent);
// --
$font = APPRAIZ.'www/sase/verdana.ttf'; // Replace path by your own font path
// $font = ''; // Replace path by your own font path
function inverse($x) {
    if (!$x) {
        throw new Exception('Division by zero.');
    }
    else return 1/$x;
}


imagettftext($im, 8, 0, 10, 20, $black, $font, $text); // Add the text
imagealphablending($im,false);
imagesavealpha($im,true);
imagepng($im);
imagedestroy($im);
exit;