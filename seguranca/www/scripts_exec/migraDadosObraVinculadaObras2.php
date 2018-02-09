<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "9024M");
ini_set("default_socket_timeout", "70000000");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

$db = new cls_banco();

include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Validacao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/FilaRestricao.class.inc";


$obrid = $_GET['obrid'];


$obridvinculado = $db->pegaUm("SELECT obridvinculado FROM obras2.obras WHERE obrid = $obrid");

if($obrid && $obridvinculado) {
    migraDadosObraVinculada($obrid, $obridvinculado);
    echo 'EXECUTADO!';
} else {
    echo 'ERRO!';
}

