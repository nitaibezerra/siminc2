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
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ContatosObra.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/SolicitacaoDesembolso.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Supervisao.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/workflow.php";


$solicitado = array();
$naoSolicitado = array();


$sql = "SELECT * FROM obras2.obras WHERE obrid NOT IN (SELECT DISTINCT obrid FROM obras2.solicitacao_desembolso  WHERE sldstatus = 'A') AND obrid IN
(
-- Quarta parcela
27405,29497,26181,33069,1007139,18640,1001735,30885,1003335,29640,31301,31300,1007213,1010152,1007514,1007477,1007434,1008889,1004450,1004120,29776,29777,1002630,33120,33122,33119,1002142,18815,18817,30945,26099,22747,1007387,1013684,31779,30152,31773,1010155,25643,26258,1007562,1007605,1000929,1002815,29760,1004494,1004874,30379,26776,31744,30370,30430,30564,30504,30328,23139,30319,30591,30401,26750,30600,30418,30470,30205,26764,1004202,1004195,1004193,1004168,31665,1004208,31542,27465,24746,22586,22586,22569,1010045,1015958,1005693,29342,18875,24957,24957,29592,27558,1000749,30881,30875,24572,23246,1000498,30728,20138,30736,31051,30727,24525,20174,1010417,20031,1010789,1010790,1010392,1000595,18271,18717,1002657,1002673,19318,25689,29716,
-- Terceira parcela
1005041,1013972,26179,1013970,1017619,1013981,1007866,1014017,1015239,1015235,1015461,1016670,1002072,1016706,1002898,1002860,1017497,1017750,1018115,1009387,1010157,1010309,1015976,1005871,27074,1015763,1015748,1009517,1009803,1009801,1009804,1009963,1009976,1009975,1018074,1010009,1010002,29336,1006408,1009519,1009531,27026,1009816,1009815,1009789,1015412,1010322,1010060,33299,1009845,1009856,1015356,1015355,1016200,1002943,1017719,1014104,1010583,1010823,31437,1017272,1006344,1014178,1016100,1015561,1014193,1016095,1016320,1017466,1004902,1015733,1009904,1009913,
-- Segunda parcela
1010181,1007289,1007264,1018037,1018085,1010052,1018699,1018680,1018586,1015346,1015272,1015274,1018095,1009469,1009473,1009474,1009403,1009453,1010158,1010159,1010747,1018231,1018589,1009986,1010358,31900,1009788,31222,1010793,1014161,1010862,1013565,1013559,1018527
)";

$obras = $db->carregar($sql);

foreach ($obras as $obra) {
    // Verificar se o percentual da solicitação é valido
    $solicitacaoDesembolso = new SolicitacaoDesembolso();
    $percSolicitado =  $solicitacaoDesembolso->pegaPercentualSolicitacao($obra['obrid']);

    if($percSolicitado <= 0) {
        $naoSolicitado[] = array('id' => $obra['obrid'], 'percentual' => $percSolicitado);
        continue;
    }

    $supervisao = new Supervisao();
    $supid = $supervisao->pegaUltSupidByObra($obra['obrid']);

    $solicitacaoDesembolso = new SolicitacaoDesembolso();
    $solicitacaoDesembolso->docid = wf_cadastrarDocumento(TPDID_SOLICITACAO_DESEMBOLSO, 'Fluxo da Solicitação de Desembolso');
    $solicitacaoDesembolso->obrid = $obra['obrid'];
    $solicitacaoDesembolso->sldjustificativa = 'Solicitação criada automaticamente por conta da validação da última parcela.';
    $solicitacaoDesembolso->sldobs = '';
    $solicitacaoDesembolso->usucpf = $_SESSION['usucpf'];
    $solicitacaoDesembolso->sldpercsolicitado = $percSolicitado;
    $solicitacaoDesembolso->supid = $supid;

    $solicitacaoDesembolso->salvar();
    $solicitacaoDesembolso->commit();

    $solicitado[] = array('id' => $obra['obrid'], 'id_solicitacao' => $solicitacaoDesembolso->sldid, 'percentual' => $percSolicitado);

}


?>

<table style="border: solid 1px">
    <tr>
        <td  style="border: solid 1px">ID</td>
        <td  style="border: solid 1px">SOLICITAÇÃO</td>
        <td  style="border: solid 1px">PERCENTUAL</td>
    </tr>
    <?foreach($solicitado as $s):?>
        <tr>
            <td  style="border: solid 1px"><?=$s['id']?></td>
            <td style="border: solid 1px"><?=$s['id_solicitacao']?></td>
            <td style="border: solid 1px"><?=$s['percentual']?></td>
        </tr>
    <?endforeach;?>
</table>

<table  style="border: solid 1px">
    <tr>
        <td  style="border: solid 1px">ID</td>
        <td  style="border: solid 1px">PERCENTUAL</td>
    </tr>
    <?foreach($naoSolicitado as $s):?>
        <tr>
            <td style="border: solid 1px"><?=$s['id']?></td>
            <td style="border: solid 1px"><?=$s['percentual']?></td>
        </tr>
    <?endforeach;?>
</table>