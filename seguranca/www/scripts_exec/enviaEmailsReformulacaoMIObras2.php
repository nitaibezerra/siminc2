<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/Empreendimento.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Restricao.class.inc";
include_once APPRAIZ . "includes/classes/modelo/entidade/Endereco.class.inc";
include_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


$sql = "
        SELECT o.obrid, o.obrnome, o.empid FROM obras2.obras o
        WHERE
        o.obrid IN (1009196,1012657,1011023,1006602,1017182,1006988,1005989,1009212,1010985,1009353,1012896,1012615,1006104,1011088,1009350,1005445,1006383,1014623,1011087,1017779,1010993,1016572,1012642,1013161,1014660,1005983,1009330,1009360,1001855,1016921,1017582,1018568,1018574,1017782,1012811,1006080,1012699,1012707,25604,1009226,1018571,1006226,1009181,1017059,1016923,1014602,1012909,1014631,20103,18112,1017576,1014526,1006759,1012660,1009343,1011077,1009249,1005696,1012879,1017551,1012650,1005588,20242,1017018,1016493,1011050,1001943,24356,1018576,1016564,1012733,1006532,1009065,20214,1006005,1006465,1017597,1012640,1005287,1012724,1006891,1016931,1006003,1017592,1017122,1012854,1014616,1013285,1012725,1002428,1017543,1012637,1012730,1012821,1016869,1013292,1016890,1012860,1012875,1005987,25348,1014667,1012553,1006721,1006220,1011112,1009269,1016899,1011108,1012566,1001932,1005650,1006361,1004339,1016910,1012863,19977,1016600,1014581,1017534,19845,1017342,1012819,1009173,1018570,1014618,1018573,1017544,1017346,1009107,1009351,1005808,1012638,1017352,1014528,1016244,1002004,1011051,1014633,1012755,1016898,1017541,1009231,1009294,1014638,1009349,1014634,1016557,1006202,1017537,1001783,1006210,1017101,1016894,1016585,1013280,1017080,1017100,1017586,1011074,1010981,30847,1009117,1009352,1016911,1009115,1005980,1016892,1013299,1017591,1003793,1014614,1017087,1006531,1012682,1005385,1017086,1017106,1012644,1009148,1018569,1024743,1016920,1016507,1009098,1016919,1009084,1009145,1017089,1009059,1012893,1003794,1016504,1017074,1002416,1014650,1014649,25135,1006208,19476,1012832,1014518,1011159,1012658,24339,24290,1012633,1010970,1009665,1014630,1014670,1018578,1002414,1001950,1014619,1013258,1009320,1016578,1012709,1014621,1016906,25587,1006604,1017595,1013284,1011019,1009243,1012763,1006354,1012747,1009239,1012795,1016887,1009146,1017554,1009183,1009260,1009354,1016896,1005530,1014659,1012892,1016574,1006584,1017166,1004112,1024748,1017102,1016563,1014620,1017096,1017571,1012654,1011036,1011155,1016893,1006368,1006723,1017354,1014658,1016932,1009101,1015732,1017085,1017780,1011022,19884,1014564,1012620,25611,1012706,1012645,1003807,1017082,1009361,1015725,1012708,1017098,1014524,1017351,1004906,1011081,1006203,1010986,1017093,19779,1006536,1005573,1012719,1012764,1016866,1012887,1009240,18905,1009125,1016903,1017581,1014515,1016598,1017334,1017781,1013281,1017119,1017585,1017079,1009228,1001792,19886,1009132,1014655,1014626,1009104,1006215,1012900,1017546,1016891,1012646,1012761,19824,1010693,1014642,1017538,1006990,1017547,1009129,1009083,1017577,1013274,19726,1010992,1006211,1017788,1006986,1005993,1006292,24329,1006199,1009358,1017084,1014613,1001803,1009311,1017540,1006399,1016880,1014579,1017347,1006706,20035,1006671,1014565,1014562,1006658,1006198,1017076,1010641,1017536,1001973,1016938,1016593,1011158,1024744,1009298,1009225,1014624,1016882,1016913,1012834,1017161,1012786,1017550,1009368,1005984,1009103,1006985,1015738,1009248,1012681,1006041,1009156,1013259,1012727,1016930,1016556,1011026,1012746,1017593,1001764,1005186,1005991,1014514,1009340,1009263,1016583,1009357,1017124,1006350,1013262,1014596,1015770,1016575,1017097,1006753,1011080,1014671,1014509,1006200,1001986,1009671,1009232,1006363,1006201,1002417,1001788,19829,1009268,1001757,1013295,1012720,1004908,1006662,1016928,1006392,25507,1012557,1017588,1002393,1013279,1016889,1012905,1006258,1006105,1017121,25025,1006233,1014566,1016885,1011068,1012669,1009370,1018582,1009339,20274,1012643,1011020,1024745,1006987,19999,1012732,1011083,1014666,1012820,1011156,1012897,1017552,1018579,1005813,1017105,1014499,1010638,1012700,1017350,1016594,1011078,1005446,1009216,1006400,1017170,1017343,1014629,1006213,1009242,1006771,1001903,1002415,1014525,1004100,1012605,1012717,1006259,25516,1016513,1006224,1009122,1016503,1012797,1013263,24351,1013277,1017380,1012737,1017167,1014598,1012662,1014644,1009139,1009124,20246,1012636,1009224,1009131,25537,1012800,1009372,1012728,1006707,1017783,1024742,1003780,1017574,1014657,1014505,1006197,1009664,1006214,1016918,1009331,1017535,1014561,1009111,1010987,1017176,1012639,1017341,1012855,1009371,1017348,1009256,1009341,1011079,1017160,1006081,1009309,1010640,1016225,1011037,1017786,1010637,1011109,1009102,1012769,1009247,1014669,1016506,1009188,1016900,1012780,1016582,1011065,1009348,1009255,1016576,1014664,1012656,1024747,1006207,1017088,1016597,1014572,1010668,1002367,19324,1014504,1009100,1012619,1006589,1012683,25125,1018580,1009126,1006893,1009244,1017175,1009280,1011054,1005818,1006365,1016884,1016505,1016904,1005163,1014641,25124,1017173,1011116,1011075,1005452,1012726,1017125,1017330,1017599,1014617,1006782,1017596,1016907,25538,1016902,1012839,1012655,1017349,1010639,1017326,1012742,1012691,1017178,1012799,1012908,1017339,1016888,1005448,1011084,1009215,1014595,1009259,1012565,1017177,1005185,1009130,1013162,1014584,1014560,1016562,1017572,1018567,1011076,1013266,1014563,1012885,1013301,1009108,1011067,1012818,1006225,1016871,25533,18972,1017539,25083,1011027,1022200,1006710,1017163,1012701,1013278,1011025,1012738,1014645,1012806,1016491,1013293,1016573,1012833,1012793,1012901,1017584,1013275,1012731,1011033,1010674,1016934,1012888,1012754,1012765,1001852,1006396,1013297,1017072,1012757,1017107,1011085,19075,1006209,1006111)
        AND o.obrid NOT IN (SELECT obrid FROM obras2.email WHERE obrid = o.obrid AND temid = 37)
        ";

