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
include_once APPRAIZ."includes/classes/dateTime.inc";

$msg = "Materialização da view de pendências feito com sucesso!";
$diff = array();

$cTmp = "obrid,obrnome,empesfera,tobid,tpoid,cloid,tooid,mundescricao,muncod,estuf,entid,docid,situacaoobra,esddsc,dataultimaalteracao,diasultimaalteracao,usuarioultimaalteracao,inuid,obrdtinclusao,diasinclusao,empdtprimeiropagto,diasprimeiropagamento,qtdpedidosdesbloqueio,qtddeferidos,qtdindeferidos,qtdnaoanalisados,desterminodeferido,versaosistema,empid,htddata,docdatainclusao,orgid,prfid,preid,pendencia,obrpercentultvistoria";
$dataS = "TO_CHAR(NOW(), 'YYMMDDHH24' ) as data";
$dataW = "data = TO_CHAR(NOW(), 'YYMMDDHH24' )";

try{
    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'HH');

    $isset = $db->pegaUm("SELECT count(*) tot FROM obras2.v_pendencia_obras_tmp WHERE $dataW");

    if($data == '18' || $isset > 0)
        executar(" TRUNCATE obras2.v_pendencia_obras_tmp ");

    executar(" INSERT INTO obras2.v_pendencia_obras_tmp (SELECT *, $dataS FROM obras2.v_pendencia_obras_base) ");

    $diff = $db->carregar("SELECT q1.obrid obr1 , q1.pendencia p1, q2.obrid obr2 , q2.pendencia p2 FROM obras2.v_pendencia_obras q1
                            FULL OUTER JOIN ( SELECT * FROM obras2.v_pendencia_obras_tmp WHERE $dataW ) q2 ON (q1.obrid || '-' || q1.pendencia = q2.obrid || '-' || q2.pendencia)
                            WHERE q2.pendencia IS NULL OR q1.pendencia IS NULL");

    executar(" INSERT INTO obras2.v_pendencia_obras
                (SELECT $cTmp FROM obras2.v_pendencia_obras_tmp WHERE $dataW AND obrid || '-' || pendencia NOT IN (SELECT obrid || '-' || pendencia FROM obras2.v_pendencia_obras)) ");

    executar(" DELETE FROM obras2.v_pendencia_obras WHERE obrid || '-' || pendencia NOT IN ( SELECT obrid || '-' || pendencia FROM obras2.v_pendencia_obras_tmp WHERE $dataW ) ");

    $msg .= "<br />";
    $msg .= "<table border='1'>";
    $msg .= "<tr style='text-align: center'><th colspan='2'>v_pendencia_obras</th><th colspan='2'>v_pendencia_obras_tmp</th></tr>";
    $msg .= "<tr style='text-align: center'><td>OBRID</td><td>PENDÊNCIA</td><td>OBRID</td><td>PENDÊNCIA</td></tr>";

    if($diff){
        foreach ($diff as $obras){
            $msg .= "<tr><td>{$obras['obr1']}</td><td>{$obras['p1']}</td><td>{$obras['obr2']}</td><td>{$obras['p2']}</td></tr>";
        }
    }

    $msg .= "</table>";

} catch (Exception $e){
    $msg = "Ocorreu um erro durante a materialização <br /><br /> " . $e->getmessage();
}

$db->commit();

$destinatarios = array($_SESSION['email_sistema']);
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
