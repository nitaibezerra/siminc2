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

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '4';

$db = new cls_banco();

try{
    $sql = " INSERT INTO estatistica.atrasodemanda (usucpf, dmdid, dtprevisaotermino, dtprocessamento) (
                SELECT d.usucpfexecutor,dmdid, dmddatafimprevatendimento, NOW() as dtprocessamento
                -- count(*) as qtd, d.usucpfexecutor
                FROM demandas.demanda as d
                    LEFT JOIN workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE d.usucpfdemandante is not null
                AND d.dmdstatus = 'A'
                AND ed.esdstatus = 'A'
                AND doc.esdid in (91,92,107,108)
                AND d.dmddatafimprevatendimento < CURRENT_DATE
                and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                and d.usucpfexecutor in
                (
                    SELECT distinct u.usucpf
                    FROM seguranca.usuario AS u
                        INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                        INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                    WHERE ur.rpustatus = 'A' AND us.susstatus = 'A' AND us.suscod = 'A'
                    and ur.pflcod in ('238')
                    and ur.celid = '2'
                )
            ) ";
    executar($sql);

} catch (Exception $e){
    $msg = "Ocorreu um erro durante a materialização <br /><br /> " . $e->getmessage();
}

$db->commit();

echo 'FIM';

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
