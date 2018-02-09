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

$sql = "select count(dirid), pesid, max(dirid) as dirid_atual from pdeinterativo.direcao where dirstatus='A' group by pesid having count(dirid)>1";
$direcao = $db->carregar($sql);

if($direcao[0]) {
	foreach($direcao as $di) {
		
		$dirids_antigo = $db->carregarColuna("select dirid from pdeinterativo.direcao where dirid!='".$di['dirid_atual']."' and pesid='".$di['pesid']."'");
		
		$db->executar("delete from pdeinterativo.direcao where dirid in('".implode("','",$dirids_antigo)."')");
		$db->commit();
	}
}

$sql = "select count(pgtid), grtid, pesid, max(pgtid) as pgtid_atual from pdeinterativo.pessoagruptrab group by grtid, pesid having count(pgtid)>1";
$pessoagruptrab = $db->carregar($sql);

if($pessoagruptrab[0]) {
	foreach($pessoagruptrab as $pgt) {
		
		$pgtids_antigo = $db->carregarColuna("select pgtid from pdeinterativo.pessoagruptrab where pgtid!='".$pgt['pgtid_atual']."' and grtid='".$pgt['grtid']."' and pesid='".$pgt['pesid']."'");
		
		$db->executar("delete from pdeinterativo.pessoagruptrab where pgtid in('".implode("','",$pgtids_antigo)."')");
		$db->commit();
	}
}


$sql = "select count(dpeid), pesid, pdeid, max(dpeid) as dpeid_atual from pdeinterativo.demaisprofissionais group by pesid, pdeid having count(dpeid)>1";
$demaisprofissionais = $db->carregar($sql);

if($demaisprofissionais[0]) {
	foreach($demaisprofissionais as $dmp) {
		
		$dpeids_antigo = $db->carregarColuna("select dpeid from pdeinterativo.demaisprofissionais where dpeid!='".$dmp['dpeid_atual']."' and pdeid='".$dmp['pdeid']."' and pesid='".$dmp['pesid']."'");
		
		$db->executar("delete from pdeinterativo.demaisprofissionais where dpeid in('".implode("','",$dpeids_antigo)."')");
		$db->commit();
	}
}

$sql = "select count(paaid), pesid, aadis, max(paaid) as paaid_atual from pdeinterativo.pessoaareaatuacao group by pesid, aadis having count(paaid)>1";
$pessoaareaatuacao = $db->carregar($sql);

if($pessoaareaatuacao[0]) {
	foreach($pessoaareaatuacao as $paa) {
		
		$paaids_antigo = $db->carregarColuna("select paaid from pdeinterativo.pessoaareaatuacao where paaid!='".$paa['paaid_atual']."' and aadis='".$paa['aadis']."' and pesid='".$paa['pesid']."'");
		
		$db->executar("delete from pdeinterativo.pessoaareaatuacao where paaid in('".implode("','",$paaids_antigo)."')");
		$db->commit();
	}
}

$sql = "select count(mceid), pesid, pdeid, max(mceid) as mceid_atual from pdeinterativo.membroconselho group by pesid, pdeid having count(mceid)>1";
$membroconselho = $db->carregar($sql);

if($membroconselho[0]) {
	foreach($membroconselho as $mce) {
		
		$mceids_antigo = $db->carregarColuna("select mceid from pdeinterativo.membroconselho where mceid!='".$mce['mceid_atual']."' and pdeid='".$mce['pdeid']."' and pesid='".$mce['pesid']."'");
		
		$db->executar("delete from pdeinterativo.membroconselho where mceid in('".implode("','",$mceids_antigo)."')");
		$db->commit();
	}
}

$sql = "select count(ptpid), tpeid, pesid, max(ptpid) as ptpid_atual from pdeinterativo.pessoatipoperfil group by tpeid, pesid having count(ptpid)>1";
$pessoatipoperfil = $db->carregar($sql);

if($pessoatipoperfil[0]) {
	foreach($pessoatipoperfil as $ptp) {
		
		$ptpids_antigo = $db->carregarColuna("select ptpid from pdeinterativo.pessoatipoperfil where ptpid!='".$ptp['ptpid_atual']."' and tpeid='".$ptp['tpeid']."' and pesid='".$ptp['pesid']."'");
		
		$db->executar("delete from pdeinterativo.pessoatipoperfil where ptpid in('".implode("','",$ptpids_antigo)."')");
		$db->commit();
	}
}

echo "fim";
?>