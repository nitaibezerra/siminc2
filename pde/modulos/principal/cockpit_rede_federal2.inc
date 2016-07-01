<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

$_SESSION['entid_campus'] = "";

if(!$_REQUEST['tipo']){
    $_SESSION['exportar1'] = "";
    $_SESSION['exportar2'] = "";
    $_SESSION['exportar3'] = "";
    $_SESSION['exportar4'] = "";
    $_SESSION['exportar5'] = "";
    $_SESSION['exportar6'] = "";
    $_SESSION['exportar7'] = "";
    $_SESSION['exportar8'] = "";
    $_SESSION['exportar9'] = "";
    $_SESSION['exportar10'] = "";
    $_SESSION['exportar11'] = "";
    $_SESSION['exportar12'] = "";
    $_SESSION['exportar13'] = "";
    $_SESSION['exportar14'] = "";
    $_SESSION['exportar15'] = "";
    $_SESSION['exportar16'] = "";
    $_SESSION['exportar17'] = "";
    $_SESSION['exportar18'] = "";
    $_SESSION['exportar19'] = "";
    $_SESSION['exportar20'] = "";
    $_SESSION['exportar21'] = "";
    $_SESSION['exportar22'] = "";
    $_SESSION['exportar23'] = "";
}

if($_REQUEST['requisicaoAjax']){
    header('content-type: text/html; charset=ISO-8859-1');
    $_REQUEST['requisicaoAjax']();
    exit;
}
if(isset($_REQUEST['detalhe_redefederal1'])){
    gerarGraficoRedeFederal1($_REQUEST['detalhe_redefederal1']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal2'])){
    gerarGraficoRedeFederal2($_REQUEST['detalhe_redefederal2']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal3'])){
    gerarGraficoRedeFederal3($_REQUEST['detalhe_redefederal3']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal4'])){
    gerarGraficoRedeFederal4($_REQUEST['detalhe_redefederal4']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal5'])){
    gerarGraficoRedeFederal5($_REQUEST['detalhe_redefederal5']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal6'])){
    gerarGraficoRedeFederal6($_REQUEST['detalhe_redefederal6']);
    die;
}
if(isset($_REQUEST['detalhe_redefederal7'])){
    gerarGraficoRedeFederal7($_REQUEST['detalhe_redefederal7']);
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
    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')}
        #popdiv{display:block;width:100%;height:357px;overflow-y: scroll;}
    </style>
    <script>
        jQuery.noConflict();
        jQuery('.div_fotos_interno').cycle({
            fx: 'scrollDown'
        });

        atualizaUsuario();

        function listaInstitutos(estado)
        {
            document.getElementById('popdiv').style.display='block';

            jQuery.ajax({
                url		: 'estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A',
                type	: 'post',
                data	: 'requisicaoAjax=listaInstitutosEstadoRedeFederal&estado='+estado,
                success	: function(e){
                    jQuery('#popdiv').html(e);
                }
            });

        }

        function abreInstituto(id)
        {
            window.location.href = 'estrategico.php?modulo=principal/cockpit_detalhe_instituto&acao=A&entid='+id;
        }

        jQuery(function(){
            jQuery('.toggle_consolidado1').click(function(){
                jQuery('#graficos_consolidados1').toggle('fade');
            });

            setTimeout(function() {jQuery('#graficos_consolidados1').hide()} , 500);

        });

        function visualizarXLS(tipo){
            window.location='estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&tipo='+tipo;
        }

        jQuery(function(){
            jQuery('.filtrar_redefederal1').click(function(){
                jQuery('#div_redefederal1').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal1='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal2').click(function(){
                jQuery('#div_redefederal2').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal2='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal3').click(function(){
                jQuery('#div_redefederal3').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal3='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal4').click(function(){
                jQuery('#div_redefederal4').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal4='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal5').click(function(){
                jQuery('#div_redefederal5').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal5='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal6').click(function(){
                jQuery('#div_redefederal6').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal6='+jQuery(this).val());
            });
            jQuery('.filtrar_redefederal7').click(function(){
                jQuery('#div_redefederal7').load('estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A&detalhe_redefederal7='+jQuery(this).val());
            });
        });
    </script>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="titulo_pagina" >
            <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
            </div>
            <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <!-- Título-->
    <tr>
        <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Expansão da Rede Federal de EPT</div></td>
    </tr>
    <!-- Fim Título-->
    <tr>
        <td class="fundo_padrao" width="25%">
            <?php
            $sql = "select count(distinct fn.entid) as campus, count(distinct a.entid) as universidades
					from entidade.funcaoentidade fn
					inner join entidade.funentassoc a on a.fueid = fn.fueid
					where fn.fuestatus = 'A'
					and fn.entid in (
						select
							distinct dsh.entid
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join entidade.entidade entcampus ON entcampus.entid = dsh.entid
						where sh.indid in (525)
						and sh.sehstatus <> 'I'
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and dsh.tidid2 = 1738
						and dsh.tidid1 IN (2596, 2597, 1736, 2257) --Retirado Novas Expansões(7107) a pedido da Loyane (25/08/2014)
					)";
            $arrDados = $db->pegaLinha($sql);
            $valorMetaCampus = 562;
            $porcentagemCampus = number_format(($arrDados['campus']/$valorMetaCampus)*100,0,",",".");
            $horizontalCampus = $porcentagemCampus * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td>
                        <?php exibirTitulo('alvo', 'Metas e Execução 2014'); ?>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        <?php echo number_format($valorMetaCampus,0,",",".") ?> Campus
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalCampus ?>px;'><?= $porcentagemCampus ?>%&nbsp;</div>
                        </div><?php echo number_format($arrDados['campus'],0,",",".") ?> Funcionando</td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2294);" width="25%">
            <?php
            $sql = "SELECT count(distinct entid) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (525)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND dsh.tidid1 IN (2596, 2597)
					AND dsh.tidid2 IN (1738)
					AND sh.sehstatus <> 'I'";
            $totalCampusFuncionando = $db->pegaUm( $sql );
            $valorMeta = 208;
            $porcentagem = number_format(($totalCampusFuncionando/$valorMeta)*100,0,",",".");
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" border="0" >
                <tr>
                    <td class="titulo_box bold" align="center">Expansão Fase III</td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center">
                        <?php exibirTitulo('alvo', '208 câmpus 2011-14'); ?>
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
                        </div><?php echo number_format($totalCampusFuncionando,0,",",".") ?> funcionando
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(525);" rowspan="2" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Situação dos Campus / Unidades'); ?>
            </div>
            <?php
            $sql = "SELECT ordem, status, valor,
                        case
                        when status = 'Preexistente até 2002' then 3
                        when status = 'Expansão 2011/2014' then 1
                        else 2
                        end as ordenacao
                    FROM (
                        SELECT
                            CASE
                                WHEN dsh.tidid1 IN (1736) THEN 1
                                WHEN dsh.tidid1 IN (2257) THEN 2
                                WHEN dsh.tidid1 IN (2596, 2597) THEN 3
                            END AS ordem,
                            CASE
                                WHEN dsh.tidid1 IN (1736) THEN 'Preexistente até 2002'
                                WHEN dsh.tidid1 IN (2257) THEN 'Criados de 2003 a 2010'
                                WHEN dsh.tidid1 IN (2596, 2597) THEN 'Expansão 2011/2014'
                            ELSE tid1.tiddsc
                            END AS status,
                            sum(dsh.dshqtde)::INTEGER AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid in (525)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid)
                        AND dsh.tidid1 IN (2257, 2596, 2597, 1736)
                        GROUP BY status, ordem
                    UNION ALL
                        SELECT
                            2 AS ordem,
                            CASE
                                WHEN dsh.tidid1 IN (1736) THEN 'Preexistente até 2002'
                            ELSE tid1.tiddsc
                            END AS status,
                            sum(dsh.dshqtde)::INTEGER AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid in (525)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid)
                        AND dsh.tidid1 IN (1736)
                        GROUP BY status, ordem
                    UNION ALL
                        SELECT
                            3 AS ordem,
                            CASE
                                WHEN dsh.tidid1 IN (1736) THEN 'Preexistente até 2002'
                                WHEN dsh.tidid1 IN (2257) THEN 'Criados de 2003 a 2010'
                            END AS status,
                            sum(dsh.dshqtde)::INTEGER AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid in (525)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid)
                        AND dsh.tidid1 IN (2257, 1736)
                        GROUP BY status, ordem
                    union all
                        select '1', 'Criados de 2003 a 2010', 0
                    union all
                        select '1', 'Expansão 2011/2014', 0
                    union all
                        select '2', 'Expansão 2011/2014', 0
                    ) AS FOO
                    ORDER BY ordem, ordenacao";
            $dados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($dados, 'ordem', 'status', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.series.name + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }",
                'corX' => "#152D56"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculasSituacao', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(2347);">
                <?php exibirTitulo('indicador', 'Ingressantes em cursos<br>técnicos regulares - Meta Pronatec'); ?>
            </div>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <td>
                        <?
                        $sql = "SELECT dpeanoref AS periodo, tipo, SUM(valor) AS valor
                                    FROM (
                                        SELECT dpe.dpeanoref, 'Matriculas' AS tipo, sum(dsh.dshqtde) AS valor
                                        FROM painel.seriehistorica sh
                                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                                        WHERE sh.indid IN (2347)
                                        AND sh.sehstatus <> 'I'
                                        AND dsh.tidid2 IN (5130,5131,5188,5190,5189)
                                        AND dpe.dpeanoref >= '2011'
                                        GROUP BY dpe.dpeanoref
                                    UNION ALL
                                        SELECT '2011' AS dpeanoref, 'Meta' AS tipo,  72000 AS valor
                                    UNION ALL
                                        SELECT '2012' AS dpeanoref, 'Meta' AS tipo, 79560 AS valor
                                    UNION ALL
                                        SELECT '2013' AS dpeanoref, 'Meta' AS tipo, 90360 AS valor
                                    UNION ALL
                                        SELECT '2014' AS dpeanoref, 'Meta' AS tipo, 101160 AS valor
                                    ) AS foo
                                    GROUP BY periodo, tipo
                                    ORDER BY periodo, tipo";
                        $arrDados = $db->carregar( $sql, null, 3200 );
                        $_SESSION['exportar1'] = $arrDados;
                        if($arrDados){
                            $arrSituacao = array();
                            foreach($arrDados as $dado){
                                $arrSituacao[$dado['periodo']][$dado['tipo']]['total'][] = $dado['valor'];
                            }

                            $arrX = array();
                            $arrA = array();
                            $arrB = array();
                            $vA = 0;
                            $vB = 0;
                            foreach($arrSituacao as $periodo => $sit)
                            {
                                $arrX[] = $periodo;
                                $vA += ((is_array($sit['Matriculas']['total'])?array_sum($sit['Matriculas']['total']):0));
                                $vB += ((is_array($sit['Meta']['total'])?array_sum($sit['Meta']['total']):0));
                                $arrA[] = $vA;
                                $arrB[] = $vB;
                            }

                            $aDados = array(
                                array('name' => 'Matriculas', 'data'=>$arrA),
                                array('name' => 'Meta', 'data'=>$arrB),
                            );

                            $formatoValores = array(
                                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                                'alinhamento' => "formatter: function() { return '1'; }"
                            );

                            echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoMatriculas", "Matrículas Rede Federal Pronatec",250, $formatoValores);
                        }
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="fundo_padrao" align="left">
                        <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(1);" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <!-- Tabela Indicadores-->
        <td class="fundo_padrao" rowspan="2" width="70%">
            <table width="100%" align="center" border="0">
                <tr>
                    <td width="20%">&nbsp;</td>
                    <td align="center">
                        <div>
                            <?php exibirTitulo('busca', 'Monitoramento de Programas e Ações por Instituto', 'Clique na bandeira para filtrar o estado'); ?>
                        </div>
                    </td>
                    <td width="20%">&nbsp;</td>
                </tr>
            </table>
            <table width = 100%>
                <tr>
                    <td width = "50%">
                        <div  align=center style="width: 100%;" id="containerMapa" >
                            <img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
                            <map name="mapaBrasil" id="mapaBrasil">
                                <area shape="rect" coords="48,124,74,151"  href="javascript:void(0);" onclick="listaInstitutos('AC');" title="Acre"/>
                                <area shape="rect" coords="364,147,432,161" href="javascript:void(0);" onclick="listaInstitutos('AL');" title="Alagoas"/>
                                <area shape="rect" coords="202,27,233,56"   href="javascript:void(0);" onclick="listaInstitutos('AP');" title="Amapá"/>
                                <area shape="rect" coords="89,76,133,107"   href="javascript:void(0);" onclick="listaInstitutos('AM');" title="Amazonas"/>
                                <area shape="rect" coords="294,155,320,183" href="javascript:void(0);" onclick="listaInstitutos('BA');" title="Bahia"/>
                                <area shape="rect" coords="311,86,341,114"  href="javascript:void(0);" onclick="listaInstitutos('CE');" title="Ceará"/>
                                <area shape="rect" coords="244,171,281,197" href="javascript:void(0);" onclick="listaInstitutos('DF');" title="Distrito Federal"/>
                                <area shape="rect" coords="331,215,369,242" href="javascript:void(0);" onclick="listaInstitutos('ES');" title="Espírito Santo"/>
                                <area shape="rect" coords="217,187,243,218" href="javascript:void(0);" onclick="listaInstitutos('GO');" title="Goiás"/>
                                <area shape="rect" coords="154,155,210,186" href="javascript:void(0);" onclick="listaInstitutos('MT');" title="Mato Grosso"/>
                                <area shape="rect" coords="156,219,202,246" href="javascript:void(0);" onclick="listaInstitutos('MS');" title="Mato Grosso do Sul"/>
                                <area shape="rect" coords="248,80,301,111"  href="javascript:void(0);" onclick="listaInstitutos('MA');" title="Maranhão"/>
                                <area shape="rect" coords="264,206,295,235" href="javascript:void(0);" onclick="listaInstitutos('MG');" title="Minas Gerais"/>
                                <area shape="rect" coords="188,84,217,112"  href="javascript:void(0);" onclick="listaInstitutos('PA');" title="Pará"/>
                                <area shape="rect" coords="368,112,433,130" href="javascript:void(0);" onclick="listaInstitutos('PB');" title="Paraíba"/>
                                <area shape="rect" coords="201,262,231,289" href="javascript:void(0);" onclick="listaInstitutos('PR');" title="Paraná"/>
                                <area shape="rect" coords="369,131,454,147" href="javascript:void(0);" onclick="listaInstitutos('PE');" title="Pernambuco"/>
                                <area shape="rect" coords="285,116,313,146" href="javascript:void(0);" onclick="listaInstitutos('PI');" title="Piauí"/>
                                <area shape="rect" coords="349,83,383,108"  href="javascript:void(0);" onclick="listaInstitutos('RN');" title="Rio Grande do Norte"/>
                                <area shape="rect" coords="189,310,224,337" href="javascript:void(0);" onclick="listaInstitutos('RS');" title="Rio Grande do Sul"/>
                                <area shape="rect" coords="302,250,334,281" href="javascript:void(0);" onclick="listaInstitutos('RJ');" title="Rio de Janeiro"/>
                                <area shape="rect" coords="98,139,141,169"  href="javascript:void(0);" onclick="listaInstitutos('RO');" title="Rondônia"/>
                                <area shape="rect" coords="112,24,147,56"   href="javascript:void(0);" onclick="listaInstitutos('RR');" title="Roraima"/>
                                <area shape="rect" coords="228,293,272,313" href="javascript:void(0);" onclick="listaInstitutos('SC');" title="Santa Catarina"/>
                                <area shape="rect" coords="233,243,268,270" href="javascript:void(0);" onclick="listaInstitutos('SP');" title="São Paulo"/>
                                <area shape="rect" coords="337,161,401,178" href="javascript:void(0);" onclick="listaInstitutos('SE');" title="Sergipe"/>
                                <area shape="rect" coords="227,130,270,163" href="javascript:void(0);" onclick="listaInstitutos('TO');" title="Tocantins"/>
                            </map>
                        </div>
                    </td>
                    <td>
                        <div id = "popdiv">
                            <?php listaInstitutosRedeFederal(); ?>
                        </div>
                    </td>
                </tr>
            </table>
        </td>

        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',2,85,'','');" align="center" width="30%">
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreRedeFederal('P');" >
            <div>
                <?php exibirTitulo('mapas', 'Mapas dos Institutos', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <div align="center">
                <img class="link" onclick="abreRedeFederal('P')" src="../imagens/cockpit/mapa_cockpit.png"  />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao" width="25%">
            <div>
                <?php exibirTitulo('doc', 'Total de matrículas','(<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>)'); ?>
            </div>
            <?php
            $sql = "SELECT
                            dpe.dpeanoref AS ano,
                            CASE
                                WHEN dsh.tidid1 IN (6995, 6994, 6996, 6997, 6998, 6999, 7867, 7869) OR sh.indid IN (2903) THEN 'Técnico'
                                WHEN dsh.tidid1 IN (6984, 6985, 6986, 6987) THEN 'Pós-Graduação'
                                WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h'
                                WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h'
                                WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                                WHEN dsh.tidid1 IN (6992, 6988, 6989, 6990, 6991, 6993) THEN 'Superior'
                            END AS etapa,
                            SUM(dsh.dshqtde) as total
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid IN (2575, 2905, 2903)
                        AND dpe.dpeanoref >= '2010'
                        GROUP BY etapa, ano
                        ORDER BY etapa, ano";
            $arrDados = $db->carregar($sql);
            $total2010 = 0;
            $total2011 = 0;
            $total2012 = 0;
            $total2013 = 0;
            $total2014 = 0;
            $total2015 = 0;
            $count = -1;
            $arrSituacao = array();
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Tipo</th>
                    <th class="" >2010</th>
                    <th class="" >2011</th>
                    <th class="" >2012</th>
                    <th class="" >2013</th>
                    <th class="" >2014</th>
                    <th class="" >2015</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                    }
                    foreach($arrSituacao as $chave => $sit):
                        $count++;
                        $soma2010 = (is_array($sit['2010']['total'])?array_sum($sit['2010']['total']):0);
                        $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                        $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                        $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                        $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                        $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $chave ?></td>
                            <td class="numero" ><?php echo number_format($soma2010,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2010+$soma2011+$soma2012+$soma2013+$soma2014+$soma2015,0,",",".") ?></td>
                        </tr>
                        <?php
                        $total2010+=$soma2010;
                        $total2011+=$soma2011;
                        $total2012+=$soma2012;
                        $total2013+=$soma2013;
                        $total2014+=$soma2014;
                        $total2015+=$soma2015;
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total2010,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2010+$total2011+$total2012+$total2013+$total2014+$total2015,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2905);" width="25%" colspan="2" rowspan="2">
            <div>
                <?php exibirTitulo('doc', 'Matrículas Bolsa<br>Formação'); ?>
            </div>
            <?php
            $sql = "SELECT
                            dpe.dpeanoref AS ano,
                            CASE
                                WHEN dsh.tidid1 IN (7866) THEN 'FIC'
                                WHEN dsh.tidid1 IN (7867,7869) THEN 'Técnico'
                            ELSE tid1.tiddsc
                            END AS etapa,
                            SUM(dshqtde) AS total
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid = 2905
                        AND dpe.dpeanoref BETWEEN '2011' AND '2015'
                        GROUP BY ano, etapa";
            $arrDados = $db->carregar($sql);
            $total2011 = 0;
            $total2012 = 0;
            $total2013 = 0;
            $total2014 = 0;
            $total2015 = 0;
            $totalMatriculas = 0;
            $count = -1;
            $arrSituacao = array();
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Etapa</th>
                    <th class="" >2011</th>
                    <th class="" >2012</th>
                    <th class="" >2013</th>
                    <th class="" >2014</th>
                    <th class="" >2015</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                    }
                    foreach($arrSituacao as $chave => $sit):
                        $count++;
                        $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                        $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                        $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                        $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                        $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                        $somaMatriculas = $soma2011 + $soma2012 + $soma2013 + $soma2014 + $soma2015;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $chave ?></td>
                            <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($somaMatriculas,0,",",".") ?></td>
                        </tr>
                        <?php
                        $total2011+=$soma2011;
                        $total2012+=$soma2012;
                        $total2013+=$soma2013;
                        $total2014+=$soma2014;
                        $total2015+=$soma2015;
                        $totalMatriculas = $total2011 + $total2012 + $total2013 + $total2014 + $total2015;
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($totalMatriculas,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2903);" width="25%" rowspan="2">
            <div>
                <?php exibirTitulo('doc', 'Matrículas E-Tec'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS ano, SUM(dsh.dshqtde) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid = 2903
                        AND dpe.dpeanoref >= '2011'
                        GROUP BY ano
                        ORDER BY ano";
            $arrDados = $db->carregar($sql);
            $totalMatriculas = 0;
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Ano</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['ano'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                        </tr>
                        <?php
                        $totalMatriculas+= $dado['valor'];
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($totalMatriculas,0,",",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" colspan="2" width="25%" rowspan="2">
            <div>
                <?php exibirTitulo('doc', 'Quadro de Pessoal'); ?>
            </div>
            <?php
            $sql = "SELECT tid1.tiddsc AS etapa, SUM(dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid = 2902
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                        GROUP BY etapa
                        ORDER BY etapa";
            $arrDados = $db->carregar($sql);
            $totalTecnicos = 0;
            $count = -1;
            ?>
            <table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(2902);">
                <tr>
                    <th class="" >Técnicos</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $totalTecnicos+=$dado['valor'];
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['etapa'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                        </tr>
                    <?php
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($totalTecnicos,0,",",".") ?></th>
                </tr>
            </table>
            <br><br>
            <?php
            $sql = "SELECT tid1.tiddsc AS etapa, SUM(dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid = 2901
                        AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                        GROUP BY etapa
                        ORDER BY etapa";
            $arrDados = $db->carregar($sql);
            $totalProfessores = 0;
            $count = -1;
            ?>
            <table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(2901);">
                <tr>
                    <th class="" >Professores</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado):
                        $totalProfessores+=$dado['valor'];
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['etapa'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                        </tr>
                    <?php
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($totalProfessores,0,",",".") ?></th>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2575);">
            <div>
                <?php exibirTitulo('doc', 'Total de matrículas regulares'); ?>
            </div>
            <?php
            $sql = "SELECT
                            dpe.dpeanoref AS ano,
                            CASE
                                WHEN dsh.tidid1 IN (6995, 6994, 6996, 6997, 6998, 6999, 7867, 7869) OR sh.indid IN (2903) THEN 'Técnico'
                                WHEN dsh.tidid1 IN (6984, 6985, 6986, 6987) THEN 'Pós-Graduação'
                                WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h Regular'
                                WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h Regular'
                                WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                                WHEN dsh.tidid1 IN (6992, 6988, 6989, 6990, 6991, 6993) THEN 'Superior'
                            END AS etapa,
                            SUM(dsh.dshqtde) as total
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.sehstatus <> 'I'
                        AND sh.indid IN (2575)
                        GROUP BY etapa, ano
                        ORDER BY etapa, ano";
            $arrDados = $db->carregar($sql);
            $total2011 = 0;
            $total2012 = 0;
            $total2013 = 0;
            $total2014 = 0;
            $total2015 = 0;
            $count = -1;
            $arrSituacao = array();
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <th class="" >Tipo</th>
                    <th class="" >2011</th>
                    <th class="" >2012</th>
                    <th class="" >2013</th>
                    <th class="" >2014</th>
                    <th class="" >2015</th>
                    <th class="" >Total</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['etapa']][$dado['ano']]['total'][] = $dado['total'];
                    }
                    foreach($arrSituacao as $chave => $sit):
                        $count++;
                        $soma2011 = (is_array($sit['2011']['total'])?array_sum($sit['2011']['total']):0);
                        $soma2012 = (is_array($sit['2012']['total'])?array_sum($sit['2012']['total']):0);
                        $soma2013 = (is_array($sit['2013']['total'])?array_sum($sit['2013']['total']):0);
                        $soma2014 = (is_array($sit['2014']['total'])?array_sum($sit['2014']['total']):0);
                        $soma2015 = (is_array($sit['2015']['total'])?array_sum($sit['2015']['total']):0);
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $chave ?></td>
                            <td class="numero" ><?php echo number_format($soma2011,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2012,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2013,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2014,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2015,0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($soma2011+$soma2012+$soma2013+$soma2014+$soma2015,0,",",".") ?></td>
                        </tr>
                        <?php
                        $total2011+=$soma2011;
                        $total2012+=$soma2012;
                        $total2013+=$soma2013;
                        $total2014+=$soma2014;
                        $total2015+=$soma2015;
                    endforeach;
                }?>
                <tr>
                    <th class="bold" >Total</th>
                    <th class="numero" ><?php echo number_format($total2011,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2012,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2013,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2014,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2015,0,",",".") ?></th>
                    <th class="numero" ><?php echo number_format($total2011+$total2012+$total2013+$total2014+$total2015,0,",",".") ?></th>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <?php
            $sql="SELECT dpe.dpeordem || '/' || dpe.dpeanoref AS edicao
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.indid = 2671";
            $edicao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('doc', 'SISU - Edição '.$edicao); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 2671";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" <?= ($valor ? 'onclick="abreIndicadorPopUp(2671);"' : '') ?>>
                    <td class="bold subtitulo_box">Vagas disponibilizadas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 1435";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(1435);">
                    <td class="bold subtitulo_box">Inscrições</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'Bolsa Permanência'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 2019";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(2019);">
                    <td class="bold subtitulo_box">Quantidade de bolsas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'PROEXT'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 334";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(334);">
                    <td class="bold subtitulo_box">Projetos apoiados</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'PARFOR'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
						AND sh.indid = 1367";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(1367);">
                    <td class="bold subtitulo_box">Turmas ativas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 1513";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(1513);">
                    <td class="bold subtitulo_box">Matrículas ativas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'PIBID'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 108";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(108);">
                    <td class="bold subtitulo_box">Bolsas aprovadas</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao"  >
            <div>
                <?php exibirTitulo('doc', 'PET'); ?>
            </div>
            <table border="0" cellpadding="5" cellspacing="5" width="100%" >
                <?php
                $sql = "SELECT SUM(dsh.dshqtde) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.sehstatus <> 'I'
						AND sh.indid = 580";
                $valor = $db->pegaUm($sql);
                ?>
                <tr class="link" onclick="abreIndicadorPopUp(580);">
                    <td class="bold subtitulo_box">Grupos participantes</td>
                    <td class="numero" ><?php echo number_format($valor,0,",",".") ?></td>
                </tr>
            </table>
        </td>
    </tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
<tr>
    <td class="fundo_padrao" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="3" onclick="" />Bolsa-Formação
            <input type="radio" class="filtrar_redefederal1" name="redefederal1" value="4" onclick="" />e-Tec
        </div>
        <br>
        <div id="div_redefederal1">
            <?php gerarGraficoRedeFederal1(1); ?>
        </div>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(2);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Evolução de ingressantes', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="3" onclick="" />Bolsa-Formação
            <input type="radio" class="filtrar_redefederal2" name="redefederal2" value="4" onclick="" />e-Tec
        </div>
        <br>
        <div id="div_redefederal2">
            <?php gerarGraficoRedeFederal2(1); ?>
        </div>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(3);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas - FIC', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal6" name="redefederal6" value="3" onclick="" />Bolsa-Formação
        </div>
        <br>
        <div id="div_redefederal6">
            <?php gerarGraficoRedeFederal6(1); ?>
        </div>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(15);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Evolução de ingressantes - FIC', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2905);">Bolsa-Formação</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal7" name="redefederal7" value="3" onclick="" />Bolsa-Formação
        </div>
        <br>
        <div id="div_redefederal7">
            <?php gerarGraficoRedeFederal7(1); ?>
        </div>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(16);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Evolução de matrículas atendidas - Técnico', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="3" onclick="" />Bolsa-Formação
            <input type="radio" class="filtrar_redefederal3" name="redefederal3" value="4" onclick="" />e-Tec
        </div>
        <br>
        <div id="div_redefederal3">
            <?php gerarGraficoRedeFederal3(1); ?>
        </div>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(4);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Evolução de ingressantes - Técnico', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2903);">e-Tec</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="3" onclick="" />Bolsa-Formação
            <input type="radio" class="filtrar_redefederal4" name="redefederal4" value="4" onclick="" />e-Tec
        </div>
        <br>
        <div id="div_redefederal4">
            <?php gerarGraficoRedeFederal4(1); ?>
        </div>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(5);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" width="50%">
        <div class="link" onclick="abreIndicadorPopUp(2575);" >
            <?php exibirTitulo('indicador', 'Evolução de matrículas de graduação'); ?>
        </div>
        <?php
        $arrEtapa = array();
        $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6990, 6993) THEN 'Tecnólogo'
                    WHEN dsh.tidid1 IN (6992, 6989) THEN 'Licenciatura'
                    WHEN dsh.tidid1 IN (6988, 6991) THEN 'Bacharelado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            AND sh.indid IN (2575)
            AND dsh.tidid1 IN (6990, 6993, 6992, 6989, 6988, 6991)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $_SESSION['exportar6'] = $arrDados;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
            }

            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrTotal = array();

            foreach($arrEtapa as $periodo => $sit)
            {
                $arrX[] = $periodo;
                $vA = Round((is_array($sit['Tecnólogo']['total'])?array_sum($sit['Tecnólogo']['total']):0));
                $vB = Round((is_array($sit['Licenciatura']['total'])?array_sum($sit['Licenciatura']['total']):0));
                $vC = Round((is_array($sit['Bacharelado']['total'])?array_sum($sit['Bacharelado']['total']):0));
                $vTotal = 0;

                $arrA[] = $vA;
                $vTotal += $vA;
                $arrB[] = $vB;
                $vTotal += $vB;
                $arrC[] = $vC;
                $vTotal += $vC;
                $arrTotal[] = $vTotal;
            }

            $aDados = array(
                array('name' => 'Tecnologo', 'data'=>$arrA),
                array('name' => 'Licenciatura', 'data'=>$arrB),
                array('name' => 'Bacharelado', 'data'=>$arrC),
                array('name' => 'Total por Ano', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasGraduacao", "Etapas/Modalidades",300,$formatoValores);
        }
        ?>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(6);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao" width="50%">
        <div class="link" onclick="abreIndicadorPopUp(2347);">
            <?php exibirTitulo('indicador', 'Evolução de ingressantes de graduação'); ?>
        </div>
        <?php
        $arrEtapa = array();
        $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5141, 5133) THEN 'Tecnólogo'
                    WHEN dsh.tidid2 IN (5140, 5134) THEN 'Licenciatura'
                    WHEN dsh.tidid2 IN (5135, 5139) THEN 'Bacharelado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            AND sh.indid IN (2347)
            AND dsh.tidid2 IN (5141, 5133, 5140, 5134, 5135, 5139)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $_SESSION['exportar7'] = $arrDados;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
            }

            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrTotal = array();

            foreach($arrEtapa as $periodo => $sit)
            {
                $arrX[] = $periodo;
                $vA = Round((is_array($sit['Tecnólogo']['total'])?array_sum($sit['Tecnólogo']['total']):0));
                $vB = Round((is_array($sit['Licenciatura']['total'])?array_sum($sit['Licenciatura']['total']):0));
                $vC = Round((is_array($sit['Bacharelado']['total'])?array_sum($sit['Bacharelado']['total']):0));
                $vTotal = 0;

                $arrA[] = $vA;
                $vTotal += $vA;
                $arrB[] = $vB;
                $vTotal += $vB;
                $arrC[] = $vC;
                $vTotal += $vC;
                $arrTotal[] = $vTotal;
            }

            $aDados = array(
                array('name' => 'Tecnologo', 'data'=>$arrA),
                array('name' => 'Licenciatura', 'data'=>$arrB),
                array('name' => 'Bacharelado', 'data'=>$arrC),
                array('name' => 'Total por Ano', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesGraduacao", "Etapas/Modalidades",300,$formatoValores);
        }
        ?>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(7);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" width="50%">
        <div class="link" onclick="abreIndicadorPopUp(2575);">
            <?php exibirTitulo('indicador', 'Evolução de matrículas de pós-graduação'); ?>
        </div>
        <?php
        $arrEtapa = array();
        $sql = "SELECT
                        dpe.dpeanoref AS periodo,
                        CASE
                            WHEN dsh.tidid1 IN (6985) THEN 'Especialização'
                            WHEN dsh.tidid1 IN (6987) THEN 'Mestrado profissional'
                            WHEN dsh.tidid1 IN (6986) THEN 'Mestrado acadêmico'
                            WHEN dsh.tidid1 IN (6984) THEN 'Doutorado'
                        END AS etapa,
                        sum(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    AND sh.indid IN (2575)
                    AND dsh.tidid1 IN (6985, 6987, 6986, 6984)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref >= '2010'
                    GROUP BY periodo, etapa
                    ORDER BY periodo, etapa";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $_SESSION['exportar8'] = $arrDados;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
            }

            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrD = array();
            $arrTotal = array();

            foreach($arrEtapa as $periodo => $sit)
            {
                $arrX[] = $periodo;
                $vA = Round((is_array($sit['Especialização']['total'])?array_sum($sit['Especialização']['total']):0));
                $vB = Round((is_array($sit['Mestrado profissional']['total'])?array_sum($sit['Mestrado profissional']['total']):0));
                $vC = Round((is_array($sit['Mestrado acadêmico']['total'])?array_sum($sit['Mestrado acadêmico']['total']):0));
                $vD = Round((is_array($sit['Doutorado']['total'])?array_sum($sit['Doutorado']['total']):0));
                $vTotal = 0;

                $arrA[] = $vA;
                $vTotal += $vA;
                $arrB[] = $vB;
                $vTotal += $vB;
                $arrC[] = $vC;
                $vTotal += $vC;
                $arrD[] = $vD;
                $vTotal += $vD;
                $arrTotal[] = $vTotal;
            }

            $aDados = array(
                array('name' => 'Especializacao', 'data'=>$arrA),
                array('name' => 'Mestrado profissional', 'data'=>$arrB),
                array('name' => 'Mestrado academico', 'data'=>$arrC),
                array('name' => 'Doutorado', 'data'=>$arrD),
                array('name' => 'Total por Ano', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasPosGraduacao", "Etapas/Modalidades",300,$formatoValores);
        }
        ?>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(8);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao" width="50%">
        <div class="link" onclick="abreIndicadorPopUp(2347);" >
            <?php exibirTitulo('indicador', 'Evolução de ingressantes de pós-graduação'); ?>
        </div>
        <?php
        $arrEtapa = array();
        $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5136) THEN 'Especialização'
                    WHEN dsh.tidid2 IN (5191) THEN 'Mestrado profissional'
                    WHEN dsh.tidid2 IN (5137) THEN 'Mestrado acadêmico'
                    WHEN dsh.tidid2 IN (5138) THEN 'Doutorado'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            AND sh.indid IN (2347)
            AND dsh.tidid2 IN (5136, 5191, 5137, 5138)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $_SESSION['exportar9'] = $arrDados;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
            }

            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrD = array();
            $arrTotal = array();

            foreach($arrEtapa as $periodo => $sit)
            {
                $arrX[] = $periodo;
                $vA = Round((is_array($sit['Especialização']['total'])?array_sum($sit['Especialização']['total']):0));
                $vB = Round((is_array($sit['Mestrado profissional']['total'])?array_sum($sit['Mestrado profissional']['total']):0));
                $vC = Round((is_array($sit['Mestrado acadêmico']['total'])?array_sum($sit['Mestrado acadêmico']['total']):0));
                $vD = Round((is_array($sit['Doutorado']['total'])?array_sum($sit['Doutorado']['total']):0));
                $vTotal = 0;

                $arrA[] = $vA;
                $vTotal += $vA;
                $arrB[] = $vB;
                $vTotal += $vB;
                $arrC[] = $vC;
                $vTotal += $vC;
                $arrD[] = $vD;
                $vTotal += $vD;
                $arrTotal[] = $vTotal;
            }

            $aDados = array(
                array('name' => 'Especializacao', 'data'=>$arrA),
                array('name' => 'Mestrado profissional', 'data'=>$arrB),
                array('name' => 'Mestrado academico', 'data'=>$arrC),
                array('name' => 'Doutorado', 'data'=>$arrD),
                array('name' => 'Total por Ano', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesPosGraduacao", "Etapas/Modalidades",300,$formatoValores);
        }
        ?>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(9);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('indicador', 'Unidades em funcionamento'); ?>
        </div>
        <?
        $sql = "SELECT
                    CASE
                        WHEN c.cmpinstalacao = 'D' THEN 'Instalações Definitivas'
                        WHEN c.cmpinstalacao = 'P' THEN 'Instalações Provisórias'
                    ELSE 'Sem Instalações'
                    END AS descricao,
                    COUNT(0) AS valor
                FROM entidade.entidade entcampus
                INNER JOIN entidade.funcaoentidade fn ON entcampus.entid = fn.entid AND fn.funid = 17 AND entcampus.entstatus = 'A' AND fn.fuestatus = 'A'
                INNER JOIN academico.campus c ON c.entid = entcampus.entid
                WHERE c.exiid IN (2,3,4,5)
                AND c.cmpsituacao = 'F'
                GROUP BY descricao
                ORDER BY descricao";
        $arrDados = $db->carregar( $sql );
        $_SESSION['exportar10'] = $arrDados;
        if($arrDados){
            geraGrafico($arrDados, "graficoPizzaUnidadesFuncionamento", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", true);
        }
        ?>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(10);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao" colspan="2">
        <div class="link" onclick="abreIndicadorPopUp(2294);">
            <?php exibirTitulo('indicador', 'Evolução das obras da Expansão Fase III (2011-2014)', 'Câmpus por situação'); ?>
        </div>
        <?
        $arrSituacao = array();
        $sql = "select dpe.dpedsc AS periodo, dpe.dpeanoref, dpe.dpeordem, sum(dsh.dshqtde) as total, tid.tiddsc as situacao
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
					where sh.indid in (2294)
					and sh.sehstatus <> 'I'
					and dpe.dpedatainicio >= '2014/08/01'
					group by dpe.dpedsc, dpe.dpeanoref, dpe.dpeordem, situacao
					order by dpe.dpeanoref, dpe.dpeordem, situacao";
        $arrDados = $db->carregar( $sql );
        $_SESSION['exportar11'] = $arrDados;
        if($arrDados){
            foreach($arrDados as $dado){
                $arrSituacao[$dado['periodo']][$dado['situacao']]['total'][] = $dado['total'];
            }
        }
        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrD = array();
        $arrE = array();
        $arrF = array();
        $arrG = array();
        $arrH = array();
        $arrI = array();
        $arrJ = array();
        $arrK = array();

        foreach($arrSituacao as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $arrA[] = Round((is_array($sit['Em Execução']['total'])?array_sum($sit['Em Execução']['total']):0));
            $arrB[] = Round((is_array($sit['Paralisada']['total'])?array_sum($sit['Paralisada']['total']):0));
            $arrC[] = Round((is_array($sit['Concluída']['total'])?array_sum($sit['Concluída']['total']):0));
            $arrD[] = Round((is_array($sit['Em Elaboração de Projetos']['total'])?array_sum($sit['Em Elaboração de Projetos']['total']):0));
            $arrE[] = Round((is_array($sit['Em Licitação']['total'])?array_sum($sit['Em Licitação']['total']):0));
            $arrF[] = Round((is_array($sit['Em Reformulação']['total'])?array_sum($sit['Em Reformulação']['total']):0));
            $arrG[] = Round((is_array($sit['Obra Cancelada']['total'])?array_sum($sit['Obra Cancelada']['total']):0));
            $arrH[] = Round((is_array($sit['Etapa Concluída']['total'])?array_sum($sit['Etapa Concluída']['total']):0));
            $arrI[] = Round((is_array($sit['Aguardando registro de preços']['total'])?array_sum($sit['Aguardando registro de preços']['total']):0));
            $arrJ[] = Round((is_array($sit['Em Planejamento pelo Proponente']['total'])?array_sum($sit['Em Planejamento pelo Proponente']['total']):0));
            $arrK[] = Round((is_array($sit['Infraestrutura recebida']['total'])?array_sum($sit['Infraestrutura recebida']['total']):0));
        }

        $aDados = array(
            array('name' => 'Em Execucao', 'data'=>$arrA),
            array('name' => 'Paralisada', 'data'=>$arrB),
            array('name' => 'Concluida', 'data'=>$arrC),
            //array('name' => 'Em Elaboracao de Projetos', 'data'=>$arrD),
            array('name' => 'Em Licitacao', 'data'=>$arrE),
            //array('name' => 'Em Reformulacao', 'data'=>$arrF),
            //array('name' => 'Obra Cancelada', 'data'=>$arrG),
            //array('name' => 'Etapa Concluida', 'data'=>$arrH),
            //array('name' => 'Aguardando registro de precos', 'data'=>$arrI),
            array('name' => 'Em Planejamento pelo Proponente', 'data'=>$arrJ),
            array('name' => 'Infraestrutura recebida', 'data'=>$arrK),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempo", "Etapas",300, $formatoValores);
        ?>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(11);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" colspan="2">
        <div class="link" onclick="abreIndicadorPopUp(2897);">
            <?php exibirTitulo('indicador', 'Investimento Total - Institutos Federais de Educação, Ciência e Tecnologia'); ?>
        </div>
        <?php
        $sql = "SELECT dpe.dpeanoref AS periodo, tid1.tidid AS modalidade, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (2897)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
        $arrDados = $db->carregar( $sql, null, 3200 );
        $_SESSION['exportar12'] = $arrDados;
        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrC = array();
        $arrSituacao = array();
        $arrTotal = array();
        if($arrDados){
            foreach($arrDados as $dado){
                $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
            }
            foreach($arrSituacao as $periodo => $sit){
                $vA = Round((is_array($sit['7838']['total'])?array_sum($sit['7838']['total']):0));
                $vB = Round((is_array($sit['7839']['total'])?array_sum($sit['7839']['total']):0));
                $vC = Round((is_array($sit['7840']['total'])?array_sum($sit['7840']['total']):0));
                $vTotal = 0;
                $arrX[] = ($periodo=='2014' ? '*2014' : $periodo);
                $arrA[] = $vA;
                $vTotal += $vA;
                $arrB[] = $vB;
                $vTotal += $vB;
                $arrC[] = $vC;
                $vTotal += $vC;
                $arrTotal[] = $vTotal;
            }

            $aDados = array(
                array('name' => 'Pessoal Ativo e Encargos Sociais', 'data'=>$arrA),
                array('name' => 'Outras Despesas Correntes (Inclui Juros e Encargos da Divida)', 'data'=>$arrB),
                array('name' => 'Investimentos (Inclui Inversoes Financeiras e Amortizacao da Divida)', 'data'=>$arrC),
                array('name' => 'Total por Ano', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoDespesaUniversidades", "Investimento Total",300, $formatoValores);
        }
        ?>
        <br><br>
        <table><tr><td class="bold">*2014: Orçamento Previsto</td></tr></table>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(12);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao link" colspan="2">
        <div>
            <?php exibirTitulo('indicador', 'Linha do tempo da criação/federalização das unidades'); ?>
        </div>
        <?php
        $sql = "SELECT count(distinct entid) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (525)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND dsh.tidid1 IN (2596, 2597)
					AND sh.sehstatus <> 'I'";
        $qtd = $db->pegaUm($sql);

        $dados = array(
            array('descricao'=>"Nilo Peçanha<br />1909-1910", 'valor'=>'20'),
            array('descricao'=>"Hermes da Fonseca<br />1911-1914", 'valor'=>'1'),
            array('descricao'=>"Venceslau Brás<br />1914-1918", 'valor'=>'1'),
            array('descricao'=>"Delfim Moreira<br />1918-1919", 'valor'=>'1'),
            array('descricao'=>"Epitácio Pessoa<br />1919-1922", 'valor'=>'3'),
            array('descricao'=>"Artur Bernardes<br />1922-1926", 'valor'=>'1'),
            array('descricao'=>"Getúlio Vargas<br />1930-1945 e 1951-1954 ", 'valor'=>'15'),
            array('descricao'=>"Gaspar Dutra<br />1946-1951", 'valor'=>'11'),
            array('descricao'=>"Café Filho<br />1954-1955", 'valor'=>'1'),
            array('descricao'=>"Juscelino Kubitschek<br />1956-1961", 'valor'=>'7'),
            array('descricao'=>"João Goulart<br />1961-1964", 'valor'=>'8'),
            array('descricao'=>"Ranieri Mazzilli<br />1964-1964", 'valor'=>'1'),
            array('descricao'=>"Castelo Branco<br />1964-1967", 'valor'=>'2'),
            array('descricao'=>"Costa e Silva<br />1967-1969", 'valor'=>'9'),
            array('descricao'=>"Emílio G. Médici<br />1969-1974", 'valor'=>'2'),
            array('descricao'=>"Ernesto Geisel<br />1974-1979", 'valor'=>'1'),
            array('descricao'=>"João B. Figueiredo<br />1979-1985", 'valor'=>'4'),
            array('descricao'=>"José Sarney<br />1985-1990", 'valor'=>'11'),
            array('descricao'=>"Fernando Collor de Mello<br />1990-1992", 'valor'=>'4'),
            array('descricao'=>"Itamar Franco<br />1992-1995", 'valor'=>'26'),
            array('descricao'=>"Fernando Henrique Cardoso<br />1995-2003", 'valor'=>'11'),
            array('descricao'=>"Luiz Inácio Lula da Silva<br />2003-2010", 'valor'=>'214'),
            array('descricao'=>"Dilma Rousseff<br />2011-2014", 'valor'=>$qtd),
        );
        $totalCampus = 354 + $qtd;
        $config = array('rotation'=>'-45', 'dataLabels'=>'true');
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColuna($dados, 'graficoColunaExpansao', null, null, $formatoValores, null, 'estrategico.php?modulo=principal/popuplistaunidades&acao=A', 1000, 600, null, 300, $config);
        ?>
        <table width="100%">
            <tr>
                <td class="titulo_box bold" align="center"><?php echo number_format($totalCampus,0,",",".") ?> campus</td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" colspan="1">
        <div>
            <?php exibirTitulo('indicador', 'Comparação do Esforço Escolar (em horas-aula)', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal EPT</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2399);">Acordo de Gratuidade</span>, <span class="link" onclick="abreIndicadorPopUp(2393);">E-TEC</span>'); ?>
        </div>
        <?php
        $sql = "select rede, tipo, sum(valor) as valor
					from(
						select
							'Rede Federal' as rede,
							'Técnico' as tipo,
							case
								when dsh.tidid2 IN (5130) then sum(dsh.dshqtde) * 3000
								when dsh.tidid2 IN (5188) then sum(dsh.dshqtde) * 2400
							else sum(dsh.dshqtde) * 1000
							end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2347)
						AND sh.sehstatus <> 'I'
						AND dsh.tidid2 IN (5130,5131,5188,5190,5189)
						AND dpe.dpeanoref >= '2011'
						GROUP BY dsh.tidid2
					UNION ALL
						select
							'Rede Federal' as rede,
							'Total' as tipo,
							case
								when dsh.tidid2 IN (5130) then sum(dsh.dshqtde) * 3000
								when dsh.tidid2 IN (5188) then sum(dsh.dshqtde) * 2400
							else sum(dsh.dshqtde) * 1000
							end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2347)
						AND sh.sehstatus <> 'I'
						AND dsh.tidid2 IN (5130,5131,5188,5190,5189)
						AND dpe.dpeanoref >= '2011'
						GROUP BY dsh.tidid2
					union all
						select
							tid2.tiddsc as rede,
							case when dsh.tidid1 = 5302 then 'FIC' else 'Técnico' end as tipo,
							case when dsh.tidid1 = 5302 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2401)
						AND sh.sehstatus <> 'I'
						and dsh.tidid2 IN (5323, 5324, 5325)
						group by rede, tipo, dsh.tidid1
					union all
						select
							tid2.tiddsc as rede,
							'Total' as tipo,
							case when dsh.tidid1 = 5302 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2401)
						AND sh.sehstatus <> 'I'
						and dsh.tidid2 IN (5323, 5324, 5325)
						group by rede, tipo, dsh.tidid1
					union all
						select
							tid2.tiddsc as rede,
							tid1.tiddsc as tipo,
							case when dsh.tidid1 = 5298 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2399)
						AND sh.sehstatus <> 'I'
						group by rede, tipo, dsh.tidid1
					union all
						select
							tid2.tiddsc as rede,
							'Total' as tipo,
							case when dsh.tidid1 = 5298 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2399)
						AND sh.sehstatus <> 'I'
						group by rede, tipo, dsh.tidid1
					union all
						SELECT
							'Rede Federal' as rede,
							'Técnico' as tipo,
							sum(dsh.dshqtde) * 1000 as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2393)
						AND sh.sehstatus <> 'I'
						AND dpe.dpeanoref >= '2011'
						AND dsh.tidid1 IN (5209)
					UNION ALL
						SELECT
							'Rede Federal' as rede,
							'Total' as tipo,
							sum(dsh.dshqtde) * 1000 as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2393)
						AND sh.sehstatus <> 'I'
						AND dpe.dpeanoref >= '2011'
						AND dsh.tidid1 IN (5209)
					) as foo
					group by rede, tipo
					order by rede, tipo";
        $dados = $db->carregar( $sql, null, 3200 );
        $_SESSION['exportar13'] = $dados;
        $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );

        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaEsforcoEscolar', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
        ?>
        <table width="100%">
            <tr>
                <td width="50%">
                    Cursos técnicos conc. e subseq. = 1000h<br>
                    Cursos técnicos integrados = 3000h<br>
                    Cursos PROEJA integrados = 2400h<br>
                    Cursos FIC = 200h
                </td>
                <td width="50%" align="right">
                    Esforço Escolar = Carga Horária X Nº de Matrículas ingressantes
                </td>
            </tr>
        </table>
        <table>
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(13);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao">
        <div>
            <?php exibirTitulo('indicador', 'Ingressantes Etapa / Modalidade', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal EPT</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2393);">E-TEC</span>'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="2" onclick="" />Rede Federal
            <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="3" onclick="" />Bolsa-Formação
            <input type="radio" class="filtrar_redefederal5" name="redefederal5" value="4" onclick="" />e-Tec
        </div>
        <br>
        <div id="div_redefederal5">
            <?php gerarGraficoRedeFederal5(1); ?>
        </div>
        <table width="100%">
            <tr>
                <td class="fundo_padrao" align="left">
                    <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(14);" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao toggle_consolidado1 link" COLSPAN="2">
        <div>
            <?php exibirTitulo('indicador', 'Matrículas ingressantes<br>em regiões específicas<br>2011 a 2015'); ?>
        </div>
    </td>
