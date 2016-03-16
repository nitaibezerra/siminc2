<?php
include_once("pdeWs.php");

$ws = new pdeWs();
$coProgramaFNDE = 96;
$anoAtual = date('Y');


ini_set("memory_limit","5024M");
set_time_limit(0);

$sql = "SELECT entcodent FROM pdeescola.wstemp limit 1";
$dados  = $db->carregar( $sql );

foreach($dados as $d){
	$r = $ws->pdeEscolaWs('atualizaAnaliseEscola', $anoAtual, $d['entcodent'], $coProgramaFNDE);
	echo $d['entcodent'] .' - '. $r .'<br>';
}

echo "FIMMMMMMMMMMMMMMM<br>";
echo "ACABOUUUUUUUUUUUUUUUUUUUUUUUUUUU";

?>