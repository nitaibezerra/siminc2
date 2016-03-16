<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';


$db = new cls_banco();

$sql = "SELECT count(DISTINCT muncod) FROM par.pdfgerado WHERE iniciado = 't'";

$total = $db->pegaUm($sql);

echo $total . ' Municípios carregados.<br>';

$tempo_passado = "2013-01-26 15:20:00";
$agora = date("Y-m-d H:i:s");
$segundos = (strtotime($agora) - strtotime($tempo_passado));
$minutos = $segundos / (60);
$horas = $minutos / (60);
$mostra_horas = intval($horas);
$mostra_minutos = intval($minutos - ($mostra_horas * 60));
$mostra_segundos = intval($segundos - ($mostra_minutos * 60 * 60) - ($mostra_minutos * 60));
echo "Já se passaram ".$mostra_horas." hora(s) e ".$mostra_minutos." minuto(s)<br><br>";


$sql = "SELECT m.muncod, m.mundescricao, m.estuf FROM par.pdfgerado pg INNER JOIN territorios.municipio m ON m.muncod = pg.muncod WHERE pg.iniciado = 'f'";
$dados = $db->carregar($sql);

if(is_array($dados)){
	foreach($dados as $dado){
		echo $dado['muncod'] . ' - '. $dado['mundescricao'] . ' - ' . $dado['estuf'] . '<br>';
	}
}

echo "<script>setTimeout('window.location=window.location;',7000)</script>";
?>