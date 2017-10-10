<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_transporte1'])){
    gerarGraficoTransporte1($_REQUEST['detalhe_transporte1']);
    die;
}
if(isset($_REQUEST['detalhe_transporte2'])){
    gerarGraficoTransporte2($_REQUEST['detalhe_transporte2']);
    die;
}
if(isset($_REQUEST['detalhe_transporte3'])){
    gerarGraficoTransporte3($_REQUEST['detalhe_transporte3']);
    die;
}
if(isset($_REQUEST['detalhe_PDDE'])){
    gerarGraficoPDDE($_REQUEST['detalhe_PDDE']);
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
            jQuery('.div_fotos_interno').cycle({
                fx: 'scrollDown'
            });
            jQuery('.filtrar_transporte1').click(function(){
                jQuery('#div_transporte1').load('estrategico.php?modulo=principal/cockpit_educacao_especial&acao=A&detalhe_transporte1='+jQuery(this).val());
            });
            jQuery('.filtrar_transporte2').click(function(){
                jQuery('#div_transporte2').load('estrategico.php?modulo=principal/cockpit_educacao_especial&acao=A&detalhe_transporte2='+jQuery(this).val());
            });
            jQuery('.filtrar_transporte3').click(function(){
                jQuery('#div_transporte3').load('estrategico.php?modulo=principal/cockpit_educacao_especial&acao=A&detalhe_transporte3='+jQuery(this).val());
            });
            jQuery('.filtrar_PDDE').click(function(){
                jQuery('#div_PDDE').load('estrategico.php?modulo=principal/cockpit_educacao_especial&acao=A&detalhe_PDDE='+jQuery(this).val());
            });
        });

        function abreObras(obrid){
            window.open('/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid);
        }
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_viver_sem_limite.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
        .div_fotos_padrao2{background-color:#152D56;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:400px;margin-bottom:2px}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_educacao_especial&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Educação Especial</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" width="50%" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Evolução da Política de Inclusão','<span class="link" onclick="abreIndicadorPopUp(702);">Matrículas em escolas regulares / classes comuns</span>,<br><span class="link" onclick="abreIndicadorPopUp(3090);">Matrículas em escolas especializadas e classes especiais</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        sh.indid AS acao,
                        dpe.dpeanoref AS ano,
                        sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (702,3090)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, ano
                    ORDER BY ano, acao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['ano']][$dado['acao']]['total'][] = $dado['valor'];
                }
            }
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrTotal = array();

            foreach($arrSituacao as $periodo => $sit)
            {
                $arrX[] = $periodo;
                $vA = Round((is_array($sit['702']['total'])?array_sum($sit['702']['total']):0));
                $vB = Round((is_array($sit['3090']['total'])?array_sum($sit['3090']['total']):0));
                $arrA[] = $vA;
                $arrB[] = $vB;
                $arrTotal[] = $vA + $vB;
            }

            $aDados = array(
                array('name' => 'Matriculas em escolas regulares / classes comuns', 'data'=>$arrA),
                array('name' => 'Matriculas em escolas especializadas e classes especiais', 'data'=>$arrB),
                array('name' => 'Total de Matriculas', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoLinhaMatriculasEspeciais", "Matrículas na Educação Especial",300, $formatoValores);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(268);" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Salas de Recursos Multifuncionais','Salas de recursos multifuncionais e kits de atualização adquiridos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref as ano,
                        CASE
                            WHEN dsh.tidid1 IN (1823, 1824) THEN 'Tipos I e II'
                        ELSE tid1.tiddsc
                        END AS tipo,
                        sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (268)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ano, tipo
					ORDER BY ano, tipo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSalas=array();
                foreach($arrDados as $dado){
                    $arrSalas[$dado['tipo']] += $dado['valor'];
                }
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'ano', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaSalas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr class="">
                        <td class="">Total de salas de recursos multifuncionais</td>
                        <td class="numero"><?=number_format($arrSalas['Tipos I e II'],0,",",".")?></td>
                    </tr>
                    <tr class="zebrado">
                        <td class="">Total de kits de atualização</td>
                        <td class="numero"><?=number_format($arrSalas['Kits de atualização'],0,",",".")?></td>
                    </tr>
                    <tr class="">
                        <td class="">Total de salas e kit</td>
                        <td class="numero"><?=number_format($arrSalas['Tipos I e II']+$arrSalas['Kits de atualização'],0,",",".")?></td>
                    </tr>
                </table>
                <?php
            }
            ?>
        </td>
	</tr>
    <tr>
        <td class="fundo_padrao" width="33%">
            <div class="link" onclick="abreIndicadorPopUp(1865);">
                <?php exibirTitulo('indicador', 'Transporte Escolar Acessível','Número de ônibus adquiridos por ano e forma de aquisição<br>(Valores Acumulados)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_transporte1" name="transporte1" value="1" checked="checked" onclick="" />Total
                <input type="radio" class="filtrar_transporte1" name="transporte1" value="2" onclick="" />Recursos FNDE
                <input type="radio" class="filtrar_transporte1" name="transporte1" value="3" onclick="" />Financiamento BNDES
                <input type="radio" class="filtrar_transporte1" name="transporte1" value="4" onclick="" />Recursos próprios
            </div>
            <div id="div_transporte1">
                <?php gerarGraficoTransporte1(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="34%">
            <div class="link" onclick="abreIndicadorPopUp(1865);">
                <?php exibirTitulo('indicador', 'Transporte Escolar Acessível', 'Recursos por ano e forma de aquisição<br>(Valores Acumulados)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_transporte2" name="transporte2" value="1" checked="checked" onclick="" />Total
                <input type="radio" class="filtrar_transporte2" name="transporte2" value="2" onclick="" />Recursos FNDE
                <input type="radio" class="filtrar_transporte2" name="transporte2" value="3" onclick="" />Financiamento BNDES
                <input type="radio" class="filtrar_transporte2" name="transporte2" value="4" onclick="" />Recursos próprios
            </div>
            <div id="div_transporte2">
                <?php gerarGraficoTransporte2(1); ?>
            </div>
        </td>
        <td class="fundo_padrao" width="33%">
            <div class="link" onclick="abreIndicadorPopUp(2962);">
                <?php exibirTitulo('indicador', 'Transporte Escolar Acessível','Viver sem Limite<br>(Valores Acumulados)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_transporte3" name="transporte3" value="1" checked="checked" onclick="" />Ônibus adquiridos
                <input type="radio" class="filtrar_transporte3" name="transporte3" value="2" onclick="" />Recursos repassados
            </div>
            <div id="div_transporte3">
                <?php gerarGraficoTransporte3(1); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(649);">
            <div>
                <?php exibirTitulo('indicador', 'Escola Acessível', 'Escolas contempladas pelo PDDE / Escola Acessível'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS acao,
                        dpe.dpeanoref AS ano,
                        COUNT(DISTINCT dsh.dshcod) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (649)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, ano
                    ORDER BY ano, acao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){

                $dadosAgrupados = agruparDadosGrafico($arrDados, 'ano', 'acao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaEscolaAcessivelContempladas', null, null, $formatoValores, null, "", 1000, 600, null, 300);

                $sql = "SELECT
                            tid1.tiddsc AS acao,
                            COUNT(DISTINCT dsh.dshcod) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid IN (649)
                        AND sh.sehstatus <> 'I'
                        GROUP BY acao";
                $arrDados = $db->carregar( $sql, null, 3200 );
                $arrEscolas=array();
                foreach($arrDados as $dado){
                    $arrEscolas[$dado['acao']] += $dado['valor'];
                }
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr class="">
                        <td class="">Total de escolas estaduais</td>
                        <td class="numero"><?=number_format($arrEscolas['Estadual'],0,",",".")?></td>
                    </tr>
                    <tr class="zebrado">
                        <td class="">Total de escolas municipais</td>
                        <td class="numero"><?=number_format($arrEscolas['Municipal'],0,",",".")?></td>
                    </tr>
                    <tr class="">
                        <td class="">Total de escolas</td>
                        <td class="numero"><?=number_format($arrEscolas['Estadual']+$arrEscolas['Municipal'],0,",",".")?></td>
                    </tr>
                </table>
            <?php
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(649);">
            <div>
                <?php exibirTitulo('indicador', 'Escola Acessível', 'Recursos repassados pelo PDDE / Escola Acessível<br>(Valores acumulados)'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dsh.tidid1 AS acao,
                        dpe.dpeanoref AS ano,
                        sum(dsh.dshqtde) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (649)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, ano
                    ORDER BY ano, acao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['ano']][$dado['acao']]['total'][] = $dado['valor'];
                }
            }
            $arrX = array();
            $arrA = array();
            $arrB = array();
            $arrC = array();
            $arrTotal = array();
            $vA=0;
            $vB=0;
            $vC=0;

            foreach($arrSituacao as $periodo => $sit)
            {
                $arrX[] = $periodo;
                $vA+= Round((is_array($sit['3128']['total'])?array_sum($sit['3128']['total']):0));
                $vB+= Round((is_array($sit['3130']['total'])?array_sum($sit['3130']['total']):0));
                $vC+= Round((is_array($sit['3129']['total'])?array_sum($sit['3129']['total']):0));
                $arrA[] = $vA;
                $arrB[] = $vB;
                $arrC[] = $vC;
                $arrTotal[] = $vA + $vB + $vC;
            }

            $aDados = array(
                array('name' => 'Estadual', 'data'=>$arrB),
                array('name' => 'Municipal', 'data'=>$arrA),
                //array('name' => 'Federal', 'data'=>$arrC),
                array('name' => 'Total', 'data'=>$arrTotal),
            );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                'alinhamento' => "formatter: function() { return '1'; }"
            );

            echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEscolaAcessivel", "",300, $formatoValores);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2957);">
            <div>
                <?php exibirTitulo('indicador', 'Pronatec','Viver sem Limite<br>Matrículas'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref AS descricao, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (2957)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColuna($dados, 'graficoColunaPronatecViver', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config, '#fff', true);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(268);">
            <div>
                <?php exibirTitulo('alvo', 'Viver Sem Limite -<br>Sala de Recursos Multifuncionais', 'Salas e kits adquiridos 2011-2014'); ?>
            </div>
            <?php
            $sql = "SELECT
                        CASE
                            WHEN dsh.tidid1 IN (1823, 1824) THEN 'salas'
                            WHEN dsh.tidid1 IN (3183) THEN 'kits'
                        END AS tipo,
                        sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (268)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref BETWEEN '2011' AND '2014'
                    GROUP BY tipo
                    ORDER BY tipo";
            $arrDados = $db->carregar($sql);
            if($arrDados){
                foreach($arrDados as $dado){
                    if($dado['tipo']=='salas'){
                        $totalSalas += $dado['valor'];
                    }else{
                        $totalKits += $dado['valor'];
                    }
                }
            }
            $valorMetaSalas = 15000;
            $porcentagemSalas = number_format(($totalSalas/$valorMetaSalas)*100,0,",",".");
            $horizontalSalas = $porcentagemSalas * 2;
            $valorMetaKits = 30000;
            $porcentagemKits = number_format(($totalKits/$valorMetaKits)*100,0,",",".");
            $horizontalKits = $porcentagemKits * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        Meta: <?php echo number_format($valorMetaSalas,0,",",".") ?> Salas
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalSalas ?>px;'><?= $porcentagemSalas ?>%&nbsp;</div>
                        </div>
                        Total: <?php echo number_format($totalSalas,0,",",".") ?> Salas
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        Meta: <?php echo number_format($valorMetaKits,0,",",".") ?> Kits
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalKits ?>px;'><?= $porcentagemKits ?>%&nbsp;</div>
                        </div>
                        Total: <?php echo number_format($totalKits,0,",",".") ?> Kits
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2962);">
            <div>
                <?php exibirTitulo('alvo', 'Viver Sem Limite -<br>Transporte Escolar Acessível', 'Ônibus adquiridos 2012-2014'); ?>
            </div>
            <?php
            $sql = "SELECT
                        sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2962)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref BETWEEN '2012' AND '2014'";
            $totalOnibus = $db->pegaUm($sql);
            $valorMetaOnibus = 2609;
            $porcentagemOnibus = number_format(($totalOnibus/$valorMetaOnibus)*100,0,",",".");
            $horizontalOnibus = $porcentagemOnibus * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        Meta: <?php echo number_format($valorMetaOnibus,0,",",".") ?> Ônibus
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalOnibus ?>px;'><?= $porcentagemOnibus ?>%&nbsp;</div>
                        </div>
                        Total: <?php echo number_format($totalOnibus,0,",",".") ?> Ônibus
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" rowspan="2">
            <div>
                <?php exibirTitulo('casas', 'Viver Sem Limite -<br>Centros de Treinamento de<br>Cães-Guia nos Institutos Federais'); ?>
            </div>
            <br>
            <?php
            $sql = "SELECT
                        obr.obrid, obr.obrdesc,
                        ent.entsig,
                        ent.entnome,
                        mun.mundescricao,
                        mun.estuf,
                        obr.obrpercexec,
                        arqid
                    FROM obras.obrainfraestrutura obr
                    INNER JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
                    INNER JOIN obras.tipoobra tob ON tob.tobaid = obr.tobraid
                    INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
                    INNER JOIN entidade.endereco edc ON edc.endid = obr.endid
                    INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
                    LEFT JOIN
                        (select MAX(arqid) AS arqid, obrid FROM obras.fotos GROUP BY obrid) AS fot ON fot.obrid = obr.obrid
                    WHERE obr.obsstatus = 'A'
                    AND obr.obrid IN (14965, 32590, 28559, 29723, 36756, 28243, 33584)
                    ORDER BY random()";
            $arrFotos = $db->carregar($sql);
            ?>
            <div style="clear:both" id="div_fotos_padrao2"  >
                <?php for($x=5;$x>=1;$x--): ?>
                    <div style="clear:both" class="div_fotos_interno"  >
                        <?php for($i=$x;$i<=(1+$x);$i++): ?>
                            <div class="div_fotos_padrao2" >
                                <table border="0" >
                                    <tr class="link" onclick="abreObras('<?=$arrFotos[$i]['obrid'] ?>');">
                                        <td>
                                            <img width="80" height="60" src="http://simec.mec.gov.br/slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
                                        </td>
                                        <td style="color:#FFFFFF">
                                            <b><?php echo $arrFotos[$i]['entsig'] . ' - ' . $arrFotos[$i]['entnome'] ?></b><br/>
                                            <b>Nome:</b> <?php echo $arrFotos[$i]['obrdesc'] ?><br/>
                                            <b>Município:</b> <?php echo $arrFotos[$i]['mundescricao'] ?>/<?php echo $arrFotos[$i]['estuf'] ?><br/>
                                            <b>Executado:</b> <?php echo $arrFotos[$i]['obrpercexec'] ?>%<br/>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                        <?php endfor; ?>
                    </div>
                <?php endfor; ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(649);">
            <div>
                <?php exibirTitulo('alvo', 'Viver Sem Limite -<br>Escola Acessível', 'Escolas contempladas 2011-2014'); ?>
            </div>
            <?php
            $sql = "SELECT
                        count(DISTINCT dsh.dshcod) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (649)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref BETWEEN '2011' AND '2014'";
            $totalEscolas = $db->pegaUm($sql);
            $valorMetaEscolas = 42000;
            $porcentagemEscolas = number_format(($totalEscolas/$valorMetaEscolas)*100,0,",",".");
            $horizontalEscolas = $porcentagemEscolas * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        Meta: <?php echo number_format($valorMetaEscolas,0,",",".") ?> Escolas
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalEscolas ?>px;'><?= $porcentagemEscolas ?>%&nbsp;</div>
                        </div>
                        Total: <?php echo number_format($totalEscolas,0,",",".") ?> Escolas
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(781);">
            <div>
                <?php exibirTitulo('alvo', 'Viver Sem Limite - Incluir', 'Universidades contempladas'); ?>
            </div>
            <?php
            $sql = "SELECT
                        count(DISTINCT dsh.dshcod) as universidades,
                        sum(dsh.dshqtde) as valor,
                        dpe.dpeanoref AS ano
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (781)
                    AND sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
                    GROUP BY ano";
            $arrDados = $db->pegaLinha($sql);
            $valorMetaUniversidades = 59;
            $porcentagemUniversidades = number_format(($arrDados['universidades']/$valorMetaUniversidades)*100,0,",",".");
            $horizontalUniversidades = $porcentagemUniversidades * 2;
            ?>
            <table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
                <tr>
                    <td class="titulo_box bold" align="center"><br>
                        Meta: <?php echo number_format($valorMetaUniversidades,0,",",".") ?> Universidades
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
                            <div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontalUniversidades ?>px;'><?= $porcentagemUniversidades ?>%&nbsp;</div>
                        </div>
                        Total: <?php echo number_format($arrDados['universidades'],0,",",".") ?> Universidades
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold" align="center">
                        <br>
                        <br>
                        Recursos disponibilizados <?=$arrDados['ano']?>: R$<?php echo number_format($arrDados['valor'],2,",",".") ?>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3102);">
            <div>
                <?php exibirTitulo('indicador', 'FUNDEB -<br>escolas privadas sem fins lucrativos', 'Matrículas'); ?>
            </div>
            <?php
            $sql = "SELECT
                tid1.tiddsc AS acao,
                dpe.dpeanoref AS ano,
                SUM(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (3102)
            AND sh.sehstatus <> 'I'
            GROUP BY acao, ano
            ORDER BY ano, acao";
            $arrDados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($arrDados, 'ano', 'acao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaFUNDEB', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'PDDE -<br>escolas privadas sem fins lucrativos', 'Número de <span class="link" onclick="abreIndicadorPopUp(3097);">escolas</span> e <span class="link" onclick="abreIndicadorPopUp(3100);">alunos</span> 2011-2014'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_PDDE" name="PDDE" value="1" checked="checked" onclick="" />Escolas
                <input type="radio" class="filtrar_PDDE" name="PDDE" value="2" onclick="" />Alunos
            </div>
            <div id="div_PDDE">
                <?php gerarGraficoPDDE(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3101);">
            <div>
                <?php exibirTitulo('indicador', 'PNAE -<br>escolas privadas sem fins lucrativos', 'Número de alunos'); ?>
            </div>
            <?php
            $sql = "SELECT
                    dpe.dpeanoref AS descricao,
                    SUM(dsh.dshqtde) as valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (3101)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid1 IN (8204) --Privada
                GROUP BY descricao
                ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($arrDados, 'graficoColunaPNAE', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2" rowspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Valores repassados para educação especial em escolas privadas sem fins lucrativos'); ?>
            </div>
            <?php
            $sql = "SELECT tipo, ano, valor
                    FROM (
                        SELECT
                            'FUNDEB' AS tipo,
                            dpe.dpeanoref AS ano,
                            SUM(dsh.dshvalor) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (3102)
                        AND sh.sehstatus <> 'I'
                        GROUP BY ano, tipo
                    UNION ALL
                        SELECT
                            'PDDE' AS tipo,
                            dpe.dpeanoref AS ano,
                            SUM(dsh.dshvalor) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (3097)
                        AND sh.sehstatus <> 'I'
                        GROUP BY ano, tipo
                    UNION ALL
                        SELECT
                            'PNAE' AS tipo,
                            dpe.dpeanoref AS ano,
                            SUM(dsh.dshvalor) as valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (3101)
                        AND sh.sehstatus <> 'I'
                        AND dsh.tidid1 IN (8204) --Privada
                        GROUP BY ano, tipo
                    ) AS FOO
                    WHERE ano >= '2011'
                    ORDER BY ano, tipo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['ano']][$dado['tipo']]['valor'] = $dado['valor'];
                }
            }
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Ano</th>
                    <th>FUNDEB*</th>
                    <th>PDDE**</th>
                    <th>PNAE***</th>
                    <th>Total</th>
                </tr>
                <?php
                $count = -1;
                $totalFUNDEB=0;
                $totalPDDE=0;
                $totalPNAE=0;
                $totalGeral=0;
                foreach($arrSituacao as $chave => $sit):
                    $count++;
                    ?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td class=""><?=$chave?></td>
                        <td class="numero"><?=number_format($sit['FUNDEB']['valor'],2,",",".")?></td>
                        <td class="numero"><?=number_format($sit['PDDE']['valor'],2,",",".")?></td>
                        <td class="numero"><?=number_format($sit['PNAE']['valor'],2,",",".")?></td>
                        <td class="numero"><?=number_format($sit['FUNDEB']['valor']+$sit['PDDE']['valor']+$sit['PNAE']['valor'],2,",",".")?></td>
                    </tr>
                <?php
                $totalFUNDEB += $sit['FUNDEB']['valor'];
                $totalPDDE += $sit['PDDE']['valor'];
                $totalPNAE += $sit['PNAE']['valor'];
                $totalGeral += $sit['FUNDEB']['valor']+$sit['PDDE']['valor']+$sit['PNAE']['valor'];
                endforeach;
                ?>
                <tr class="">
                    <th>Total</th>
                    <th><?=number_format($totalFUNDEB,2,",",".")?></th>
                    <th><?=number_format($totalPDDE,2,",",".")?></th>
                    <th><?=number_format($totalPNAE,2,",",".")?></th>
                    <th><?=number_format($totalGeral,2,",",".")?></th>
                </tr>
            </table>
            <br>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                <tr>
                    <td>* Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação.</td>
                </tr>
                <tr>
                    <td>** Programa Dinheiro Direto na Escola. Valor pago.</td>
                </tr>
                <tr>
                    <td>*** Programa Nacional de Alimentação Escolar. Valor pago.</td>
                </tr>
                <tr>
                    <td>O recurso diferenciado da alimentação escolar para a educação especial teve inicio em janeiro de 2013.</td>
                </tr>
            </table>
        </td>
    </tr>
	<tr>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,30,'','');" align="center" >
			<?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
			<img src="cockpit/images/alinhamentoEstrategico.png" />
		</td>
		<!-- FIM Tabela Alinhamento Estratégico -->
	</tr>
    <tr>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro', 'Escola Acessível'); ?>
            </div>
            <?=exibirTabelaFinanceiro(30);?>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro', 'Salas de recursos multifuncionais'); ?>
            </div>
            <?=exibirTabelaFinanceiro(54);?>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro', 'Incluir'); ?>
            </div>
            <?=exibirTabelaFinanceiro(36);?>
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
function gerarGraficoTransporte1($detalhe){

    global $db;

    if($detalhe==1){
        $sql = "SELECT
                tid2.tiddsc AS acao,
                dpe.dpeanoref AS ano,
                SUM(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (1865)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (4300) --Urbano Acessível
            GROUP BY acao, ano
            ORDER BY ano, acao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        foreach($arrDados as $dado){
            $valorAcumulado += $dado['valor'];
            if($testaAno!=$dado['ano']){
                $arrDados[] = array(
                    'acao'=>$dado['acao'],
                    'ano'=>$dado['ano'],
                    'valor'=>$valorAcumulado,
                );
                $testaAno=$dado['ano'];
            }
        }

        $dadosAgrupados = agruparDadosGrafico($arrDados, 'ano', 'acao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTransporte1', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }else{
        $where = "";
        $valorAcumulado = 0;
        $testaAno="";

        if($detalhe==2) $where = "AND dsh.tidid2 IN (4301)";
        if($detalhe==3) $where = "AND dsh.tidid2 IN (4302)";
        if($detalhe==4) $where = "AND dsh.tidid2 IN (4303)";

        $sql = "SELECT
                dpe.dpeanoref AS descricao,
                SUM(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (1865)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (4300) --Urbano Acessível
            $where
            GROUP BY descricao
            ORDER BY descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        foreach($arrDados as $dado){
            $valorAcumulado += $dado['valor'];
            if($testaAno!=$dado['descricao']){
                $arrDados2[] = array(
                    'descricao'=>$dado['descricao'],
                    'valor'=>$valorAcumulado,
                );
                $testaAno=$dado['descricao'];
            }
        }

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );

        geraGraficoColuna($arrDados2, 'graficoColunaTransporte1', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }
}

function gerarGraficoTransporte2($detalhe){

    global $db;
    $where = "";

    if($detalhe==1){
        $sql = "SELECT
                tid2.tiddsc AS acao,
                dpe.dpeanoref AS ano,
                SUM(dsh.dshvalor) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (1865)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (4300) --Urbano Acessível
            GROUP BY acao, ano
            ORDER BY ano, acao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        foreach($arrDados as $dado){
            $valorAcumulado += $dado['valor'];
            if($testaAno!=$dado['ano']){
                $arrDados[] = array(
                    'acao'=>$dado['acao'],
                    'ano'=>$dado['ano'],
                    'valor'=>$valorAcumulado,
                );
                $testaAno=$dado['ano'];
            }
        }

        $dadosAgrupados = agruparDadosGrafico($arrDados, 'ano', 'acao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>R$ '+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTransporte2', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }else{
        if($detalhe==2) $where = "AND dsh.tidid2 IN (4301)";
        if($detalhe==3) $where = "AND dsh.tidid2 IN (4302)";
        if($detalhe==4) $where = "AND dsh.tidid2 IN (4303)";

        $sql = "SELECT
                dpe.dpeanoref AS descricao,
                SUM(dsh.dshvalor) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (1865)
            AND sh.sehstatus <> 'I'
            AND dsh.tidid1 IN (4300) --Urbano Acessível
            $where
            GROUP BY descricao
            ORDER BY descricao";
        $arrDados = $db->carregar( $sql, null, 3200 );
        foreach($arrDados as $dado){
            $valorAcumulado += $dado['valor'];
            if($testaAno!=$dado['descricao']){
                $arrDados2[] = array(
                    'descricao'=>$dado['descricao'],
                    'valor'=>$valorAcumulado,
                );
                $testaAno=$dado['descricao'];
            }
        }

        $formatoValores = array(
            'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
        geraGraficoColuna($arrDados2, 'graficoColunaTransporte2', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }
}

function gerarGraficoTransporte3($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshqtde) AS valor";
    }else{
        $campo = "SUM(dsh.dshvalor) AS valor";
    }

    $sql = "SELECT dpe.dpeanoref AS descricao, $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (2962)
            AND sh.sehstatus <> 'I'
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $valorAcumulado = 0;
        $testaAno = "";
        foreach($arrDados as $dado){
            $valorAcumulado += $dado['valor'];
            if($testaAno!=$dado['descricao']){
                $arrDados2[] = array(
                    'descricao'=>$dado['descricao'],
                    'valor'=>$valorAcumulado,
                );
                $testaAno=$dado['descricao'];
            }
        }
        if($detalhe==1){
            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($arrDados2, 'graficoColunaViver', null, null, $formatoValores, null, "", 1000, 600, null, 300);
        }else{
            $formatoValores = array(
                'y' => ",formatter: function () { return 'R$' + number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($arrDados2, 'graficoColunaViver', null, null, $formatoValores, null, "", 1000, 600, null, 300);
        }
    }
}

function gerarGraficoPDDE($detalhe){

    global $db;

    if($detalhe==1) $where = "AND sh.indid IN (3097)";
    if($detalhe==2) $where = "AND sh.indid IN (3100)";

    $sql = "SELECT
                dpe.dpeanoref AS descricao,
                SUM(dsh.dshqtde) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.sehstatus <> 'I'
            $where
            AND dpe.dpeanoref BETWEEN '2011' AND '2014'
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $formatoValores = array(
        'y' => ",formatter: function () { return + number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
    );
    geraGraficoColuna($arrDados, 'graficoColunaPDDE', null, null, $formatoValores, null, "", 1000, 600, null, 300);
}
?>