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
        .fundo_titulo{background-image:url('cockpit/images/bannerInglesSemFronteira.png');background-repeat:no-repeat;background-color:#FFFFFF;background-position:center;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
        #popdiv{display:block;width:100%;height:357px;overflow-y: scroll;}
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
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_ingles_sem_fronteiras&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:25px" >&nbsp;</div></td>
    </tr>
    <tr>
        <td class="fundo_padrao center" width="33%">
            <span class="titulo_box"><font size="+3">Centros Aplicadores<br>CA</font></span>
        </td>
        <td class="fundo_padrao center" width="34%">
            <span class="titulo_box"><font size="+3">My English Online<br>MEO</font></span>
        </td>
        <td class="fundo_padrao center" width="33%">
            <span class="titulo_box"><font size="+3">Núcleos de Línguas<br>NucLI</font></span>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="33%">
            <?php
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (3088)
                    and sh.sehstatus <> 'I'";
            $valor3088 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2026)
                    and sh.sehstatus <> 'I'";
            $valor2026 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (3089)
                    and sh.sehstatus <> 'I'";
            $valor3089 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2025)
                    and sh.sehstatus <> 'I'";
            $valor2025 = $db->pegaUm($sql);
            ?>
            <div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(3088);"> <?php exibirTitulo('executiverel', 'Vagas ofertadas<br>para o teste<br>'.number_format($valor3088,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2026);"> <br><br><?php exibirTitulo('executive', 'Inscrições<br>'.number_format($valor2026,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(3089);"> <?php exibirTitulo('executiverel', 'Testes corrigidos<br>(válidos)<br>'.number_format($valor3089,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2025);"> <?php exibirTitulo('casas', 'Instituições<br>credenciadas<br>'.number_format($valor2025,0,",",".") . '</span>'); ?><br><br></div>
            </div>

            <div>
                <?php exibirTitulo('alvo', 'Testes corrigidos', 'Frente ao total de testes adquiridos pelo IsF'); ?>
            </div>
            <?php
            $valorMeta = 500000;
            $porcentagem = number_format(($valor3089/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" border="0" >
                <tr>
                    <td class="titulo_box bold" align="center">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
                        </div><?php echo number_format($valor3089,0,",",".") ?> (de <?php echo number_format($valorMeta,0,",",".") ?> testes)
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="34%">
            <?php
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2005)
                    and sh.sehstatus <> 'I'";
            $valor2005 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2006)
                    and sh.sehstatus <> 'I'";
            $valor2006 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2007)
                    and sh.sehstatus <> 'I'";
            $valor2007 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (3098)
                    and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid and sh.sehstatus <> 'I')
                    and sh.sehstatus <> 'I'";
            $valor3098 = $db->pegaUm($sql);
            ?>
            <div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2005);"> <?php exibirTitulo('executiverel', 'Senhas solicitadas<br>'.number_format($valor2005,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2006);"> <?php exibirTitulo('executiverel', 'Senhas liberadas<br>'.number_format($valor2006,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2007);"> <?php exibirTitulo('executiverel', 'Senhas ativas<br>'.number_format($valor2007,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(3098);"> <?php exibirTitulo('executive', 'Estudantes<br>concluintes<br>'.number_format($valor3098,0,",",".") . '</span>'); ?><br><br></div>
            </div>
            <!--div>
                <?php exibirTitulo('alvo', 'Senhas ativas', 'Frente ao total de senhas disponíveis no IsF'); ?>
            </div>
            <?php
            $valorMeta = 2000000;
            $porcentagem = number_format(($valor2007/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" border="0" >
                <tr>
                    <td class="titulo_box bold" align="center">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
                        </div><?php echo number_format($valor2007,0,",",".") ?> (de <?php echo number_format($valorMeta,0,",",".") ?> de senhas)
                    </td>
                </tr>
            </table-->
        </td>
        <td class="fundo_padrao" width="33%">
            <?php
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (3096)
                    and sh.sehstatus <> 'I'";
            $valor3096 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (3093)
                    and sh.sehstatus <> 'I'";
            $valor3093 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2028)
                    and sh.sehstatus <> 'I'";
            $valor2028 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (2024)
                    and sh.sehstatus <> 'I'";
            $valor2024 = $db->pegaUm($sql);
            $sql = "select sum(dsh.dshqtde) AS total
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    where sh.indid in (3103)
                    and sh.sehstatus <> 'I'";
            $valor3103 = $db->pegaUm($sql);
            ?>
            <div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(3096);"> <?php exibirTitulo('executiverel', 'Vagas ofertadas<br>'.number_format($valor3096,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(3093);"> <?php exibirTitulo('executive', 'Inscritos<br>'.number_format($valor3093,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2028);"> <?php exibirTitulo('executive', 'Selecionados<br>'.number_format($valor2028,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 50%;" class="link" onclick="abreIndicadorPopUp(2024);"> <?php exibirTitulo('casas', 'Instituições<br>credenciadas<br>'.number_format($valor2024,0,",",".") . '</span>'); ?><br><br></div>
                <div style="float: left; width: 100%;" class="link" onclick="abreIndicadorPopUp(3103);"> <?php exibirTitulo('financeiro', 'Repasse para os NucLI<br>R$ '.number_format($valor3103,0,",",".") . '</span>'); ?><br><br></div>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3088);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de testes ofertados por demanda'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS periodo,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (3088)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Testes', 'data'=>$arrA),
                    array('name' => 'Testes Acumulados', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaDemanda", "Nº de testes ofertados por demanda",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2005);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de senhas solicitadas por bimestre'); ?>
            </div>
            <?php
            $sql = "SELECT periodo, valor
                    FROM (
                        SELECT
                            (SELECT Replace(Replace(d.dpedsc,'º ','o<br>'),'Bimestre/','Bim.<br>') FROM painel.detalheperiodicidade d WHERE d.perid = 8 AND (dpe.dpedatainicio BETWEEN d.dpedatainicio AND d.dpedatafim)) AS periodo,
                            sum(dsh.dshqtde) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (2005)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    ) AS FOO
                    ORDER BY substr(Replace(periodo,'o<br>Bim.<br>', ''),2,4)||substr(Replace(periodo,'o<br>Bim.<br>', ''),1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Solicitacoes', 'data'=>$arrA),
                    array('name' => 'Solicitacoes Acumuladas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaSenhas", "Nº de senhas solicitadas por bimestre",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3096);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de vagas por oferta'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS periodo,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (3096)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Vagas', 'data'=>$arrA),
                    array('name' => 'Vagas Acumuladas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaOferta", "Nº de vagas por oferta",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2026);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de inscrições no teste de<br>nivelamento/proficiência por demanda'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS periodo,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (2026)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Inscricoes', 'data'=>$arrA),
                    array('name' => 'Inscricoes Acumuladas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaNivelamento", "Nº de inscrições no teste de nivelamento/proficiência",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2006);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de senhas liberadas por bimestre'); ?>
            </div>
            <?php
            $sql = "SELECT periodo, valor
                    FROM (
                        SELECT
                            (SELECT Replace(Replace(d.dpedsc,'º ','o<br>'),'Bimestre/','Bim.<br>') FROM painel.detalheperiodicidade d WHERE d.perid = 8 AND (dpe.dpedatainicio BETWEEN d.dpedatainicio AND d.dpedatafim)) AS periodo,
                            sum(dsh.dshqtde) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (2006)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    ) AS FOO
                    ORDER BY substr(Replace(periodo,'o<br>Bim.<br>', ''),2,4)||substr(Replace(periodo,'o<br>Bim.<br>', ''),1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Senhas', 'data'=>$arrA),
                    array('name' => 'Senhas Acumuladas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaSenhasLiberadas", "Nº de senhas liberadas por bimestre",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3093);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de alunos inscritos por oferta'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS periodo,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (3093)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Alunos', 'data'=>$arrA),
                    array('name' => 'Alunos Acumulados', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaAlunosInscritosOferta", "Nº de alunos inscritos por oferta",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3089);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de testes corrigidos (válidos)<br>por demanda'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS periodo,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (3089)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Testes', 'data'=>$arrA),
                    array('name' => 'Testes Acumulados', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaTestes", "Nº de testes corrigidos (válidos) por demanda",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2007);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de senhas ativas por bimestre'); ?>
            </div>
            <?php
            $sql = "SELECT periodo, valor
                    FROM (
                        SELECT
                            (SELECT Replace(Replace(d.dpedsc,'º ','o<br>'),'Bimestre/','Bim.<br>') FROM painel.detalheperiodicidade d WHERE d.perid = 8 AND (dpe.dpedatainicio BETWEEN d.dpedatainicio AND d.dpedatafim)) AS periodo,
                            sum(dsh.dshqtde) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (2007)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    ) AS FOO
                    ORDER BY substr(Replace(periodo,'o<br>Bim.<br>', ''),2,4)||substr(Replace(periodo,'o<br>Bim.<br>', ''),1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Alunos', 'data'=>$arrA),
                    array('name' => 'Alunos Acumulados', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaSenhasAtivas", "Nº de senhas ativas por bimestre",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2028);">
            <div>
                <?php exibirTitulo('indicador', 'Nº de alunos selecionados por oferta'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS periodo,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (2028)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1)";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrX = array();
            $arrA = array();
            $arrTotal = array();
            $vTotal = 0;
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $arrA[] = Round($dado['valor']);
                    $vTotal += Round($dado['valor']);
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Vagas', 'data'=>$arrA),
                    array('name' => 'Vagas Acumuladas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaSelecionadosOferta", "Nº de alunos selecionados por oferta",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('executiverel', 'Percentual de testes corrigidos (válidos)<br>frente ao total de inscrições', '<span class="link" onclick="abreIndicadorPopUp(3089);">Testes corrigidos</span>, <span class="link" onclick="abreIndicadorPopUp(2026);">Inscrições</span>'); ?>
            </div>
            <?php
            $sql = "SELECT periodo, CASE WHEN SUM(valor2026) > 0 THEN ((SUM(valor3089) / SUM(valor2026)) * 100) ELSE 0 END AS valor
                    FROM (
                        SELECT
                            tid1.tiddsc AS periodo,
                            CASE sh.indid WHEN 3089 THEN dsh.dshqtde END AS valor3089,
                            CASE sh.indid WHEN 2026 THEN dsh.dshqtde END AS valor2026,
                            substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1) AS ordem
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid in (3089, 2026)
                        AND sh.sehstatus <> 'I'
                        AND dsh.tidid1 NOT IN (8218,8213)
                    ) AS FOO
                    GROUP BY periodo, ordem
                    ORDER BY ordem";
            $arrDados = $db->carregar( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Edição</th>
                    <th>Taxa de corrigidos</th>
                </tr>
                <?php
                if($arrDados){
                    $count = -1;
                    foreach($arrDados as $dado):
                        $count++;
                        if(number_format($dado['valor'],0,",",".") > 0){
                        ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $dado['periodo'] ?></td>
                                <td class="numero" ><?php echo number_format($dado['valor'],2,",",".") ?>%</td>
                            </tr>
                        <?php
                        }
                    endforeach;
                }
                ?>
            </table>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('executiverel', 'Percentual de senhas ativas e concluintes<br>frente às liberadas; e de senhas liberadas<br>frente às solicitadas*', '<span class="link" onclick="abreIndicadorPopUp(2007);">Senhas ativas</span>, <span class="link" onclick="abreIndicadorPopUp(2006);">Senhas liberadas</span>, <span class="link" onclick="abreIndicadorPopUp(2005);">Senhas solicitadas</span>, <span class="link" onclick="abreIndicadorPopUp(3098);">Alunos</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        CASE WHEN SUM(valor2006) > 0 THEN ((SUM(valor2007) / SUM(valor2006)) * 100) ELSE 0 END AS valorsenhas,
                        CASE WHEN SUM(valor2006) > 0 THEN ((SUM(valor3098) / SUM(valor2006)) * 100) ELSE 0 END AS valoralunos,
                        CASE WHEN SUM(valor2005) > 0 THEN ((SUM(valor2006) / SUM(valor2005)) * 100) ELSE 0 END AS valorsolicitadas
                    FROM (
                        SELECT
                            CASE sh.indid WHEN 2007 THEN dsh.dshqtde END AS valor2007,
                            CASE sh.indid WHEN 2006 THEN dsh.dshqtde END AS valor2006,
                            CASE sh.indid WHEN 2005 THEN dsh.dshqtde END AS valor2005,
                            0 AS valor3098
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (2007, 2006, 2005)
                        AND sh.sehstatus <> 'I'
                    UNION ALL
                        SELECT
                            0 AS valor2007,
                            0 AS valor2006,
                            0 AS valor2005,
                            SUM(dsh.dshqtde) AS valor3098
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        WHERE sh.indid IN (3098)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    ) AS FOO";
            $arrDados = $db->pegalinha( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Tipo</th>
                    <th>Percentual</th>
                </tr>
                <tr class="">
                    <td>Senhas ativas / liberadas</td>
                    <td class="numero" ><?php echo number_format($arrDados['valorsenhas'],2,",",".") ?>%</td>
                </tr>
                <tr class="zebrado">
                    <td>Alunos concluintes / senhas liberadas</td>
                    <td class="numero" ><?php echo number_format($arrDados['valoralunos'],2,",",".") ?>%</td>
                </tr>
                <tr class="">
                    <td>Senhas liberadas / solicitadas</td>
                    <td class="numero" ><?php echo number_format($arrDados['valorsolicitadas'],2,",",".") ?>%</td>
                </tr>
            </table>
            <table>
                <tr>
                    <td>* Percentual calculado a partir dos valores totais acumulados.</td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('executiverel', 'Percentual de estudantes selecionados<br>frente aos inscritos e à oferta de vagas ', '<span class="link" onclick="abreIndicadorPopUp(2028);">Selecionados</span>, <span class="link" onclick="abreIndicadorPopUp(3093);">Inscritos</span>, <span class="link" onclick="abreIndicadorPopUp(3096);">Vagas ofertadas</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        periodo,
                        CASE WHEN SUM(valor3093) > 0 THEN ((SUM(valor2028) / SUM(valor3093)) * 100) ELSE 0 END AS valorinscritos,
                        CASE WHEN SUM(valor3096) > 0 THEN ((SUM(valor2028) / SUM(valor3096)) * 100) ELSE 0 END AS valorvagas
                    FROM (
                        SELECT
                            tid1.tiddsc AS periodo,
                            CASE sh.indid WHEN 2028 THEN dsh.dshqtde END AS valor2028,
                            CASE sh.indid WHEN 3093 THEN dsh.dshqtde END AS valor3093,
                            CASE sh.indid WHEN 3096 THEN dsh.dshqtde END AS valor3096,
                            substr(tid1.tiddsc,3,4)||substr(tid1.tiddsc,1,1) AS ordem
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid in (2028, 3093, 3096)
                        AND sh.sehstatus <> 'I'
                        AND dsh.tidid1 IN (8188, 8189,8190,8191,8192,8219, 8223, 8224, 8225, 8226, 8227, 8228, 8174, 8175,8176,8177,8178,8179, 8220,8221,8229,8230,8180,8181)
                    ) AS FOO
                    GROUP BY periodo, ordem
                    ORDER BY ordem";
            $arrDados = $db->carregar( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Edição</th>
                    <th>Selecionados / inscritos</th>
                    <th>Selecionados / oferta de vagas</th>
                </tr>
                <?php
                if($arrDados){
                    $count = -1;
                    foreach($arrDados as $dado):
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['periodo'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valorinscritos'],2,",",".") ?>%</td>
                            <td class="numero" ><?php echo number_format($dado['valorvagas'],2,",",".") ?>%</td>
                        </tr>
                    <?php
                    endforeach;
                }
                ?>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2025);">
            <div>
                <?php exibirTitulo('mapas', 'Centros aplicadores credenciados', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica seh where indid = 2025 and sehstatus != 'I';";
            $arrValores = $db->pegaLinha($sql);
            $sql = "select
                        count(distinct dshcodmunicipio)
                    from
                        painel.seriehistorica seh
                    inner join
                        painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    where
                        indid = 2025
                    and
                        seh.sehstatus != 'I';";
            $totalMun = $db->pegaUm($sql);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >IES(s)</span>
                        <br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
                    </td>
                    <td class="link" valign="bottom">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2007);">
            <div>
                <?php exibirTitulo('indicador', 'Senhas ativas por módulo'); ?>
            </div>
            <?php
            $sql = "SELECT periodo, modulo, valor
                    FROM (
                        SELECT
                            (SELECT Replace(Replace(d.dpedsc,'º ','o<br>'),'Semestre','Sem.') FROM painel.detalheperiodicidade d WHERE d.perid = 2 AND (dpe.dpedatainicio BETWEEN d.dpedatainicio AND d.dpedatafim)) AS periodo,
                            tid1.tiddsc AS modulo,
                            SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid IN (2007)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo, modulo
                    ) AS FOO
                    ORDER BY substr(Replace(periodo,'o<br>Sem./', ''),2,4)||substr(Replace(periodo,'o<br>Sem./', ''),1,1), modulo";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'periodo', 'modulo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaSenhas', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2024);">
            <div>
                <?php exibirTitulo('mapas', 'Núcleos de ensino<br>de línguas credenciados', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica seh where indid = 2024 and dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid) and sehstatus != 'I';";
            $arrValores = $db->pegaLinha($sql);
            $sql = "select
                        count(distinct dshcodmunicipio)
                    from
                        painel.seriehistorica seh
                    inner join
                        painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    where
                        indid = 2024
                    and
                        dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = seh.indid)
                    and
                        seh.sehstatus != 'I';";
            $totalMun = $db->pegaUm($sql);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >IES(s)</span>
                        <br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
                    </td>
                    <td class="link" valign="bottom">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3089);">
            <div>
                <?php exibirTitulo('indicador', 'Percentual de notas dos testes corrigidos,<br>por nível do Quadro Comum Europeu'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS descricao,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (3089)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizza3089", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3098);">
            <div>
                <?php exibirTitulo('indicador', 'Estudantes concluintes<br>por módulos (senhas/concluintes)'); ?>
            </div>
            <?php
            $sql = "select
                           --public.removeacento(Replace(dpe.dpedsc, '/', '<br>')) AS ano,
                           (SELECT Replace(Replace(d.dpedsc,'º ','o<br>'),'Semestre','Sem.') FROM painel.detalheperiodicidade d WHERE d.perid = 2 AND (dpe.dpedatainicio BETWEEN d.dpedatainicio AND d.dpedatafim)) AS ano,
                           tid1.tiddsc as dependencia,
                           SUM(dsh.dshqtde) AS valor,
                           dpeanoref||dpemesref AS ordem
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    where sh.indid in (3098)
                    and sh.sehstatus <> 'I'
                    group by ano, dependencia, ordem
                    order by ordem, dependencia";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'dependencia', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaEstudantesConcluintes', null, null, $formatoValores, null, "", 1000, 600, null, 300, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3103);">
            <div>
                <?php exibirTitulo('financeiro', 'Repasses - Núcleos de Ensino de Línguas'); ?>
            </div>
            <div id="popdiv">
                <?php
                $sql = "SELECT
                        UPPER(iesdsc) AS instituto,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::integer
                    WHERE sh.indid in (3103)
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.sehstatus <> 'I'
                    GROUP BY instituto
                    ORDER BY instituto";
                $arrDados = $db->carregar( $sql, null, 3200 );
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th>IES</th>
                        <th>Reais</th>
                    </tr>
                    <?php
                    if($arrDados){
                        $count = -1;
                        $totalvalor = 0;
                        foreach($arrDados as $dado):
                            $count++;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td><?php echo $dado['instituto'] ?></td>
                                <td class="numero" ><?php $totalvalor+=$dado['valor']; echo number_format($dado['valor'],2,",",".") ?></td>
                            </tr>
                        <?php endforeach;
                    }
                    ?>
                    <tr>
                        <th class="bold" >Total</th>
                        <th class="numero bold" ><?php echo number_format($totalvalor,2,",",".") ?></th>
                    </tr>
                </table>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3092);" colspan="2" rowspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Pagamentos - Bolsas do IsF'); ?>
            </div>
            <?php
            $sql = "SELECT
                        public.removeacento(Replace(dpe.dpedsc, '/', '<br>')) AS periodo,
                        dsh.tidid1 AS acao,
                        SUM(dsh.dshqtde) AS valor,
                        dpeanoref||dpemesref AS ordem
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (3092)
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
                $arrC = array();
                $arrD = array();
                $arrTotal = array();

                foreach($arrSituacao as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = ((is_array($sit['8194']['total'])?array_sum($sit['8194']['total']):0));
                    $vB = ((is_array($sit['8195']['total'])?array_sum($sit['8195']['total']):0));
                    $vC = ((is_array($sit['8196']['total'])?array_sum($sit['8196']['total']):0));
                    $vD = ((is_array($sit['8197']['total'])?array_sum($sit['8197']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrC[] = $vC;
                    $arrD[] = $vD;
                    $arrTotal[] = $vA + $vB + $vC + $vD;
                }

                $aDados = array(
                    array('name' => 'Bolsa Professor', 'data'=>$arrA),
                    array('name' => 'Bolsa Coordenador-pedagogico', 'data'=>$arrB),
                    array('name' => 'Bolsa Coordenador de Centro Aplicador', 'data'=>$arrC),
                    array('name' => 'Bolsa Coordenador-geral', 'data'=>$arrD),
                    array('name' => 'Total de Bolsas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoInvestimentoBolsas", "Pagamentos - Bolsas do IsF",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
    <tr>
        <!--<td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(163);?>
        </td>-->
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,163,'','');" align="center" >
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