</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="0" cellpadding="0" class="tabela_painel">
    <tr id="graficos_consolidados1">
        <td width="100%">
            <table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" class="tabela_painel">
                <tr>
                    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);" width="50%">
                        <div>
                            <?php exibirTitulo('indicador', 'Áreas Indígenas'); ?>
                        </div>
                        <?
                        $sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2015'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 3)
							GROUP BY descricao
							ORDER BY descricao";
                        $arrDados = $db->carregar( $sql, null, 3200 );
                        geraGrafico($arrDados, "graficoPizzaIndio", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        ?>
                    </td>
                    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);">
                        <div>
                            <?php exibirTitulo('indicador', 'Semiárido'); ?>
                        </div>
                        <?
                        $sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2015'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 12)
							GROUP BY descricao
							ORDER BY descricao";
                        $arrDados = $db->carregar( $sql, null, 3200 );

                        geraGrafico($arrDados, "graficoPizzaSemiarido", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);">
                        <div>
                            <?php exibirTitulo('indicador', 'Territórios da Cidadania'); ?>
                        </div>
                        <?
                        $sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2015'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 5)
							GROUP BY descricao
							ORDER BY descricao";
                        $arrDados = $db->carregar( $sql, null, 3200 );

                        geraGrafico($arrDados, "graficoPizzaTerritorioCidadania", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        ?>
                    </td>
                    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);">
                        <div>
                            <?php exibirTitulo('indicador', 'Grandes Cidades e Capitais'); ?>
                        </div>
                        <?
                        $sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2015'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 1)
							GROUP BY descricao
							ORDER BY descricao";
                        $arrDados = $db->carregar( $sql, null, 3200 );

                        geraGrafico($arrDados, "graficoPizzaGrandesCidades", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        ?>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_td_laranja" colspan="2">
            <div style="text-align:center;"  >
                <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
                <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
                <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
            </div>
        </td>
    </tr>
</table>
</body>
</html>
<?php
//GERAR DADOS EM EXCEL
$gerarExcel = $_REQUEST['tipo'];
switch($gerarExcel){
    case 1:
        //Ingressantes em cursos técnicos regulares – Meta Pronatec
        $sql = "SELECT dpe.dpeanoref AS Ano, tid1.tiddsc AS Situacao, tid2.tiddsc AS Modalidade, sum(dsh.dshqtde)::integer AS Matriculas
			FROM painel.seriehistorica sh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
			WHERE sh.indid IN (2347)
			AND sh.sehstatus <> 'I'
			AND dsh.tidid2 IN (5130,5131,5188,5190,5189)
			AND dpe.dpeanoref >= '2011'
			GROUP BY dpe.dpeanoref, tid1.tiddsc, tid2.tiddsc
			ORDER BY dpe.dpeanoref, tid1.tiddsc, tid2.tiddsc";
        $dadosExcel = $db->carregar( $sql );
        ob_clean();
        $db->sql_to_excel( $dadosExcel, "Ingressantes_Cursos_Tecnicos_Regulares" );
        break;
    case 2:
        //Evolução de matrículas atendidas
        $sql = "SELECT
                dpe.dpeanoref AS ano,
                CASE
                    WHEN dsh.tidid1 IN (6994,6995,6996,6997,6998,6999, 5303, 5304) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid1 IN (6987,6986,6985,6984) THEN 'Pós-Graduação'
                    WHEN dsh.tidid1 IN (7000, 9264, 5302) THEN 'FIC'
                    WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                    WHEN dsh.tidid1 IN (6988,6989,6990,6991,6992,6993) THEN 'Superior'
                END AS etapa,
                sum(dsh.dshqtde)::integer as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE (sh.indid in (2575, 2903) OR (sh.indid in (2401) AND dsh.tidid2 IN (5323)))
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY ano, etapa
            ORDER BY ano, etapa";
        $dadosExcel = $db->carregar( $sql );
        ob_clean();
        $db->sql_to_excel( $dadosExcel, "Evolucao_Matriculas_Atendidas" );
        break;
    case 3:
        //Evolução de ingressantes
        $sql = "SELECT dpe.dpeanoref as periodo,
                CASE
                        WHEN dsh.tidid2 IN (5130, 5131, 5132, 5188, 5190, 5189) OR dsh.tidid1 IN (5303, 5304) OR sh.indid in (2903) THEN 'Técnico'
                        WHEN dsh.tidid2 IN (5136, 5137, 5138, 5191) THEN 'Pós-Graduação'
                        WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302) THEN 'FIC'
                        WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
                        WHEN dsh.tidid2 IN (5135, 5140, 5141, 5133, 5134, 5139) THEN 'Superior'
                END AS etapa,
                SUM(dsh.dshqtde)::integer AS total
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE (sh.indid in (2347, 2903) OR (sh.indid in (2401) AND dsh.tidid2 IN (5323)))
                AND sh.sehstatus <> 'I'
                AND dpe.dpeanoref >= '2010'
                GROUP BY periodo, etapa
                ORDER BY periodo, etapa";
        $dadosExcel = $db->carregar( $sql );
        ob_clean();
        $db->sql_to_excel( $dadosExcel, "Evolucao_de_ingressantes" );
        break;
    case 4:
        //Evolução de matrículas atendidas - Técnico
        $sql = "SELECT
                    dpe.dpeanoref AS periodo,
                    CASE
                        WHEN dsh.tidid1 IN (6994, 7867, 5303) THEN 'Técnico concomitante'
                        WHEN dsh.tidid1 IN (6995) THEN 'Técnico integrado'
                        WHEN dsh.tidid1 IN (6996) THEN 'Técnico PROEJA concomitante'
                        WHEN dsh.tidid1 IN (6997) THEN 'Técnico PROEJA integrado'
                        WHEN dsh.tidid1 IN (6999, 7869, 5304) THEN 'Técnico subsequente presencial'
                        WHEN dsh.tidid1 IN (6998) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
                    END AS etapa,
                    sum(dsh.dshqtde) as total
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE (
                        (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))
                        OR
                        (sh.indid in (2401) AND dsh.tidid2 IN (5323) AND dsh.tidid1 NOT IN (5302))
                        OR
                        (sh.indid in (2903))
                      )
                AND sh.sehstatus <> 'I'
                AND dpe.dpeanoref >= '2010'
                GROUP BY periodo, etapa
                ORDER BY periodo, etapa";
        $dadosExcel = $db->carregar( $sql );
        ob_clean();
        $db->sql_to_excel( $dadosExcel, "Evolucao_matriculas_atendidas_tecnico");
        break;
    case 5:
        //Evolução de ingressantes - Técnico
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar5'], "Evolucao_ingressantes_tecnico");
        break;
    case 6:
        //Evolução de matrículas de graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar6'], "Evolucao_matriculas_graduacao");
        break;
    case 7:
        //Evolução de ingressantes de graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar7'], "Evolucao_ingressantes_graduacao");
        break;
    case 8:
        //Evolução de matrículas de pós-graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar8'], "Evolucao_matriculas_posgraduacao");
        break;
    case 9:
        //Evolução de ingressantes de pós-graduação
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar9'], "Evolucao_ingressantes_posgraduacao");
        break;
    case 10:
        //Unidades em funcionamento
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar10'], "Unidades_em_funcionamento");
        break;
    case 11:
        //Evolução das obras da Expansão Fase III (2011-2014)
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar11'], "Evolucao_obras");
        break;
    case 12:
        //Investimento Total - Institutos Federais de Educação, Ciência e Tecnologia
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar12'], "Investimento_total");
        break;
    case 13:
        //Comparação do Esforço Escolar (em horas-aula)
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar13'], "Esforco_escolar");
        break;
    case 14:
        //Ingressantes etapa/modalidade
        $sql = "SELECT
				dpe.dpeanoref AS ano,
				CASE
					WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) OR dsh.tidid1 IN (5303, 5304) OR sh.indid in (2393) THEN 'Técnico'
					WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
					WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302) THEN 'FIC'
					WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
					WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
				END AS tipo,
				tid1.tiddsc AS situacao,
				sum(dsh.dshqtde)::integer as matricula
			FROM painel.seriehistorica sh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
			WHERE
                (
                    sh.indid in (2347)
                    OR
                    (sh.indid in (2401) AND dsh.tidid2 IN (5323))
                    OR
                    (sh.indid in (2393) AND dsh.tidid1 IN (5209) AND dpe.dpeanoref >= '2011')
                )
			AND sh.sehstatus <> 'I'
			GROUP BY ano, tipo, situacao
			ORDER BY ano, tipo, situacao";
        $dadosExcel = $db->carregar( $sql );
        ob_clean();
        $db->sql_to_excel( $dadosExcel, "Ingressantes_Etapa_Modalidade" );
        break;
    case 15:
        //Evolução de matrículas atendidas - FIC
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar15'], "Evolucao_matriculas_fic");
        break;
    case 16:
        //Evolução de ingressantes - FIC
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar16'], "Evolucao_ingressantes_tecnico");
        break;
}

