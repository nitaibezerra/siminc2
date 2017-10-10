<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_emi1'])){
    gerarGraficoEMI1($_REQUEST['detalhe_emi1']);
    die;
}
if(isset($_REQUEST['detalhe_emi2'])){
    gerarGraficoEMI2($_REQUEST['detalhe_emi2']);
    die;
}
if(isset($_REQUEST['detalhe_emi3'])){
    gerarGraficoEMI3($_REQUEST['detalhe_emi3']);
    die;
}
if(isset($_REQUEST['detalhe_emi4'])){
    gerarGraficoEMI4($_REQUEST['detalhe_emi4']);
    die;
}
if(isset($_REQUEST['detalhe_emi5'])){
    gerarGraficoEMI5($_REQUEST['detalhe_emi5']);
    die;
}
if(isset($_REQUEST['detalhe_emi6'])){
    gerarGraficoEMI6($_REQUEST['detalhe_emi6']);
    die;
}
if(isset($_REQUEST['detalhe_emi7'])){
    gerarGraficoEMI7($_REQUEST['detalhe_emi7']);
    die;
}
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

        jQuery(function(){
            jQuery('.filtrar_emi1').click(function(){
                jQuery('#div_emi1').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi1='+jQuery(this).val());
            });
            jQuery('.filtrar_emi2').click(function(){
                jQuery('#div_emi2').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi2='+jQuery(this).val());
            });
            jQuery('.filtrar_emi3').click(function(){
                jQuery('#div_emi3').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi3='+jQuery(this).val());
            });
            jQuery('.filtrar_emi4').click(function(){
                jQuery('#div_emi4').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi4='+jQuery(this).val());
            });
            jQuery('.filtrar_emi5').click(function(){
                jQuery('#div_emi5').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi5='+jQuery(this).val());
            });
            jQuery('.filtrar_emi6').click(function(){
                jQuery('#div_emi6').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi6='+jQuery(this).val());
            });
            jQuery('.filtrar_emi7').click(function(){
                jQuery('#div_emi7').load('estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A&detalhe_emi7='+jQuery(this).val());
            });
        });
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_enem.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
    </style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
                <div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Ensino Médio</div></td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="33%">
            <div class="link" onclick="abreIndicadorPopUp(442);">
                <?php exibirTitulo('indicador', 'Matrículas no Ensino Médio - Total', 'Inclui matrículas no ensino médio Normal/Magistério e<br>ensino médio integrado'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi1" name="emi1" value="1" checked="checked" onclick="" />Total
                <input type="radio" class="filtrar_emi1" name="emi1" value="2" onclick="" />Público (Estadual + Municipal + Federal)
                <input type="radio" class="filtrar_emi1" name="emi1" value="3" onclick="" />Privada
            </div>
            <br>
            <div id="div_emi1">
                <?php gerarGraficoEMI1(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="34%">
            <div class="link" onclick="abreIndicadorPopUp(703);">
                <?php exibirTitulo('indicador', 'Matrículas de Ensino Médio na zona rural', 'Inclui matrículas no ensino médio Normal/Magistério e<br>ensino médio integrado'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi2" name="emi2" value="1" checked="checked" onclick="" />Total
                <input type="radio" class="filtrar_emi2" name="emi2" value="2" onclick="" />Público (Estadual + Municipal + Federal)
                <input type="radio" class="filtrar_emi2" name="emi2" value="3" onclick="" />Privada
            </div>
            <br>
            <div id="div_emi2">
                <?php gerarGraficoEMI2(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Número de Estabelecimentos<br>Públicos de Ensino Médio', '<span class="link" onclick="abreIndicadorPopUp(20);">Ensino Médio Regular</span>, <span class="link" onclick="abreIndicadorPopUp(21);">Ensino Médio Integrado</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi3" name="emi3" value="1" checked="checked" onclick="" />Total
                <input type="radio" class="filtrar_emi3" name="emi3" value="2" onclick="" />Ensino Médio Regular
                <input type="radio" class="filtrar_emi3" name="emi3" value="3" onclick="" />Ensino Médio Integrado
            </div>
            <br>
            <div id="div_emi3">
                <?php gerarGraficoEMI3(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(609);">
            <div>
                <?php exibirTitulo('indicador', 'IDEB Ensino Médio'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (609)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                foreach($arrDados as $dado){
                    $arrX[] = Round($dado['periodo']);
                    $arrA[] = Round($dado['valor'],2);
                }

                $aDados = array(
                    array('name' => 'Indice', 'data'=>$arrA),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaIDEBEnsinoMedio", "IDEB Ensino Médio",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'ENEM - 2014', '<span class="link" onclick="abreIndicadorPopUp(271);">Inscritos</span>, <span class="link" onclick="abreIndicadorPopUp(27);">Realizaram a prova</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dsh.dshuf AS uf,
                        CASE sh.indid
                            WHEN 271 THEN 'Inscritos'
                            WHEN 27 THEN 'Realizaram a prova'
                        END AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (27, 271)
                    AND sh.sehstatus <> 'I'
                    --AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND dpe.dpeanoref = '2014'
                    GROUP BY uf, situacao
                    ORDER BY uf, situacao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['situacao']] += $dado['valor'];
                }

                $dadosAgrupados = agruparDadosGrafico($arrDados, 'uf', 'situacao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaENEM', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);

                echo "<table align='center'>";
                if(number_format($arrSituacao['Inscritos'],0,',','.') > 0){
                    echo "  <tr>";
                    echo "      <td class='titulo_box bold'>Total de Inscritos: ".number_format($arrSituacao['Inscritos'],0,',','.')."</td>";
                    echo "  </tr>";
                }
                if(number_format($arrSituacao['Realizaram a prova'],0,',','.') > 0){
                    echo "  <tr>";
                    echo "      <td class='titulo_box bold'>Total que Realizaram a Prova: ".number_format($arrSituacao['Realizaram a prova'],0,',','.')."</td>";
                    echo "  </tr>";
                }
                echo "</table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3114);">
            <div>
                <?php exibirTitulo('indicador', 'Programa Ensino Médio Inovador', 'Escolas participantes'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (3114)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                foreach($arrDados as $dado){
                    $arrX[] = Round($dado['periodo']);
                    $arrA[] = Round($dado['valor']);
                }

                $aDados = array(
                    array('name' => 'Escolas', 'data'=>$arrA),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaProEMI1", "Escolas participantes",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(664);">
            <div>
                <?php exibirTitulo('indicador', 'Programa Ensino Médio Inovador', 'Alunos beneficiados pelo PDDE ProEMI'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (664)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                foreach($arrDados as $dado){
                    $arrX[] = Round($dado['periodo']);
                    $arrA[] = Round($dado['valor']);
                }

                $aDados = array(
                    array('name' => 'Alunos', 'data'=>$arrA),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaProEMI2", "Alunos beneficiados pelo PDDE ProEMI",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(661);">
            <div>
                <?php exibirTitulo('indicador', 'Programa Ensino Médio Inovador', 'Recursos repassados via PDDE'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (661)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                foreach($arrDados as $dado){
                    $arrX[] = Round($dado['periodo']);
                    $arrA[] = Round($dado['valor']);
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaProEMI3", "Recursos repassados via PDDE",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'Pacto pelo Fortalecimento do Ensino Médio', '<span class="link" onclick="abreIndicadorPopUp(3106);">Professores inscritos</span>, <span class="link" onclick="abreIndicadorPopUp(3107);">Equipe pedagógica</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi4" name="emi4" value="1" checked="checked" onclick="" />Professores inscritos
                <input type="radio" class="filtrar_emi4" name="emi4" value="2" onclick="" />Equipe pedagógica
            </div>
            <br>
            <div id="div_emi4">
                <?php gerarGraficoEMI4(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3106);" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Pacto pelo Fortalecimento do Ensino Médio', 'Professores inscritos por Estado'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dsh.dshuf AS uf,
                        tid1.tiddsc AS tipo,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid IN (3106)
                    AND sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    GROUP BY uf, tipo
                    ORDER BY uf, tipo";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                $totalAgrupado = "";
                foreach ($arrDados as $dado){
                    $totalAgrupado += $dado['valor'];
                }
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'uf', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaPactoEMI', null, null, $formatoValores, null, "", 1000, 600, null, 300, true);

                echo "<table align='center'>";
                echo "  <tr>";
                echo "      <td class='titulo_box bold'>Total: ".number_format($totalAgrupado,0,',','.')."</td>";
                echo "  </tr>";
                echo "</table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3106);">
            <div>
                <?php exibirTitulo('indicador', 'Pacto pelo Fortalecimento do Ensino Médio', 'Professores Inscritos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS descricao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid IN (3106)
                    AND sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaPactoEMI", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true,"",false,'#fff',true);
            }
            ?>
        </td>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(3108);">
                <?php exibirTitulo('financeiro', 'Bolsas do Pacto'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi7" name="emi7" value="1" checked="checked" onclick="" />Bolsistas
                <input type="radio" class="filtrar_emi7" name="emi7" value="2" onclick="" />R$
            </div>
            <br>
            <div id="div_emi7">
                <?php gerarGraficoEMI7(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" rowspan="2">
            <div>
                <?php exibirTitulo('indicador', 'PNAE', '<span class="link" onclick="abreIndicadorPopUp(296);">Alunos</span>, <span class="link" onclick="abreIndicadorPopUp(680);">R$</span>, <span class="link" onclick="abreIndicadorPopUp(679);">Escolas</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi5" name="emi5" value="1" checked="checked" onclick="" />Alunos
                <input type="radio" class="filtrar_emi5" name="emi5" value="2" onclick="" />R$
                <input type="radio" class="filtrar_emi5" name="emi5" value="3" onclick="" />Escolas
            </div>
            <br>
            <div id="div_emi5">
                <?php gerarGraficoEMI5(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" rowspan="2">
            <div class="link" onclick="abreIndicadorPopUp(677);">
                <?php exibirTitulo('indicador', 'PNATE'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_emi6" name="emi6" value="1" checked="checked" onclick="" />Alunos
                <input type="radio" class="filtrar_emi6" name="emi6" value="2" onclick="" />R$
            </div>
            <br>
            <div id="div_emi6">
                <?php gerarGraficoEMI6(1); ?>
            </div>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,104,'','');" align="center" width="30%">
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(104);?>
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
<?php
function gerarGraficoEMI1($detalhe){

    global $db;

    if($detalhe==2){
        $where = "AND dsh.tidid2 IN (1683,1684,1685)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid2 IN (1686)";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (442)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (1689, 1688, 1687)
            AND dpe.dpeanoref >= '2003'
            $where
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $arrX = array();
        $arrA = array();
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
        }

        $aDados = array(
            array('name' => 'Matriculas', 'data'=>$arrA),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoCensoEMI", "Matrículas no Ensino Médio - Total",300, $formatoValores);
    }
}
function gerarGraficoEMI2($detalhe){

    global $db;

    if($detalhe==2){
        $where = "AND dsh.tidid2 IN (2672,2673,2674)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid2 IN (2675)";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (703)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (2669, 2670, 2671)
            $where
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $arrX = array();
        $arrA = array();
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
        }

        $aDados = array(
            array('name' => 'Matriculas', 'data'=>$arrA),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoCensoEMIRural", "Matrículas de Ensino Médio na zona rural",300, $formatoValores);
    }

}
function gerarGraficoEMI3($detalhe){

    global $db;

    if($detalhe==1){
        $where = "WHERE sh.indid in (20,21)";
    }elseif($detalhe==2){
        $where = "WHERE sh.indid in (20)";
    }elseif($detalhe==3){
        $where = "WHERE sh.indid in (21)";
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $arrX = array();
        $arrA = array();
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
        }

        $aDados = array(
            array('name' => 'Escolas', 'data'=>$arrA),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEPEnsinoMedio", "Número de Estabelecimentos Públicos de Ensino Médio",300, $formatoValores);
    }
}
function gerarGraficoEMI4($detalhe){

    global $db;

    if($detalhe==1){
        $where = "WHERE sh.indid in (3106)";
        $tipo = "Professores";
    }else{
        $where = "WHERE sh.indid in (3107) AND dsh.tidid1 NOT IN (8239, 8626)";
        $tipo = "Pessoas";
    }

    $sql = "SELECT
                public.removeacento(Replace(dpe.dpedsc, '/', '<br>')) AS periodo,
                SUM(dsh.dshqtde)::integer AS valor,
                dpeanoref||dpemesref AS ordem
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            GROUP BY periodo, ordem
            ORDER BY ordem";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $arrX = array();
        $arrA = array();
        foreach($arrDados as $dado){
            $arrX[] = $dado['periodo'];
            $arrA[] = Round($dado['valor']);
        }

        $aDados = array(
            array('name' => $tipo, 'data'=>$arrA),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaPactoFortalecimentoEM", "Pacto pelo Fortalecimento do Ensino Médio",300, $formatoValores);
    }

}
function gerarGraficoEMI5($detalhe){

    global $db;

    if($detalhe==1){
        $where = 296;
        $tipo = "Alunos";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }",
            'rotation'=> "formatter: function() { return '-45'; }",
        );
    }elseif($detalhe==2){
        $where = 680;
        $tipo = "Reais";
        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }",
            'rotation'=> "formatter: function() { return '-45'; }",
        );
    }else{
        $where = 679;
        $tipo = "Escolas";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }",
            'rotation'=> "formatter: function() { return '-45'; }",
        );
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in ($where)
            AND sh.sehstatus <> 'I'
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $vTotal = 0;
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
            $vTotal += Round($dado['valor']);
            $arrTotal[] = $vTotal;
        }

        if($detalhe==2){
            $aDados = array(
                array('name' => $tipo, 'data'=>$arrA),
                array('name' => 'Total', 'data'=>$arrTotal),
            );
        }else{
            $aDados = array(
                array('name' => $tipo, 'data'=>$arrA),
            );
        }

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaPNAE", "PNAE",300, $formatoValores);
    }

}
function gerarGraficoEMI6($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
        $tipo = "Alunos";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );
    }else{
        $campo = "SUM(dsh.dshvalor) AS valor";
        $tipo = "Reais";
        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );
    }

    $sql = "SELECT dpe.dpeanoref AS periodo, $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (677)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (2470)
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $arrX = array();
        $arrA = array();
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
        }

        $aDados = array(
            array('name' => $tipo, 'data'=>$arrA),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaPNATE", "PNATE",300, $formatoValores);
    }
}
function gerarGraficoEMI7($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );
    }else{
        $campo = "SUM(dsh.dshvalor) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );
    }

    $sql = "SELECT
                public.removeacento(Replace(dpe.dpedsc, '/', '<br>')) AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (8308) THEN 1 --Professores
                ELSE 2 --Equipe Pedagógica
                END AS acao,
                $campo,
                dpeanoref||dpemesref AS ordem
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (3108)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, periodo, ordem
            ORDER BY ordem";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $arrSituacao = array();
        foreach($arrDados as $dado){
            $arrSituacao[$dado['periodo']][$dado['acao']]['total'][] = $dado['valor'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();

        foreach($arrSituacao as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
            $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
            $arrA[] = $vA;
            $arrB[] = $vB;
        }

        $aDados = array(
            array('name' => 'Professores bolsistas', 'data'=>$arrA),
            array('name' => 'Equipe pedagogica', 'data'=>$arrB),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoBolsasPacto", "Bolsas do Pacto",300, $formatoValores);
    }
}
?>