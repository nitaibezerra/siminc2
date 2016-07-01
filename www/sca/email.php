<?php
ini_set('SMTP','smtp2.mec.gov.br');
$msgTemAnexo = false;

$nome        = "PROINFO INTEGRADO";
$email       = $_SESSION['email_sistema'];
$boundary    = "XYZ-".date("dmYis")."-ZYX";
$formAssunto = "Teste SERVIDOR";
 
$headers = "MIME-Version: 1.0\n";
$headers.= "Content-Type: multipart/mixed; charset=iso-8859-1\r\n";
$headers.= "boundary=".$boundary."\r\n";
$headers.= "$boundary\n";
$headers.= "From: ".$nome ." <".$email.">\r\n";
$headers.= "X-Priority: 1\r\n";
$headers.= "X-MSMail-Priority: High\r\n";
$headers.= "X-Mailer: Just My Server";

$mensagem = "--$boundary\n";
$mensagem.= "Content-Type: text/html; charset='utf-8'\n";
$mensagem.= "<strong>Nome: </strong> $nome \r\n";
$mensagem.= "--$boundary \n";

try {
    mail($formCopia, $formAssunto, $mensagem, $headers);
    echo "enviado 1<br>";
} catch (Exception $e) {
    var_dump( $e->getMessage() );
    echo "<br>";
}

try {
    mail($_SESSION['email_sistema'], $formAssunto, $mensagem, $headers);
    echo "enviado 2<br>";
} catch (Exception $e) {
    var_dump( $e->getMessage() );
}