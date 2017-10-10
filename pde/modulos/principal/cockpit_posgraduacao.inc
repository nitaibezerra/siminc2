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
		
		function abreMetaPronatec(){
	        window.open('/pde/estrategico.php?modulo=principal/popupMetasPronatec&acao=A');
	    }
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_posgraduacao&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >Pós-Graduação (<i>Stricto Sensu</i>)</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2961);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Cursos de pós-graduação<br>por modalidade'); ?>
			</div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2961)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaCursosModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
		</td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2961);" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Cursos de pós-graduação<br>por dependência administrativa'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2961)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaCursosDependencia', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
        </td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(572);">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas de pós-graduação<br>por modalidade'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (572)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(572);">
            <div>
                <?php exibirTitulo('indicador', 'Matrículas de pós-graduação<br>por dependência administrativa'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (572)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaDependencia', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(726);">
            <div>
                <?php exibirTitulo('indicador', 'Matriculados em universidades federais<br>por modalidade'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (726)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaUniModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(112);">
            <div>
                <?php exibirTitulo('indicador', 'Professores estrangeiros participantes do<br>Programa Escola de Altos Estudos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS rede,
                        'Professores' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (112)
                    AND sh.sehstatus <> 'I'
                    GROUP BY rede, tipo
                    ORDER BY rede, tipo";
            $dados = $db->carregar( $sql, null, 3200 );
            $valorAcumulado = 0;
            if($dados){
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    $dados[] = array(
                        'rede'=>$dado['rede'],
                        'tipo'=>'Professores Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaEsforcoEscolar', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(574);">
            <div>
                <?php exibirTitulo('indicador', 'Titulados em pós-graduação<br>por modalidade'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (574)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTituladoModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(574);">
            <div>
                <?php exibirTitulo('indicador', 'Titulados em pós-graduação<br>por dependência administrativa'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (574)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaTituladoDependencia', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(114);">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas concedidas no país por modalidade'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (114)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaBolsasModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(527);">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas concedidas no exterior por modalidade'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS acao,
                        tiddsc AS situacao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (527)
                    AND sh.sehstatus <> 'I'
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaBolsaExtsModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(505);" rowspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Acessos ao Portal de Periódicos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpe.dpeanoref AS descricao, sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (505)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColuna($dados, 'graficoColunaPortalPeriodico', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            ?>
        </td>
        <td class="fundo_padrao link"  align="left" onclick="window.location='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Ciência sem Fronteiras'); ?>
            </div>
        </td>
    </tr>
	<tr>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,96,'','');" align="center" >
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
            <?=exibirTabelaFinanceiro(96);?>
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