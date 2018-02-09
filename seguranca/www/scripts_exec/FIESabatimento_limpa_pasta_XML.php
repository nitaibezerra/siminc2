<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// ver(glob(APPRAIZ."arquivos/fiesabatimento/WSDL_USER*.xml"),d);

foreach (glob(APPRAIZ."arquivos/fiesabatimento/WSDL_USER*.xml") as $filename) {
// 	echo "$filename size " . filesize($filename) . "\n";
	unlink($filename);
}


?>