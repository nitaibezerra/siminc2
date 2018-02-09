<?php

// Iniciamos o "contador"
list($usec, $sec) = explode(' ', microtime());
$script_start = (float) $sec + (float) $usec;

date_default_timezone_set ('America/Sao_Paulo');

set_time_limit(100000);
ini_set("memory_limit", "10000M");

if(!$_SESSION['usucpf']){
	$_SESSION['usucpforigem'] 	= '00000000191';
	$_SESSION['usucpf'] 		= '00000000191';
}

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Respostas_Formulario.class.inc';

// O mes de ver no formato 01/2015
$mes = $_REQUEST['mes'] ? $_REQUEST['mes'] : false;

// Puxa os formularios do mes anterior caso seja o 1º dia do mes
if(!$mes){
	$date = date('Y-m-d H:i:s'); 
	$timestamp1 = strtotime($date);
	$timestamp2 = strtotime('-1 month', $timestamp1);
	$mes = array();
	
	$mes[] = date('m/Y', $timestamp1);
	$mes[] = date('m/Y', $timestamp2);
}

if($_REQUEST['puxtaTudo']) $mes=null;

$obFormularios = new Ws_Respostas_Formulario();
if(is_array($mes)){
	if($mes){
		foreach($mes as $m){
			$obFormularios->importaFormulariosMaisMedicos($m);
		}
	}
}else{	
	$obFormularios->importaFormulariosMaisMedicos($mes);
}

// Terminamos o "contador" e exibimos
list($usec, $sec) = explode(' ', microtime());
$script_end = (float) $sec + (float) $usec;
$elapsed_time = round($script_end - $script_start, 5);
echo '<p>&nbsp;</p>Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb';

?>