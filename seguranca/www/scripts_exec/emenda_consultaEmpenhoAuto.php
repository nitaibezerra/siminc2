<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

error_reporting( E_ALL ^ E_NOTICE );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

// carrega as funções específicas do módulo
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "emenda/classes/ExecucaoFinanceira.class.inc";
include_once APPRAIZ . "emenda/classes/ExecFinanceiraHistorico.class.inc";
require_once APPRAIZ . "emenda/classes/WSContaCorrente.class.inc";
require_once APPRAIZ . "emenda/classes/ContaCorrente.class.inc";
require_once APPRAIZ . "emenda/classes/ContaCorrenteHistorico.class.inc";
require_once APPRAIZ . "emenda/classes/LogErroWS.class.inc";
include_once APPRAIZ . 'www/emenda/_funcoes.php';

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

//$_SESSION['sisid'] = '57';
//$_SESSION['suscod'] = 'A';
//$_SESSION['sisdiretorio'] = 'emenda';
//$_SESSION['sisarquivo'] = 'emenda';
//$_SESSION['sisdsc'] = 'Módulo de Emendas';
//$_SESSION['exercicio_atual'] = date('Y');
//$_SESSION['exercicio'] = date('Y');

$db = new cls_banco();

$arDados = array();
$arDados['usuario'] = 'USAP_WS_SIGARP';
$arDados['senha']   = '03422625';

$obEmpenho = new WSEmpenho($db);
$obEmpenho->consultarEmpenho($arDados, false, true);
unset($obEmpenho);

$obContaCorrente = new WSContaCorrente();
$obContaCorrente->consultarContaCorrente($arDados, true, true);
unset($obContaCorrente);
?>