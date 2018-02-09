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
include_once APPRAIZ . "includes/classes/modelo/obras2/DestinatarioEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Email.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/EmailItemMonitoramento.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/EmailAtividadeMonitoramento.class.inc";

$where7DiasTarefa = "AND itm.itmdtlimiteconclusao <= now() + '7 days' AND  itm.itmdtlimiteconclusao >= NOW();";
$whereTarefaVencida = "AND itm.itmdtlimiteconclusao < NOW();";

$where7DiasAtividade = "AND atm.atmdtlimiteconclusao <= now() + '7 days' AND  atm.atmdtlimiteconclusao >= NOW()";
$whereAtividadeVencida = "AND atm.atmdtlimiteconclusao < NOW()";

$where = '';

$sqlTarefa = "SELECT
            itm.itmid,
            itm.itmnome,
            itm.itmobs,
            COALESCE(TO_CHAR(itm.itmdtlimiteconclusao, 'dd/mm/YYYY')) AS itmdtlimiteconclusao,
            itm.itmqtddiasstatusrepeticao,
            usu_resp.usuemail,
            usu_resp.usunome,
            atm.atmnome,
            obr.obrid
        FROM obras2.itemmonitoramento itm
        INNER JOIN obras2.atividademonitoramento           atm ON atm.atmid       = itm.atmid
        LEFT  JOIN obras2.obras                            obr ON obr.obrid       = atm.obrid
        INNER JOIN obras2.responsavelitemmonitoramento    resp ON itm.itmid       = resp.itmid AND resp.rimstatus = 'A'
        INNER JOIN seguranca.usuario                  usu_resp ON usu_resp.usucpf = resp.usucpf_responsavel
        INNER JOIN entidade.entidade              usu_resp_ent ON usu_resp.usucpf = usu_resp_ent.entnumcpfcnpj
        INNER JOIN workflow.documento 	                 doc ON doc.docid       = itm.docid
        WHERE  itm.itmstatus = 'A' %s";


$sqlAtividade = "SELECT
    obr.obrid,
    ent.entnome as entidade,
    atm.atmid,
    atm.atmnome,
    atm.atmobs,
    tam.tamdesc,
    sam.samdesc,
    est.estuf,
    mun.mundescricao,
    COALESCE(TO_CHAR(atm.atmdtlimiteconclusao, 'dd/mm/YYYY')) AS atmdtlimiteconclusao,
    esd.esddsc,
    usu.usunome,
    usu.usuemail

FROM obras2.atividademonitoramento atm
INNER JOIN obras2.tipoatividademonitoramento    tam ON atm.tamid  = tam.tamid
INNER JOIN obras2.subtipoatividademonitoramento sam ON atm.samid  = sam.samid
INNER JOIN territorios.estado                   est ON atm.estuf  = est.estuf
INNER JOIN territorios.municipio                mun ON atm.muncod = mun.muncod
INNER JOIN entidade.entidade                    ent ON atm.entid  = ent.entid
LEFT JOIN obras2.obras                          obr ON atm.obrid  = obr.obrid
LEFT JOIN workflow.documento 	            doc ON doc.docid  = atm.docid AND doc.tpdid = 196
LEFT JOIN workflow.estadodocumento              esd ON esd.esdid  = doc.esdid
INNER JOIN seguranca.usuario                    usu ON atm.usucpf_inclusao = usu.usucpf
WHERE  atm.atmstatus = 'A' %s;
";

// Envia um e-mail 7 dias antes do vencimento
// Tarefa
$tarefas = $db->carregar(sprintf($sqlTarefa, $where7DiasTarefa));
enviaEmail7DiasTarefa($tarefas);

// Atividade
$atividades = $db->carregar(sprintf($sqlAtividade, $where7DiasAtividade));
enviaEmail7DiasAtividade($atividades);


// Envia email diario após vencimento
$tarefas = $db->carregar(sprintf($sqlTarefa, $whereTarefaVencida));
enviaEmailTarefaVencida($tarefas);

$atividades = $db->carregar(sprintf($sqlAtividade, $whereAtividadeVencida));
enviaEmailAtividadeVencida($atividades);


