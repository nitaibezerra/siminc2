<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_orcamentario1'])){
    gerarGraficoOrcamentario1($_REQUEST['detalhe_orcamentario1']);
    die;
}
if(isset($_REQUEST['detalhe_orcamentario2'])){
    gerarGraficoOrcamentario2($_REQUEST['detalhe_orcamentario2']);
    die;
}
if(isset($_REQUEST['detalhe_orcamentario3'])){
    gerarGraficoOrcamentario3($_REQUEST['detalhe_orcamentario3']);
    die;
}
if(isset($_REQUEST['detalhe_orcamentario4'])){
    gerarGraficoOrcamentario4($_REQUEST['detalhe_orcamentario4']);
    die;
}
if(isset($_REQUEST['detalhe_orcamentario5'])){
    gerarGraficoOrcamentario5($_REQUEST['detalhe_orcamentario5']);
    die;
}
if(isset($_REQUEST['detalhe_orcamentario6'])){
    gerarGraficoOrcamentario6($_REQUEST['detalhe_orcamentario6']);
    die;
}
if(isset($_REQUEST['detalhe_orcamentario7'])){
    gerarGraficoOrcamentario7($_REQUEST['detalhe_orcamentario7']);
    die;
}
if(!empty($_REQUEST['atualizarAbrangencia'])){
    carregarDetalhesAbrangenciaEstado(array('esiid'=>$_REQUEST['esiid']));
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
            jQuery('.filtrar_orcamentario1').click(function(){
                jQuery('#div_orcamentario1').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario1='+jQuery(this).val());
            });
        });
        jQuery(function(){
            jQuery('.filtrar_orcamentario2').click(function(){
                jQuery('#div_orcamentario2').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario2='+jQuery(this).val());
            });
        });
        jQuery(function(){
            jQuery('.filtrar_orcamentario3').click(function(){
                jQuery('#div_orcamentario3').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario3='+jQuery(this).val());
            });
        });
        jQuery(function(){
            jQuery('.filtrar_orcamentario4').click(function(){
                jQuery('#div_orcamentario4').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario4='+jQuery(this).val());
            });
        });
        jQuery(function(){
            jQuery('.filtrar_orcamentario5').click(function(){
                jQuery('#div_orcamentario5').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario5='+jQuery(this).val());
            });
        });
        jQuery(function(){
            jQuery('.filtrar_orcamentario6').click(function(){
                jQuery('#div_orcamentario6').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario6='+jQuery(this).val());
            });
        });
        jQuery(function(){
            jQuery('.filtrar_orcamentario7').click(function(){
                jQuery('#div_orcamentario7').load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&detalhe_orcamentario7='+jQuery(this).val());
            });
        });

        function detalharAbrangenciaEstado(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 7;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_orcamento&acao=A&atualizarAbrangencia=1&esiid='+esiid);
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_orcamento&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >Orçamento e Execução</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(240);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Evolução da execução orçamentária do MEC'); ?>
			</div>
            <?php
            $sql = "SELECT acao, periodo, valor
                    FROM (
                        SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (240)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    UNION ALL
                        SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                        WHERE sh.indid in (240)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo, ipc.ipcindice
                    ) AS FOO
                    ORDER BY periodo";
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
                    $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                    $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
                    $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] = $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                    array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoMEC", "Evolução da execução orçamentária do MEC",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
            }
            ?>
		</td>
		<td class="fundo_padrao" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Investimento em educação em relação ao PIB (%)', '<span class="link" onclick="abreIndicadorPopUp(1538);">Investimento público total em educação</span>, <span class="link" onclick="abreIndicadorPopUp(626);">Investimento público direto em educação</span>'); ?>
			</div>
            <?php
            $sql = "SELECT
                        sh.indid AS acao,
                        dpe.dpeanoref AS ano,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (1538,626)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, ano
                    ORDER BY ano, acao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['ano']][$dado['acao']]['total'][] = $dado['valor'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrTotal = array();

                foreach($arrSituacao as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = ((is_array($sit['1538']['total'])?array_sum($sit['1538']['total']):0));
                    $vB = ((is_array($sit['626']['total'])?array_sum($sit['626']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] = $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'Investimento total em relacao ao PIB', 'data'=>$arrA),
                    array('name' => 'Investimento direto em relacao ao PIB', 'data'=>$arrB),
                    //array('name' => 'Total de Matriculas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaInvestimentoPIB", "Investimento em educação em relação ao PIB (%)",300, $formatoValores);
            }
            ?>
		</td>
	</tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária do FNDE', '<span class="link" onclick="abreIndicadorPopUp(3109);">Com FIES e cota salário-educação</span>, <span class="link" onclick="abreIndicadorPopUp(3110);">Sem FIES e cota salário-educação</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario1" name="orcamentario1" value="1" checked="checked" onclick="" />Com FIES e cota salário-educação
                <input type="radio" class="filtrar_orcamentario1" name="orcamentario1" value="2" onclick="" />Sem FIES e cota salário-educação
            </div>
            <br>
            <div id="div_orcamentario1">
                <?php gerarGraficoOrcamentario1(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3111);">
            <div>
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária da CAPES'); ?>
            </div>
            <?php
            $sql = "SELECT acao, periodo, valor
                    FROM (
                        SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (3111)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    UNION ALL
                        SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                        WHERE sh.indid in (3111)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo, ipc.ipcindice
                    ) AS FOO
                    ORDER BY periodo";
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
                    $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                    $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
                    $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] = $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                    array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoCAPES", "Evolução da execução orçamentária da CAPES",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3112);">
            <div>
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária do INEP'); ?>
            </div>
            <?php
            $sql = "SELECT acao, periodo, valor
                    FROM (
                        SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (3112)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    UNION ALL
                        SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                        WHERE sh.indid in (3112)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo, ipc.ipcindice
                    ) AS FOO
                    ORDER BY periodo";
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
                    $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                    $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
                    $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] = $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                    array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoINEP", "Evolução da execução orçamentária do INEP",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3113);">
            <div>
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária da EBSERH'); ?>
            </div>
            <?php
            $sql = "SELECT acao, periodo, valor
                    FROM (
                        SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (3113)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    UNION ALL
                        SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                        WHERE sh.indid in (3113)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo, ipc.ipcindice
                    ) AS FOO
                    ORDER BY periodo";
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
                    $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                    $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
                    $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] = $vA + $vB;
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                    array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoEBSERH", "Evolução da execução orçamentária da EBSERH",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(3118);">
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária das<br>Instituições Federais de Educação Profissional e Tecnológica'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario2" name="orcamentario2" value="1" checked="checked" onclick="" />Total geral
                <input type="radio" class="filtrar_orcamentario2" name="orcamentario2" value="2" onclick="" />Total geral (sem inativos)
                <input type="radio" class="filtrar_orcamentario2" name="orcamentario2" value="3" onclick="" />Total geral (sem inativos e convênios)
            </div>
            <br>
            <div id="div_orcamentario2">
                <?php gerarGraficoOrcamentario2(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(3119);">
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária das<br>Instituições Federais de Educação Superior'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario3" name="orcamentario3" value="1" checked="checked" onclick="" />Total geral
                <input type="radio" class="filtrar_orcamentario3" name="orcamentario3" value="2" onclick="" />Total geral (sem inativos)
                <input type="radio" class="filtrar_orcamentario3" name="orcamentario3" value="3" onclick="" />Total geral (sem inativos e convênios)
            </div>
            <br>
            <div id="div_orcamentario3">
                <?php gerarGraficoOrcamentario3(1); ?>
            </div>
        </td>
    </tr>
<?php /* ?>
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(2897);">
                <?php exibirTitulo('indicador', 'Despesa Total dos Institutos Federais de<br>Educação, Ciência e Tecnologia'); ?>
            </div>
            <br>
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
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoDespesaInstitutos", "Despesa total (sem inativos)",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2014: Previsão</td></tr></table>";
            }
            ?>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(2895);">
                <?php exibirTitulo('indicador', 'Despesa Total das Universidades e<br>Hospitais Universitários Federais'); ?>
            </div>
            <br>
            <?php
            $sql = "SELECT dpe.dpeanoref AS periodo, tid1.tidid AS modalidade, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN entidade.entidade e ON e.entunicod::varchar = dsh.unicod::varchar
                    WHERE sh.indid in (2895)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, modalidade
                    ORDER BY periodo, modalidade";
            $arrDados = $db->carregar( $sql, null, 3200 );
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
                    $vA = Round((is_array($sit['7831']['total'])?array_sum($sit['7831']['total']):0));
                    $vB = Round((is_array($sit['7834']['total'])?array_sum($sit['7834']['total']):0));
                    $vC = Round((is_array($sit['7835']['total'])?array_sum($sit['7835']['total']):0));
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
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoDespesaUniversidades", "Despesa total (sem inativos)",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2014: Previsão</td></tr></table>";
            }
            ?>
        </td>
    </tr>
 <?php */ ?>
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(519);">
                <?php exibirTitulo('indicador', 'FUNDEB'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario4" name="orcamentario4" value="1" checked="checked" onclick="" />Recursos totais
                <input type="radio" class="filtrar_orcamentario4" name="orcamentario4" value="2" onclick="" />Receita dos estados e municípios
                <input type="radio" class="filtrar_orcamentario4" name="orcamentario4" value="3" onclick="" />Complementação da união
            </div>
            <br>
            <div id="div_orcamentario4">
                <?php gerarGraficoOrcamentario4(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'Salário-Educação', '<span class="link" onclick="abreIndicadorPopUp(569);">Evolução da arrecadação bruta</span>, <span class="link" onclick="abreIndicadorPopUp(537);">Evolução da distribuição para estados e municípios</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario5" name="orcamentario5" value="1" checked="checked" onclick="" />Evolução da arrecadação bruta
                <input type="radio" class="filtrar_orcamentario5" name="orcamentario5" value="2" onclick="" />Evolução da distribuição para estados e municípios
            </div>
            <br>
            <div id="div_orcamentario5">
                <?php gerarGraficoOrcamentario5(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'Recursos para Manutenção e Desenvolvimento do Ensino', '<span class="link" onclick="abreIndicadorPopUp(3120);">Em R$ mil</span>, <span class="link" onclick="abreIndicadorPopUp(3121);">Em % da Arrecadação de Impostos</span>'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario6" name="orcamentario6" value="1" checked="checked" onclick="" />Em % da Arrecadação de Impostos
                <input type="radio" class="filtrar_orcamentario6" name="orcamentario6" value="2" onclick="" />Em R$ mil
            </div>
            <div id="div_orcamentario6">
                <?php gerarGraficoOrcamentario6(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'Educação básica X superior', '<span class="link" onclick="abreIndicadorPopUp(3116);">Participação no total</span>, <span class="link" onclick="abreIndicadorPopUp(3117);">Gasto do MEC com educação básica e superior</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_orcamentario7" name="orcamentario7" value="1" checked="checked" onclick="" />% de participação no total<br>
                <input type="radio" class="filtrar_orcamentario7" name="orcamentario7" value="2" onclick="" />Gasto do MEC com educação básica e superior (em R$ correntes)<br>
                <input type="radio" class="filtrar_orcamentario7" name="orcamentario7" value="3" onclick="" />Gasto do MEC com educação básica e superior (em R$ constantes)
            </div>
            <br>
            <div id="div_orcamentario7">
                <?php gerarGraficoOrcamentario7(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(3104);">
                <?php exibirTitulo('indicador', 'Orçamento e execução por programa e ação'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS periodo,
                        'TOTAL' AS programa,
                        tid2.tiddsc AS atividade,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid in (3104)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, programa, atividade";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['programa']][$dado['periodo']][$dado['atividade']]['total'] = $dado['valor'];
                }
            }

            $sql = "SELECT
                        periodo,
                        '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharAbrangenciaEstado(\''||codigo||'\',this);\">&nbsp;' || programa as programa,
                        atividade,
                        valor
                    FROM (
                        SELECT
                            dpe.dpeanoref AS periodo,
                            CASE
                                WHEN tid1.tidid IN (8348) THEN '0089 - PREVIDENCIA DE INATIVOS E PENSIONISTAS DA UNIAO'
                                WHEN tid1.tidid IN (8349,8350,8351,8352,8353,8354) THEN '0901 - OPERACOES ESPECIAIS: CUMPRIMENTO DE SENTENCAS JUDICIAIS'
                                WHEN tid1.tidid IN (8355) THEN '0902 - OPERACOES ESPECIAIS: FINANCIAMENTOS COM RETORNO'
                                WHEN tid1.tidid IN (8356) THEN '0903 - OPERACOES ESPECIAIS: TRANSFERENCIAS CONSTITUCIONAIS E AS DECORRENTES DE LEGISLACAO ESPECIFICA'
                                WHEN tid1.tidid IN (8357) THEN '0905 - OPERACOES ESPECIAIS: SERVICO DA DIVIDA INTERNA (JUROS EAMORTIZACOES)'
                                WHEN tid1.tidid IN (8358) THEN '0906 - OPERACOES ESPECIAIS: SERVICO DA DIVIDA EXTERNA (JUROS EAMORTIZACOES)'
                                WHEN tid1.tidid IN (8359,8360,8361,8362,8363) THEN '0909 - OPERACOES ESPECIAIS: OUTROS ENCARGOS ESPECIAIS'
                                WHEN tid1.tidid IN (8364,8365,8366,8367,8368,8369,8370,8371) THEN '0910 - OPERACOES ESPECIAIS: GESTAO DA PARTICIPACAO EM ORGANISMOS INTERNACIONAIS'
                                WHEN tid1.tidid IN (8372,8373,8374,8375,8376,8377,8378,8379,8380,8381,8382,8383,8384,8385,8386,8387,8388,8389,8390,8391,8392,8393,8394,8395,8396,8397,8398,8399,8400,8401) THEN '2030 - EDUCACAO BASICA'
                                WHEN tid1.tidid IN (8402,8403,8404,8405,8406,8407,8408,8409) THEN '2031 - EDUCACAO PROFISSIONAL E TECNOLOGICA'
                                WHEN tid1.tidid IN (8410,8411,8412,8413,8414,8415,8416,8417,8418,8419,8420,8421,8422,8423,8424,8425,8426,8427,8428,8429,8430,8431,8432,8433,8434,8435,8436,8437,8438,8439,8440,8441,8442,8443,8444,8445,8446,8447,8448,8449,8450,8451,8452,8453,8454,8455,8456,8457,8458,8459) THEN '2032 - EDUCACAO SUPERIOR - GRADUACAO, POS-GRADUACAO, ENSINO, PESQUISA E EXTENSAO'
                                WHEN tid1.tidid IN (8460) THEN '2055 - DESENVOLVIMENTO PRODUTIVO'
                                WHEN tid1.tidid IN (8461,8462,8463,8464,8465,8466,8467,8468,8469,8470,8471,8472,8473,8474,8475,8476,8477,8478,8479,8480,8481,8482,8483,8484,8485,8486,8487) THEN '2109 - PROGRAMA DE GESTAO E MANUTENCAO DO MINISTERIO DA EDUCACAO'
                            ELSE 'NÃO CATEGORIZADO'
                            END AS programa,
                            CASE
                                WHEN tid1.tidid IN (8348) THEN '8348'
                                WHEN tid1.tidid IN (8349,8350,8351,8352,8353,8354) THEN '8349,8350,8351,8352,8353,8354'
                                WHEN tid1.tidid IN (8355) THEN '8355'
                                WHEN tid1.tidid IN (8356) THEN '8356'
                                WHEN tid1.tidid IN (8357) THEN '8357'
                                WHEN tid1.tidid IN (8358) THEN '8358'
                                WHEN tid1.tidid IN (8359,8360,8361,8362,8363) THEN '8359,8360,8361,8362,8363'
                                WHEN tid1.tidid IN (8364,8365,8366,8367,8368,8369,8370,8371) THEN '8364,8365,8366,8367,8368,8369,8370,8371'
                                WHEN tid1.tidid IN (8372,8373,8374,8375,8376,8377,8378,8379,8380,8381,8382,8383,8384,8385,8386,8387,8388,8389,8390,8391,8392,8393,8394,8395,8396,8397,8398,8399,8400,8401) THEN '8372,8373,8374,8375,8376,8377,8378,8379,8380,8381,8382,8383,8384,8385,8386,8387,8388,8389,8390,8391,8392,8393,8394,8395,8396,8397,8398,8399,8400,8401'
                                WHEN tid1.tidid IN (8402,8403,8404,8405,8406,8407,8408,8409) THEN '8402,8403,8404,8405,8406,8407,8408,8409'
                                WHEN tid1.tidid IN (8410,8411,8412,8413,8414,8415,8416,8417,8418,8419,8420,8421,8422,8423,8424,8425,8426,8427,8428,8429,8430,8431,8432,8433,8434,8435,8436,8437,8438,8439,8440,8441,8442,8443,8444,8445,8446,8447,8448,8449,8450,8451,8452,8453,8454,8455,8456,8457,8458,8459) THEN '8410,8411,8412,8413,8414,8415,8416,8417,8418,8419,8420,8421,8422,8423,8424,8425,8426,8427,8428,8429,8430,8431,8432,8433,8434,8435,8436,8437,8438,8439,8440,8441,8442,8443,8444,8445,8446,8447,8448,8449,8450,8451,8452,8453,8454,8455,8456,8457,8458,8459'
                                WHEN tid1.tidid IN (8460) THEN '8460'
                                WHEN tid1.tidid IN (8461,8462,8463,8464,8465,8466,8467,8468,8469,8470,8471,8472,8473,8474,8475,8476,8477,8478,8479,8480,8481,8482,8483,8484,8485,8486,8487) THEN '8461,8462,8463,8464,8465,8466,8467,8468,8469,8470,8471,8472,8473,8474,8475,8476,8477,8478,8479,8480,8481,8482,8483,8484,8485,8486,8487'
                            ELSE '0'
                            END AS codigo,
                            tid2.tiddsc AS atividade,
                            SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                        WHERE sh.indid in (3104)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo, programa, codigo, atividade
                    ) AS FOO
                    ORDER BY periodo, programa, atividade";
            $arrDados = $db->carregar( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th rowspan="3">PROGRAMA / PROJETO/ATIVIDADE</th>
                </tr>
                <tr>
                    <th>2012</th>
                    <th>2013</th>
                    <th>2014</th>
                    <th>2015</th>
                </tr>
                <tr>
                    <th>EMPENHADO</th>
                    <th>EMPENHADO</th>
                    <th>EMPENHADO</th>
                    <th>PLOA</th>
                </tr>
                <?php
                if($arrDados){
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['programa']][$dado['periodo']][$dado['atividade']]['total'] = $dado['valor'];
                    }
                    $count=-1;
                    foreach($arrSituacao as $programa => $sit){
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td class="" ><?php echo $programa ?></td>
                            <td class="numero" ><?php echo number_format($sit['2012']['Empenhado']['total'],2,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2013']['Empenhado']['total'],2,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2014']['Empenhado']['total'],2,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2015']['PLOA']['total'],2,",",".") ?></td>
                        </tr>
                    <?php
                    }
                }?>
            </table>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="2">
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
function gerarGraficoOrcamentario1($detalhe){

    global $db;

    if($detalhe==1){
        $where = 3109;
    }else{
        $where = 3110;
    }

    $sql = "SELECT acao, periodo, valor
            FROM (
                SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in ($where)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
            UNION ALL
                SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in ($where)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo, ipc.ipcindice
            ) AS FOO
            ORDER BY periodo";
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
            $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
            $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
            $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
            $arrA[] = $vA;
            $arrB[] = $vB;
            $arrTotal[] = $vA + $vB;
        }

        $aDados = array(
            array('name' => 'Reais', 'data'=>$arrA),
            array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoFNDE", "Evolução da execução orçamentária do FNDE",300, $formatoValores);
        echo "<br><br>";
        echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
    }
}

function gerarGraficoOrcamentario2($detalhe){

    global $db;

    $where = "";

    if($detalhe==2){
        $where = "AND dsh.tidid1 IN (8325, 8319, 8321,8322, 8323, 8324)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 IN (8325, 8319, 8321,8322, 8323)";
    }

    $sql = "SELECT acao, periodo, valor
            FROM (
                SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (3118)
                AND sh.sehstatus <> 'I'
                $where
                GROUP BY periodo
            UNION ALL
                SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in (3118)
                AND sh.sehstatus <> 'I'
                $where
                GROUP BY periodo, ipc.ipcindice
            ) AS FOO
            ORDER BY periodo";
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
            $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
            $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
            $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
            $arrA[] = $vA;
            $arrB[] = $vB;
            $arrTotal[] = $vA + $vB;
        }

        $aDados = array(
            array('name' => 'Reais', 'data'=>$arrA),
            array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoIFET", "Evolução da execução orçamentária das IFET",300, $formatoValores);
        echo "<br><br>";
        echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
    }
}
function gerarGraficoOrcamentario3($detalhe){

    global $db;

    $where = "";

    if($detalhe==2){
        $where = "AND dsh.tidid1 IN (8326, 8328, 8329, 8330, 8331, 8332)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 IN (8326, 8328, 8329, 8330, 8332)";
    }

    $sql = "SELECT acao, periodo, valor
            FROM (
                SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (3119)
                AND sh.sehstatus <> 'I'
                $where
                GROUP BY periodo
            UNION ALL
                SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in (3119)
                AND sh.sehstatus <> 'I'
                $where
                GROUP BY periodo, ipc.ipcindice
            ) AS FOO
            ORDER BY periodo";
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
            $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
            $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
            $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
            $arrA[] = $vA;
            $arrB[] = $vB;
            $arrTotal[] = $vA + $vB;
        }

        $aDados = array(
            array('name' => 'Reais', 'data'=>$arrA),
            array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoIFES", "Evolução da execução orçamentária das IFES",300, $formatoValores);
        echo "<br><br>";
        echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
    }

}
function gerarGraficoOrcamentario4($detalhe){

    global $db;

    $where = "";

    if($detalhe==2){
        $where = "AND dsh.tidid1 IN (1678, 1677)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 IN (1679)";
    }

    $sql = "SELECT acao, periodo, valor
            FROM (
                SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (519)
                AND sh.sehstatus <> 'I'
                $where
                GROUP BY periodo
            UNION ALL
                SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in (519)
                AND sh.sehstatus <> 'I'
                $where
                GROUP BY periodo, ipc.ipcindice
            ) AS FOO
            ORDER BY periodo";
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
            $arrTotal[] = $vA + $vB;
        }

        $aDados = array(
            array('name' => 'Reais', 'data'=>$arrA),
            array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoFUNDEB", "FUNDEB",300, $formatoValores);
    }

}
function gerarGraficoOrcamentario5($detalhe){

    global $db;

    if($detalhe==1){
        $where = 569;
    }else{
        $where = 537;
    }

    $sql = "SELECT acao, periodo, valor
            FROM (
                SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in ($where)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
            UNION ALL
                SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in ($where)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo, ipc.ipcindice
            ) AS FOO
            ORDER BY periodo";
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
            $arrTotal[] = $vA + $vB;
        }

        $aDados = array(
            array('name' => 'Reais', 'data'=>$arrA),
            array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
        );

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
            'alinhamento' => "formatter: function() { return '1'; }"
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoSalarioEducacao", "Salário-Educação",300, $formatoValores);
    }

}
function gerarGraficoOrcamentario6($detalhe){

    global $db;

    if($detalhe==1){
        $where = "3121";
        $titulo = "Em % da Arrecadação de Impostos<br>(Mínimo de 18% da Arrecadação de Impostos)";
        $tipo = "%";
    }else{
        $where = "3120";
        $titulo = "Em R$ mil";
        $tipo = "";
    }

    $sql = "SELECT tid1.tiddsc AS tipo, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            WHERE sh.indid in ($where)
            AND sh.sehstatus <> 'I'
            GROUP BY periodo, tipo, dsh.tidid1
            ORDER BY periodo, dsh.tidid1";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">Ano</th>
            <th colspan="3"><?=$titulo?></th>
        </tr>
        <tr>
            <th>LOA</th>
            <th>Autorizado</th>
            <th>Empenho Liquidado</th>
        </tr>
        <?php
        if($arrDados){
            $arrSituacao = array();
            foreach($arrDados as $dado){
                $arrSituacao[$dado['periodo']][$dado['tipo']]['total'] = $dado['valor'];
            }
            $count=-1;
            foreach($arrSituacao as $periodo => $sit){
                $count++;
                ?>
                <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                    <td class="" ><?php echo ($periodo=='2015' ? '*2015' : $periodo) ?></td>
                    <td class="numero" ><?php echo number_format($sit['LOA']['total'],2,",",".") ?><?=$tipo?></td>
                    <td class="numero" ><?php echo number_format($sit['Autorizado']['total'],2,",",".") ?><?=$tipo?></td>
                    <td class="numero" ><?php echo number_format($sit['Empenho Liquidado']['total'],2,",",".") ?><?=$tipo?></td>
                </tr>
                <?php
            }
            //echo"<tr><td class='bold' colspan='4'>*2015: Previsão</td></tr>";
        }?>
    </table>
    <?php
}
function gerarGraficoOrcamentario7($detalhe){

    global $db;

    if($detalhe==1){
        $sql = "SELECT dsh.tidid1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                WHERE sh.indid in (3116)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 IN (8339, 8340)
                GROUP BY acao, periodo
                ORDER BY periodo";
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
                $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                $vA = ((is_array($sit['8339']['total'])?array_sum($sit['8339']['total']):0));
                $vB = ((is_array($sit['8340']['total'])?array_sum($sit['8340']['total']):0));
                $arrA[] = $vA;
                $arrB[] = $vB;
                $arrTotal[] = $vA + $vB;
            }

            $aDados = array(
                array('name' => 'Educacao basica', 'data'=>$arrA),
                array('name' => 'Ensino superior (sem FIES)', 'data'=>$arrB),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 2, ',', '.') + '%</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoPercentualParticipacao", "% de participação no total",300, $formatoValores);
            echo "<br><br>";
            echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
        }

    }elseif($detalhe==2){
        $sql = "SELECT tid1.tiddsc AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                WHERE sh.indid in (3117)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 IN (8342, 8343)
                GROUP BY acao, periodo
                ORDER BY periodo";
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
                $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                $vA = ((is_array($sit['Educação básica']['total'])?array_sum($sit['Educação básica']['total']):0));
                $vB = ((is_array($sit['Ensino superior (sem FIES)']['total'])?array_sum($sit['Ensino superior (sem FIES)']['total']):0));
                $arrA[] = $vA;
                $arrB[] = $vB;
                $arrTotal[] = $vA + $vB;
            }

            $aDados = array(
                array('name' => 'Educacao basica', 'data'=>$arrA),
                array('name' => 'Ensino superior (sem FIES)', 'data'=>$arrB),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoGastosMEC", "Gastos do MEC (em R$ correntes)",300, $formatoValores);
            echo "<br><br>";
            echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
        }
    }else{
        $sql = "SELECT tid1.tiddsc AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in (3117)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 IN (8342, 8343)
                GROUP BY acao, periodo, ipc.ipcindice
                ORDER BY periodo";
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
                $arrX[] = ($periodo=='2015' ? '*2015' : $periodo);
                $vA = ((is_array($sit['Educação básica']['total'])?array_sum($sit['Educação básica']['total']):0));
                $vB = ((is_array($sit['Ensino superior (sem FIES)']['total'])?array_sum($sit['Ensino superior (sem FIES)']['total']):0));
                $arrA[] = $vA;
                $arrB[] = $vB;
                $arrTotal[] = $vA + $vB;
            }

            $aDados = array(
                array('name' => 'Educacao basica', 'data'=>$arrA),
                array('name' => 'Ensino superior (sem FIES)', 'data'=>$arrB),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoGastosMECComIPCA", "Gastos do MEC (em R$ constantes)",300, $formatoValores);
            echo "<br><br>";
            echo"<table><tr><td class='bold'>*2015: Previsão</td></tr></table>";
        }
    }
}
function carregarDetalhesAbrangenciaEstado($dados) {
    global $db;
    $sql = "SELECT dpe.dpeanoref AS periodo, tid1.tidid AS idprograma, tid1.tiddsc AS programa, tid2.tiddsc AS atividade, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            WHERE sh.indid in (3104)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN ({$dados['esiid']})
            GROUP BY periodo, idprograma, programa, atividade
            ORDER BY periodo, idprograma, programa, atividade";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="3">PROGRAMA / PROJETO/ATIVIDADE</th>
        </tr>
        <tr>
            <th>2012</th>
            <th>2013</th>
            <th>2014</th>
            <th>2015</th>
        </tr>
        <tr>
            <th>EMPENHADO</th>
            <th>EMPENHADO</th>
            <th>EMPENHADO</th>
            <th>PLOA</th>
        </tr>
        <?php
        if($arrDados){
            $arrSituacao = array();
            foreach($arrDados as $dado){
                $arrSituacao[$dado['programa']][$dado['periodo']][$dado['atividade']]['total'] = $dado['valor'];
            }
            $count=-1;
            foreach($arrSituacao as $programa => $sit){
                $count++;
                ?>
                <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                    <td class="" ><?php echo $programa ?></td>
                    <td class="numero" ><?php echo number_format($sit['2012']['Empenhado']['total'],2,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2013']['Empenhado']['total'],2,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2014']['Empenhado']['total'],2,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2015']['PLOA']['total'],2,",",".") ?></td>
                </tr>
            <?php
            }
        }?>
    </table>
<?php
}
?>