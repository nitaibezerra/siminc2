<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "4024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";


try{
    executar(" INSERT INTO obras2.historicopendenciaobras (obrid, hpodata, hpopendencia) (SELECT obrid, NOW(), pendencia FROM obras2.vm_pendencia_obras) ");

} catch (Exception $e){
    $msg = "Ocorreu um erro durante a materialização <br /><br /> " . $e->getmessage();
}

$db->commit();

$destinatarios = array(
    $_SESSION['email_sistema'],
);

$msg = 'Inserção no LOG de pendências efetuado com sucesso.';

$remetente = array("nome" => SIGLA_SISTEMA. " - Monitoramento de Obras", "email" => $_SESSION['email_sistema']);
enviar_email($remetente, $destinatarios, "Materialização da view de pendências", $msg, '', $_SESSION['email_sistema']);

ver($msg, 'FIM', d);

function executar($SQL)
{
    global $db;
    if (gettype( cls_banco::$link[$db->nome_bd] ) != "resource") {
        cls_banco::$link[$db->nome_bd] = null;
        cls_banco::cls_banco();
    }

    $SQL = trim($SQL);
    //detecta operacao e tabela (Insert, Update ou Delete)
    preg_match('/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*FROM|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smui', utf8_encode($SQL), $matches);
    $audtipoCompleto = strtoupper($matches[1]);
    $audtipo         = substr($audtipoCompleto, 0, 1);

    $_SESSION['sql'] = $SQL;

    // Inicia a transação quando nao estiver iniciada e obrigatoriamente quando
    // a operação for diferente de SELECT
    if (!isset($_SESSION['transacao']) && $audtipo != 'S') {
        $db->resultado = pg_query(cls_banco::$link[$db->nome_bd], 'begin transaction; ');
        $_SESSION['transacao'] = '1';
    }

    $db->resultado = @pg_query(cls_banco::$link[$db->nome_bd], $SQL);

    if ($db->resultado == null)
        throw new Exception( $SQL . pg_errormessage( cls_banco::$link[$db->nome_bd] ) );

    return $db->resultado;
}
