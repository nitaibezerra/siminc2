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
include_once APPRAIZ . "includes/classes/dateTime.inc";


enviaEmailDiligencia();
enviaEmailDiligenciaProrrogacao();
enviaEmailDiligenciaCumprimentoObjeto();
echo '<h1>Executado!!!</h1>';


function enviaEmailDiligencia()
{
    enviaAlertaTipo(17, array('fabio.cardoso@fnde.gov.br', 'livia.neves@fnde.gov.br', 'fabricio.araujo@fnde.gov.br'));
}

function enviaEmailDiligenciaProrrogacao()
{
    enviaAlertaTipo(24, array('fabio.cardoso@fnde.gov.br', 'fabricio.araujo@fnde.gov.br', 'cristiane.tavares@fnde.gov.br', 'rosemary.montalvao@fnde.gov.br'));
}

function enviaEmailDiligenciaCumprimentoObjeto()
{
    enviaAlertaTipo(16, array('fabio.cardoso@fnde.gov.br', 'fabricio.araujo@fnde.gov.br', 'jose.sanctis@fnde.gov.br', 'francisca.vale@fnde.gov.br', 'ana.lima@fnde.gov.br'));
}

function enviaAlertaTipo($tprid, $destinatarios)
{
    global $db;

    $sql = "SELECT
                obr.obrid,
                CASE WHEN rst.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
                esd_ri.esddsc as situacao, -- Situação
                mun.estuf as estado, -- * Estado
                mun.mundescricao as municipio, -- * MUNICIPIO
                CASE WHEN rst.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
                tr.tprdsc as tipo, -- * Tipo
                '(' || obr.obrid || ') ' || obr.obrnome  as nome_obra, -- * Nome da Obra
                TO_CHAR(rst.rstdtinclusao, 'DD/MM/YYYY') as data_cadastro, -- * Data Cadastro
                usu.usunome as usucriacao, -- * Criado por
                usu.usuemail,
                TO_CHAR(rst.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS previsao_providencia_dt -- * Previsão da Providência

             FROM obras2.restricao rst
             JOIN obras2.obras                      obr ON obr.obrid  = rst.obrid AND obr.obrstatus = 'A' AND obr.obridpai IS NULL
             LEFT JOIN workflow.documento        doc_ri ON doc_ri.docid  = rst.docid
             LEFT JOIN workflow.estadodocumento  esd_ri ON esd_ri.esdid  = doc_ri.esdid
             LEFT JOIN obras2.tiporestricao          tr ON tr.tprid   = rst.tprid   AND tr.tprstatus   = 'A'
             LEFT JOIN obras2.faserestricao          fr ON fr.fsrid   = rst.fsrid   AND fr.fsrstatus   = 'A'
             LEFT JOIN entidade.endereco           ende ON ende.endid = obr.endid AND ende.endstatus = 'A' AND ende.tpeid = 4
             LEFT JOIN territorios.municipio        mun ON mun.muncod = ende.muncod
             LEFT JOIN territorios.estado            uf ON mun.estuf  = uf.estuf
             LEFT JOIN seguranca.usuario            usu ON usu.usucpf = rst.usucpf

             WHERE rst.rststatus = 'A'
              AND tr.tprid = $tprid
              AND esd_ri.esdid IN ( 1140,1141,1144 )
              AND rst.rstdtprevisaoregularizacao < NOW()
              AND rst.rstdtsuperacao IS NULL
              AND CASE WHEN tr.tprid IN(24,16) THEN rst.rstitem IN ('R', 'I') ELSE rst.rstitem IN ('R') END
             ORDER BY rst.rstid, obr.obrid
    ";

    $obras = $db->carregar($sql);
    $body = "";

    if (!$obras)
        return;

    $usuObras = array();

    foreach ($obras as $obra) {
        $str = "
            <tr>
                <td style='text-align: center;'>{$obra['obrid']}</td>
                <td style='text-align: center;'>{$obra['nome_obra']}</td>
                <td style='text-align: center;'>{$obra['estado']}</td>
                <td style='text-align: center;'>{$obra['municipio']}</td>
                <td style='text-align: center;'>{$obra['fase']}</td>
                <td style='text-align: center;'>{$obra['tipo']}</td>
                <td style='text-align: center;'>{$obra['data_cadastro']}</td>
                <td style='text-align: center;'>{$obra['usucriacao']}</td>
                <td style='text-align: center;'>{$obra['previsao_providencia_dt']}</td>
            </tr>
        ";

        $objObras = new Obras($obra['obrid']);
        $situacao = $objObras->getEstadoObraWf();
        if ($situacao['esdid'] != ESDID_OBJ_CANCELADO) {
            $body .= $str;
            $usuObras[$obra['usuemail']] .= $str;
        }
    }
    $tipo = $db->pegaUm("SELECT tprdsc FROM obras2.tiporestricao WHERE tprid = $tprid");
    $dados = array(
        'usucpf' => $_SESSION['usucpf'],
        'emlconteudo' => getConteudo($body, $tprid),
        'emlassunto' => 'Restrições e/ou Inconformidades em ' . $tipo,
        'temid' => 1,
        'emlregistroatividade' => 'false',
        'obrid' => null
    );

    $dadosDestinatario = $destinatarios;

    $email = new Email();

    $email->popularDadosObjeto($dados);
    $email->salvar($dadosDestinatario);
    $email->enviar();


//    foreach ($usuObras as $email => $body) {
//        $dados = array(
//            'usucpf' => $_SESSION['usucpf'],
//            'emlconteudo' => getConteudo($body),
//            'emlassunto' => 'Restrições e/ou Inconformidades em Diligência',
//            'temid' => 1,
//            'emlregistroatividade' => 'false',
//            'obrid' => null
//        );
//        $dadosDestinatario = array($email);
//
//        $email = new Email();
//
//        $email->popularDadosObjeto($dados);
//        $email->salvar($dadosDestinatario);
//        $email->enviar();
//    }

}
function getConteudo($body, $tprid){
    global $db;
    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

    $tipo = $db->pegaUm("SELECT tprdsc FROM obras2.tiporestricao WHERE tprid = $tprid");

    return '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,'. base64_encode( file_get_contents( APPRAIZ. '/www/' . 'imagens/brasao.gif' ) ) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                '.$data.'
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                                &nbsp;
                                            </td>
                                        </tr>

                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Restrições e/ou Inconformidades em '.$tipo.'</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>As seguintes obras possuem Restrições e/ou Inconformidades em '.$tipo.' cuja data de superação encontra-se vencida:</p>

                                                <table border="1px">
                                                     <thead>
                                                        <tr>
                                                            <td style="text-align: center;">ID</td>
                                                            <td style="text-align: center;">Nome</td>
                                                            <td style="text-align: center;">Estado</td>
                                                            <td style="text-align: center;">Município</td>
                                                            <td style="text-align: center;">Fase</td>
                                                            <td style="text-align: center;">Tipo</td>
                                                            <td style="text-align: center;">Dt de Cadastro</td>
                                                            <td style="text-align: center;">Criado Por</td>
                                                            <td style="text-align: center;">Dt. Previsão Superação</td>
                                                        </tr>
                                                    </thead>
                                                    <tbody>
                                                        '.$body.'
                                                    </tbody>
                                                </table>

                                            </td>
                                        </tr>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ';
}

?>