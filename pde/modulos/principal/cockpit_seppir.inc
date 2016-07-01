<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['semestre_vagas'])){
    gerarGraficoVagas($_REQUEST['semestre_vagas']);
    die;
}
if(isset($_REQUEST['raca1'])){
    gerarGraficoProuni($_REQUEST['raca1']);
    die;
}
if(isset($_REQUEST['raca2'])){
    gerarGraficoFies($_REQUEST['raca2']);
    die;
}
if(isset($_REQUEST['raca3'])){
    gerarGraficoCiencia($_REQUEST['raca3']);
    die;
}
if(isset($_REQUEST['nivel1'])){
    gerarGraficoLeiCotas1($_REQUEST['nivel1']);
    die;
}
if(isset($_REQUEST['nivel2'])){
    gerarGraficoLeiCotas2($_REQUEST['nivel2']);
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
            jQuery('.filtrar_semestre3').click(function(){
                jQuery('#div_vagas').load('estrategico.php?modulo=principal/cockpit_seppir&acao=A&semestre_vagas='+jQuery(this).val());
            });
            jQuery('.filtrar_raca1').click(function(){
                jQuery('#div_prouni').load('estrategico.php?modulo=principal/cockpit_seppir&acao=A&raca1='+jQuery(this).val());
            });
            jQuery('.filtrar_raca2').click(function(){
                jQuery('#div_fies').load('estrategico.php?modulo=principal/cockpit_seppir&acao=A&raca2='+jQuery(this).val());
            });
            jQuery('.filtrar_raca3').click(function(){
                jQuery('#div_ciencia').load('estrategico.php?modulo=principal/cockpit_seppir&acao=A&raca3='+jQuery(this).val());
            });
            jQuery('.filtrar_nivel1').click(function(){
                jQuery('#div_leicotas1').load('estrategico.php?modulo=principal/cockpit_seppir&acao=A&nivel1='+jQuery(this).val());
            });
            jQuery('.filtrar_nivel2').click(function(){
                jQuery('#div_leicotas2').load('estrategico.php?modulo=principal/cockpit_seppir&acao=A&nivel2='+jQuery(this).val());
            });
        });
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{
            background: url('./cockpit/images/img_seppir_topo.gif') no-repeat center top;
            font-weight:bold;
            font-size:30px;
            color:#FFFFFF;
            text-shadow:#000 2px 2px 6px !important;
        }
        #popdiv{display:block;width:100%;height:357px;overflow-y: scroll;}
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
            <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_seppir&acao=A';">
                <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
<tr>
    <td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Secretaria de Políticas de Promoção da Igualdade Racial</div></td>
