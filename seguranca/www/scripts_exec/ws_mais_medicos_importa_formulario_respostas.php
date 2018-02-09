<?php

// Iniciamos o "contador"
list($usec, $sec) = explode(' ', microtime());
$script_start = (float) $sec + (float) $usec;

date_default_timezone_set ('America/Sao_Paulo');

set_time_limit(100000);
ini_set("memory_limit", "10000M");

// $_SESSION['baselogin'] 		= "simec_espelho_producao";
$_SESSION['baselogin'] 		= "simec_desenvolvimento";
if(!$_SESSION['usucpf']){
	$_SESSION['usucpforigem'] 	= '00000000191';
	$_SESSION['usucpf'] 		= '00000000191';
}

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'maismedicos/classes/Ws_Respostas_Formulario_Itens.class.inc';

$obFormularios = new Ws_Respostas_Formulario_Itens();
$obFormularios->importaRespostasFormularioMaisMedicos();

// Terminamos o "contador" e exibimos
list($usec, $sec) = explode(' ', microtime());
$script_end = (float) $sec + (float) $usec;
$elapsed_time = round($script_end - $script_start, 5);
echo 'Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb';

?>