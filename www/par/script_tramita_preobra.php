<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';

$db = new cls_banco();

if($_GET['parte1']) {
	$sql = "select pre.preid, pre.docid from  obras.preobra pre
	inner join territorios.municipio mun on mun.muncod = pre.muncod
	inner join workflow.documento doc on doc.docid = pre.docid and doc.esdid = 217
	inner join territorios.muntipomunicipio mtm on mtm.muncod = mun.muncod and mtm.tpmid in  (150,151)";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		$n = 1;
		foreach($dados as $d) {
			$result = wf_alterarEstado( $d['docid'], 531 , $cmddsc = '', $dados = array('preid' => $d['preid']) );
			echo "$n - DOCID: ".$d['docid']." | Result: ".$result."<br>";
			$n++;
		}
	
	}
	
	$db->commit();
	dbg("Foi");
}

if($_GET['parte2']){
	$sql = "select pre.preid, pre.docid from  obras.preobra pre
	inner join territorios.municipio mun on mun.muncod = pre.muncod
	inner join workflow.documento doc on doc.docid = pre.docid and doc.esdid = 210
	inner join territorios.muntipomunicipio mtm on mtm.muncod = mun.muncod and mtm.tpmid in  (150,151)";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		$n = 1;
		foreach($dados as $d) {
			$result = wf_alterarEstado( $d['docid'], 516 , $cmddsc = 'Enviado para correção', $dados = array('preid' => $d['preid']) );
			echo "$n - DOCID: ".$d['docid']." | Result: ".$result."<br>";
			$n++;
		}
	
	}
	
	$db->commit();
	dbg("Foi");
}

// Feito pelo Alexandre a pedido do Daniel... 24/06/11
if($_GET['parte3']){
	$sql = "SELECT DISTINCT  doc.docid
			FROM obras.preobra pre
			INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
			inner join territorios.muntipomunicipio tpm on tpm.muncod = mun.muncod and tpm.tpmid = 152
			INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND doc.esdid = 193 
			where pre.prestatus = 'A'";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		$n = 1;
		foreach($dados as $d) {
			$result = wf_alterarEstado( $d['docid'], 573, $cmddsc = 'Obra arquivada por não ter sido enviada para análise no prazo, conforme RESOLUÇÃO CD/FNDE  Nº 8, DE 25 DE FEVEREIRO DE 2011', $dados = array('preid' => $d['preid']) );
			echo "$n - DOCID: ".$d['docid']." | Result: ".$result."<br>";
			$n++;
		}
	
	}
	
	$db->commit();
	dbg("Foi");
}

?>