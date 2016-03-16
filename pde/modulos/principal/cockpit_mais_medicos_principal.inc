<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A');
?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=9" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="JavaScript" src="../includes/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <script>
        atualizaUsuario();
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('cockpit/images/banner/bannerMaisMedicos.jpg');}
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
                <div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_medicos_principal&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Mais Médicos</div></td>
    </tr>
    <tr>
        <td width="33%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', '<font size="+3">Projeto Mais Médicos<br>para o Brasil</font>'); ?>
            </div>
        </td>
        <td width="34%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', '<font size="+3">Graduação em Medicina</font>'); ?>
            </div>
        </td>
        <td width="33%" class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_residencias&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', '<font size="+3">Residências</font>'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2792);">
            <div>
                <?php exibirTitulo('alvo', 'Médicos'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2792)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMeta = $db->pegaUm( $sql );
            $valorMeta = 14000;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="60%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> médicos ativos
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> médicos previstos (até 2016)</font>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" rowspan="2">
            <div>
                <?php exibirTitulo('alvo', 'Vagas'); ?>
            </div>
            <?php
            $sql = "SELECT sh.indid, COALESCE(dsh.tidid1,0) AS cod, sum(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2823, 2889, 2678)
                    AND sh.sehstatus <> 'I'
                    GROUP BY sh.indid, dsh.tidid1";
            $rsGraduacao = $db->carregar($sql, null, 3200);
            if($rsGraduacao){
                foreach($rsGraduacao as $g){
                    $arInd[$g['indid']][$g['cod']] = $g['total'];
                }
            }

            $totalGraduacao = $arInd['2678']['7263'] + $arInd['2678']['7262'] + $arInd['2823']['7588'] + $arInd['2823']['7589'] + $arInd['2889']['0'];
            $totalGraduacaoPublica = $arInd['2678']['7263'] + $arInd['2678']['7262'];
            $totalGraduacaoPrivada = $arInd['2823']['7588'] + $arInd['2823']['7589'] + $arInd['2889']['0'];
            $valorMeta = 11447;
            $valorMetaPublica = 3615;
            $valorMetaPrivada = 7832;
            $porcentagem = number_format(($totalGraduacao*100)/$valorMeta,0,",",".");
            $porcentagemPublica = number_format(($totalGraduacaoPublica*100)/$valorMetaPublica,0,",",".");
            $porcentagemPrivada = number_format(($totalGraduacaoPrivada*100)/$valorMetaPrivada,0,",",".");

            $titlePublica = '<table><tr><td>Cursos novos</td>';
            $titlePublica .= '<td>'.number_format($arInd['2678']['7262'],0,",",".").'</td>';
            $titlePublica .= '</tr><tr><td>Aumento de vagas</td>';
            $titlePublica .= '<td>'.number_format($arInd['2678']['7263'],0,",",".").'</td>';
            $titlePublica .= '</tr></table>';

            $titlePrivada = '<table><tr><td>Cursos novos</td>';
            $titlePrivada .= '<td>'.number_format($arInd['2823']['7589'],0,",",".").'</td>';
            $titlePrivada .= '</tr><tr><td>Aumento de vagas</td>';
            $titlePrivada .= '<td>'.number_format($arInd['2823']['7588'],0,",",".").'</td>';
            $titlePrivada .= '</tr><tr><td>Reativadas</td>';
            $titlePrivada .= '<td>'.number_format($arInd['2889']['0'],0,",",".").'</td>';
            $titlePrivada .= '</tr></table>';
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalGraduacao,0,",",".")?> vagas criadas
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> vagas (até 2017)</font>
                    </td>
                </tr>
            </table>
            <br>
            <br>

            <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
            <script type="text/javascript" src="../includes/remedial.js"></script>
            <script type="text/javascript" src="../includes/superTitle.js"></script>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" border="1" align="center" >
                <tr>
                    <td class="titulo_box bold center">Federais</td>
                    <td class="titulo_box bold center">Privadas</td>
                </tr>
                <tr>
                    <td>
                        <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                            <tr>
                                <td class="titulo_box bold">
                                    <?=number_format($totalGraduacaoPublica,0,",",".")?> vagas criadas
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <div onmousemove="SuperTitleOn(this, '<?php echo $titlePublica; ?>' );" onmouseout="SuperTitleOff( this );" style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                                        <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagemPublica) ?>%;">
                                            <?= $porcentagemPublica ?>%
                                        </div>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <font size=-1><?=number_format($valorMetaPublica,0,",",".")?> vagas (até 2017)</font>
                                </td>
                            </tr>
                        </table>
                     </td>
                    <td>
                        <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                            <tr>
                                <td class="titulo_box bold">
                                    <?=number_format($totalGraduacaoPrivada,0,",",".")?> vagas criadas
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <div onmousemove="SuperTitleOn(this, '<?php echo $titlePrivada; ?>' );" onmouseout="SuperTitleOff( this );" style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                                        <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagemPrivada) ?>%;">
                                            <?= $porcentagemPrivada ?>%
                                        </div>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <font size=-1><?=number_format($valorMetaPrivada,0,",",".")?> vagas (até 2017)</font>
                                </td>
                            </tr>
                        </table>
                    </td>
                 </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('alvo', 'Vagas Criadas para Medicina Geral<br>de Família e Comunidade'); ?>
            </div>
            <?php
            $sql = "";
            //$totalMeta = $db->pegaUm( $sql );
            $totalMeta = 0;
            $valorMeta = 16500;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> vagas criadas
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> vagas (até 2018)</font>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2293);">
            <div>
                <?php exibirTitulo('alvo', 'Supervisores'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2293)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMeta = $db->pegaUm( $sql );
            $valorMeta = 1750;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> supervisores incorporados
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> supervisores previstos (até 2016)</font>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('alvo', 'Vagas criadas para o Mais Especialidades'); ?>
            </div>
            <?php
            $sql = "";
            //$totalMeta = $db->pegaUm( $sql );
            $totalMeta = 0;
            $valorMeta = 4000;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> vagas criadas
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> vagas (até 2021)</font>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2292);">
            <div>
                <?php exibirTitulo('alvo', 'Tutores'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2292)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMeta = $db->pegaUm( $sql );
            $valorMeta = 175;
            $porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalMeta,0,",",".")?> tutores incorporados
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> tutores previstos (até 2016)</font>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" rowspan="2">
            <div>
                <?php exibirTitulo('executiverel', 'Vagas de graduação criadas', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <?php
            $arrEtapa = array();
            $sql = "SELECT ano, tipo, sum(valor) AS total
                    FROM (
                        SELECT
                            dpe.dpeanoref::INTEGER AS ano,
                            'Vagas criadas' AS tipo,
                            sum(dsh.dshqtde) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        AND sh.indid IN (2823, 2678, 2889)
                        AND sh.sehstatus <> 'I'
                        GROUP BY ano, tipo
                    UNION ALL
                        SELECT 2013 AS ano, 'Metas' AS tipo, 1452 AS valor
                    UNION ALL
                        SELECT 2014 AS ano, 'Metas' AS tipo, 5435 AS valor
                    UNION ALL
                        SELECT 2015 AS ano, 'Metas' AS tipo, 1940 AS valor
                    UNION ALL
                        SELECT 2016 AS ano, 'Metas' AS tipo, 2260 AS valor
                    UNION ALL
                        SELECT 2017 AS ano, 'Metas' AS tipo, 360 AS valor
                    ) AS foo
                    GROUP BY ano, tipo
                    ORDER BY ano, tipo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['ano']][$dado['tipo']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $somavA = 0;
                $somavB = 0;

                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['Metas']['total'])?array_sum($sit['Metas']['total']):0));
                    $vB = Round((is_array($sit['Vagas criadas']['total'])?array_sum($sit['Vagas criadas']['total']):0));
                    $somavA += $vA;
                    $somavB += $vB;

                    $arrA[] = $somavA;
                    if($vB > 0){
                        $arrB[] = $somavB;
                    }
                }

                $aDados = array(
                    array('name' => 'Metas', 'data'=>$arrA, 'color'=>'red'),
                    array('name' => 'Vagas criadas', 'data'=>$arrB, 'color'=>'blue'),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoVagas", "Vagas de graduação criadas",300,$formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);" rowspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Medicina Geral de Família e Comunidade'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('casas', 'Instituições Supervisoras'); ?>
            </div>
            <?php
            $sql = "SELECT sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2291)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalIS = $db->pegaUm( $sql );

            $sql = "SELECT count(distinct dshcodmunicipio) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2792)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND sh.sehstatus <> 'I'";
            $totalMun = $db->pegaUm( $sql );
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <span class="link" onclick="abreIndicadorPopUp(2291);"><?=number_format($totalIS,0,",",".")?> instituições supervisoras</span> atuando em <span class="link" onclick="abreIndicadorPopUp(2792);"><?=number_format($totalMun,0,",",".")?> municípios</span>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae', '3', '166', '', '');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'Vagas criadas, por cursos novos<br>ou existentes', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Privadas</span>'); ?>
            </div>
            <?
            $sql = "SELECT
                        CASE
                            WHEN dsh.tidid1 IN (7262, 7589) THEN 'Cursos novos'
                            WHEN dsh.tidid1 IN (7263, 7588) THEN 'Cursos existentes'
                        END AS descricao,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid in (2678, 2823)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaVagas", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Mais Especialidades'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_td_laranja" colspan="3">
            <div style="text-align:center;" >
                <img src="../imagens/icones/icons/executiverel.png" style="vertical-align:middle;" />
                <input type="text" onclick="this.style.color='#000000';this.value='';" name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;' title='' class=' normal' />
                <img src="../imagens/icones/icons/Find.png" style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);" />
            </div>
        </td>
    </tr>
</table>
</body>
</html>