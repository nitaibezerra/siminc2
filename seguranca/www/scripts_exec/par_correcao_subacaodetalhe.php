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


$sql = "select count(sbdid), sbaid, sbdano, max(sbdid) as ult from par.subacaodetalhe group by sbaid, sbdano having count(sbdid)>1";
$subs = $db->carregar($sql);

if($subs[0]) {
	foreach($subs as $sub) {
		$db->executar("delete from par.subacaodetalhe where sbdid!=".$sub['ult']." and sbaid=".$sub['sbaid']." and sbdano='".$sub['sbdano']."'");
		$db->commit();
	}
}

echo "fim";
?>