function enviaEmail7DiasTarefa($tarefas)
{
    if(empty($tarefas))
        return;
    foreach ($tarefas as $tarefa) {
        $email = new Email();


        if(verificaEmailEnviadoTarefa($tarefa['itmid']))
            continue;


        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";

        global $db;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
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
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
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
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Tarefa a vencer.</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>A tafefa ' . $tarefa['itmnome'] . ' está se aproximento da data limite para sua conclusão.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Vencimento da tarefa ' . $tarefa['itmnome'],
            'temid' => 23,
            'emlregistroatividade' => 'false',
            'obrid' => null
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar(array($tarefa['usuemail']));
        $email->enviar();

        $data = array(
            'itmid' => $tarefa['itmid'],
            'emlid' => $email->emlid
        );

        $item = new EmailItemMonitoramento();
        $item->popularDadosObjeto($data);
        $item->salvar();
        $item->commit();
    }
}

function enviaEmail7DiasAtividade($atividades)
{
    if(empty($atividades))
        return;
    foreach ($atividades as $atividade) {
        $email = new Email();


        if(verificaEmailEnviadoAtividade ($atividade['atmid']))
            continue;


        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";

        global $db;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
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
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
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
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Atividade a vencer.</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>A atividade ' . $atividade['atmnome'] . ' está se aproximento da data limite para sua conclusão.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Vencimento da atividade ' . $atividade['atmnome'],
            'temid' => 22,
            'emlregistroatividade' => 'false',
            'obrid' => null
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar(array($atividade['usuemail']));
        $email->enviar();

        $data = array(
            'atmid' => $atividade['atmid'],
            'emlid' => $email->emlid
        );

        $atividade = new EmailAtividadeMonitoramento();
        $atividade->popularDadosObjeto($data);
        $atividade->salvar();
        $atividade->commit();
    }
}

function enviaEmailTarefaVencida($tarefas)
{
    if(empty($tarefas))
        return;
    foreach ($tarefas as $tarefa) {
        $email = new Email();

        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";

        global $db;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
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
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
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
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Tarefa a vencer.</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>A tafefa ' . $tarefa['itmnome'] . ' ultrapassou o prazo de sua conclusão.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Vencimento da tarefa ' . $tarefa['itmnome'],
            'temid' => 23,
            'emlregistroatividade' => 'false',
            'obrid' => null
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar(array($tarefa['usuemail'], 'fabio.cardoso@fnde.gov.br', 'fabricio.araujo@fnde.gov.br'));
        $email->enviar();

        $data = array(
            'itmid' => $tarefa['itmid'],
            'emlid' => $email->emlid
        );

        $item = new EmailItemMonitoramento();
        $item->popularDadosObjeto($data);
        $item->salvar();
        $item->commit();
    }
}

function enviaEmailAtividadeVencida($atividades)
{
    if(empty($atividades))
        return;
    foreach ($atividades as $atividade) {
        $email = new Email();

        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";

        global $db;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
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
                                                <p><img  src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
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
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Atividade a vencer.</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>A atividade ' . $atividade['atmnome'] . ' ultrapassou o prazo de sua conclusão.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Vencimento da atividade ' . $atividade['atmnome'],
            'temid' => 22,
            'emlregistroatividade' => 'false',
            'obrid' => null
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar(array($atividade['usuemail'], 'fabio.cardoso@fnde.gov.br', 'fabricio.araujo@fnde.gov.br'));
        $email->enviar();

        $data = array(
            'atmid' => $atividade['atmid'],
            'emlid' => $email->emlid
        );

        $atividade = new EmailAtividadeMonitoramento();
        $atividade->popularDadosObjeto($data);
        $atividade->salvar();
        $atividade->commit();
    }
}

function verificaEmailEnviadoTarefa($itmid)
{
    global $db;
    $sql = "SELECT COUNT(*) FROM obras2.emailitemmonitoramento WHERE itmid = $itmid AND eimdtinclusao <= now() + '7 days'";

    if($db->pegaUm($sql) > 0)
        return true;
    else
        return false;
}

function verificaEmailEnviadoAtividade($atmid)
{
    global $db;
    $sql = "SELECT COUNT(*) FROM obras2.emailatividademonitoramento WHERE atmid = $atmid AND eamdtinclusao <= now() + '7 days'";

    if($db->pegaUm($sql) > 0)
        return true;
    else
        return false;
}

