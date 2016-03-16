<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
include APPRAIZ . 'contratogestao/classes/models/Hierarquiacontrato.php';
include APPRAIZ . 'contratogestao/classes/models/Contrato.php';

$conid = (int)$_GET['conid'];

$hierarquiaContrato = new Model_Hierarquiacontrato();
$contratoModel = new Model_Contrato();

$contrato = $contratoModel->getContratoById($conid);

$whereAndIn = '';
$where = '';
$arrSubprocessos = array();

?>
    <!DOCTYPE HTML>
    <html>
    <head>
        <meta http-equiv="X-UA-Compatible" content="IE=9"/>
        <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
        <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

        <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
        <script language="javascript" type="text/javascript"
                src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
        <script language="javascript" type="text/javascript" src="pde/js/estrategico.js"></script>

        <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
        <script language="javascript" type="text/javascript"
                src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
        <script language="javascript" type="text/javascript"
                src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

        <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
        <script language="javascript" type="text/javascript"
                src="/library/jquery_totem/jquery.totemticker.min.js"></script>

        <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
        <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

        <script language="javascript" src="/estrutura/js/funcoes.js"></script>
        <script language="JavaScript" src="../includes/funcoes.js"></script>

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <link rel='stylesheet' type='text/css' href='/pde/css/cockpit.css'/>
        <script>

        </script>

        <style type="text/css">
            #div-ciclos {
                height: 900px;
            }

            #div-qtd {
                height: 550px;
            }

            .fundo_titulo {
                background-image: url('../imagens/fundobrasilpro.jpg')
            }

            ;
        </style>
    </head>
    <body>
    <table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
        <tr>
            <td class="titulo_pagina">
                <div style="cursor:pointer;"
                     onclick="window.location = 'contratogestao.php?modulo=principal/contratos&acao=C';">
                    <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"/>

                    <div style="float:left" class="titulo_box">SIMEC<br/><span class="subtitulo_box">Monitoramento Estratégico</span>
                    </div>
                </div>
                <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"
                     src="/pde/cockpit/images/voltar.png" style="vertical-align:middle;"/>

                <div style="float:right;cursor:pointer;"
                     onclick="window.location = 'contratogestao.php?modulo=relatorio/dashboard&acao=A';">
                    <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;"/>
                </div>
            </td>
        </tr>
    </table>
    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
        <tr>
            <td class="fundo_titulo" style="text-align:center" colspan="2">
                <div style="margin:28px">Contrato de Gestão</div>
            </td>
        </tr>
        <tr>
            <td class="fundo_padrao" colspan="2">
                <div>
                    <?php exibirTitulo('financeiro', 'Cronograma de Desembolso'); ?>
                </div>
                <?php
                $arrDados = $hierarquiaContrato->getNos();
                $hierarquiaContrato->sumTotalContrato($arrDados);
                ?>
                <table
                    class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                    <tr>
                        <th>Macroprocesso</th>
                        <th>Subprocesso</th>
                        <th>Data Inicial</th>
                        <th>Data Final</th>
                        <th>Total</th>
                    </tr>
                    <?php
                    if ($arrDados) {

                        $count = -1;
                        foreach ($arrDados as $subprocesso):
                            if ($subprocesso['hqcnivel'] == 1) {
                                $contrato = $subprocesso;
                            }
                            if ($subprocesso['hqcnivel'] == 2) {
                                $macroprocesso = $subprocesso;
                            }
                            if ($subprocesso['hqcnivel'] == 3) {
                                $count++;
                                ?>
                                <tr class="<?= ($count % 2) ? 'zebrado' : ''; ?>">
                                    <td><?= $macroprocesso['condescricao'] ?></td>
                                    <td><?= $subprocesso['condescricao'] ?></td>
                                    <td><?= formata_data($contrato['datainicial']); ?></td>
                                    <td><?= formata_data($contrato['datafinal'])  ?></td>
                                    <td><?= formata_valor($subprocesso['fatvalordesembolso']) ?></td>
                                </tr>
                            <?php } ?>
                        <?php endforeach;
                    }
                    ?>
                </table>
            </td>
        </tr>
        <?php

        if ($conid) {
            $where = ' AND (q.h).hqcnivel = 3';
            $arrSubprocessos = $hierarquiaContrato->getNosComPai($contrato['hqcid'], $where);
        }
        if ($arrSubprocessos) {

            $contaLinhas = 0;
            foreach ($arrSubprocessos as $sub) {
                if (($contaLinhas % 2) == 0)
                    echo "<tr>";
                $contaLinhas++;
                ?>
                <td class="fundo_padrao" width="50%"
                    colspan="<?= (($contaLinhas % 2) == 1 && count($arrSubprocessos) == $contaLinhas ? '2' : '1') ?>">
                    <div>
                        <?php exibirTitulo('indicador', 'Indicadores de Monitoramento - ' . $sub['descricao']); ?>
                    </div>
                    <?php
                    $sql = "SELECT con.consigla, con.condescricao, con.datainicial, con.datafinal, con.conmeta, COALESCE(resultado, 0) as resultado, '' AS status, desembolso
                            FROM contratogestao.contrato con
                            INNER JOIN contratogestao.hierarquiacontrato hqc1 ON hqc1.hqcid = con.hqcid AND hqc1.hqcnivel = 6
                            LEFT JOIN (
                                SELECT SUM(fatvalordesembolso) AS desembolso, AVG(COALESCE(cof.cofpeso,0) * COALESCE(tem.tempeso,0)) AS resultado, hqc.hqcidpai
                                FROM contratogestao.fatoravaliado fat
                                INNER JOIN contratogestao.contrato con ON con.conid = fat.conid
                                INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                                LEFT JOIN contratogestao.conformidade cof ON cof.cofid = fat.cofid
                                LEFT JOIN contratogestao.tempestividade tem ON tem.temid = fat.temid
				                WHERE fat.fatstatus = 'A'
                                GROUP BY hqc.hqcidpai
                            ) fator ON fator.hqcidpai = hqc1.hqcid
                            INNER JOIN contratogestao.hierarquiacontrato hqc2 ON hqc2.hqcid = hqc1.hqcidpai AND hqc2.hqcnivel = 5
                            INNER JOIN contratogestao.hierarquiacontrato hqc3 ON hqc3.hqcid = hqc2.hqcidpai AND hqc3.hqcnivel = 4
                            WHERE hqc3.hqcidpai = {$sub['hqcid']} AND con.constatus = 'A' AND hqc2.hqcstatus = 'A' AND hqc3.hqcstatus = 'A'
                            ORDER BY con.hqcid, hqc1.hqcordem";
                    $arrDados = $db->carregar($sql);
                    ?>
                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                        <tr>
                            <th>Código</th>
                            <th>Ação</th>
                            <th>Data Inicial</th>
                            <th>Data Final</th>
                            <th>Meta</th>
                            <th>Resultado</th>
                            <th>Status</th>
                            <th>Desembolso</th>
                        </tr>
                        <?php
                        if ($arrDados) {
                            $count = -1;
                            foreach ($arrDados as $dado) {
                                $count++;
                                ?>
                                <tr class="<?= ($count % 2) ? 'zebrado' : ''; ?>">
                                    <td><?= $dado['consigla'] ?></td>
                                    <td><?= $dado['condescricao'] ?></td>
                                    <td><?= $dado['datainicial'] ?></td>
                                    <td><?= $dado['datafinal'] ?></td>
                                    <td class="numero"><?= number_format($dado['conmeta'], 0, ",", ".") ?></td>
                                    <td class="numero"><?= number_format($dado['resultado'], 2, ",", ".") ?></td>
                                    <td class="center"><?=($dado['resultado'] < 3 ? '<img src="/imagens/0_inativo.png">' : ($dado['resultado'] >= 3 && $dado['resultado'] < 5 ? '<img src="/imagens/0_ativo.png">' : '<img src="/imagens/0_concluido.png">')) ?></td>
                                    <td class="numero">R$<?= number_format($dado['desembolso'], 2, ",", ".") ?></td>
                                </tr>
                            <?php
                            }
                        }
                        ?>
                    </table>
                </td>
                <?php
                if (($contaLinhas % 2) == 0)
                    echo "</tr>";
            }
        }
        ?>
    </table>
