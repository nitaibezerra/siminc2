<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_pronatec&acao=A');

if(isset($_REQUEST['tipo_selecionado'])){
    gerarTabelaPronatec($_REQUEST['tipo_selecionado']);
    die;
}
?>

<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <link rel="stylesheet" type="text/css" href="/library/jquery/jquery-ui-1.10.3/themes/dark-hive/jquery-ui-1.10.3.custom.min.css"/>
    
    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
	<script language="javascript" type="text/javascript" src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>    
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>

    <script>
        atualizaUsuario();

        jQuery(function(){
            jQuery('.filtrar_tipo').click(function(){
                jQuery('#div_pronatec').load('estrategico.php?modulo=principal/popupMetasPronatec&acao=A&tipo_selecionado='+jQuery(this).val());
            });
        });

        function visualizarXLS(tipo){
            window.location='estrategico.php?modulo=principal/popupMetasPronatec&acao=A&tipo='+tipo;
        }
    </script>

    <style type="text/css">
        #div-ciclos{
            height: 900px;
        }
        #div-qtd{
            height: 550px;
        }
        .fundo_titulo{
            background-image:url('../imagens/fundobrasilpro.jpg');
        }
    </style>

</head>
<body>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" >Pronatec<br>(2011 - 2014)</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" align="middle">
			<div>
				<?php exibirTitulo('alvo', 'Metas / Execução'); ?>
			</div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_tipo" name="tipo" value="1" checked="checked" onclick="" />Metas/Execução
                <input type="radio" class="filtrar_tipo" name="tipo" value="2" onclick="" />Metas
                <input type="radio" class="filtrar_tipo" name="tipo" value="3" onclick="" />Execução
            </div>
            <br>
            <br>
            <div id="div_pronatec">
                <?php gerarTabelaPronatec(1); ?>
            </div>
		</td>
	</tr>
