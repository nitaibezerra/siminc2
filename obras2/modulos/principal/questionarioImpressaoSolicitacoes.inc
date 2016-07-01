<?php
$obrid = $_REQUEST['obrid'];
$slcid = $_REQUEST['slcid'];
$qrpid = $_REQUEST['qrpid'];

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

function analiseEngenharia($qrpid, $obrid, $slcid = null) {

    global $db;

    $obra = new Obras();
    $dados = $obra->getDadosCabecalho($obrid);
    $query = <<<DML
        SELECT com.cmddsc AS observacao, aca.aeddscrealizada AS acao, usu.usunome AS nome, TO_CHAR(hst.htddata,'DD/MM/YYYY') AS data, tp.tslnome
        FROM obras2.solicitacao sol
        INNER JOIN obras2.tiposolicitacao_solicitacao tps ON (sol.slcid = tps.slcid)
        INNER JOIN obras2.tiposolicitacao tp ON (tps.tslid = tp.tslid)
        INNER JOIN workflow.documento doc ON sol.docid = doc.docid
        INNER JOIN workflow.historicodocumento hst ON doc.docid = hst.docid
        INNER JOIN workflow.acaoestadodoc aca ON hst.aedid = aca.aedid
        LEFT  JOIN workflow.comentariodocumento com ON hst.hstid = com.hstid
        INNER JOIN seguranca.usuario usu on hst.usucpf = usu.usucpf
        WHERE sol.slcid = {$slcid}
            AND aca.esdiddestino in (1572,1573,1574)
        ORDER BY hst.hstid DESC
        LIMIT 1;
DML;
    $dadosSolicitacao = $db->pegaLinha($query);

    $query = <<<DML
        SELECT e.entnome AS prefeito, mun.estuf AS uf, mun.mundescricao AS estado
        FROM par.entidade  e
        INNER JOIN par.dadosunidadetipo dt ON dt.dutid = e.dutid
        INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
        INNER JOIN territorios.municipio mun ON iu.muncod = mun.muncod
        WHERE entstatus = 'A'
            AND iu.muncod = (SELECT m.muncod FROM obras2.obras o INNER JOIN entidade.endereco e ON e.endid = o.endid INNER JOIN territorios.municipio m ON m.muncod = e.muncod WHERE o.obrid = {$obrid})
        	AND dt.dutid = 7;
DML;
    $dadosPrefeitura = $db->pegaLinha($query);

    $sub_titulo = 'Análise de Engenharia - CGIMP/DIGAPE/FNDE';
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
            E-mail: obras@fnde.gov.br<br/><br/>
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
            <p>
                Senhor(a) Prefeito(a) <?=$dadosPrefeitura['prefeito']?> da prefeitura de <?=$dadosPrefeitura['estado'] . ' - ' . $dadosPrefeitura['uf'] ?>.
            </p>
            <table class="tabelaQuestionarioIdentificacao" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Tipo de ensino:</b></td>
                    <td><?=$dados['orgdesc']?></td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\" width=\"20%\"><b>Empreendimento:</b></td>
                    <td><?='('.$dados['empid'].') '.$dados['empdsc']?></td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\" width=\"20%\"><b>Município - UF:</b></td>
                    <td><?=$dados['mundescricao'].' - '.$dados['estuf']?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Obra:</b></td>
                    <td><?='('.$dados['obrid'].') '.$dados['obrnome']?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>ID Solicitação:</b></td>
                    <td><?=$slcid?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Tipo Solicitação:</b></td>
                    <td><?=$dadosSolicitacao['tslnome']?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Técnico:</b></td>
                    <td><?=$dadosSolicitacao['nome']?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Coordenador:</b></td>
                    <td>FÁBIO LÚCIO DE A. CARDOSO</td>
                </tr>
            </table>
        </div>
        <br/>
        <div id='questionarioImpressaoSolicitacoes'>
        <?php
        $obImprime = new QImpressao(array('cabecalho' => 'N', 'qrpid' => $qrpid));
        if($qrpid != 'null'){
            echo $obImprime->montaArvore();
        }else{
            echo <<<HTML
                <style>
                    .divPergunta {
                        font-family:verdana, arial, sans-serif;
                        font-size: 11px;
                        background-color: #f1f1f1;
                        margin-top:5px;
                        padding-bottom:5px;
                        border:1px solid #cccccc;
                    }
                    .divTituloPergunta {
                        background-color: #dddddd;
                        padding-top:3px;
                        padding-bottom:5px;
                        border-bottom:1px solid #cccccc;
                    }
                   .divResposta {
                        font-family: verdana, arial, sans-serif;
                        font-size: 11px;
                        background-color: #f1f1f1;
                        padding-top: 3px;
                        padding-left: 10px;
                    }
                </style>
                <div class="divPergunta"><div class="divTituloPergunta">observações</div>{$dadosSolicitacao['observacao']}</div>
HTML;
        }
        ?>
        </div>
        <div>
            <p>
                <center>
                    <?
                    $img = base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png'));
                    ?>                    
                    <img src="data:image/png;base64,<?=$img?>" alt='Assinatura' width="120px" height="80px"> <br>
                    FÁBIO LÚCIO DE A. CARDOSO <br>
                    Coordenador-Geral de Infraestrutura Educacional - CGEST <br>
                    Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP <br>
                    Fundo Nacional de Desenvolvimento de Educação - FNDE
                </center>
            </p>
        </div>
        <script>
            document.getElementsByClassName('divTituloQuestionario')[0].style.display = 'none';
            <? if($qrpid != 'null'): ?>
            var referenceNode = document.getElementsByClassName('divTituloQuestionario')[0];
            var newNode = document.createElement('div');
            newNode.innerHTML = '<div class="divPergunta"><div class="divTituloPergunta">observações</div><div class="divResposta"><?=$dadosSolicitacao['observacao']?></div></div>';
            referenceNode.parentNode.insertBefore(newNode, referenceNode.nextSibling);
            <? endif; ?>
        </script>
        <br/>
    </div>

    <?php
}

analiseEngenharia($qrpid, $obrid, $slcid);

if ($_REQUEST['requisicao'] == 'gerarPdf') {

    $html = ob_get_contents();
    ob_clean();

    $content = http_build_query(array('conteudoHtml' => utf8_encode($html)));
    $context = stream_context_create(array('http' => array('method' => 'POST', 'content' => $content)));

    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=checklist_solicitacao_" . $slcid.".pdf");
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