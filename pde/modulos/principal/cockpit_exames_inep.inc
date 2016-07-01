<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['detalhe_enade'])){
    gerarGraficoEnade($_REQUEST['detalhe_enade']);
    die;
}
if(isset($_REQUEST['detalhe_matricula'])){
    gerarGraficoMatriculaBasica($_REQUEST['detalhe_matricula']);
    die;
}
if(isset($_REQUEST['detalhe_graduacao'])){
    gerarGraficoMatriculaGraduacao($_REQUEST['detalhe_graduacao']);
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
            jQuery('.filtrar_enade').click(function(){
                jQuery('#div_detalhe_enade').load('estrategico.php?modulo=principal/cockpit_exames_inep&acao=A&detalhe_enade='+jQuery(this).val());
            });
            jQuery('.filtrar_matricula').click(function(){
                jQuery('#div_detalhe_matricula').load('estrategico.php?modulo=principal/cockpit_exames_inep&acao=A&detalhe_matricula='+jQuery(this).val());
            });
            jQuery('.filtrar_graduacao').click(function(){
                jQuery('#div_detalhe_graduacao').load('estrategico.php?modulo=principal/cockpit_exames_inep&acao=A&detalhe_graduacao='+jQuery(this).val());
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
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_exames_inep&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Exames e Avaliações do INEP</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" colspan="3" align="middle">
			<div>
				<?php exibirTitulo('executiverel', 'Cronograma'); ?>
			</div>
			<img src="cockpit/images/cronograma_inep.png" />
		</td>
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('mapas', '', 'Escolha o que deseja visualizar no mapa'); ?>
			</div>
			<?
			//$sql = "";
			//$arrDados = $db->carregar( $sql, null, 3200 );
			?>
			<table border=0 cellspacing="4" cellpadding="5" align="center">
			<tr>
			<td>
				<br><br><br><br><br><br>
				<span class="subtitulo_box link" onclick="abreIndicadorPopUp(271);" >ENEM - Inscritos</span><br />
				<span class="subtitulo_box link" onclick="abreIndicadorPopUp(22);" >SAEB - Escolas</span><br />
				<span class="subtitulo_box link" onclick="abreIndicadorPopUp(2824);" >SINAES - IES</span><br />
			</td>
			<td class="" valign="bottom">
				<img src="../imagens/cockpit/mapa_cockpit.png"  />
			</td>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" align="middle" width="1%">
            <br><br><br><br><br><br>
			<img src="cockpit/images/ed_basica.png" />
		</td>
		<td class="fundo_padrao" width="33%">
			<div class="link" onclick="abreIndicadorPopUp(442);">
				<?php exibirTitulo('indicador', 'Matrículas na Educação Básica -<br>Censo Escolar (2003 - 2013)'); ?>
			</div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_matricula" name="matricula" value="1" checked="checked" onclick="" />Todas
                <input type="radio" class="filtrar_matricula" name="matricula" value="2" onclick="" />Municipal
                <input type="radio" class="filtrar_matricula" name="matricula" value="3" onclick="" />Estadual
                <input type="radio" class="filtrar_matricula" name="matricula" value="4" onclick="" />Federal
                <input type="radio" class="filtrar_matricula" name="matricula" value="5" onclick="" />Privada
            </div>
            <br>
            <div id="div_detalhe_matricula">
                <?php gerarGraficoMatriculaBasica(1); ?>
            </div>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(23);" width="33%">
			<div>
				<?php exibirTitulo('indicador', 'SAEB - Prova Brasil', 'Nº alunos inscritos 5° e 9° ano'); ?>
			</div>
			<?
			$sql = "SELECT
                        dpe.dpeanoref AS ano,
                        Replace(Replace(tid1.tiddsc, 'ª','a'), 'º','o') AS etapa,
                        sum(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (23)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ano, etapa
                    ORDER BY ano, etapa";
			$dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'etapa', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaProvaBrasil', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
			?>
		</td>
		<td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'ENEM', '<span class="link" onclick="abreIndicadorPopUp(555);">N° de Inscritos no ENEM - Total Brasil</span><br><span class="link" onclick="abreIndicadorPopUp(556);">N° de pessoas que realizaram a prova do ENEM - Total Brasil</span>'); ?>
            </div>
            <br>
            <?php
            $sql = "SELECT sh.indid, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (555,556)
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref >= '2003'
                    GROUP BY sh.indid, periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrEtapa = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['indid']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['555']['total'])?array_sum($sit['555']['total']):0));
                    $vB = Round((is_array($sit['556']['total'])?array_sum($sit['556']['total']):0));

                    $arrA[] = $vA;
                    $arrB[] = $vB;
                }

                $aDados = array(
                    array('name' => 'Inscritos no ENEM', 'data'=>$arrA),
                    array('name' => 'Realizaram a prova', 'data'=>$arrB),
                );


                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEnemEB", "",300, $formatoValores);
            }
            ?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" align="middle" rowspan="2">
            <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
            <img src="cockpit/images/ed_superior.png" />
		</td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(439);">
                <?php exibirTitulo('indicador', 'Matrículas de Graduação -<br>CenSup (2003 - 2013)'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_graduacao" name="graduacao" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_graduacao" name="graduacao" value="2" onclick="" />Presencial
                <input type="radio" class="filtrar_graduacao" name="graduacao" value="3" onclick="" />A distância
            </div>
            <br>
            <div id="div_detalhe_graduacao">
                <?php gerarGraficoMatriculaGraduacao(1); ?>
            </div>
        </td>
		<td class="fundo_padrao">
            <div>
                <?php exibirTitulo('indicador', 'SINAES - ENADE'); ?>
            </div>
            <div style="width:100%;text-align:left" >
                <input type="radio" class="filtrar_enade" name="enade" value="1" checked="checked" onclick="" /><span class="link" onclick="abreIndicadorPopUp(129);">N° de participantes ENADE</span><br>
                <input type="radio" class="filtrar_enade" name="enade" value="2" onclick="" /><span class="link" onclick="abreIndicadorPopUp(284);">N° total de cursos avaliados ENADE</span>
            </div>
            <br>
            <br>
            <div id="div_detalhe_enade">
                <?php gerarGraficoEnade(1); ?>
            </div>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(127);">
			<div>
				<?php exibirTitulo('indicador', 'SINAES – Cursos e Instituições', 'N° avaliações IES e cursos de graduação realizadas'); ?>
			</div>
			<?
			$sql = "SELECT
                        dpe.dpeanoref AS ano,
                        tid1.tiddsc AS etapa,
                        sum(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (127)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ano, etapa
                    ORDER BY ano, etapa";
			$dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'etapa', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaCursosInstituicoes', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
			?>
		</td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(445);">
            <div>
                <?php exibirTitulo('indicador', 'CPC', 'Nº de cursos por CPC'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS descricao,
                        dpe.dpeanoref AS rede,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (445)
                    --AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref >= '2009'
                    GROUP BY descricao, rede
                    ORDER BY descricao, rede";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'rede', 'descricao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Cursos: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaCPC', null, null, $formatoValores, null, null, 1000, 600, null, 300);
                //geraGrafico($arrDados, "graficoPizzaCPC", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2824);">
            <div>
                <?php exibirTitulo('indicador', 'IGC', 'Nº de IES por IGC'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid1.tiddsc AS descricao,
                        dpe.dpeanoref AS rede,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (2824)
                    --AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s WHERE s.indid = sh.indid AND s.sehstatus <> 'I')
                    AND sh.sehstatus <> 'I'
                    AND dpe.dpeanoref >= '2009'
                    GROUP BY descricao, rede
                    ORDER BY descricao, rede";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'rede', 'descricao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Cursos: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaIGC', null, null, $formatoValores, null, null, 1000, 600, null, 300);
                //geraGrafico($arrDados, "graficoPizzaIGC", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2910);">
            <div>
                <?php exibirTitulo('indicador', 'REVALIDA'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref as descricao, tiddsc as rede, sum(dsh.dshqtde::integer) as valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (2910)
                AND sh.sehstatus <> 'I'
                GROUP BY descricao, rede
                ORDER BY descricao, rede";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'rede', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Matrículas: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaRevalida', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
        </td>
    </tr>
	<tr>
		<td class="fundo_padrao" align="middle">
            <br><br><br><br>
			<img src="cockpit/images/ed_certificacao.png" />
		</td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(450);">
            <div>
                <?php exibirTitulo('indicador', 'ENCCEJA', 'Nº de Inscritos'); ?>
            </div>
            <?
            $sql = "SELECT
							dpe.dpeanoref AS ano,
							tid2.tiddsc AS etapa,
							sum(dsh.dshqtde) AS valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (450)
						AND sh.sehstatus <> 'I'
						GROUP BY ano, etapa
						ORDER BY ano, etapa";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'etapa', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaENCCEJA', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php //exibirTitulo('indicador', 'ENEM', '<span class="link" onclick="abreIndicadorPopUp(1354);">N° de inscritos no ENEM que solicitaram certificação</span><br><span class="link" onclick="abreIndicadorPopUp(1355);">N° de inscritos no ENEM que solicitaram certificação<br>em unidades prisionais</span>'); ?>
                <?php exibirTitulo('indicador', 'ENEM', '<span class="link" onclick="abreIndicadorPopUp(1354);">N° de inscritos no ENEM que solicitaram certificação</span>'); ?>
            </div>
            <br>
            <?php
            $sql = "SELECT sh.indid, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (1354,1355)
                    AND sh.sehstatus <> 'I'
                    GROUP BY sh.indid, periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            $arrEtapa = array();
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['indid']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['1354']['total'])?array_sum($sit['1354']['total']):0));
                    $vB = Round((is_array($sit['1355']['total'])?array_sum($sit['1355']['total']):0));

                    $arrA[] = $vA;
                    $arrB[] = $vB;
                }

                $aDados = array(
                    array('name' => 'Inscritos no ENEM', 'data'=>$arrA),
                   // array('name' => 'Solicitaram certificacao', 'data'=>$arrB),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEnemCer", "",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2825);">
            <div>
                <?php exibirTitulo('indicador', 'CELPE-BRAS'); ?>
            </div>
            <?php
            $sql = "SELECT dpe.dpeanoref as descricao, tiddsc as rede, sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2825)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao, rede
                    ORDER BY descricao, rede";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'rede', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Matrículas: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaCELPE', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
        </td>
	</tr>
	<tr>
		<td class="fundo_padrao" rowspan="2" colspan="3">
			<div>
				<?php exibirTitulo('financeiro', 'Orçamentário/Financeiro'); ?>
			</div>
			<?
			//$sql = "";
			//$arrDados = $db->carregar( $sql, null, 3200 );
			?>
		</td>
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('indicador', 'Indicadores'); ?>
			</div>
			<?
			//$sql = "";
			//$arrDados = $db->carregar( $sql, null, 3200 );
			?>
		</td>
	</tr>
	<tr>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,28,'','');" align="center" colspan="2">
			<?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
			<img src="cockpit/images/alinhamentoEstrategico.png" />
		</td>
		<!-- FIM Tabela Alinhamento Estratégico -->
	</tr>
	<tr>
		<td class="fundo_td_laranja" colspan="4">
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
function gerarGraficoEnade($detalhe){

    global $db;

    if($detalhe==1){
        $where = "WHERE sh.indid in (129)";
    }elseif($detalhe==2){
        $where = "WHERE sh.indid in (284)";
    }

    $sql = "SELECT dpe.dpeanoref AS descricao, SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            $where
            AND sh.sehstatus <> 'I'
            GROUP BY descricao
            ORDER BY descricao";
    $dados = $db->carregar( $sql, null, 3200 );

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
    );
    geraGraficoColuna($dados, 'graficoColunaEnade', null, null, $formatoValores, null, "", 1000, 600, null, 300);
}