function gerarGraficoRedeFederal1($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2575))";
        break;
        case 3:
            $where = "WHERE (sh.indid in (2401) AND dsh.tidid2 IN (5323))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (sh.indid in (2575, 2903) OR (sh.indid in (2401) AND dsh.tidid2 IN (5323)))";
        break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (6994,6995,6996,6997,6998,6999, 5303, 5304) OR sh.indid in (2903) THEN 'Técnico'
                    WHEN dsh.tidid1 IN (6987,6986,6985,6984) THEN 'Pós-Graduação'
                    WHEN dsh.tidid1 IN (7000, 9264, 5302) THEN 'FIC'
                    WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
                    WHEN dsh.tidid1 IN (6988,6989,6990,6991,6992,6993) THEN 'Superior'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar2'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }
    }
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrD = array();
    $arrE = array();
    $arrTotal = array();

    foreach($arrEtapa as $periodo => $sit)
    {
        $arrX[] = $periodo;
        $vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
        $vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
        $vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
        $vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
        $vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
        $vTotal = 0;

        $arrA[] = $vA;
        $vTotal += $vA;
        $arrB[] = $vB;
        $vTotal += $vB;
        $arrC[] = $vC;
        $vTotal += $vC;
        $arrD[] = $vD;
        $vTotal += $vD;
        $arrE[] = $vE;
        $vTotal += $vE;
        $arrTotal[] = $vTotal;
    }

    $aDados = array(
        array('name' => 'Tecnico', 'data'=>$arrA),
        array('name' => 'Pos-Graduacao', 'data'=>$arrB),
        array('name' => 'FIC', 'data'=>$arrC),
        array('name' => 'Ensino Medio', 'data'=>$arrD),
        array('name' => 'Superior', 'data'=>$arrE),
        array('name' => 'Total por Ano', 'data'=>$arrTotal),
    );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
        'alinhamento' => "formatter: function() { return '1'; }"
    );

    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutos", "Etapas/Modalidades",300, $formatoValores);
    
}
function gerarGraficoRedeFederal2($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2347))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2401) AND dsh.tidid2 IN (5323))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (sh.indid in (2347, 2903) OR (sh.indid in (2401) AND dsh.tidid2 IN (5323)))";
            break;
    }

    $sql = "SELECT dpe.dpeanoref as periodo,
                    CASE
							WHEN dsh.tidid2 IN (5130, 5131, 5132, 5188, 5190, 5189) OR dsh.tidid1 IN (5303, 5304) OR sh.indid in (2903) THEN 'Técnico'
							WHEN dsh.tidid2 IN (5136, 5137, 5138, 5191) THEN 'Pós-Graduação'
							WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302) THEN 'FIC'
							WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
							WHEN dsh.tidid2 IN (5135, 5140, 5141, 5133, 5134, 5139) THEN 'Superior'
					END AS etapa,
                    SUM(dsh.dshqtde)::integer AS total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					$where
					AND sh.sehstatus <> 'I'
					AND dpe.dpeanoref >= '2010'
					GROUP BY periodo, etapa
                    ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar3'] = $arrDados;
    $arrEtapa = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }
    }
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrD = array();
    $arrE = array();
    $arrTotal = array();


    foreach($arrEtapa as $periodo => $sit)
    {
        $arrX[] = $periodo;
        $vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
        $vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
        $vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
        $vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
        $vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
        $vTotal = 0;

        $arrA[] = $vA;
        $vTotal += $vA;
        $arrB[] = $vB;
        $vTotal += $vB;
        $arrC[] = $vC;
        $vTotal += $vC;
        $arrD[] = $vD;
        $vTotal += $vD;
        $arrE[] = $vE;
        $vTotal += $vE;
        $arrTotal[] = $vTotal;

        //$vTotalA += $vA;
        //$vTotalB += $vB;
        //$vTotalC += $vC;
        //$vTotalD += $vD;
        //$vTotalE += $vE;
        //$vTotal += $vA + $vB + $vC + $vD + $vE;
        //$arrTotal[] = $vTotal;
        //$arrA[] = $vTotalA;
        //$arrB[] = $vTotalB;
        //$arrC[] = $vTotalC;
        //$arrD[] = $vTotalD;
        //$arrE[] = $vTotalE;
    }

    $aDados = array(
        array('name' => 'Tecnico', 'data'=>$arrA),
        array('name' => 'Pos-Graduacao', 'data'=>$arrB),
        array('name' => 'FIC', 'data'=>$arrC),
        array('name' => 'Ensino Medio', 'data'=>$arrD),
        array('name' => 'Superior', 'data'=>$arrE),
        array('name' => 'Total por Ano', 'data'=>$arrTotal),
    );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
        'alinhamento' => "formatter: function() { return '1'; }"
    );

    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantes", "Ingressantes",300,$formatoValores);

}
function gerarGraficoRedeFederal3($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2401) AND dsh.tidid2 IN (5323) AND dsh.tidid1 NOT IN (5302))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (
					        (sh.indid in (2575) AND dsh.tidid1 IN (6994, 6995, 6996, 6997, 6999, 6998))
					        OR
					        (sh.indid in (2401) AND dsh.tidid2 IN (5323) AND dsh.tidid1 NOT IN (5302))
					        OR
					        (sh.indid in (2903))
					      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
						dpe.dpeanoref AS periodo,
						CASE
							WHEN dsh.tidid1 IN (6994, 7867, 5303) THEN 'Técnico concomitante'
							WHEN dsh.tidid1 IN (6995) THEN 'Técnico integrado'
							WHEN dsh.tidid1 IN (6996) THEN 'Técnico PROEJA concomitante'
							WHEN dsh.tidid1 IN (6997) THEN 'Técnico PROEJA integrado'
							WHEN dsh.tidid1 IN (6999, 7869, 5304) THEN 'Técnico subsequente presencial'
							WHEN dsh.tidid1 IN (6998) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
						END AS etapa,
						sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
					$where
					AND sh.sehstatus <> 'I'
					AND dpe.dpeanoref >= '2010'
					GROUP BY periodo, etapa
					ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar4'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }
    }
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrD = array();
    $arrE = array();
    $arrF = array();
    $arrTotal = array();

    foreach($arrEtapa as $periodo => $sit)
    {
        $arrX[] = $periodo;
        $vA = Round((is_array($sit['Técnico concomitante']['total'])?array_sum($sit['Técnico concomitante']['total']):0));
        $vB = Round((is_array($sit['Técnico integrado']['total'])?array_sum($sit['Técnico integrado']['total']):0));
        $vC = Round((is_array($sit['Técnico PROEJA concomitante']['total'])?array_sum($sit['Técnico PROEJA concomitante']['total']):0));
        $vD = Round((is_array($sit['Técnico PROEJA integrado']['total'])?array_sum($sit['Técnico PROEJA integrado']['total']):0));
        $vE = Round((is_array($sit['Técnico subsequente presencial']['total'])?array_sum($sit['Técnico subsequente presencial']['total']):0));
        $vF = Round((is_array($sit['Técnico subsequente a distância']['total'])?array_sum($sit['Técnico subsequente a distância']['total']):0));
        $vTotal = 0;

        $arrA[] = $vA;
        $vTotal += $vA;
        $arrB[] = $vB;
        $vTotal += $vB;
        $arrC[] = $vC;
        $vTotal += $vC;
        $arrD[] = $vD;
        $vTotal += $vD;
        $arrE[] = $vE;
        $vTotal += $vE;
        $arrF[] = $vF;
        $vTotal += $vF;
        $arrTotal[] = $vTotal;
    }

    $aDados = array(
        array('name' => 'Tecnico concomitante', 'data'=>$arrA),
        array('name' => 'Tecnico integrado', 'data'=>$arrB),
        array('name' => 'Tecnico PROEJA concomitante', 'data'=>$arrC),
        array('name' => 'Tecnico PROEJA integrado', 'data'=>$arrD),
        array('name' => 'Tecnico subsequente presencial', 'data'=>$arrE),
        array('name' => 'Tecnico subsequente a distancia', 'data'=>$arrF),
        array('name' => 'Total por Ano', 'data'=>$arrTotal),
    );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
        'alinhamento' => "formatter: function() { return '1'; }"
    );

    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosTecnicos", "Etapas/Modalidades",300,$formatoValores);

}
function gerarGraficoRedeFederal4($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2347) AND dsh.tidid2 IN (5131, 5130, 5189, 5188, 5190, 5132))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2401) AND dsh.tidid2 IN (5323) AND dsh.tidid1 NOT IN (5302))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2903))";
            break;
        default:
            $where = "WHERE (
					        (sh.indid in (2347) AND dsh.tidid2 IN (5131, 5130, 5189, 5188, 5190, 5132))
					        OR
					        (sh.indid in (2401) AND dsh.tidid2 IN (5323) AND dsh.tidid1 NOT IN (5302))
					        OR
					        (sh.indid in (2903))
					      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid2 IN (5131) OR dsh.tidid1 IN (7867, 5303) THEN 'Técnico concomitante'
                    WHEN dsh.tidid2 IN (5130) THEN 'Técnico integrado'
                    WHEN dsh.tidid2 IN (5189) THEN 'Técnico PROEJA concomitante'
                    WHEN dsh.tidid2 IN (5188) THEN 'Técnico PROEJA integrado'
                    WHEN dsh.tidid2 IN (5190) OR dsh.tidid1 IN (7869, 5304) THEN 'Técnico subsequente presencial'
                    WHEN dsh.tidid2 IN (5132) OR sh.indid in (2903) THEN 'Técnico subsequente a distância'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar5'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }
    }
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrD = array();
    $arrE = array();
    $arrF = array();
    $arrTotal = array();

    foreach($arrEtapa as $periodo => $sit)
    {
        $arrX[] = $periodo;
        $vA = Round((is_array($sit['Técnico concomitante']['total'])?array_sum($sit['Técnico concomitante']['total']):0));
        $vB = Round((is_array($sit['Técnico integrado']['total'])?array_sum($sit['Técnico integrado']['total']):0));
        $vC = Round((is_array($sit['Técnico PROEJA concomitante']['total'])?array_sum($sit['Técnico PROEJA concomitante']['total']):0));
        $vD = Round((is_array($sit['Técnico PROEJA integrado']['total'])?array_sum($sit['Técnico PROEJA integrado']['total']):0));
        $vE = Round((is_array($sit['Técnico subsequente presencial']['total'])?array_sum($sit['Técnico subsequente presencial']['total']):0));
        $vF = Round((is_array($sit['Técnico subsequente a distância']['total'])?array_sum($sit['Técnico subsequente a distância']['total']):0));
        $vTotal = 0;

        $arrA[] = $vA;
        $vTotal += $vA;
        $arrB[] = $vB;
        $vTotal += $vB;
        $arrC[] = $vC;
        $vTotal += $vC;
        $arrD[] = $vD;
        $vTotal += $vD;
        $arrE[] = $vE;
        $vTotal += $vE;
        $arrF[] = $vF;
        $vTotal += $vF;
        $arrTotal[] = $vTotal;
    }

    $aDados = array(
        array('name' => 'Tecnico concomitante', 'data'=>$arrA),
        array('name' => 'Tecnico integrado', 'data'=>$arrB),
        array('name' => 'Tecnico PROEJA concomitante', 'data'=>$arrC),
        array('name' => 'Tecnico PROEJA integrado', 'data'=>$arrD),
        array('name' => 'Tecnico subsequente presencial', 'data'=>$arrE),
        array('name' => 'Tecnico subsequente a distancia', 'data'=>$arrF),
        array('name' => 'Total por Ano', 'data'=>$arrTotal),
    );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
        'alinhamento' => "formatter: function() { return '1'; }"
    );

    echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesTecnicos", "Etapas/Modalidades",300, $formatoValores);

}
function gerarGraficoRedeFederal5($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE (sh.indid in (2347))";
            break;
        case 3:
            $where = "WHERE (sh.indid in (2401) AND dsh.tidid2 IN (5323))";
            break;
        case 4:
            $where = "WHERE (sh.indid in (2393) AND dsh.tidid1 IN (5209) AND dpe.dpeanoref >= '2011')";
            break;
        default:
            $where = "WHERE
                        (
                            sh.indid in (2347)
                            OR
                            (sh.indid in (2401) AND dsh.tidid2 IN (5323))
                            OR
                            (sh.indid in (2393) AND dsh.tidid1 IN (5209) AND dpe.dpeanoref >= '2011')
                        )";
            break;
    }

    $sql = "SELECT
                CASE
                    WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) OR dsh.tidid1 IN (5303, 5304) OR sh.indid in (2393) THEN 'Técnico'
                    WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
                    WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302) THEN 'FIC'
                    WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
                    WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
                END AS descricao,
                sum(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar14'] = $arrDados;
    geraGrafico($arrDados, "graficoPizzaIngressantes", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);

}

