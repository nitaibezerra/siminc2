<?php 

ini_set("memory_limit", "3024M");
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

// carrega as funções gerais
// include_once "config.inc";
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

$db = new cls_banco();

include_once APPRAIZ . 'www/par/_constantes.php';
include_once APPRAIZ . 'www/par/_funcoes.php';
include_once APPRAIZ . 'www/par/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/par/EmailPrazoProvidenciaRestricao.class.inc";

try{
    $EmailPrazoProvidenciaRestricao = new EmailPrazoProvidenciaRestricao();
    $EmailPrazoProvidenciaRestricao->enviarEmailPrazoProvidenciaRestricao();
    echo '<h1>Executado!!!</h1>';
} catch (Exception $ex) {
    echo '<h1>Não foi possível enviar os e-mails. Erro: </h1>';
    echo '<pre>';
        var_dump($ex);
    echo '</pre>';
}