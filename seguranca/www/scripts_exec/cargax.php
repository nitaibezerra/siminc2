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
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$x = file("cargax.txt");

$sql = "DELETE FROM mapa.temadado WHERE tmaid='197'";
$db->executar($sql);

foreach($x as $a) {
	$dd = explode("	",$a);
	
	$sql = "INSERT INTO mapa.temadado(
            tmaid, tmdvalor, muncod, tmdboleano, tmdtexto)
    		VALUES ('197', '".$dd[1]."', '".$dd[0]."', NULL, NULL);";
	
	$db->executar($sql);
	
}

$x = file("carga_deferida.txt");

$sql = "DELETE FROM mapa.temadado WHERE tmaid='196'";
$db->executar($sql);

foreach($x as $a) {
	$dd = explode("	",$a);
	
	$sql = "INSERT INTO mapa.temadado(
            tmaid, tmdvalor, muncod, tmdboleano, tmdtexto)
    		VALUES ('196', '".$dd[1]."', '".$dd[0]."', NULL, NULL);";
	
	$db->executar($sql);
	
}

$x = file("carga_emanalise.txt");

$sql = "DELETE FROM mapa.temadado WHERE tmaid='195'";
$db->executar($sql);

foreach($x as $a) {
	$dd = explode("	",$a);
	
	$sql = "INSERT INTO mapa.temadado(
            tmaid, tmdvalor, muncod, tmdboleano, tmdtexto)
    		VALUES ('195', '".$dd[1]."', '".$dd[0]."', NULL, NULL);";
	
	$db->executar($sql);
	
}


$db->commit();

echo "fimmmmmmmmmmmmm";

?>