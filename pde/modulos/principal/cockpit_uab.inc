<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
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
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo1_bolsa_formacao.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
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
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_uab&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
<tr>
    <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Universidade Aberta do Brasil - UAB</div></td>
</tr>
<tr>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1531);" width="33%">
        <div>
            <?php exibirTitulo('indicador', 'Polo de apoio presencial<br>por situação'); ?>
        </div>
        <?
        $sql = "SELECT
                    tid1.tiddsc AS descricao,
                    SUM(dsh.dshqtde::integer) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                WHERE sh.indid IN (1531)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                AND sh.sehstatus <> 'I'
                GROUP BY descricao
                ORDER BY descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        geraGrafico($arrDados, "graficoPizzaPoloSituacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
        ?>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(547);" width="34%">
        <div>
            <?php exibirTitulo('indicador', 'Matrículas ativas'); ?>
        </div>
        <?php
        $sql = "SELECT dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN
                    (
                        SELECT MAX(s.dpeid) as dpeid, dpeanoref
                        FROM painel.seriehistorica s
                        INNER JOIN painel.detalheperiodicidade d on d.dpeid = s.dpeid
                        WHERE s.indid = 547 AND s.sehstatus <> 'I' GROUP BY dpeanoref) a ON a.dpeid = sh.dpeid
                WHERE sh.indid in (547)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
                ORDER BY periodo";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        $total = 0;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrX[] = $dado['periodo'];
                $arrA[] = Round($dado['valor']);
                $total +=  $dado['valor'];
                $arrTotal[] = $total;
            }

            $aDados = array(
                array('name' => 'Matriculas', 'data'=>$arrA),
                //array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            //echo "<table width='90%' border='0'><tr><td>";
            echo geraGraficoLinha($aDados, $arrX, "graficoMatriculasAtivas", "", 300, $formatoValores);
            //echo "</td></tr></table>";
        }
        ?>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1502);" width="33%">
        <div>
            <?php exibirTitulo('indicador', 'Matrículas ativas de Docentes'); ?>
        </div>
        <?php
        $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (1502)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
                ORDER BY periodo";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        $total = 0;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrX[] = $dado['periodo'];
                $arrA[] = Round($dado['valor']);
                $total +=  $dado['valor'];
                $arrTotal[] = $total;
            }

            $aDados = array(
                array('name' => 'Matriculas', 'data'=>$arrA),
                //array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            //echo "<table width='90%' border='0'><tr><td>";
            echo geraGraficoLinha($aDados, $arrX, "graficoMatriculasDocentes", "", 300, $formatoValores);
            //echo "</td></tr></table>";
        }
        ?>
    </td>
</tr>
<tr>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1531);">
        <div>
            <?php exibirTitulo('mapas', 'Polo de apoio presencial<br>por localização'); ?>
        </div>
        <?php
        $sql = "SELECT
                    COUNT(DISTINCT polid) AS polos,
                    COUNT(DISTINCT dshuf) AS uf,
                    COUNT(DISTINCT dshcodmunicipio) AS municipios
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                WHERE sh.indid IN (1531)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                AND sh.sehstatus <> 'I'";
        $arrValores = $db->pegaLinha($sql,null,3200);
        ?>
        <table border=0 cellspacing="4" cellpadding="5" align="center">
            <tr>
                <td>
                    <br><br><br><br><br><br>
                    <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['polos'],0,",",".") ?> </span><span class="subtitulo_box" >Polos</span><br />
                    <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['uf'],0,",",".") ?> </span><span class="subtitulo_box" >UFs</span><br />
                    <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['municipios'],0,",",".") ?> </span><span class="subtitulo_box" >Municípios</span>
                </td>
                <td class="link" valign="bottom" onclick="abreMapaMaisMedicos();">
                    <img src="../imagens/cockpit/mapa_cockpit.png"  />
                </td>
        </table>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1422);">
        <div>
            <?php exibirTitulo('indicador', 'Ingressantes'); ?>
        </div>
        <?php
        $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (1422)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
                ORDER BY periodo";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        $total = 0;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrX[] = $dado['periodo'];
                $arrA[] = Round($dado['valor']);
                $total +=  $dado['valor'];
                $arrTotal[] = $total;
            }

            $aDados = array(
                array('name' => 'Matriculas', 'data'=>$arrA),
               //array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            //echo "<table width='90%' border='0'><tr><td>";
            echo geraGraficoLinha($aDados, $arrX, "graficoIngressantes", "", 300, $formatoValores);
            //echo "</td></tr></table>";
        }
        ?>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1501);">
        <div>
            <?php exibirTitulo('indicador', 'Docentes ingressantes'); ?>
        </div>
        <?php
        $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (1501)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
                ORDER BY periodo";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        $total = 0;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrX[] = $dado['periodo'];
                $arrA[] = Round($dado['valor']);
                $total +=  $dado['valor'];
                $arrTotal[] = $total;
            }

            $aDados = array(
                array('name' => 'Matriculas', 'data'=>$arrA),
                //array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            //echo "<table width='90%' border='0'><tr><td>";
            echo geraGraficoLinha($aDados, $arrX, "graficoIngressantesDocentes", "", 300, $formatoValores);
            //echo "</td></tr></table>";
        }
        ?>
    </td>
