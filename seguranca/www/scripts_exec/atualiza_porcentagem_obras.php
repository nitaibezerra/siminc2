<?php 

ini_set("memory_limit", "3024M");
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
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

if($_REQUEST['cleanup']){
    $sqlClean = "UPDATE obras2.empreendimento SET emppercentultvistoriaempresa = null, empdtultvistoriaempresa = null;";
    $db->executar($sqlClean);
}


$sqlDiferenca = "

    SELECT
        e.empid,
        e.emppercentultvistoriaempresa,
        foo.percent,
        e.empdtultvistoriaempresa,
        foo.suedtsupervisao
    FROM
        obras2.empreendimento e
    JOIN
        (SELECT
              sue.empid,
              MAX(sue.sueid) sueid,
              MAX(sue.suedtsupervisao) suedtsupervisao,
              MAX(s.supid) supid,
              (SELECT
                   sum(si.spivlritemsobreobraexecanterior)
                FROM obras2.supervisaoitem si
                JOIN obras2.itenscomposicaoobra itc ON itc.icoid = si.icoid AND itc.obrid = MAX(s.obrid)
                WHERE si.supid =  MAX(s.supid)) AS percent
            FROM obras2.supervisaoempresa sue
              JOIN obras2.supervisao s
            ON s.sueid = sue.sueid AND s.supstatus = 'A'
              LEFT JOIN workflow.documento d
            ON d.docid = sue.docid
              LEFT JOIN workflow.estadodocumento ed
            ON ed.esdid = d.esdid
            WHERE sue.suestatus = 'A' AND ed.esdid IN (734, 756, 757)
            GROUP BY sue.empid) foo ON foo.empid = e.empid
    WHERE e.empstatus = 'A' AND COALESCE(e.emppercentultvistoriaempresa, -1) != foo.percent

";

try{
    $empDiff = $db->carregar($sqlDiferenca);
    $c = 0;
    if($empDiff){
        foreach($empDiff as $diff){
            $c++;
            $sql = "UPDATE obras2.empreendimento SET emppercentultvistoriaempresa = {$diff['percent']}, empdtultvistoriaempresa = '{$diff['suedtsupervisao']}' WHERE empid = {$diff['empid']}";
            echo " - $c - <br />";
            echo "Empreendimento: {$diff['empid']} <br />";
            echo "Percentual anterior: {$diff['emppercentultvistoriaempresa']} <br />";
            echo "Atualizado para: {$diff['percent']} <br />";
            echo "SQL: $sql <br />";
            echo "----------------------------------------- <br />";
            echo "<br />";
            $db->executar($sql);
        }
    }
} catch (Exception $e) {
    $db->rollback();
}

$db->commit();
echo "<br /><b>TOTAL: $c</b><br />";
echo "<br /><b>Sucesso!</b><br />";
?>