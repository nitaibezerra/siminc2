<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select o.proid, l.lwsid, l.lwsmsgretorno, p.pagid, h.hwpxmlretorno
		from par.pagamento p 
			inner join par.empenho e on e.empid = p.empid
		    inner join par.processoobraspar o on o.pronumeroprocesso = e.empnumeroprocesso
		    inner join par.logws l on l.pagid = p.pagid
		    inner join par.historicowsprocessoobrapar h on h.proid = o.proid and h.lwsid = l.lwsid
		where p.parnumseqob is null";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	
	$xml = simplexml_load_string( stripslashes($v['hwpxmlretorno']));
	
	$sql = "UPDATE par.pagamento SET parnumseqob = ".(($xml->body->nu_registro_ob)?"'".$xml->body->nu_registro_ob."'":"NULL")." WHERE pagid = ".$v['pagid'];
	$db->executar($sql);
}
$db->commit();