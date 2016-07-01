<?php

include APPRAIZ . 'includes/classes/class.diagram.php';
   
$diagram = new Diagram();

function caminho($esdid) {
	global $xml, $_estsituacao, $db;
	
	$sql = "SELECT aed.aeddscrealizada, aed.aeddscrealizar, esd.esdordem, esd.esddsc, esd.esdid, esd.esddsc FROM workflow.acaoestadodoc aed 
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino 
			WHERE esdidorigem='".$esdid."' AND aed.aedstatus='A'";
	
	$estados = $db->carregar($sql);
	
	if($estados[0]) {
		foreach($estados as $estado) {
			$xml .= "<node name=\"".removeacentos($estado['aeddscrealizada'])."\" borderconnection=\"2\" connectioncolor=\"#a00\">";
			$xml .= $estado['esdordem']." - ".removeacentos($estado['esddsc']);
			if(!$_estsituacao[$estado['esdid']]) {
				$_estsituacao[$estado['esdid']] = true;
				caminho($estado['esdid']);
			}
			$xml .= "</node>";
		}			
	}

}

$sql = "SELECT * FROM workflow.estadodocumento WHERE tpdid='".$_REQUEST['tpdid']."' ORDER BY esdordem";
$estado = $db->pegaLinha($sql);

$_estsituacao[$estado['esdid']] = true;
$xml .= "<?xml version=\"1.0\" encoding=\"UTF-8\"?><diagram width=\"100\" namealign=\"center\"><node>";
$xml .= $estado['esdordem']." - ".removeacentos($estado['esddsc']);
caminho($estado['esdid']);
$xml .= "</node></diagram>";


$diagram->loadXmlData($xml);
$diagram->Draw();


?>