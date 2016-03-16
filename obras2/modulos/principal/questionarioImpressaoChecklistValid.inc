<?php
$obrid = isset($_SESSION['obras2']['checklistfnde']['obrid']) ? $_SESSION['obras2']['checklistfnde']['obrid'] : $_REQUEST['obrid'];
$ckfid = isset($_SESSION['obras2']['checklistfnde']['ckfid']) ? $_SESSION['obras2']['checklistfnde']['ckfid'] : $_REQUEST['ckfid'];
$qrpid = isset($_SESSION['obras2']['checklistfnde']['qrpid']) ? $_SESSION['obras2']['checklistfnde']['qrpid'] : $_REQUEST['qrpid'];

if (empty($obrid) && empty($qrpid) && ($_SESSION['obras2']['obrid'] != $obrid)) {
    echo "
        <script>
                alert('Sessão expirada.');
                window.close();
        </script>";
    die();
}

include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";

function analiseEngenharia($ckfid, $qrpid, $obrid) {

    global $db;

    $obra = new Obras();
    $dados = $obra->getDadosCabecalho($obrid);

    $tipo = $_SESSION['obras2']['checklistfnde']['tipo'];

    if ($tipo == 'adm') {
        $sub_titulo = 'Análise Administrativa - CGIMP / DIGAPE / FNDE';
    } else {
        $sub_titulo = 'Análise Técnica - CGIMP / DIGAPE / FNDE';
    }

    $arDataHora = explode(" ", date('Y-m-d H:i:s'));
    $arData = explode("-", $arDataHora[0]);

    switch ($arData[1]) {
        case '1':
            $mes = "Janeiro";
            break;
        case '2':
            $mes = "Fevereiro";
            break;
        case '3':
            $mes = "Março";
            break;
        case '4':
            $mes = "Abril";
            break;
        case '5':
            $mes = "Maio";
            break;
        case '6':
            $mes = "Junho";
            break;
        case '7':
            $mes = "Julho";
            break;
        case '8':
            $mes = "Agosto";
            break;
        case '9':
            $mes = "Setembro";
            break;
        case '10':
            $mes = "Outubro";
            break;
        case '11':
            $mes = "Novembro";
            break;
        case '12':
            $mes = "Dezembro";
            break;
    }
    
    ?>

    <div style="margin-left:30px;">
        <center>
            <img width="100" alt="" src="http://simec.mec.gov.br/imagens/brasao.gif"><br/>
            MINISTÉRIO DA EDUCAÇÃO<br/>
            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/> 
            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF<br/>
            Telefone: (61) 2022-4350 - E-mail: cgaap@fnde.gov.br<br/><br/> 
        </center>
        <div style="font-family: arial, sans-serif;">
            <p align="center"><b> Checklist - <?php echo $sub_titulo; ?></b></p><br/>
            <p align="right">Brasília, <?php echo $arData[2] ?> de <?php echo $mes ?> de <?php echo $arData[0] ?> </p>
            <br/>
            <style>

                table.tabelaQuestionarioIdentificacao {
                    border-left: 1px solid #000000;
                    border-right: 1px solid #000000;
                    border-top: 1px solid #000000;
                    border-bottom: 1px solid #000000;
                    /* background-color:#cccccc; */
                    font-size: 14px;	
                }

            </style>
            <table class="tabelaQuestionarioIdentificacao" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
                <?php echo "<tr>
                                <td class=\"SubTituloDireita\" width=\"20%\"><b>Tipo de ensino:</b></td>
                                <td>
                                        {$dados['orgdesc']}
                                </td>
                            </tr>
                            <tr>
                                <td class=\"SubTituloDireita\" width=\"20%\"><b>Empreendimento:</b></td>
                                <td>
                                        ({$dados['empid']}) {$dados['empdsc']}
                                </td>
                            </tr>
                            <tr>
                                <td class=\"SubTituloDireita\" width=\"20%\"><b>Município - UF:</b></td>
                                <td>
                                    {$dados['mundescricao']} - {$dados['estuf']}
                                </td>
                            </tr>
                            <tr>
                                <td class=\"SubTituloDireita\" width=\"20%\"><b>Obra:</b></td>
                                <td>
                                    ({$dados['obrid']}) {$dados['obrnome']}
                                </td>
                            </tr>
                     "; ?>
            </table>
        </div>
        <br/>
        <?php
        $obImprime = new QImpressao(array('cabecalho' => 'N', 'qrpid' => $qrpid));
        echo $obImprime->montaArvore();
        ?>
        <br/>
    </div>

    <?php
}

analiseEngenharia($ckfid, $qrpid, $obrid);

if ($_REQUEST['requisicao'] == 'gerarPdf') {

    $html = ob_get_contents();
    ob_clean();

    $content = http_build_query(array('conteudoHtml' => utf8_encode($html)));
    $context = stream_context_create(array('http' => array('method' => 'POST', 'content' => $content)));

    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=checklist_" . $ckfid);
    echo $contents;
    exit;
} else {

    if (!isset($_REQUEST['copia'])) {
        echo "<script>
                        function gerarPdf()
                        {
                            document.location.href = document.location.href+'&requisicao=gerarPdf';
                        }
                  </script>";
        echo '<center><input type="button" value="Gerar Pdf" onclick="gerarPdf()" /></center>';
    }
}
?>