$obras = $db->carregar($sql);

if(!$db->pegaUm('SELECT temid FROM obras2.tipoemail WHERE temid = 37')){
    $db->pegaUm("INSERT INTO obras2.tipoemail (temid, temnome, temdescricao) VALUES (37, 'Reformulação MI', 'Reformulação MI')");
    $db->commit();
}

foreach ($obras as $obra) {
    $email = new Email();
    $date = new Data();
    $data = $date->formataData($date->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
    $dados = array(
        'usucpf' => '21269017500',
        'emlconteudo' => '
                    <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS<br/>
                                                SBS Quadra 02 - Bloco F - 12º ANDAR - Edifício FNDE - CEP - 70070-929 - Brasília, DF - E-mail: reformulação.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Prezado Gestor ,</p>

                                                <p>
                                                    Informamos que as obras pactuadas através de Metodologia Inovadora que se
                                                    encontram no SIMEC-módulo PAR/Lista de obras, na situação "Em Reformulação MI para convencional",
                                                    deverão ser encaminhadas para análise da equipe técnica do FNDE até o dia 30/07/2015, data esta em que o sistema
                                                    será bloqueado para envio.
                                                </p>

                                                <p>
                                                    Assim, solicitamos tomar as medidas necessárias  para que o pleito seja
                                                    encaminhado o mais rápido possível, visando agilidade no processo licitatório
                                                    que ficará a cargo do município.
                                                </p>

                                                <p>
                                                    Em caso de dúvidas favor consultar o manual de reformulação MI para
                                                    convencional, disponibilizado no link http://www.fnde.gov.br/programas/proinfancia/proinfancia-manuais ,
                                                    ou encaminha e-mail para reformulacao.obras@fnde.gov.br.
                                                </p>

                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 10px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                                    <br />
                                                    <b>Fábio Lúcio de Almeida Cardoso<b>
                                                    <br />
                                                    Coordenador-Geral de Infraestrutura Educacional - CGEST
                                                    <br />
                                                    Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP
                                                    <br />
                                                    Fundo Nacional de Desenvolvimento da Educação-FNDE
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>',
        'emlassunto' => SIGLA_SISTEMA. ' - Obras 2.0 - Reformulação MI para convencional',
        'temid' => 37,
        'emlregistroatividade' => true,
        'obrid' => $obra['obrid']
    );

    $dadosDestinatario = pegaGestores($obra['empid']);

    if (count($dadosDestinatario) > 0) {
        echo $obra['obrid'] . '<br />';

        $email->popularDadosObjeto($dados);
        $email->salvar($dadosDestinatario);
        $email->enviar();
    }

}

echo '<br />EXECUTADO!';

function pegaGestores ($empid){
    global $db;

    $sql = "select
                u.usuemail
            from obras2.usuarioresponsabilidade ur
            inner join seguranca.usuario u on u.usucpf = ur.usucpf
            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
            left join obras2.empreendimento e on e.empid = $empid
            where
                ur.rpustatus = 'A' AND
                u.suscod = 'A' AND
                ur.pflcod = 946 AND
                ur.entid = e.entidunidade";
    return $db->carregarColuna($sql);
}