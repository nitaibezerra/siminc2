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

$sql = "select nucid, entid from projovemurbano.nucleo where nucstatus='A' and entid is not null";
$nucleos = $db->carregar($sql);

if($nucleos[0]) {
	foreach($nucleos as $nu) {
		$db->executar("INSERT INTO projovemurbano.nucleoescola(
            			nucid, entid, nueqtdturma, nuetipo, nuestatus)
    				   VALUES ('".$nu['nucid']."', '".$nu['entid']."', '5', 'S','A');");
		$db->commit();
	}
}

echo "fim";
?>