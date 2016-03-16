<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );
/*
$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
*/
$db = new cls_banco();

include APPRAIZ . "includes/historico.php";

header( 'Content-Type: text/plain' );

$resultado = <<<EOT
HTTP/1.1 200 OK Server: Microsoft-IIS/5.0 Date: Mon, 28 Jan 2008 18:59:13 GMT X-Powered-By: ASP.NET X-AspNet-Version: 1.1.4322 Cache-Control: private, max-age=0 Content-Type: text/xml; charset=utf-8 Content-Length: 327 <?xml version="1.0" encoding="utf-8"?> <retornoSIGPLAN xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://www.sigplan.gov.br/xml/"> <codigo>0</codigo> <numerocarga>434</numerocarga> <mensagem>Dados recebidos!</mensagem> <descricao /> </retornoSIGPLAN>
EOT;


$resultado = substr( $resultado, strpos( $resultado, "<?xml " ) );

echo $resultado;	