function gerarGraficoMatriculaBasica($detalhe){

    global $db;

    if($detalhe==2){
        $where = "AND dsh.tidid2 in (1683)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid2 in (1684)";
    }elseif($detalhe==4){
        $where = "AND dsh.tidid2 in (1685)";
    }elseif($detalhe==5){
        $where = "AND dsh.tidid2 in (1686)";
    }

    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                CASE
                    WHEN dsh.tidid1 IN (1680) THEN 'EJA'
                    WHEN dsh.tidid1 IN (1681) THEN 'Especial'
                    WHEN dsh.tidid1 IN (651) THEN 'Infantil'
                    WHEN dsh.tidid1 IN (1682) THEN 'Profissional de Nível Técnico'
                    WHEN dsh.tidid1 IN (652) THEN 'Fundamental'
                    WHEN dsh.tidid1 IN (1687, 1688, 1689, 654) THEN 'Médio'
                END AS descricao,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            WHERE sh.indid IN (442)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref BETWEEN '2003' AND '2013'
            $where
            GROUP BY periodo, descricao
            ORDER BY periodo, descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $dadosAgrupados = agruparDadosGrafico($arrDados, 'periodo', 'descricao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }",
            'ocultartotal' => "SIM"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaBasica', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }
}

function gerarGraficoMatriculaGraduacao($detalhe){

    global $db;

    if($detalhe==2){
        $where = "AND dsh.tidid1 in (640)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 in (641)";
    }

    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                tid2.tiddsc AS descricao,
                SUM(dsh.dshqtde) AS valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = tidid2
            WHERE sh.indid IN (439)
            AND sh.sehstatus <> 'I'
            AND dpe.dpeanoref BETWEEN '2003' AND '2013'
            AND dsh.tidid1 NOT IN (1848, 1849)
            $where
            GROUP BY periodo, descricao
            ORDER BY periodo, descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        $dadosAgrupados = agruparDadosGrafico($arrDados, 'periodo', 'descricao', 'valor');

        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }",
            'ocultartotal' => "SIM"
        );
        geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaGraduacao', null, null, $formatoValores, null, "", 1000, 600, null, 300);
    }
}
?>