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
include_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";

if(!isset($_GET['limit']))
    exit;

$email = new Email();
$data = new Data();
$data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

$conteudo = '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center; font-size: 12px;">
                                                <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style=" text-align:justify">
                                                <p style="text-align: right;">' . $data . '</p>
                                                <p>Assunto: <b>Informação sobre solicitação e deferimento de repasse de parcelas.</b></p>

                                                <p>Senhor(a) Prefeito(a),</p>

                                                <p>1. Informamos que o art. 10, da Resolução CD/FNDE nº 13, de 08 de junho de 2012, que estabelece os critérios de transferência de recursos para execução das obras no âmbito do PAC 2, foi alterado pela Resolução CD/FNDE nº 07, de 05 de agosto de 2015, passando a ter a seguinte redação:</p>

                                                <p><i>Art. 10º. Os recursos serão transferidos em parcelas, de acordo com a execução da obra, sendo a primeira no montante de até 15%, após inserção da ordem de serviço de início de execução da obra, no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - Simec, módulo Obras 2.0.</i></p>

                                                <p><i>Parágrafo único. As demais parcelas serão transferidas após a aferição da evolução física da obra, comprovada mediante o relatório de vistoria inserido no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - Simec, módulo Obras.2.0, e aprovado pela equipe técnica do FNDE.</i></p>

                                                <p>2. Em razão das novas regras de transferência de recursos aos municípios, estados e Distrito Federal, o FNDE, com escopo de aprimorar o processo de integração entre os entes federados e esta Autarquia, criou ferramenta no módulo Obras 2.0 (SIMEC), na qual ocorrerá a solicitação do desembolso, bem como o acompanhamento de todo o procedimento até a deliberação do pedido. Outrossim, é importante que o ente observe, desde já, as respectivas orientações  de modo a cumpri-las no transcorrer das etapas, até o repasse final do recurso. A propósito, segue, abaixo, as correspondentes orientações:</p>

                                                <p style="margin:0 0 0 40px">1º) A solicitação de liberação de parcelas passa a depender do preenchimento completo e obrigatório dos documentos solicitados nas abas "Contratação", "Cronograma", "Vistorias" e "Execução Orçamentária" do Simec - Obras 2.0 (Vide Anexo I).</p>

                                                <p style="margin:0 0 0 40px">2º) Caso a solicitação de liberação das parcelas comporte evolução física de obra inferior a 10% do percentual repassado na última liberação, deverá ser apresentada justificativa pelos Municípios, Estados e Distrito Federal (através de boletim de medição, verificação de saldo bancário, dentre outros);</p>

                                                <p style="margin:0 0 0 40px">3º) O cronograma de execução da obra deve estar atualizado em todas as suas etapas. (vide Anexo I).</p>

                                                <p style="margin:0 0 0 40px">4º) O boletim de medição dos serviços executados deve ser compatível com o percentual solicitado na liberação da parcela, podendo ser apresentado, nesse caso, o boletim de medição acumulada (vide Anexo I).</p>

                                                <p style="margin:0 0 0 40px">5º) Para que a solicitação de liberação de parcela seja submetida à análise dos técnicos do FNDE, todas obras pactuadas com os Municípios, Estados e Distrito Federal devem apresentar seus dados atualizados no Simec - Obras 2.0, ou seja, com vistorias inseridas há menos de 60 dias;</p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p style="margin:0 0 0 40px">6º) A existência de restrições na obra, sob a responsabilidade dos Municípios, Estados e Distrito Federal, enquanto não superadas, impede a liberação de parcelas para esta, salvo se providenciada sua correção e forem tramitadas para análise do técnico do FNDE;</p>

                                                <p style="margin:0 0 0 40px">7º) Se, durante a análise da solicitação de liberação de parcelas, forem cadastradas restrições na obra pactuada com o Município/Estado/DF solicitante, será, para esta, indeferido o pedido de repasse de recursos até que os problemas apontados sejam sanados;</p>

                                                <p style="margin:0 0 0 40px">8º) O Município/Estado/DF deverá aguardar a deliberação do pedido de desembolso para que novos pedidos sejam solicitados;</p>

                                                <p style="margin:0 0 0 40px">9º) O acompanhamento da solicitação de desembolso de parcelas será disponibilizado no SIMEC - OBRAS 2.0;</p>
                                                <p>
                                                        <br /><br /><br />Atenciosamente,
                                                </p>
                                                <p style="text-align: center;">
                                                        <img align="center" style="height:80px;" src="data:image/png;base64,' . $email->getAssinatura() . '" />
                                                        <br />
                                                        <b>Fabrício Batista de Araújo<b>
                                                        <br />
                                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                        <br />
                                                        CGIMP/DIRPE/FNDE/MEC
                                                </p>
                                             </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </body>
                        </html>
                                    ';
$dompdf = new DOMPDF();
$dompdf->load_html($conteudo);
$dompdf->render();

$pdfoutput = $dompdf->output();

$file = new FilesSimec(null, null, "obras2");
$file->setPasta('obras2');
$arqid = $file->setStream('conteudo_email', $pdfoutput, 'application/pdf', '.pdf', false, 'conteudo_email.pdf');


$sql = "SELECT obrid FROM obras2.obras WHERE obridpai IS NULL AND obrstatus = 'A' AND obrid NOT IN (SELECT obrid FROM obras2.registroatividade  WHERE rgadscsimplificada = 'E-mail enviado (Alerta de Solicitações de Desembolso)' AND rgadsccompleta = 'E-mail enviado (Alerta de Solicitações de Desembolso) para: Gestores e Fiscais') LIMIT {$_GET['limit']}";
$obras = $db->carregarColuna($sql);

foreach ($obras as $obrid) {
    registraAtividade($arqid, $obrid);
}
$db->commit();
var_dump($obras);



function registraAtividade($arqidConteudo, $obrid) {
    global $db;

    // Monta o arquivo com corpo
    $sql = "select * from obras2.tipoemail where temid = 44";
    $tipo = $db->pegaLinha($sql);

    $arDado = array();

    $arDado['arqid'] = $arqidConteudo;
    $arDado['obrid'] = $obrid;
    $arDado['rgaautomatica'] = 'true';
    $arDado['rgadscsimplificada'] = 'E-mail enviado (' . $tipo['temnome'] . ')';
    $arDado['rgadsccompleta'] = 'E-mail enviado (' . $tipo['temnome'] . ') para: Gestores e Fiscais';

    if (empty($arDado['arqid']))
        $arDado['arqid'] = 'NULL';

    $sql = "INSERT INTO obras2.registroatividade (arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                  {$arDado['arqid']},
                  {$arDado['obrid']},
                  {$arDado['rgaautomatica']},
                  '{$arDado['rgadscsimplificada']}',
                  '{$arDado['rgadsccompleta']}'
                  )";

    $db->executar($sql);
}