<?php /*?>
    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
        <tr>
            <td class="fundo_padrao" width="33%">
                <div>
                    <?php exibirTitulo('indicador', 'ENEM/2014'); ?>
                </div>
                <?php
                if ($strConIds) {
                    $whereAndIn = " AND con.conid in ({$strConIds}) ";
                }
                $sql = "SELECT acao, tipo, valor
                    FROM(
                        SELECT con.condescricao AS acao, 'Meta' AS tipo, con.conmeta AS valor, hqc.hqcordem
                        FROM contratogestao.contrato con
                        INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                        WHERE hqc.hqcnivel = 5
                        $whereAndIn
                    UNION ALL
                        SELECT con.condescricao AS acao, 'Apurado' AS tipo, 2 AS valor, hqc.hqcordem
                        FROM contratogestao.contrato con
                        INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                        WHERE hqc.hqcnivel = 5
                        $whereAndIn
                    ) AS FOO
                    ORDER BY hqcordem";

                $dados = $db->carregar($sql);
                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'tipo', 'valor');
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarra1', null, null, $formatoValores, null, "", 1000, 600, null, 300);
                ?>
            </td>
            <td class="fundo_padrao" width="34%">
                <div>
                    <?php exibirTitulo('indicador', 'REVALIDA/2014'); ?>
                </div>
                <?php
                if ($strConIds) {
                    $whereAndIn = " AND con.conid in ({$strConIds}) ";
                }
                $sql = "SELECT acao, tipo, valor
                    FROM(
                        SELECT con.condescricao AS acao, 'Meta' AS tipo, con.conmeta AS valor, hqc.hqcordem
                        FROM contratogestao.contrato con
                        INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                        WHERE hqc.hqcnivel = 5
                        $whereAndIn
                    UNION ALL
                        SELECT con.condescricao AS acao, 'Apurado' AS tipo, 2 AS valor, hqc.hqcordem
                        FROM contratogestao.contrato con
                        INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                        WHERE hqc.hqcnivel = 5
                        $whereAndIn
                    ) AS FOO
                    ORDER BY hqcordem";
                $dados = $db->carregar($sql);

                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarra2', null, null, $formatoValores, null, "", 1000, 600, null, 300);
                ?>
            </td>
            <td class="fundo_padrao" width="33%">
                <div>
                    <?php exibirTitulo('indicador', 'INDICADORES DE GESTÃO'); ?>
                </div>
                <?php
                if ($strConIds) {
                    $whereAndIn = " AND con.conid in ({$strConIds}) ";
                }

                $sql = "SELECT acao, tipo, valor
                    FROM(
                        SELECT con.condescricao AS acao, 'Meta' AS tipo, con.conmeta AS valor, hqc.hqcordem
                        FROM contratogestao.contrato con
                        INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                        WHERE hqc.hqcnivel = 5
                        $whereAndIn
                    UNION ALL
                        SELECT con.condescricao AS acao, 'Apurado' AS tipo, 2 AS valor, hqc.hqcordem
                        FROM contratogestao.contrato con
                        INNER JOIN contratogestao.hierarquiacontrato hqc ON hqc.hqcid = con.hqcid
                        WHERE hqc.hqcnivel = 5
                        $whereAndIn
                    ) AS FOO
                    ORDER BY hqcordem";

                $dados = $db->carregar($sql);
                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'tipo', 'valor');
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarra3', null, null, $formatoValores, null, "", 1000, 600, null, 300);
                ?>
            </td>
        </tr>
    </table>
 <?php */ ?>
    </body>
    </html>
<?php die; ?>