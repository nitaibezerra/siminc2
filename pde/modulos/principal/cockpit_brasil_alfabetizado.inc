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
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_maiseducacao.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_brasil_alfabetizado&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Programa Brasil Alfabetizado</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2579);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Turmas Finalizadas', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Turmas' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2579)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Turmas Acumuladas',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTurmaAcumulada', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2579);" width="34%">
            <?php
            $sql = "SELECT
                        TO_CHAR(MIN(dpe.dpedatainicio), 'MM/YYYY') AS datainicio,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2579)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 = 7028 --Ciclo 2012
                    ";
            $arrDados = $db->pegaLinha( $sql );
            ?>
            <div>
                <div style="float: left; width: 100%;"> <?php exibirTitulo('executive', 'Ciclo 2012 - Em finalização:<br>'.number_format($arrDados['valor'],0,",",".").' turmas', 'Início do ciclo: '.$arrDados['datainicio']); ?></div>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2579);" width="33%">
            <?php
            $sql = "SELECT
                        TO_CHAR(MIN(dpe.dpedatainicio), 'MM/YYYY') AS datainicio,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2579)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 = 7518 --Ciclo 2013
                    ";
            $arrDados = $db->pegaLinha( $sql );
            ?>
            <div>
                <div style="float: left; width: 100%;"> <?php exibirTitulo('executive', 'Ciclo 2013 - Em execução:<br>'.number_format($arrDados['valor'],0,",",".").' turmas', 'Início do ciclo: '.$arrDados['datainicio']); ?></div>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2581);" colspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas Iniciais de Alfabetizandos', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Alfabetizandos' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2581)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Alfabetizandos Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaAfabetizandoAcumulado', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <!--td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas Finais de Alfabetizandos', 'Por ciclo'); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);">
            <div>
                <?php exibirTitulo('indicador', 'Estudantes Alfabetizados', 'Por ciclo'); ?>
            </div>
        </td-->
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2580);">
            <div>
                <?php exibirTitulo('indicador', 'Alfabetizadores bolsistas', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Bolsas' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2580)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Bolsas Acumuladas',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaBolsaAcumulada', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2584);">
            <div>
                <?php exibirTitulo('indicador', 'Coordenadores bolsistas', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Alfabetizador' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2584)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Alfabetizadores Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaAlfabetizadorAcumulado', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2585);">
            <div>
                <?php exibirTitulo('indicador', 'Tradutores/interpretes de LIBRAS<br>bolsistas ', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Alfabetizador' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2585)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Alfabetizadores Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTradutorAcumulado', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2580);">
            <div>
                <?php exibirTitulo('financeiro', 'Alfabetizadores bolsistas -<br>Investimentos', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Bolsas' AS tipo,
                        SUM(dsh.dshvalor) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2580)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Bolsas Acumuladas',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaBolsaAcumulada2', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2584);">
            <div>
                <?php exibirTitulo('financeiro', 'Coordenadores bolsistas -<br>Investimentos', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Alfabetizador' AS tipo,
                        SUM(dsh.dshvalor) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2584)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Alfabetizadores Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaAlfabetizadorAcumulado2', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2585);">
            <div>
                <?php exibirTitulo('financeiro', 'Tradutores/interpretes de LIBRAS<br>bolsistas - Investimentos', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS ciclo,
                        'Alfabetizador' AS tipo,
                        SUM(dsh.dshvalor) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2585)
                    AND sh.sehstatus <> 'I'
                    GROUP BY ciclo, tipo
                    ORDER BY ciclo, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'ciclo'=>$dado['ciclo'],
                        'tipo'=>'Alfabetizadores Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'ciclo', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTradutorAcumulado2', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2583);" rowspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Valor de apoio enviado para<br>pagamento aos estados', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS descricao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2583)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }"
            );

            geraGraficoColuna($dados, 'graficoColunaPagamentoEstados', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config, '#fff', true);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2582);" rowspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Valor de apoio enviado para<br>pagamento direto aos municípios ', 'Por ciclo'); ?>
            </div>
            <?php
            $sql = "SELECT
                        tid2.tiddsc AS descricao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    WHERE sh.indid IN (2582)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }"
            );

            geraGraficoColuna($dados, 'graficoColunaPagamentoMunicipios', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config, '#fff', true);
            ?>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,1,'','');" align="center" >
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
            <?=exibirTabelaFinanceiro(1);?>
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