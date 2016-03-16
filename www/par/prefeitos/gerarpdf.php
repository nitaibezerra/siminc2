<?php
set_time_limit(0);
ini_set("memory_limit", "40000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";
$geraPdf = true;

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
//include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";
include_once "dompdf/dompdf_config.inc.php";

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';


$db = new cls_banco();

//$sql = "SELECT muncod, estuf, REPLACE(REPLACE(removeAcento(mundescricao), '''', ''),' ','_') as mundescricao FROM territorios.municipio WHERE muncod IN ('1200500') AND muncod NOT IN(SELECT muncod FROM par.pdfgerado WHERE iniciado=true) LIMIT 1";
$sql = "SELECT muncod, estuf, REPLACE(REPLACE(removeAcento(mundescricao), '''', ''),' ','_') as mundescricao FROM territorios.municipio WHERE estuf IN ('AC','AL','AP','AM','BA','CE','ES','GO','MA','MT','MS','PA') AND muncod NOT IN(SELECT muncod FROM par.pdfgerado WHERE iniciado=true) LIMIT 1";

$municipios = $db->carregar($sql);

if($municipios[0]) {
	foreach($municipios as $mu) {
		
		$sql = "SELECT pdgid FROM par.pdfgerado WHERE muncod='".$mu['muncod']."'";
		$pdgid = $db->pegaUm($sql);
		
		if(!$pdgid) {
		  $sql = "INSERT INTO par.pdfgerado(muncod) VALUES ('".$mu['muncod']."') RETURNING pdgid;";
		  $pdgid = $db->pegaUm($sql);
		  $db->commit();
		}
		
		$_REQUEST['muncod'] = $mu['muncod'];
		ob_start();
		include 'prefeitos.php';
		$html = ob_get_contents();
		ob_clean();
		$dompdf = new DOMPDF();
		$dompdf->load_html($html);
		$dompdf->render();
		
		$pdfoutput = $dompdf->output();

		$caminho = APPRAIZ . 'www/par/prefeitos/pdfs/' . $mu['estuf'] . '_' . $mu['mundescricao'] . '.pdf';
    	
    	$fp = fopen($caminho, "w+");
    	

		  stream_set_write_buffer($fp, 0);
		  fwrite($fp, $pdfoutput);
		  fclose($fp);
		  $sql = "UPDATE par.pdfgerado SET iniciado=true WHERE pdgid='".$pdgid."'";
		  $db->executar($sql);
		  $db->commit();
		
	}
} else {
	echo "ACABOU";
}

echo "FIM:".date("d/m/Y h:i:s");
//echo "<script>setTimeout('window.location=window.location;',7000)</script>";
echo "<script>window.location=window.location;</script>";

?>