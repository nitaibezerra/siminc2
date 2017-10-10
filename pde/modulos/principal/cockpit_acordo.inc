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
		.fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')};
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_acordo&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Acordo de Gratuidade</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" width="33%">
			<div>
				<?php exibirTitulo('executive', 'SENAI'); ?>
			</div>
			<?
			$sql = "select 'A' as curso, 0 as matricula
					union all
					select 'B' as curso, 0 as matricula
					union all
					select 'C' as curso, 0 as matricula";
			$arrDados = $db->carregar( $sql, null, 3200 );
			$totalMatricula = 0;
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th>Cursos</th>
					<th>Matrículas</th>
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $dado):
				    $count++;?>
					<tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
						<td><?php echo $dado['curso'] ?></td>
						<td class="numero" ><?php $totalMatricula+=$dado['matricula']; echo number_format($dado['matricula'],0,",",".") ?></td>
					</tr>
				<?php endforeach; ?>
				<tr>
					<th>Total</th>
					<th class="numero"><?php echo number_format($totalMatricula,0,",",".") ?></th>
				</tr>
			</table>
		</td>
		<td class="fundo_padrao link" onclick="abreMetaPronatec();" width="34%">
			<div>
				<?php exibirTitulo('doc', 'Matrículas'); ?>
			</div>
			<?
			$sql = "SELECT
						SUM(dsh.dshqtde::integer) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2399)
					AND sh.sehstatus <> 'I'";
			$totalMeta = $db->pegaUm( $sql, null, 3200 );
			$valorMeta = 3412443;
			$porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
			$horizontal = $porcentagem * 2;
			?>
			<table cellpadding="0" cellspacing="0" border="0" valign="top" width="100%" >
				<tr>
					<td class="titulo_box bold" align="center"><br>
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'><?= $porcentagem ?>%&nbsp;</div>
					</div><?php echo number_format($totalMeta,0,",",".") ?></td>
				</tr>
				<tr>
					<td>
						<?php exibirTitulo('alvo', 'Meta '.number_format($valorMeta,0,",",".").' matrículas até 2014'); ?>
					</td>
				</tr>
			</table>
		</td>
		<td class="fundo_padrao" width="33%">
			<div>
				<?php exibirTitulo('executive', 'SENAC'); ?>
			</div>
			<?
			$sql = "select 'A' as curso, 0 as matricula
					union all
					select 'B' as curso, 0 as matricula
					union all
					select 'C' as curso, 0 as matricula";
			$arrDados = $db->carregar( $sql, null, 3200 );
			$totalMatricula = 0;
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th>Cursos</th>
					<th>Matrículas</th>
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $dado):
				    $count++;?>
					<tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
						<td><?php echo $dado['curso'] ?></td>
						<td class="numero" ><?php $totalMatricula+=$dado['matricula']; echo number_format($dado['matricula'],0,",",".") ?></td>
					</tr>
				<?php endforeach; ?>
				<tr>
					<th>Total</th>
					<th class="numero" ><?php echo number_format($totalMatricula,0,",",".") ?></th>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2399);">
			<div>
				<?php exibirTitulo('indicador', 'SENAI'); ?>
			</div>
			<?
			$sql = "SELECT
						tid1.tiddsc AS descricao,
						SUM(dsh.dshqtde::integer) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid IN (2399)
					AND sh.sehstatus <> 'I'
					AND dsh.tidid2 = 5300 --SENAI
					GROUP BY descricao
					ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoPizzaSENAI", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2512);">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas por Categoria de Situação'); ?>
			</div>
			<?php
			$sql = "SELECT
						tid1.tiddsc as ofertante,
						".montaColunaCategoriaPronatec()." AS situacao,
						sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					WHERE sh.indid IN (2512)
					AND sh.sehstatus <> 'I'
					GROUP BY ofertante, situacao
					ORDER BY ofertante, situacao";
			$dados = $db->carregar($sql, null, 3200);

            $dadosAgrupados = agruparDadosGrafico($dados, 'situacao', 'ofertante', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

			geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculasSituacao', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
			?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2399);">
			<div>
				<?php exibirTitulo('indicador', 'SENAC'); ?>
			</div>
			<?
			$sql = "SELECT
						tid1.tiddsc AS descricao,
						SUM(dsh.dshqtde::integer) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid IN (2399)
					AND sh.sehstatus <> 'I'
					AND dsh.tidid2 = 5301 --SENAC
					GROUP BY descricao
					ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoPizzaSENAC", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2399);" colspan="2" rowspan="3">
			<div>
				<?php exibirTitulo('mapas', 'Número de Matrículas por Estado'); ?>
			</div>
			<?php
			$sql = "SELECT
						dshuf as descricao,
						tid2.tiddsc AS rede,
						SUM(dsh.dshqtde::integer) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					WHERE sh.indid IN (2399)
					AND sh.sehstatus <> 'I'
					GROUP BY descricao, rede
					ORDER BY descricao, rede";
			$arrDados = $db->carregar( $sql, null, 3200 );

			$dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'rede', 'valor');

			$formatoValores = array(
				'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
				'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
			);
			geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaEstado', null, null, $formatoValores, null, null, 1000, 600, null, 300);
			?>
		</td>
		<td class="fundo_padrao link" onclick="abreAcaoPainel(146);">
			<div>
				<?php exibirTitulo('indicador', 'Indicadores'); ?>
			</div>
		</td>
	</tr>
	<tr>
		<!-- Tabela Processos -->
		<td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/')";>
			<div>
				<?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
			</div>
		</td>
		<!-- Tabela Processos -->
	</tr>
	<tr>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',2,146,'','');" align="center" >
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