</table>
</body>
</html>
<?php
function gerarTabelaPronatec($tipo){

    global $db;

    $sql = "SELECT acao, ano, SUM(executado) AS executado, SUM(meta) AS meta
            FROM (
                SELECT '1-Bolsa-Formação Estudante (Inclui Sisutec)' AS acao, 2011 AS ano, 0 AS executado, 9415 AS meta
                UNION ALL
                SELECT '1-Bolsa-Formação Estudante (Inclui Sisutec)' AS acao, 2012 AS ano, 0 AS executado, 99149 AS meta
                UNION ALL
                SELECT '1-Bolsa-Formação Estudante (Inclui Sisutec)' AS acao, 2013 AS ano, 0 AS executado, 151313 AS meta
                UNION ALL
                SELECT '1-Bolsa-Formação Estudante (Inclui Sisutec)' AS acao, 2014 AS ano, 0 AS executado, 151313 AS meta
                UNION ALL
                SELECT '2-Brasil Profissionalizado' AS acao, 2011 AS ano, 0 AS executado, 33295 AS meta
                UNION ALL
                SELECT '2-Brasil Profissionalizado' AS acao, 2012 AS ano, 0 AS executado, 90563 AS meta
                UNION ALL
                SELECT '2-Brasil Profissionalizado' AS acao, 2013 AS ano, 0 AS executado, 172321 AS meta
                UNION ALL
                SELECT '2-Brasil Profissionalizado' AS acao, 2014 AS ano, 0 AS executado, 233781 AS meta
                UNION ALL
                SELECT '3-E-TEC Brasil' AS acao, 2011 AS ano, 0 AS executado, 74000 AS meta
                UNION ALL
                SELECT '3-E-TEC Brasil' AS acao, 2012 AS ano, 0 AS executado, 150000 AS meta
                UNION ALL
                SELECT '3-E-TEC Brasil' AS acao, 2013 AS ano, 0 AS executado, 200000 AS meta
                UNION ALL
                SELECT '3-E-TEC Brasil' AS acao, 2014 AS ano, 0 AS executado, 250000 AS meta
                UNION ALL
                SELECT '4-Acordo de Gratuidade Sistema S' AS acao, 2011 AS ano, 0 AS executado, 56416 AS meta
                UNION ALL
                SELECT '4-Acordo de Gratuidade Sistema S' AS acao, 2012 AS ano, 0 AS executado, 76119 AS meta
                UNION ALL
                SELECT '4-Acordo de Gratuidade Sistema S' AS acao, 2013 AS ano, 0 AS executado, 110545 AS meta
                UNION ALL
                SELECT '4-Acordo de Gratuidade Sistema S' AS acao, 2014 AS ano, 0 AS executado, 161389 AS meta
                UNION ALL
                SELECT '5-Rede Federal de EPCT' AS acao, 2011 AS ano, 0 AS executado, 72000 AS meta
                UNION ALL
                SELECT '5-Rede Federal de EPCT' AS acao, 2012 AS ano, 0 AS executado, 79560 AS meta
                UNION ALL
                SELECT '5-Rede Federal de EPCT' AS acao, 2013 AS ano, 0 AS executado, 90360 AS meta
                UNION ALL
                SELECT '5-Rede Federal de EPCT' AS acao, 2014 AS ano, 0 AS executado, 101160 AS meta
                UNION ALL
                SELECT '6-Bolsa-Formação Trabalhador' AS acao, 2011 AS ano, 0 AS executado, 226421 AS meta
                UNION ALL
                SELECT '6-Bolsa-Formação Trabalhador' AS acao, 2012 AS ano, 0 AS executado, 590937 AS meta
                UNION ALL
                SELECT '6-Bolsa-Formação Trabalhador' AS acao, 2013 AS ano, 0 AS executado, 743717 AS meta
                UNION ALL
                SELECT '6-Bolsa-Formação Trabalhador' AS acao, 2014 AS ano, 0 AS executado, 1013027 AS meta
                UNION ALL
                SELECT '7-Acordo de Gratuidade Sistema S' AS acao, 2011 AS ano, 0 AS executado, 421723 AS meta
                UNION ALL
                SELECT '7-Acordo de Gratuidade Sistema S' AS acao, 2012 AS ano, 0 AS executado, 570020 AS meta
                UNION ALL
                SELECT '7-Acordo de Gratuidade Sistema S' AS acao, 2013 AS ano, 0 AS executado, 821965 AS meta
                UNION ALL
                SELECT '7-Acordo de Gratuidade Sistema S' AS acao, 2014 AS ano, 0 AS executado, 1194266 AS meta
                UNION ALL
                SELECT '8-Rede Federal de EPCT (FIC >= 160h)' AS acao, 2011 AS ano, 0 AS executado, 0 AS meta
                UNION ALL
                SELECT '8-Rede Federal de EPCT (FIC >= 160h)' AS acao, 2012 AS ano, 0 AS executado, 0 AS meta
                UNION ALL
                SELECT '8-Rede Federal de EPCT (FIC >= 160h)' AS acao, 2013 AS ano, 0 AS executado, 0 AS meta
                UNION ALL
                SELECT '8-Rede Federal de EPCT (FIC >= 160h)' AS acao, 2014 AS ano, 0 AS executado, 0 AS meta
                UNION ALL
                SELECT
                    CASE
                        WHEN ((sh.indid IN (2299) AND dsh.tidid1 IN (4860, 9582)) OR sh.indid IN (2302)) THEN '1-Bolsa-Formação Estudante (Inclui Sisutec)'
                        WHEN sh.indid IN (2539) THEN '2-Brasil Profissionalizado'
                        WHEN sh.indid IN (2393) OR (sh.indid IN (2347) AND dsh.tidid2 IN (5132)) THEN '3-E-TEC Brasil'
                        WHEN sh.indid IN (2399) AND dsh.tidid1 IN (5299) THEN '4-Acordo de Gratuidade Sistema S'
                        WHEN sh.indid IN (2347) AND dsh.tidid2 IN (5130,5131,5188,5190,5189) THEN '5-Rede Federal de EPCT'
                        WHEN sh.indid IN (2299) AND dsh.tidid1 IN (4858) THEN '6-Bolsa-Formação Trabalhador'
                        WHEN sh.indid IN (2399) AND dsh.tidid1 IN (5298) THEN '7-Acordo de Gratuidade Sistema S'
                        WHEN sh.indid IN (2347) AND dsh.tidid2 IN (5129) THEN '8-Rede Federal de EPCT (FIC >= 160h)'
                    END AS acao,
                    --TEMPORARIAMENTE ATÉ INCLUIRMOS O ANO DE 2015--
                    --CASE
                    --    WHEN dpeanoref::INTEGER > 2014 THEN 2014
                    --    WHEN dpeanoref::INTEGER < 2011 THEN 2011
                    --ELSE dpeanoref::INTEGER
                    --END AS ano,
                    ----------------------------------------------
                    dpe.dpeanoref::INTEGER AS ano,
                    sum(dsh.dshqtde::INTEGER) AS executado,
                    0 AS meta
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE (
                    (sh.indid IN (2299, 2399))
                    OR
                    (sh.indid IN (2539))
                    OR
                    (sh.indid IN (2393))
                    OR
                    --MANTÉM O 5132 POIS CONTABILIZA NO E-TEC
                    (sh.indid IN (2347) AND dsh.tidid2 IN (5130,5131,5188,5190,5189,5129,5132))
                )
                AND sh.sehstatus <> 'I'
                GROUP BY acao, ano
            ) AS FOO
            WHERE ano BETWEEN '2011' AND '2014'
            GROUP BY acao, ano
            ORDER BY acao, ano";
    $arrDados = $db->carregar( $sql );
    //GERAR DADOS EM EXCEL
    $gerarExcel = $_REQUEST['tipo'];
    if($gerarExcel==1){
        ob_clean();
        $db->sql_to_excel( $arrDados, "Dados PRONATEC" );
    }
    if($arrDados){
        foreach($arrDados as $dado){
            $arrAcao[$dado['acao']][$dado['ano']]['meta']['total'] = $dado['meta'];
            $arrAcao[$dado['acao']][$dado['ano']]['executado']['total'] = $dado['executado'];
        }
    }
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th>Cursos Técnicos</th>
            <th>&nbsp;</th>
            <th>2011</th>
            <th>2012</th>
            <th>2013</th>
            <th>2014</th>
            <th>Total 2011-2014</th>
        </tr>
        <?php
        $count=-1;
        $somaMeta2011=0;
        $somaMeta2012=0;
        $somaMeta2013=0;
        $somaMeta2014=0;
        $somaExecutado2011=0;
        $somaExecutado2012=0;
        $somaExecutado2013=0;
        $somaExecutado2014=0;
        foreach($arrAcao as $acao => $aca){
            $count++;
            if($count==5){
                echo"<tr>";
                echo"   <td colspan='7'>&nbsp;</td>";
                echo"</tr>";
                echo"<tr>";
                echo"   <th>Cursos Formação Inicial e Continuada</th>";
                echo"   <th>&nbsp;</th>";
                echo"   <th>2011</th>";
                echo"   <th>2012</th>";
                echo"   <th>2013</th>";
                echo"   <th>2014</th>";
                echo"   <th>Total 2011-2014</th>";
                echo"</tr>";
            }
            ?>
            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                <td rowspan="2"><?php echo str_replace(array("1-","2-","3-","4-","5-","6-","7-","8-"),"",$acao) ?></td>
                <?php if($tipo==1 || $tipo==2){?>
                    <td class="bold">Meta</td>
                    <td class="numero bold" ><?php echo number_format($aca['2011']['meta']['total'],0,",",".") ?></td>
                    <td class="numero bold" ><?php echo number_format($aca['2012']['meta']['total'],0,",",".") ?></td>
                    <td class="numero bold" ><?php echo number_format($aca['2013']['meta']['total'],0,",",".") ?></td>
                    <td class="numero bold" ><?php echo number_format($aca['2014']['meta']['total'],0,",",".") ?></td>
                    <td class="numero bold" ><?php echo number_format($aca['2011']['meta']['total']+$aca['2012']['meta']['total']+$aca['2013']['meta']['total']+$aca['2014']['meta']['total'],0,",",".") ?></td>
                <?php } ?>
            </tr>
            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                <?php if($tipo==1 || $tipo==3){ ?>
                    <td class="bold">Executado</td>
                    <td class="numero" ><?php echo number_format($aca['2011']['executado']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($aca['2012']['executado']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($aca['2013']['executado']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($aca['2014']['executado']['total'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($aca['2011']['executado']['total']+$aca['2012']['executado']['total']+$aca['2013']['executado']['total']+$aca['2014']['executado']['total'],0,",",".") ?></td>
                <?php } ?>
            </tr>
            <?php
            $somaMeta2011+=$aca['2011']['meta']['total'];
            $somaMeta2012+=$aca['2012']['meta']['total'];
            $somaMeta2013+=$aca['2013']['meta']['total'];
            $somaMeta2014+=$aca['2014']['meta']['total'];
            $somaExecutado2011+=$aca['2011']['executado']['total'];
            $somaExecutado2012+=$aca['2012']['executado']['total'];
            $somaExecutado2013+=$aca['2013']['executado']['total'];
            $somaExecutado2014+=$aca['2014']['executado']['total'];
            $totalMeta2011+=$aca['2011']['meta']['total'];
            $totalMeta2012+=$aca['2012']['meta']['total'];
            $totalMeta2013+=$aca['2013']['meta']['total'];
            $totalMeta2014+=$aca['2014']['meta']['total'];
            $totalExecutado2011+=$aca['2011']['executado']['total'];
            $totalExecutado2012+=$aca['2012']['executado']['total'];
            $totalExecutado2013+=$aca['2013']['executado']['total'];
            $totalExecutado2014+=$aca['2014']['executado']['total'];
            if($count==4 || $count==7){
                echo"<tr>";
                echo"   <th rowspan='2'>TOTAL</th>";
                if($tipo==1 || $tipo==2){
                    echo"   <th>Total Meta</th>";
                    echo"   <th>".number_format($somaMeta2011,0,",",".")."</th>";
                    echo"   <th>".number_format($somaMeta2012,0,",",".")."</th>";
                    echo"   <th>".number_format($somaMeta2013,0,",",".")."</th>";
                    echo"   <th>".number_format($somaMeta2014,0,",",".")."</th>";
                    echo"   <th>".number_format($somaMeta2011+$somaMeta2012+$somaMeta2013+$somaMeta2014,0,",",".")."</th>";
                }
                echo"</tr>";
                echo"<tr>";
                if($tipo==1 || $tipo==3){
                    echo"   <th>Total Executado</th>";
                    echo"   <th>".number_format($somaExecutado2011,0,",",".")."</th>";
                    echo"   <th>".number_format($somaExecutado2012,0,",",".")."</th>";
                    echo"   <th>".number_format($somaExecutado2013,0,",",".")."</th>";
                    echo"   <th>".number_format($somaExecutado2014,0,",",".")."</th>";
                    echo"   <th>".number_format($somaExecutado2011+$somaExecutado2012+$somaExecutado2013+$somaExecutado2014,0,",",".")."</th>";
                }
                echo"</tr>";
                $somaMeta2011=0;
                $somaMeta2012=0;
                $somaMeta2013=0;
                $somaMeta2014=0;
                $somaExecutado2011=0;
                $somaExecutado2012=0;
                $somaExecutado2013=0;
                $somaExecutado2014=0;
            }
            ?>
        <?php
        }
        echo"<tr>";
        echo"   <td colspan='7'>&nbsp;</td>";
        echo"</tr>";
        echo"<tr>";
        echo"   <th rowspan='2'>TOTAL</th>";
        if($tipo==1 || $tipo==2){
            echo"   <th>Total Meta</th>";
            echo"   <th>".number_format($totalMeta2011,0,",",".")."</th>";
            echo"   <th>".number_format($totalMeta2012,0,",",".")."</th>";
            echo"   <th>".number_format($totalMeta2013,0,",",".")."</th>";
            echo"   <th>".number_format($totalMeta2014,0,",",".")."</th>";
            echo"   <th>".number_format($totalMeta2011+$totalMeta2012+$totalMeta2013+$totalMeta2014,0,",",".")."</th>";
        }
        echo"</tr>";
        echo"<tr>";
        if($tipo==1 || $tipo==3){
            echo"   <th>Total Executado</th>";
            echo"   <th>".number_format($totalExecutado2011,0,",",".")."</th>";
            echo"   <th>".number_format($totalExecutado2012,0,",",".")."</th>";
            echo"   <th>".number_format($totalExecutado2013,0,",",".")."</th>";
            echo"   <th>".number_format($totalExecutado2014,0,",",".")."</th>";
            echo"   <th>".number_format($totalExecutado2011+$totalExecutado2012+$totalExecutado2013+$totalExecutado2014,0,",",".")."</th>";
        }
        echo"</tr>";
        echo"<tr>";
        echo"   <td class='fundo_padrao' align='left' colspan='6'>";
        echo"      <input type='button' name='btnXls' value='Visualizar XLS' onclick='visualizarXLS(1);' style='cursor: pointer;'/>";
        echo"   </td>";
        echo"</tr>";
    echo"</table>";
}
?>