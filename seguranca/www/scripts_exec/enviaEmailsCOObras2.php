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
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";

$arObras = array(1368,1370,1386,1397,1414,1415,1419,1432,1435,1444,1458,1464,1486,1516,1537,1541,1566,1572,1578,1579,1581,1616,1630,1635,1640,1652,1682,1689,1711,1720,1723,1739,1741,1777,1796,1809,1867,1952,1953,1959,2001,2030,2033,2036,2059,2068,2078,2088,2102,2107,2119,2120,2136,2137,2148,2158,2168,2176,2182,2188,2207,2232,2241,2431,2768,3551,3576,3764,3770,3800,4284,4291,4437,4438,4474,7883,7893,7897,7905,8377,8379,8380,8381,8390,8400,8401,8409,8411,8412,8418,8429,8471,8487,8488,8493,8501,8509,8516,8520,8521,8524,8532,8543,8548,8555,8564,8569,8573,8590,8605,8606,8610,8611,8626,8635,8636,8645,8667,8668,8670,8682,8685,8692,8708,8715,8726,8742,8753,8758,8760,8772,8776,8779,8784,8792,8801,8802,8803,8824,8843,8856,8868,8890,8954,11561,11686,11720,11727,11731,11788,11796,11808,11810,11814,11825,11829,11836,11841,11871,11907,12606,12614,12617,12648,13081,13248,13252,13275,13288,13289,13291,13297,13339,13351,13358,13359,13366,13368,13372,13474,13542,13636,14327,14594,14603,14636,14808,17446,17448,17449,17501,17513,17536,17952,17953,18075,18076,18082,18087,18096,18193,18472,18906,18907,18912,18939,18984,18999,19021,19028,19051,19177,19189,19190,19255,19275,19358,19361,19400,19401,19402,19441,19447,19520,19539,19687,19721,19759,19772,19856,19869,19894,19948,19980,19985,19989,19990,20021,20073,20088,20125,20134,20142,20146,20148,20158,20159,20188,20189,20211,20227,20263,20266,23235,23270,23769,24286,24384,24542,24573,24578,24681,24682,24683,24718,24950,25142,25143,25149,25158,25163,25166,25386,25387,25389,25390,25391,25392,25397,25398,25400,25401,25402,25459,25460,25461,25462,25465,25468,25482,25503,25539,25574,25579,25634,25687,25707,25716,25728,25738,25739,25740,25770,27722);
$_SESSION['usuemail'] = $_SESSION['email_sistema'];
try{
    $email = new Email();
    $count = 1;
    foreach ($arObras as $key => $obra) {
        if($email->verificaEmailEnviado(46,$obra)){
            continue;
        }
        $email->enviaEmailObraConcluida($obra,20);
        if($count == $_GET['limit']) {
            break;
        }
        $count++;
    }
    echo '<h1>Executado!!!</h1>';
} catch (Exception $ex) {
    echo '<h1>Não foi possível enviar os e-mails. Erro: </h1>';
    echo '<pre>';
    var_dump($ex);
    echo '</pre>';
}




///   /seguranca/www/scripts_exec/enviaEmailsObras2.php

?>