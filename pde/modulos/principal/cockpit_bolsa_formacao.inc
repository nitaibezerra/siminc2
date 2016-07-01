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
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_bolsa_formacao&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Bolsa-Formação</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" width="33%">
			<?php montaTabelaEstrategico('Bolsa-Formação', 129397); ?>
		</td>
		<td class="fundo_padrao" width="34%">
			<div>
				<?php exibirTitulo('alvo', '<span class="link" onclick="abreMetaPronatec();">Meta 2011-2014: 2.985.292 vagas</span>','<span class="link" onclick="abreIndicadorPopUp(2299);">Matrículas por Eixo Tecnológico</span>, <span class="link" onclick="abreIndicadorPopUp(2302);">SISUTEC</span>'); ?>
			</div>
			<?php
			$sql = "SELECT tipo, sum(valor) as valor
					from (
						SELECT case dsh.tidid1 when 4858 then 'FIC' else 'Técnico' end as tipo, sum(dsh.dshqtde::integer) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						WHERE sh.indid IN (2299,2302)
						AND sh.sehstatus <> 'I'
						GROUP BY tipo
					) as foo
					GROUP BY tipo
					ORDER BY tipo";
			$arrDados = $db->carregar( $sql, null, 3200 );
			foreach($arrDados as $dado){
				if($dado['tipo']=='FIC'){
					$totalFIC = $dado['valor'];
				}else{
					$totalTecnico = $dado['valor'];
				}
				$totalExecucao += $dado['valor'];
			}
			$percentualFIC = ($totalFIC / $totalExecucao) * 100;
			$percentualTecnico = ($totalTecnico / $totalExecucao) * 100;
			?>
			<br>
			<br>
			<div>
				<div style="float: left; width: 100%;"> <?php exibirTitulo('executive', 'Execução<br />'.number_format($totalExecucao,0,",",".") . '<br>matrículas'); ?></div>
				<div style="float: left; width: 50%;"> <?php exibirTitulo('configs', 'FIC<br />'.number_format($percentualFIC,0,",",".").'%' . '<br />' . number_format($totalFIC,0,",",".")); ?></div>
				<div style="float: left; width: 50%;"> <?php exibirTitulo('configs', 'TEC<br />'.number_format($percentualTecnico,0,",",".").'%' . '<br />' . number_format($totalTecnico,0,",",".")); ?></div>
			</div>
		</td>
		<td class="fundo_padrao" width="33%">
			<div>
				<?php exibirTitulo('indicador', 'SISUTEC', '<span class="link" onclick="abreIndicadorPopUp(2304);">Inscrições</span>, <span class="link" onclick="abreIndicadorPopUp(2303);">Inscritos</span>, <span class="link" onclick="abreIndicadorPopUp(2332);">Vagas Ofertadas</span>, <span class="link" onclick="abreIndicadorPopUp(2302);">Matrículas</span>'); ?>
			</div>
			<?php
			// SISUTEC
			$sql = "SELECT  case sh.indid
								when 2332 then 'Vagas Ofertadas'
								when 2304 then 'Inscrições'
								when 2303 then 'Inscritos'
								when 2302 then 'Matrículas'
							end as descricao, sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2332,2304,2303,2302)
					AND sh.sehstatus <> 'I'
					group by descricao";
			$dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
			geraGraficoColuna($dados, 'graficoColunaSisutec', null, null, $formatoValores, null, "", 1000, 600, null, 300);
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" colspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas por Rede Ofertante', '<span class="link" onclick="abreIndicadorPopUp(2300);">Bolsa-Formação - FIC</span>, <span class="link" onclick="abreIndicadorPopUp(2333);">Bolsa-Formação - Técnico</span>, <span class="link" onclick="abreIndicadorPopUp(2302);">SISUTEC</span>'); ?>
			</div>
			<?php
			// Bolsa-Formação - Matrículas por Rede Ofertante
			$sql = "SELECT tid.tiddsc as descricao, sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
					WHERE sh.indid IN (2333,2300,2302)
					AND sh.sehstatus <> 'I'
					GROUP BY descricao
					ORDER BY descricao";
			$dados = $db->carregar( $sql, null, 3200 );
			if($dados){
				geraGrafico($dados, "graficoPizzaMatriculasOfertante", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			}
			?>
		</td>
<!--
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas concluídas por Rede Ofertante', '<span class="link" onclick="abreIndicadorPopUp(2300);">Bolsa-Formação - FIC</span>, <span class="link" onclick="abreIndicadorPopUp(2333);">Bolsa-Formação - Técnico</span>, <span class="link" onclick="abreIndicadorPopUp(2302);">SISUTEC</span>'); ?>
			</div>
			<?
			$sql = "SELECT tid.tiddsc AS descricao, SUM(dsh.dshqtde) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
					WHERE (
					    (sh.indid IN (2300, 2333) AND dsh.tidid2 IN (6046,6116,6125,6055))
					    OR
					    (sh.indid IN (2302))
					    )
					AND sh.sehstatus <> 'I'
					GROUP BY descricao
					ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoPizzaConcluintes", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
-->
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas por Categoria de Situação', '<span class="link" onclick="abreIndicadorPopUp(2300);">Bolsa-Formação - FIC</span>, <span class="link" onclick="abreIndicadorPopUp(2333);">Bolsa-Formação - Técnico</span>, <span class="link" onclick="abreIndicadorPopUp(2302);">SISUTEC</span>'); ?>
			</div>
			<?
			$sql = "SELECT
						".montaColunaCategoriaPronatec()." AS descricao,
						sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (2333,2300,2302)
					AND sh.sehstatus <> 'I'
					GROUP BY descricao
					ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoPizzaMatriculasSituacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
<?php
/*
?>
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('indicador', 'Pactuação 2014 - Rede Ofertante'); ?>
			</div>
			<?
			$sql = "select 'Exemplo 1' as descricao, 100 as valor
					union all
					select 'Exemplo 2' as descricao, 200 as valor";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoRoscaOfertante", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
<?php
*/
?>
	</tr>
	<tr>
		<td class="fundo_padrao" colspan="3">
			<div>
				<?php exibirTitulo('indicador', 'Vagas ofertadas por Rede Ofertante', '<span class="link" onclick="abreIndicadorPopUp(733);">Bolsa-Formação - Vagas</span>, <span class="link" onclick="abreIndicadorPopUp(2332);">SISUTEC - Vagas</span>'); ?>
			</div>
			<?php
			$sql = "SELECT 
						CASE WHEN sh.indid IN (2332) THEN 'Instituições Privadas'
						ELSE tid.tiddsc
						END AS descricao, sum(dsh.dshqtde::integer) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
					WHERE sh.indid IN (733,2332)
					AND sh.sehstatus <> 'I'
					GROUP BY descricao
					ORDER BY descricao";
			$dados = $db->carregar( $sql, null, 3200 );

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
			geraGraficoColuna($dados, 'graficoColunaVagasOfertadas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
			?>
		</td>
<?php
/*
?>
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('indicador', 'Esforço'); ?>
			</div>
			<?
			$sql = "SELECT
						CASE sh.indid
							WHEN 752 THEN 'Esforço'
							WHEN 733 THEN 'Oferta'
							WHEN 2301 THEN 'Matrícula'
						END AS acao,
						dpe.dpeanoref AS situacao,
						SUM(dsh.dshqtde) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
					WHERE sh.indid IN (752,733,2301)
					AND sh.sehstatus <> 'I'
					GROUP BY acao, situacao
					ORDER BY acao, situacao";
			$dados = $db->carregar( $sql, null, 3200 );

			$acoes = array();
			$situacaos = array();
			$dadosAgrupados = array();

			foreach ($dados as $dado) {
				$dadosAgrupados[tirar_acentos($dado['acao'])][tirar_acentos($dado['situacao'])] = (float) $dado['valor'];
				$acoes[$dado['acao']] = tirar_acentos($dado['acao']);
				$situacaos[$dado['situacao']] = tirar_acentos($dado['situacao']);
			}

			$dadosFinais = array();
			foreach ($situacaos as $situacao) {
				foreach ($acoes as $acao) {
					if(!isset($dadosAgrupados[$acao][$situacao])){
						$dadosFinais[$situacao][] = 0;
					} else {
						$dadosFinais[$situacao][] = $dadosAgrupados[$acao][$situacao];
					}
				}
			}
			$aDados = array();
			foreach ($dadosFinais as $acao => $aDado) {
				$aDados[] = array('name' => $acao, 'data'=>$aDado);
			}
			
			$formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );

			geraGraficoColunaAgrupado($aDados, array_values($acoes), 'graficoColunaMatriculaSituacao', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
			?>
		</td>
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('indicador', 'Pactuação 2014 - Tipo de Curso'); ?>
			</div>
			<?
			$sql = "select 'Exemplo 1' as descricao, 100 as valor
					union all
					select 'Exemplo 2' as descricao, 200 as valor";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoRoscaTipoCurso", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
<?php
*/
?>
	</tr>
	<tr>
		<td class="fundo_padrao" rowspan="2">
			<div>
				<?php exibirTitulo('casas', 'Matriculas por Demandante', '<span class="link" onclick="abreIndicadorPopUp(2328);">Bolsa-Formação - FIC</span>, <span class="link" onclick="abreIndicadorPopUp(2329);">Bolsa-Formação - Técnico</span>,<br><span class="link" onclick="abreIndicadorPopUp(2302);">SISUTEC</span>'); ?>
			</div>
			<?php
			// Bolsa-Formação - Demandante
			$sql = "SELECT
						tid.tiddsc as demandante,
						case
							when sh.indid IN (2328) then 'FIC'
							when sh.indid IN (2329, 2302) then 'Técnico'
						end as tipo,
						sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
					WHERE sh.indid IN (2328,2329,2302)
					AND sh.sehstatus <> 'I'
					GROUP BY demandante, tipo
					ORDER BY demandante, tipo";
			$arrDados = $db->carregar( $sql, null, 3200 );
			$arrTipo = array();
			$somaFIC = 0;
			$somaTecnico = 0;
			$somaTotal = 0;
			$totalFIC = 0;
			$totalTecnico = 0;
			$totalGeral = 0;
			if($arrDados){
					foreach($arrDados as $dado){
						$arrTipo[$dado['demandante']][$dado['tipo']]['valor'][] = $dado['valor'];
				}
			}
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr class="">
					<th class="center bold" >Demandante</th>
					<th class="center bold" >FIC</th>
					<th class="center bold" >Técnico</th>
					<th class="center bold" >Total</th>
				</tr>
				<?php
				$count = -1;
				foreach($arrTipo as $chave => $onb):
					$somaFIC = (is_array($onb['FIC']['valor'])?array_sum($onb['FIC']['valor']):0);
					$somaTecnico = (is_array($onb['Técnico']['valor'])?array_sum($onb['Técnico']['valor']):0);
					$somaTotal = $somaFIC + $somaTecnico;
					$count++;
					?>
					<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?>>
						<td class="" ><?=$chave ?></td>
						<td class="numero" ><?=number_format($somaFIC,0,",",".")?></td>
						<td class="numero" ><?=number_format($somaTecnico,0,",",".")?></td>
						<td class="numero" ><?=number_format($somaTotal,0,",",".")?></td>
					</tr>
					<?php
					$totalFIC += $somaFIC;
					$totalTecnico += $somaTecnico;
					$totalGeral += $somaTotal;
				endforeach; ?>
				<tr class="">
					<th class="bold" >Total</th>
					<th class="numero" ><?=number_format($totalFIC,0,",",".")?></th>
					<th class="numero" ><?=number_format($totalTecnico,0,",",".")?></th>
					<th class="numero" ><?=number_format($totalGeral,0,",",".")?></th>
				</tr>
			</table>
		</td>
		<td class="fundo_padrao" rowspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas por Curso', '<span class="link" onclick="abreIndicadorPopUp(2403);">Tipo X Curso</span>, <span class="link" onclick="abreIndicadorPopUp(2402);">Rede X Curso</span>'); ?>
			</div>
			<?
			$sql = "SELECT
						'Técnico' AS tipo,
						tid1.tiddsc AS curso,
						SUM(dsh.dshqtde) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid IN (2403)
					AND sh.sehstatus <> 'I'
					AND dsh.tidid2 IN (5331, 5330) --Técnico
					GROUP BY curso
					ORDER BY valor DESC, tipo, curso
					LIMIT 10";
			$arrDadosTecnico = $db->carregar( $sql, null, 3200 );
			
			$sql = "SELECT
						'FIC' AS tipo,
						tid1.tiddsc AS curso,
						SUM(dsh.dshqtde) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					WHERE sh.indid IN (2403)
					AND sh.sehstatus <> 'I'
					AND dsh.tidid2 IN (5329) --FIC
					GROUP BY curso
					ORDER BY valor DESC, curso
					LIMIT 10";
			$arrDadosFic = $db->carregar( $sql, null, 3200 );
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th>FIC</th>
					<th>Técnico</th>
				</tr>
				<tr>
					<td>
						<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
							<tr>
								<th class="center bold" >Curso</th>
								<th class="center bold" >Matrículas</th>
							</tr>
							<?php
							$count = -1;
							foreach($arrDadosFic as $dado):
								$count++;
								?>
								<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?> height="30px">
									<td class="" ><?=$dado['curso'] ?></td>
									<td class="numero" ><?=number_format($dado['valor'],0,",",".") ?></td>
								</tr>
							<?php
							endforeach; ?>
						</table>
					</td>
					<td>
						<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
							<tr>
								<th class="center bold" >Curso</th>
								<th class="center bold" >Matrículas</th>
							</tr>
							<?php
							$count = -1;
							foreach($arrDadosTecnico as $dado):
								$count++;
								?>
								<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?> height="30px">
									<td class="" ><?=$dado['curso'] ?></td>
									<td class="numero" ><?=number_format($dado['valor'],0,",",".") ?></td>
								</tr>
							<?php
							endforeach; ?>
						</table>
					</td>
				</tr>
			</table>
		</td>
		<td class="fundo_padrao" rowspan="1">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas por Modalidade', '<span>Copa</span>, <span class="link" onclick="abreIndicadorPopUp(2330);">BSM</span>, <span class="link" onclick="abreIndicadorPopUp(2957);">Viver</span>, <span class="link" onclick="abreIndicadorPopUp(2082);">Campo</span>'); ?>
			</div>
			<?php
			$sql = "SELECT
						CASE sh.indid
							WHEN 2331 THEN 'Copa'
							WHEN 2330 THEN 'BSM'
							WHEN 2957 THEN 'Viver'
							WHEN 2082 THEN 'Campo'
						END AS acao,
                        ".montaColunaCategoriaPronatec()." AS situacao,
						SUM(dsh.dshqtde) AS valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
					WHERE sh.indid IN (2331,2330,2957,2082)
					AND sh.sehstatus <> 'I'
					GROUP BY acao, situacao
					ORDER BY acao, situacao";
ver($sql);
			$dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
			geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaModalidade', null, null, $formatoValores, null, null, 1000, 600, null, 300);
			?>
		</td>
	</tr>
	<tr>
		<!-- INÍCIO DO FINANCEIRO -->
		<?php
		// Parâmetros para a nova conexão com o banco do SIAFI
		$servidor_bd = $servidor_bd_siafi;
		$porta_bd    = $porta_bd_siafi;
		$nome_bd     = $nome_bd_siafi;
		$usuario_db  = $usuario_db_siafi;
		$senha_bd    = $senha_bd_siafi;
		// Cria o novo objeto de conexão
		$db2 = new cls_banco();
		// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
		$dados_conexao = array(
			'servidor_bd' => $servidor_bd_siafi,
			'porta_bd' => $porta_bd_siafi,
			'nome_bd' => $nome_bd_siafi,
			'usuario_db' => $usuario_db_siafi,
			'senha_bd' => $senha_bd_siafi
		);
		?>
		<td class="fundo_padrao">
			<?php
			$sql = "SELECT
						ano,
						sum(valor1) AS empenhado,
						sum(valor2) AS dotacao,
						sum(valor3) AS pago
					FROM (
						SELECT
							2011 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2011 sld
						WHERE substr(sld.plicod, 2, 4) in ('FP05')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2012 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2012 sld
						WHERE substr(sld.plicod, 2, 4) in ('FP05')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2013 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2013 sld
						WHERE substr(sld.plicod, 2, 4) in ('FP05')
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					) as foo
					GROUP BY ano
					ORDER BY ano";
			$arrDados = $db2->carregar($sql,null,3200);
			$total_dotacao = 0;
			$total_empenhado = 0;
			$total_pago = 0;
			?>
			<div>
				<?php exibirTitulo('financeiro', 'Orçamentário/Financeiro'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Ano</th>
					<th class="center bold" >Empenhado</th>
					<!--<th class="center bold" >Pago</th>-->
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $chave => $dado):
				    $count++;?>
					<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=FP05&alterar_ano=0')" >
						<td><?php echo $dado['ano'] ?></td>
						<td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
						<!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
					</tr>
				<?php endforeach; ?>
				<tr>
					<th class="bold" >Total</th>
					<th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
					<!--<th class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></th>-->
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2401);" colspan="2">
			<div>
				<?php exibirTitulo('mapas', 'Bolsa-Formação - Número de Matrículas por Estado'); ?>
			</div>
			<?php
			// Bolsa-Formação - Número de Matrículas por Estado
			$sql = "SELECT dsh.dshuf as descricao, tid.tiddsc as rede, sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = tidid1
					WHERE sh.indid IN (2401)
					AND sh.sehstatus <> 'I'
					GROUP BY descricao, rede
					ORDER BY descricao, rede";
			$dados = $db->carregar( $sql, null, 3200 );

			$dadosAgrupados = agruparDadosGrafico($dados, 'descricao', 'rede', 'valor');

			$formatoValores = array(
				'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
				'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
			);
			geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaEstado', null, null, $formatoValores, null, null, 1000, 600, null, 300);
			?>
		</td>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',2,145,'','');" align="center" >
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