</tr>
<tr>
    <td class="fundo_padrao" width="33%">
        <div>
            <?php exibirTitulo('indicador', 'Indicadores'); ?>
        </div>
        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
            <?php
            $sql = "SELECT (item::numeric / total::numeric) * 100
                    FROM (
                        SELECT
                            (SUM(qt_cor_raca_preta) + SUM(qt_cor_raca_parda)) AS item,
                            (SUM(qt_cor_raca_branca) + SUM(qt_cor_raca_preta) + SUM(qt_cor_raca_parda) + SUM(qt_cor_raca_amarela) + SUM(qt_cor_raca_indigena)) AS total
                        FROM enem.ts_enem_inscricao_2014
                    ) AS FOO";
            $percentual = $db->pegaUm($sql);
            ?>
            <tr>
                <td class="bold">ENEM 2014 – <?=number_format($percentual,2,",",".")?>% de negros inscritos</td>
            </tr>
            <?php
            $sql = "SELECT CASE WHEN COALESCE(SUM(total),0) > 0 THEN (SUM(item) / SUM(total)) * 100 ELSE 0 END
                    FROM (
                        select 0 AS item, sum(dsh.dshqtde) AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        where sh.indid in (2893)
                        and sh.sehstatus <> 'I'
                        and dsh.tidid1 NOT IN (7812) --Não Informada
                    UNION ALL
                        select sum(dsh.dshqtde) AS item, 0 AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        where sh.indid in (2893)
                        and sh.sehstatus <> 'I'
                        and dsh.tidid1 IN (7810, 7811) --preta e parda
                    ) AS FOO";
            $percentual = $db->pegaUm($sql);
            ?>
            <tr class="zebrado link" onclick="abreIndicadorPopUp(2893);">
                <td class="bold">PROUNI – <?=number_format($percentual,2,",",".")?>%  de bolsas ocupadas por negros</td>
            </tr>
            <?php
            $sql = "SELECT (SUM(item) / SUM(total)) * 100
                    FROM (
                        select 0 AS item, sum(dsh.dshqtde) AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        where sh.indid in (778)
                        and sh.sehstatus <> 'I'
                        and dsh.tidid2 NOT IN (3028) --Não Informada
                    UNION ALL
                        select sum(dsh.dshqtde) AS item, 0 AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        where sh.indid in (778)
                        and sh.sehstatus <> 'I'
                        and dsh.tidid2 IN (3026, 3027) --preta e parda
                    ) AS FOO";
            $percentual = $db->pegaUm($sql);
            ?>
            <tr class="link" onclick="abreIndicadorPopUp(778);">
                <td class="bold">FIES – <?=number_format($percentual,2,",",".")?>%  de contratos firmados por negros</td>
            </tr>
            <?php
            $sql = "SELECT (SUM(item) / CASE WHEN COALESCE(SUM(total),0) = 0 THEN 1 ELSE SUM(total) END) * 100
                    FROM (
                        select 0 AS item, sum(dsh.dshqtde) AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        where sh.indid in (2886)
                        and sh.sehstatus <> 'I'
                        and dsh.tidid2 NOT IN (7781) --Não Informada
                    UNION ALL
                        select sum(dsh.dshqtde) AS item, 0 AS total
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        where sh.indid in (2886)
                        and sh.sehstatus <> 'I'
                        and dsh.tidid2 IN (7783, 7784) --preta e parda
                    ) AS FOO";
            $percentual = $db->pegaUm($sql);
            ?>
            <tr class="zebrado link" onclick="abreIndicadorPopUp(2886);">
                <td class="bold">CIÊNCIA SEM FRONTEIRAS – <?=number_format($percentual,2,",",".")?>% de bolsas concedidas para negros</td>
            </tr>
        </table>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(271);" width="67%" colspan="2">
        <div>
            <?php exibirTitulo('executive', 'Exame Nacional do Ensino Médio - ENEM', 'Inscrições por Cor/Raça'); ?>
        </div>
        <?php
        $sql = "SELECT dpe.dpeanoref AS ano, tid1.tiddsc AS cor, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (271)
                AND sh.sehstatus <> 'I'
                GROUP BY ano, cor
                ORDER BY ano, cor";
        $arrDados = $db->carregar($sql, null, 3200);
        if($arrDados){
            $total2012=0;
            $total2013=0;
            $total2014=0;
            $soma2012=0;
            $soma2013=0;
            $soma2014=0;
            foreach($arrDados as $dado){
                $arrAcao[$dado['cor']][$dado['ano']]['total'] = $dado['valor'];
                if($dado['ano']==2012){
                    $total2012+=$dado['valor'];
                }
                if($dado['ano']==2013){
                    $total2013+=$dado['valor'];
                }
                if($dado['ano']==2014){
                    $total2014+=$dado['valor'];
                }
            }
        }
        ?>
        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
            <tr>
                <th rowspan="2">Cor/Raça</th>
                <th colspan="2">2012</th>
                <th colspan="2">2013</th>
                <th colspan="2">2014</th>
            </tr>
            <tr>
                <th>Inscrições</th>
                <th>Percentual</th>
                <th>Inscrições</th>
                <th>Percentual</th>
                <th>Inscrições</th>
                <th>Percentual</th>
            </tr>
            <?php
            $count=-1;
            foreach($arrAcao as $acao => $aca){
                $count++;
                ?>
                <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                    <td><?php echo $acao ?></td>
                    <td class="numero" ><?php echo number_format($aca['2012']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format(($aca['2012']['total']/$total2012)*100,2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($aca['2013']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format(($aca['2013']['total']/$total2013)*100,2,",",".") ?>%</td>
                    <td class="numero" ><?php echo number_format($aca['2014']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format(($aca['2014']['total']/$total2014)*100,2,",",".") ?>%</td>
                </tr>
            <?php
                $soma2012+=$aca['2012']['total'];
                $soma2013+=$aca['2013']['total'];
                $soma2014+=$aca['2014']['total'];
            }
            ?>
            <tr>
                <th>Total</th>
                <th><?php echo number_format($soma2012,0,",",".") ?></th>
                <th>100%</th>
                <th><?php echo number_format($soma2013,0,",",".") ?></th>
                <th>100%</th>
                <th><?php echo number_format($soma2014,0,",",".") ?></th>
                <th>100%</th>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="fundo_padrao">
        <div class="link" onclick="abreIndicadorPopUp(2671);">
            <?php exibirTitulo('indicador', 'Sistema de seleção unificada - SISU', 'Vagas disponibilizadas por modalidade da concorrência (2010-2014)'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_semestre3" name="semestre3" value="1" checked="checked" onclick="" />1º Semestre
            <input type="radio" class="filtrar_semestre3" name="semestre3" value="2" onclick="" />2º Semestre
        </div>

        <div id="div_vagas">
            <?php gerarGraficoVagas(1); ?>
        </div>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2672);" colspan="2">
        <div>
            <?php exibirTitulo('indicador', 'Sistema de seleção unificada - SISU', 'Percentual reservado para lei de cotas<br>por instituição e processo seletivo / semestre (2013/2014)'); ?>
        </div>
        <div id="popdiv">
        <?php
        $sql = "SELECT descricao, SUM(valor1) AS valor1, SUM(valor2) AS valor2, SUM(valor3) AS valor3, SUM(valor4) AS valor4
                FROM(
                    select UPPER(iesdsc) as descricao, sum(dshqtde) as valor1, 0 as valor2, 0 as valor3, 0 valor4
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                    where sh.indid in (2672)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = 1222 --1º Semestre/2013
                    GROUP BY descricao
                UNION ALL
                    select UPPER(iesdsc) as descricao, 0 as valor1, sum(dshqtde) as valor2, 0 as valor3, 0 valor4
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                    where sh.indid in (2672)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = 1223 --2º Semestre/2013
                    GROUP BY descricao
                UNION ALL
                    select UPPER(iesdsc) as descricao, 0 as valor1, 0 as valor2, sum(dshqtde) as valor3, 0 valor4
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                    where sh.indid in (2672)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = 1248 --1º Semestre/2014
                    GROUP BY descricao
                 UNION ALL
                    select UPPER(iesdsc) as descricao, 0 as valor1, 0 as valor2, 0 as valor3, sum(dshqtde) as valor4
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                    where sh.indid in (2672)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = 1249 --2º Semestre/2014
                    GROUP BY descricao
                ) AS FOO
                GROUP BY descricao
                ORDER BY descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        ?>
        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
            <tr>
                <th>Instituição</th>
                <th>1º Semestre/2013</th>
                <th>2º Semestre/2013</th>
                <th>1º Semestre/2014</th>
                <th>2º Semestre/2014</th>
            </tr>
            <?php
            if($arrDados){
                $count = -1;
                foreach($arrDados as $dado):
                    $count++;?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td><?php echo $dado['descricao'] ?></td>
                        <td class="numero" ><?php echo number_format($dado['valor1'],2,",",".") ?>%</td>
                        <td class="numero" ><?php echo number_format($dado['valor2'],2,",",".") ?>%</td>
                        <td class="numero" ><?php echo number_format($dado['valor3'],2,",",".") ?>%</td>
                        <td class="numero" ><?php echo number_format($dado['valor4'],2,",",".") ?>%</td>
                    </tr>
                <?php
                endforeach;
            }
             ?>
        </table>
        </div>
    </td>
</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
<tr>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3180);" width="50%">
        <div>
            <?php exibirTitulo('indicador', 'Lei de Cotas - Universidades Federais', 'Percentual reservado para lei de cotas<br>por universidade e processo seletivo / semestre (2013/2014)'); ?>
        </div>
        <br>
        <br>
        <br>
        <div id="popdiv">
            <?php
            $sql = "SELECT descricao, SUM(valor1) AS valor1, SUM(valor2) AS valor2, SUM(valor3) AS valor3, SUM(valor4) AS valor4, SUM(valortotal1) AS valortotal1, SUM(valortotal2) AS valortotal2, SUM(valortotal3) AS valortotal3, SUM(valortotal4) AS valortotal4
                    FROM(
                        select UPPER(iesdsc) as descricao, dshqtde as valor1, 0 as valor2, 0 as valor3, 0 as valor4, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal1, 0 AS valortotal2, 0 AS valortotal3, 0 AS valortotal4
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                        inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 12 and fn.fuestatus = 'A'
                        where sh.indid in (3180)
                        and sh.sehstatus <> 'I'
                        AND sh.dpeid = 1222 --1º Semestre/2013
                    UNION ALL
                        select UPPER(iesdsc) as descricao, 0 as valor1, dshqtde as valor2, 0 as valor3, 0 as valor4, 0 AS valortotal1, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal2, 0 AS valortotal3, 0 AS valortotal4
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                        inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 12 and fn.fuestatus = 'A'
                        where sh.indid in (3180)
                        and sh.sehstatus <> 'I'
                        AND sh.dpeid = 1223 --2º Semestre/2013
                    UNION ALL
                        select UPPER(iesdsc) as descricao, 0 as valor1, 0 as valor2, dshqtde as valor3, 0 as valor4, 0 AS valortotal1, 0 AS valortotal2, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal3, 0 AS valortotal4
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                        inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 12 and fn.fuestatus = 'A'
                        where sh.indid in (3180)
                        and sh.sehstatus <> 'I'
                        AND sh.dpeid = 1248 --1º Semestre/2014
                    UNION ALL
                        select UPPER(iesdsc) as descricao, 0 as valor1, 0 as valor2, 0 as valor3, dshqtde as valor4, 0 AS valortotal1, 0 AS valortotal2, 0 AS valortotal3, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal4
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                        inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 12 and fn.fuestatus = 'A'
                        where sh.indid in (3180)
                        and sh.sehstatus <> 'I'
                        AND sh.dpeid = 1249 --2º Semestre/2014
                    ) AS FOO
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $somavalortotal1 = 0;
            $somavalortotal2 = 0;
            $somavalortotal3 = 0;
            $somavalortotal4 = 0;
            $contaUniversidades1 = 0;
            $contaUniversidades2 = 0;
            $contaUniversidades3 = 0;
            $contaUniversidades4 = 0;
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Universidade</th>
                    <th>1º Semestre/2013</th>
                    <th>2º Semestre/2013</th>
                    <th>1º Semestre/2014</th>
                    <th>2º Semestre/2014</th>
                </tr>
                <?php
                if($arrDados){
                    $count = -1;
                    foreach($arrDados as $dado):
                        $count++;?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['descricao'] ?></td>
                            <td class="numero" ><?php echo ($dado['valor1'] > 0 ? number_format(($dado['valortotal1']/$dado['valor1'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                            <td class="numero" ><?php echo ($dado['valor2'] > 0 ? number_format(($dado['valortotal2']/$dado['valor2'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                            <td class="numero" ><?php echo ($dado['valor3'] > 0 ? number_format(($dado['valortotal3']/$dado['valor3'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                            <td class="numero" ><?php echo ($dado['valor4'] > 0 ? number_format(($dado['valortotal4']/$dado['valor4'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                        </tr>
                        <?php
                        if($dado['valor1'] > 0){
                            $contaUniversidades1 += 1;
                            $somavalortotal1 += $dado['valortotal1']/$dado['valor1'];
                        }
                        if($dado['valor2'] > 0){
                            $contaUniversidades2 += 1;
                            $somavalortotal2 += $dado['valortotal2']/$dado['valor2'];
                        }
                        if($dado['valor3'] > 0){
                            $contaUniversidades3 += 1;
                            $somavalortotal3 += $dado['valortotal3']/$dado['valor3'];
                        }
                        if($dado['valor4'] > 0){
                            $contaUniversidades4 += 1;
                            $somavalortotal4 += $dado['valortotal4']/$dado['valor4'];
                        }
                    endforeach;
                }
                ?>
                <tr>
                    <th>Total</th>
                    <th><?php echo ($contaUniversidades1 > 0 ? number_format(($somavalortotal1/$contaUniversidades1)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                    <th><?php echo ($contaUniversidades2 > 0 ? number_format(($somavalortotal2/$contaUniversidades2)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                    <th><?php echo ($contaUniversidades3 > 0 ? number_format(($somavalortotal3/$contaUniversidades3)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                    <th><?php echo ($contaUniversidades4 > 0 ? number_format(($somavalortotal4/$contaUniversidades4)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                </tr>
            </table>
        </div>
    </td>
    <td class="fundo_padrao" width="50%">
        <div class="link" onclick="abreIndicadorPopUp(3180);">
            <?php exibirTitulo('indicador', 'Lei de Cotas - Rede Federal de<br>Educação Profissional e Tecnológica', 'Percentual reservado para lei de cotas<br>por instituição e processo seletivo / semestre (2013/2014)'); ?>
        </div>
        <br>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_nivel1" name="nivel1" value="1" checked="checked" onclick="" />Todos
            <input type="radio" class="filtrar_nivel1" name="nivel1" value="2" onclick="" />Ensino Superior
            <input type="radio" class="filtrar_nivel1" name="nivel1" value="3" onclick="" />Ensino Técnico de Nível Médio
        </div>
        <br>
        <div id="div_leicotas1">
            <?php gerarGraficoLeiCotas1(1); ?>
        </div>
    </td>
</tr>
<tr>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3180);">
        <div>
            <?php exibirTitulo('indicador', 'Percentual reservado para Lei de Cotas<br>no Ensino Superior Federal'); ?>
        </div>
        <?php
        $sql = "SELECT periodo, tipo, CASE WHEN COALESCE(SUM(total),0) > 0 THEN (SUM(totaldetalhe)/SUM(total))*100 ELSE 0 END AS valor
                FROM (
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        3 AS ordemdetalhe,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        'Total Ensino Superior' AS tipo,
                        SUM(dsh.dshqtde::INTEGER) AS total,
                        0 AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.funid IN (11,12) AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (8892, 8895)
                    GROUP BY ordem, ordemdetalhe, periodo, tipo
                UNION ALL
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        3 AS ordemdetalhe,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        'Total Ensino Superior' AS tipo,
                        0 AS total,
                        SUM(dsh.dshqtde::INTEGER) AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.funid IN (11,12) AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (8892)
                    GROUP BY ordem, ordemdetalhe, periodo, tipo
                UNION ALL
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        CASE
                            WHEN fn.funid = 12 THEN 1
                            WHEN fn.funid = 11 THEN 2
                        END AS ordemdetalhe,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        CASE
                            WHEN fn.funid = 12 THEN 'Universidades'
                            WHEN fn.funid = 11 THEN 'Institutos'
                        END AS tipo,
                        SUM(dsh.dshqtde::INTEGER) AS total,
                        0 AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.funid IN (11,12) AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (8892, 8895)
                    GROUP BY ordem, ordemdetalhe, periodo, tipo
                UNION ALL
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        CASE
                            WHEN fn.funid = 12 THEN 1
                            WHEN fn.funid = 11 THEN 2
                        END AS ordemdetalhe,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        CASE
                            WHEN fn.funid = 12 THEN 'Universidades'
                            WHEN fn.funid = 11 THEN 'Institutos'
                        END AS tipo,
                        0 AS total,
                        SUM(dsh.dshqtde::INTEGER) AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.funid IN (11,12) AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (8892)
                    GROUP BY ordem, ordemdetalhe, periodo, tipo
                ) AS FOO
                GROUP BY ordem, ordemdetalhe, periodo, tipo
                ORDER BY ordem, ordemdetalhe";
        $dados = $db->carregar($sql, null, 3200);
        if($dados){
            $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.') + '%'; }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '%</b>'; }",
                'exibirvalor' => ",format: '{point.y:.2f}%'"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaLeiCotasESF', null, null, $formatoValores, null, "", 1000, 600, true, 300, false);
        }
        ?>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3180);">
        <div>
            <?php exibirTitulo('indicador', 'Percentual reservado para Lei de Cotas<br>no Ensino Técnico de Nível Médio Federal'); ?>
        </div>
        <?php
        $sql = "SELECT periodo AS descricao, CASE WHEN COALESCE(SUM(total),0) > 0 THEN (SUM(totaldetalhe)/SUM(total))*100 ELSE 0 END AS valor
                FROM (
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        SUM(dsh.dshqtde::INTEGER) AS total,
                        0 AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.funid IN (11) AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (8893, 8896)
                    GROUP BY ordem, periodo
                UNION ALL
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        0 AS total,
                        SUM(dsh.dshqtde::INTEGER) AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.funid IN (11) AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (8893)
                    GROUP BY ordem, periodo
                ) AS FOO
                GROUP BY ordem, descricao
                ORDER BY ordem";
        $dados = $db->carregar( $sql, null, 3200 );
        if($dados){
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.') + '%'; }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '%</b>'; }",
                'exibirvalor' => "formatter:function () { return number_format(this.y, 2, ',', '.') + '%'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaLeiCotasETNMF', null, null, $formatoValores, null, "", 1000, 600, null, 300);
        }
        ?>
    </td>
</tr>
<tr>
    <td class="fundo_padrao" width="50%">
        <div class="link" onclick="abreIndicadorPopUp(3180);">
            <?php exibirTitulo('indicador', 'Percentual reservado para Lei de Cotas<br>no Ensino Superior - por grupos de cotistas'); ?>
        </div>
        <br>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_nivel2" name="nivel2" value="1" checked="checked" onclick="" />Universidades
            <input type="radio" class="filtrar_nivel2" name="nivel2" value="2" onclick="" />Institutos
            <input type="radio" class="filtrar_nivel2" name="nivel2" value="3" onclick="" />Total Ensino Superior
        </div>
        <br>
        <div id="div_leicotas2">
            <?php gerarGraficoLeiCotas2(1); ?>
        </div>
    </td>
    <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3180);">
        <div>
            <?php exibirTitulo('indicador', 'Percentual reservado para Lei de Cotas<br>no Ensino Técnico de Nível Médio - por grupos de cotistas'); ?>
        </div>
        <?php
        $sql = "SELECT
                    periodo,
                    vagas,
                    CASE WHEN COALESCE(SUM(total),0) > 0 THEN (SUM(totaldetalhe)/SUM(total))*100 ELSE 0 END AS valor
                FROM (
                    SELECT
                        dpe.dpeanoref||dpe.dpeordem AS ordem,
                        Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                        tid1.tiddsc AS vagas,
                        CASE dsh.tidid1
                            WHEN 8888 THEN 1
                            WHEN 8889 THEN 2
                            WHEN 8890 THEN 3
                            WHEN 8891 THEN 4
                        END AS ordemdetalhe,
                        total,
                        SUM(dsh.dshqtde::INTEGER) AS totaldetalhe
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.fuestatus = 'A'
                    INNER JOIN (
                        SELECT
                            SUM(dsh.dshqtde::INTEGER) AS total,
                            sh.dpeid
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                        INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.fuestatus = 'A'
                        WHERE sh.indid in (3180)
                        AND sh.sehstatus <> 'I'
                        AND sh.dpeid IN (1222, 1248) --1º Semestre/2013 e 1º Semestre/2014
                        AND fn.funid IN (11,12) AND dsh.tidid2 IN (8892, 8895)
                        GROUP BY sh.dpeid) AS foo ON foo.dpeid = sh.dpeid
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 NOT IN (8894)
                    AND sh.dpeid IN (1222, 1248) --1º Semestre/2013 e 1º Semestre/2014
                    AND fn.funid IN (11,12) AND dsh.tidid2 IN (8893, 8896)
                    GROUP BY ordem, ordemdetalhe, periodo, vagas, total
                ) AS FOO1
                GROUP BY ordem, ordemdetalhe, periodo, vagas
                ORDER BY ordem, ordemdetalhe";
        $dados = $db->carregar($sql, null, 3200);
        if($dados){
            $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'vagas', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.') + '%'; }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '%</b>'; }",
                'exibirvalor' => ",format: '{point.y:.2f}%'"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaLeiCotasETNM', null, null, $formatoValores, null, "", 1000, 600, true, 300, false);
        }
        ?>
    </td>
</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
<tr>
    <td class="fundo_padrao" width="33%">
        <div class="link" onclick="abreIndicadorPopUp(2893);">
            <?php exibirTitulo('indicador', 'Programa Universidade para Todos -<br>PROUNI', 'Bolsas ocupadas por cor/raça (2005-2014)'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_raca1" name="raca1" value="1" checked="checked" onclick="" />Negra
            <input type="radio" class="filtrar_raca1" name="raca1" value="2" onclick="" />Parda e Preta
        </div>

        <div id="div_prouni">
            <?php gerarGraficoProuni(1); ?>
        </div>
    </td>
    <td class="fundo_padrao" width="34%">
        <div class="link" onclick="abreIndicadorPopUp(778);">
            <?php exibirTitulo('indicador', 'Programa de Financiamento Estudantil - FIES', 'Contratos firmados por cor/raça (2010-2014)'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_raca2" name="raca2" value="1" checked="checked" onclick="" />Negra
            <input type="radio" class="filtrar_raca2" name="raca2" value="2" onclick="" />Parda e Preta
        </div>

        <div id="div_fies">
            <?php gerarGraficoFies(1); ?>
        </div>
    </td>
    <td class="fundo_padrao" width="33%">
        <div class="link" onclick="abreIndicadorPopUp(2886);">
            <?php exibirTitulo('indicador', 'Ciência sem fronteiras', 'Bolsas concedidas por cor/raça (2013-2014)'); ?>
        </div>
        <div style="width:100%;text-align:center" >
            <input type="radio" class="filtrar_raca3" name="raca3" value="1" checked="checked" onclick="" />Negra
            <input type="radio" class="filtrar_raca3" name="raca3" value="2" onclick="" />Parda e Preta
        </div>

        <div id="div_ciencia">
            <?php gerarGraficoCiencia(1); ?>
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
<?php
function gerarGraficoVagas($semestre)
{
    global $db;

    if($semestre){
        $where = "AND dpe.dpeordem = {$semestre}";
    }
    $sql = "select
				dpeanoref as descricao,
				tiddsc as modalidade,
				sum(dsh.dshqtde) as valor
			from painel.seriehistorica sh
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			where sh.indid in (2671)
			and sh.sehstatus <> 'I'
			$where
			group by descricao, modalidade
			order by descricao, modalidade";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'modalidade', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaVagas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }
}

function gerarGraficoCiencia($tipo){
    global $db;

    if($tipo==1){
        $sql = "select CASE WHEN dsh.tidid2 IN (7783,7784) THEN 'Negra' ELSE tid.tiddsc END as descricao, sum(dsh.dshqtde) as valor
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
                    where i.indid in (2886)
                    and sh.sehstatus <> 'I'
                    and dsh.tidid2 NOT IN (7781) --NÃO INFORMADO
                    group by descricao
                    order by descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        if($arrDados){
            $formatoValores = "{point.percentage:.2f} %";
            geraGrafico($arrDados, "graficoPizzaRacaCor", "","<b>{series.name}: {point.percentage:.2f}%</b>",$formatoValores,"Ocorrências", null, null, null, null, true);
        }
    }else{
        $sql = "select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
                    where i.indid in (2886)
                    and sh.sehstatus <> 'I'
                    and dsh.tidid2 NOT IN (7781) --NÃO INFORMADO
                    group by descricao
                    order by descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        if($arrDados){
            $formatoValores = "{point.percentage:.2f} %";
            geraGrafico($arrDados, "graficoPizzaRacaCor", "","<b>{series.name}: {point.percentage:.2f}%</b>",$formatoValores,"Ocorrências", null, null, null, null, true);
        }
    }
}

function gerarGraficoFies($tipo){
    global $db;

    if($tipo==1){
        $sql = "select CASE WHEN dsh.tidid2 IN (3026,3027) THEN 'Negra' ELSE tid.tiddsc END as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
					where i.indid in (778)
					and sh.sehstatus <> 'I'
					and dsh.tidid2 NOT IN (3028) --NÃO INFORMADO
					group by descricao
					order by descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        if($arrDados){
            $formatoValores = "{point.percentage:.2f} %";
            geraGrafico($arrDados, "graficoPizzaRacaCorFIES", "","<b>{series.name}: {point.percentage:.2f}%</b>",$formatoValores,"Ocorrências", null, null, null, null, true);
        }
    }else{
        $sql = "select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
					where i.indid in (778)
					and sh.sehstatus <> 'I'
					and dsh.tidid2 NOT IN (3028) --NÃO INFORMADO
					group by descricao
					order by descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        if($arrDados){
            $formatoValores = "{point.percentage:.2f} %";
            geraGrafico($arrDados, "graficoPizzaRacaCorFIES", "","<b>{series.name}: {point.percentage:.2f}%</b>",$formatoValores,"Ocorrências", null, null, null, null, true);
        }
    }
}

function gerarGraficoProuni($tipo){
    global $db;

    if($tipo==1){
        $sql = "select CASE WHEN dsh.tidid1 IN (7810,7811) THEN 'Negra' ELSE tid1.tiddsc END as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid1 on tid1.tidid = dsh.tidid1
					where i.indid in (2893)
					and sh.sehstatus <> 'I'
					and dsh.tidid1 NOT IN (7812) --NÃO INFORMADO
					group by descricao
					order by descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        if($arrDados){
            $formatoValores = "{point.percentage:.2f} %";
            geraGrafico($arrDados, "graficoPizzaRacaCorPROUNI", "","<b>{series.name}: {point.percentage:.2f}%</b>",$formatoValores,"Ocorrências", null, null, null, null, true);
        }
    }else{
        $sql = "select tid1.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid1 on tid1.tidid = dsh.tidid1
					where i.indid in (2893)
					and sh.sehstatus <> 'I'
					and dsh.tidid1 NOT IN (7812) --NÃO INFORMADO
					group by descricao
					order by descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        if($arrDados){
            $formatoValores = "{point.percentage:.2f} %";
            geraGrafico($arrDados, "graficoPizzaRacaCorPROUNI", "","<b>{series.name}: {point.percentage:.2f}%</b>",$formatoValores,"Ocorrências", null, null, null, null, true);
        }
    }
}

function gerarGraficoLeiCotas1($tipo){

    global $db;

    if($tipo==2){
        $where = "AND dsh.tidid2 IN (8892, 8895)";
    }if($tipo==3){
        $where = "AND dsh.tidid2 IN (8893, 8896)";
    }

    $sql = "SELECT descricao, SUM(valor1) AS valor1, SUM(valor2) AS valor2, SUM(valor3) AS valor3, SUM(valor4) AS valor4, SUM(valortotal1) AS valortotal1, SUM(valortotal2) AS valortotal2, SUM(valortotal3) AS valortotal3, SUM(valortotal4) AS valortotal4
            FROM(
                select UPPER(iesdsc) as descricao, dshqtde as valor1, 0 as valor2, 0 as valor3, 0 as valor4, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal1, 0 AS valortotal2, 0 AS valortotal3, 0 AS valortotal4
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 11 and fn.fuestatus = 'A'
                where sh.indid in (3180)
                and sh.sehstatus <> 'I'
                AND sh.dpeid = 1222 --1º Semestre/2013
                $where
            UNION ALL
                select UPPER(iesdsc) as descricao, 0 as valor1, dshqtde as valor2, 0 as valor3, 0 as valor4, 0 AS valortotal1, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal2, 0 AS valortotal3, 0 AS valortotal4
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 11 and fn.fuestatus = 'A'
                where sh.indid in (3180)
                and sh.sehstatus <> 'I'
                AND sh.dpeid = 1223 --2º Semestre/2013
                $where
            UNION ALL
                select UPPER(iesdsc) as descricao, 0 as valor1, 0 as valor2, dshqtde as valor3, 0 as valor4, 0 AS valortotal1, 0 AS valortotal2, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal3, 0 AS valortotal4
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 11 and fn.fuestatus = 'A'
                where sh.indid in (3180)
                and sh.sehstatus <> 'I'
                AND sh.dpeid = 1248 --1º Semestre/2014
                $where
            UNION ALL
                select UPPER(iesdsc) as descricao, 0 as valor1, 0 as valor2, 0 as valor3, dshqtde as valor4, 0 AS valortotal1, 0 AS valortotal2, 0 AS valortotal3, CASE WHEN dsh.tidid1 NOT IN (8894) THEN dshqtde ELSE 0 END valortotal4
                from painel.seriehistorica sh
                inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                inner join painel.ies ies ON ies.iesid = dsh.dshcod::integer
                inner join entidade.funcaoentidade fn on ies.entid = fn.entid and fn.funid = 11 and fn.fuestatus = 'A'
                where sh.indid in (3180)
                and sh.sehstatus <> 'I'
                AND sh.dpeid = 1249 --2º Semestre/2014
                $where
            ) AS FOO
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $contaUniversidades1 = 0;
    $contaUniversidades2 = 0;
    $contaUniversidades3 = 0;
    $contaUniversidades4 = 0;
    $somavalortotal1 = 0;
    $somavalortotal2 = 0;
    $somavalortotal3 = 0;
    $somavalortotal4 = 0;
    ?>
    <div id="popdiv">
        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
            <tr>
                <th>Instituição</th>
                <th>1º Semestre/2013</th>
                <th>2º Semestre/2013</th>
                <th>1º Semestre/2014</th>
                <th>2º Semestre/2014</th>
            </tr>
            <?php
            if($arrDados){
                $count = -1;
                foreach($arrDados as $dado):
                    $count++;?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td><?php echo $dado['descricao'] ?></td>
                        <td class="numero" ><?php echo ($dado['valor1'] > 0 ? number_format(($dado['valortotal1']/$dado['valor1'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                        <td class="numero" ><?php echo ($dado['valor2'] > 0 ? number_format(($dado['valortotal2']/$dado['valor2'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                        <td class="numero" ><?php echo ($dado['valor3'] > 0 ? number_format(($dado['valortotal3']/$dado['valor3'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                        <td class="numero" ><?php echo ($dado['valor4'] > 0 ? number_format(($dado['valortotal4']/$dado['valor4'])*100,2,",",".").'%' : '<center>-</center>') ?></td>
                    </tr>
                    <?php
                    if($dado['valor1'] > 0){
                        $contaUniversidades1 += 1;
                        $somavalortotal1 += $dado['valortotal1']/$dado['valor1'];
                    }
                    if($dado['valor2'] > 0){
                        $contaUniversidades2 += 1;
                        $somavalortotal2 += $dado['valortotal2']/$dado['valor2'];
                    }
                    if($dado['valor3'] > 0){
                        $contaUniversidades3 += 1;
                        $somavalortotal3 += $dado['valortotal3']/$dado['valor3'];
                    }
                    if($dado['valor4'] > 0){
                        $contaUniversidades4 += 1;
                        $somavalortotal4 += $dado['valortotal4']/$dado['valor4'];
                    }
                endforeach;
            }
            ?>
            <tr>
                <th>Total</th>
                <th><?php echo ($contaUniversidades1 > 0 ? number_format(($somavalortotal1/$contaUniversidades1)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                <th><?php echo ($contaUniversidades2 > 0 ? number_format(($somavalortotal2/$contaUniversidades2)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                <th><?php echo ($contaUniversidades3 > 0 ? number_format(($somavalortotal3/$contaUniversidades3)*100,2,",",".").'%' : '<center>-</center>') ?></th>
                <th><?php echo ($contaUniversidades4 > 0 ? number_format(($somavalortotal4/$contaUniversidades4)*100,2,",",".").'%' : '<center>-</center>') ?></th>
            </tr>
        </table>
    </div>
<?php
}
function gerarGraficoLeiCotas2($tipo){

    global $db;

    if($tipo==1){
        $where = "AND fn.funid IN (12)";
    }if($tipo==2){
        $where = "AND fn.funid IN (11)";
    }if($tipo==3){
        $where = "AND fn.funid IN (11,12) AND dsh.tidid2 IN (8892, 8895)";
    }

    $sql = "SELECT periodo, vagas, CASE WHEN COALESCE(SUM(total),0) > 0 THEN (SUM(totaldetalhe)/SUM(total))*100 ELSE 0 END AS valor
            FROM (
                SELECT
                    dpe.dpeanoref||dpe.dpeordem AS ordem,
                    Replace(dpe.dpedsc, 'º Semestre', 'o Sem.') AS periodo,
                    tid1.tiddsc AS vagas,
                    CASE dsh.tidid1
                        WHEN 8888 THEN 1
                        WHEN 8889 THEN 2
                        WHEN 8890 THEN 3
                        WHEN 8891 THEN 4
                    END AS ordemdetalhe,
                    total,
                    SUM(dsh.dshqtde::INTEGER) AS totaldetalhe
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.fuestatus = 'A'
                INNER JOIN (
                    SELECT
                        SUM(dsh.dshqtde::INTEGER) AS total,
                        sh.dpeid
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::INTEGER
                    INNER JOIN entidade.funcaoentidade fn ON ies.entid = fn.entid AND fn.fuestatus = 'A'
                    WHERE sh.indid in (3180)
                    AND sh.sehstatus <> 'I'
                    AND sh.dpeid IN (1222, 1248) --1º Semestre/2013 e 1º Semestre/2014
                    $where
                    GROUP BY sh.dpeid) AS foo ON foo.dpeid = sh.dpeid
                WHERE sh.indid in (3180)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 NOT IN (8894)
                AND sh.dpeid IN (1222, 1248) --1º Semestre/2013 e 1º Semestre/2014
                $where
                GROUP BY ordem, ordemdetalhe, periodo, vagas, total
            ) AS FOO1
            GROUP BY ordem, ordemdetalhe, periodo, vagas
            ORDER BY ordem, ordemdetalhe";
    $dados = $db->carregar($sql, null, 3200);
    if($dados){
        $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'vagas', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.') + '%'; }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '%</b>'; }",
            'exibirvalor' => ",format: '{point.y:.2f}%'"
        );

        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaLeiCotasES', null, null, $formatoValores, null, "", 1000, 600, true, 300, false);
    }
}
?>