</tr>
<tr>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(305);">
        <div>
            <?php exibirTitulo('indicador', 'Instituições de Educação Superior<br>integrantes do Sistema UAB'); ?>
        </div>
        <?php
        $sql = "SELECT
                    COUNT(DISTINCT dshcod) AS ies,
                    COUNT(DISTINCT dshuf) AS uf,
                    COUNT(DISTINCT dshcodmunicipio) AS municipios
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                WHERE sh.indid IN (305)
                AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                AND sh.sehstatus <> 'I'";
        $arrValores = $db->pegaLinha($sql,null,3200);
        ?>
        <table border=0 cellspacing="4" cellpadding="5" align="center">
            <tr>
                <td>
                    <br><br><br><br><br><br>
                    <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['ies'],0,",",".") ?> </span><span class="subtitulo_box" >IES</span><br />
                    <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['uf'],0,",",".") ?> </span><span class="subtitulo_box" >UFs</span><br />
                    <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['municipios'],0,",",".") ?> </span><span class="subtitulo_box" >Municípios</span>
                </td>
                <td class="link" valign="bottom" onclick="abreMapaMaisMedicos();">
                    <img src="../imagens/cockpit/mapa_cockpit.png"  />
                </td>
        </table>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1423);">
        <div>
            <?php exibirTitulo('indicador', 'Concluintes'); ?>
        </div>
        <?php
        $sql = "SELECT dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN
                    (
                        SELECT MAX(s.dpeid) as dpeid, dpeanoref
                        FROM painel.seriehistorica s
                        INNER JOIN painel.detalheperiodicidade d on d.dpeid = s.dpeid
                        WHERE s.indid = 1423 AND s.sehstatus <> 'I' GROUP BY dpeanoref) a ON a.dpeid = sh.dpeid
                WHERE sh.indid in (1423)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
                ORDER BY periodo";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        $total = 0;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrX[] = $dado['periodo'];
                $arrA[] = Round($dado['valor']);
                $total +=  $dado['valor'];
                $arrTotal[] = $total;
            }

            $aDados = array(
                array('name' => 'Formandos', 'data'=>$arrA),
                //array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            //echo "<table width='90%' border='0'><tr><td>";
            echo geraGraficoLinha($aDados, $arrX, "graficoConcluintes", "", 300, $formatoValores);
            //echo "</td></tr></table>";
        }
        ?>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1503);">
        <div>
            <?php exibirTitulo('indicador', 'Docentes concluintes'); ?>
        </div>
        <?php
        $sql = "SELECT dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (1503)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
                ORDER BY periodo";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $arrX = array();
        $arrA = array();
        $arrTotal = array();
        $total = 0;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrX[] = $dado['periodo'];
                $arrA[] = Round($dado['valor']);
                $total +=  $dado['valor'];
                $arrTotal[] = $total;
            }

            $aDados = array(
                array('name' => 'Matriculas', 'data'=>$arrA),
                //array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            //echo "<table width='90%' border='0'><tr><td>";
            echo geraGraficoLinha($aDados, $arrX, "graficoConcluintesDocentes", "", 300, $formatoValores);
            //echo "</td></tr></table>";
        }
        ?>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" colspan="2">
        <div>
            <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
        </div>
        <?=exibirTabelaFinanceiro(22);?>
    </td>
    <!-- Tabela Alinhamento Estratégico -->
    <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,22,'','');" align="center" >
        <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
        <img src="cockpit/images/alinhamentoEstrategico.png" />
    </td>
    <!-- FIM Tabela Alinhamento Estratégico -->
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