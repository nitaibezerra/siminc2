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


$sql = "select pre.docid, esd.esdid, pre.preid from obras.preobra pre
		inner join workflow.documento doc on doc.docid = pre.docid
		inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
		inner join territorios.muntipomunicipio tmu on pre.muncod = tmu.muncod and tmu.tpmid = 152
		where esd.esdid not in (193,214,229,228) and prestatus = 'A'";

$_arr = array('214' => '609',
			  '213' => '608',
			  '221' => '613',
			  '210' => '605',
			  '217' => '610',
			  '212' => '607');


$dados = $db->carregar($sql);

if($dados[0]) {

	foreach($dados as $d) {
		$result = wf_alterarEstado( $d['docid'], $aedid = $_arr[$d['esdid']], $cmddsc = 'Obra tramitada para "em cadastramento" para eventuais correções e envio (referente à 2ª chamada G3).', $dados = array('preid' => $d['preid']));
		echo "PREID:".$d['preid']." | Result.".$result."<br>";
	}

}

$db->commit();

?>