function gerarGraficoRedeFederal6($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2575)  AND dsh.tidid1 IN (7000, 9264)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905) AND dsh.tidid1 IN (7866)";
            break;
        default:
            $where = "WHERE (
                        (sh.indid in (2575) AND dsh.tidid1 IN (7000, 9264))
                      OR
                        (sh.indid in (2905) AND dsh.tidid1 IN (7866))
                      )";
            break;
    }

    $arrEtapa = array();
    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (7000, 7866) THEN 'FIC >= 160h'
                    WHEN dsh.tidid1 IN (9264) THEN 'FIC < 160h'
                END AS etapa,
                sum(dsh.dshqtde) as total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar15'] = $arrDados;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrTotal = array();

        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['FIC >= 160h']['total'])?array_sum($sit['FIC >= 160h']['total']):0));
            $vB = Round((is_array($sit['FIC < 160h']['total'])?array_sum($sit['FIC < 160h']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'FIC >= 160h', 'data'=>$arrA),
            array('name' => 'FIC < 160h', 'data'=>$arrB),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculasInstitutosFIC", "Etapas/Modalidades",300, $formatoValores);
    }
}
function gerarGraficoRedeFederal7($detalhe){

    global $db;

    switch($detalhe){
        case 2:
            $where = "WHERE sh.indid in (2347) AND dsh.tidid2 IN (5129, 9263)";
            break;
        case 3:
            $where = "WHERE sh.indid in (2905) AND dsh.tidid1 IN (7866)";
            break;
        default:
            $where = "WHERE (
                            (sh.indid in (2347) AND dsh.tidid2 IN (5129, 9263))
                      OR
                            (sh.indid in (2905) AND dsh.tidid1 IN (7866))
                      )";
            break;
    }

    $sql = "SELECT dpe.dpeanoref as periodo,
            CASE
                    WHEN dsh.tidid2 IN (5129) OR dsh.tidid1 IN (7866) THEN 'FIC >= 160h'
                    WHEN dsh.tidid2 IN (9263) THEN 'FIC < 160h'
                END AS etapa,
            SUM(dsh.dshqtde)::integer AS total
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref >= '2010'
            GROUP BY periodo, etapa
            ORDER BY periodo, etapa";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $_SESSION['exportar16'] = $arrDados;
    $arrEtapa = array();
    if($arrDados){
        foreach($arrDados as $dado){
            $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
        }

        $arrX = array();
        $arrA = array();
        $arrB = array();
        $arrTotal = array();


        foreach($arrEtapa as $periodo => $sit)
        {
            $arrX[] = $periodo;
            $vA = Round((is_array($sit['FIC >= 160h']['total'])?array_sum($sit['FIC >= 160h']['total']):0));
            $vB = Round((is_array($sit['FIC < 160h']['total'])?array_sum($sit['FIC < 160h']['total']):0));
            $vTotal = 0;

            $arrA[] = $vA;
            $vTotal += $vA;
            $arrB[] = $vB;
            $vTotal += $vB;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'FIC >= 160h', 'data'=>$arrA),
            array('name' => 'FIC < 160h', 'data'=>$arrB),
            array('name' => 'Total por Ano', 'data'=>$arrTotal),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoIngressantesFIC", "Ingressantes",300,$formatoValores);
    }
}
?>