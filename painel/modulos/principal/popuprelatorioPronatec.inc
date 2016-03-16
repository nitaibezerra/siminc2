<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
?>
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
	<?php
    $exportarWord =isset($_REQUEST['exportarWord'])?$_REQUEST['exportarWord']:NULL;
    if(!empty($exportarWord)){
        header("Content-type: application/vnd.ms-word");
        header("Content-type: application/force-download");
        header("Content-Disposition: attachment; filename=RelatorioPronatec.doc");
        header("Pragma: no-cache");
    }
	?>
	<style>
		@media print{    
			.no-print, .no-print *{
				display: none !important;
			}
		}
		.fundo_titulo{
			background-repeat:repeat-xt;
			background-position:2px -50px;
			font-weight:bold;
			font-size:30px;
			color:#FFFFFF;
			text-shadow:#000000 0px 4px 2px;
		}

		.fundo_td{background-color:#0F6D39}
		.fundo_td:hover {background-color:#0D8845}
		.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
		.titulo_box{font-weight:bold;font-size:18px;color:#000000;margin-top:15px;}
		.subtitulo_box{font-weight:normal;font-size:12px;color:#000000}
		.fundo_td{text-align:left;vertical-align:top;}
		.tabela_painel{font-weight:bold;font-size:8px;color:#000000;font-family:fantasy}
		.numero{text-align:right}
		.center{text-align:center}
		.titulo_box a{color:#FFFFFF;text-decoration:none;}
		.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
		.div_fotos_interno{margin-bottom:2px;width:98%}
		.bold{font-weight:bold}
		.link{cursor:pointer}

		.left{float:left;}
		.right{float:right;}
		.clear{clear:both;}

		td{vertical-align: top;font-size:15px;}

		.fundo_padrao{background-color:#152D56}
		.fundo_padrao:hover {background-color:#1F3864}
		.fundo_azul{background-color:#2B86EE}
		.fundo_azul_padrao{background-color:#4F81BD}
		.fundo_verde{background-color:#0F6D39}
		.fundo_verde:hover{background-color:#32CD32}
		.fundo_laranja{background-color:#EE9200}
		.fundo_laranja:hover{background-color:#EBB513}
		.fundo_vermelho{background-color:#BB0000}
		.fundo_vermelho:hover{background-color:#DD0000}
		.fundo_roxo{background-color:#5333AD}
		.fundo_roxo:hover{background-color:#6A5ACD}
		.fundo_azul_escuro{background-color:#152D56}
		.fundo_azul_escuro:hover{background-color:#1F3864}
		.fundo_amarelo{background-color:#DAA520}

		.box_texto{
		   width: 160px; 
		   height: 60px;
		   margin: 5px;
		   float: left; 
		   text-align: left;
		   vertical-align: middle; 
		   font-family: Verdana; 
		   font-size: 12px; 
		   cursor: pointer;
		}

		.tabela_box{color:#444;}
		.tabela_box tr{ background-color:#eee; }
		.tabela_box tr.zebrado{ background-color:#ddd; }
		.tabela_box tr:hover{ background-color:#fff; }

		.tabela_indicadores td{ background-color:#C0D9D9 !important; color:#444 !important; text-shadow: none;}

		.scrollbar{
			overflow: hidden;
			position: relative;
		}
	</style>
	<script>
		function exportarWord(){
			window.location.href+='&exportarWord=1';
		}
	</script>
</head>
<?php
$regcod = $_REQUEST['regcod'];
$estuf = $_REQUEST['estuf'];
$muncod = $_REQUEST['muncod'];
$codper = $_REQUEST['codper'];

if($codper==2){
    $periodo = "AND dpe.dpeanoref BETWEEN '2011' AND '2014'";
    $tituloperiodo = " (2011 - 2014)";
}elseif($codper==3){
    $periodo = "AND dpe.dpeanoref BETWEEN '2015' AND '2018'";
    $tituloperiodo = " (2015 - 2018)";
}else{
    $periodo = "";
    $tituloperiodo = "";
}

if($regcod){
	$where = "AND est.regcod = '{$regcod}' ";
}elseif($estuf){
	$where = "AND dsh.dshuf = '{$estuf}' ";
}elseif($muncod){
	$where = "AND dsh.dshcodmunicipio = '{$muncod}' ";
}

if($regcod){
	$sql = "SELECT 'Região ' || regdescricao AS titulo FROM territorios.regiao WHERE regcod = '{$regcod}' ";
}elseif($estuf){
	if($estuf == 'AL' || $estuf == 'MG' || $estuf == 'PE' || $estuf == 'RO' || $estuf == 'RR' || $estuf == 'SC' || $estuf == 'SE' || $estuf == 'SP'){
		$preposicao = "de";
	}elseif($estuf == 'BA' || $estuf == 'PB'){
		$preposicao = "da";
	}else{
		$preposicao = "do";
	}
	$sql = "SELECT 'Estado {$preposicao} ' || estdescricao AS titulo FROM territorios.estado WHERE estuf = '{$estuf}' ";
}elseif($muncod){
	$sql = "SELECT 'Município de ' || mundescricao AS titulo FROM territorios.municipio WHERE muncod = '{$muncod}' ";
}else{
	$sql = "SELECT 'Nacional' AS titulo  ";
}
$titulo = $db->pegaUm($sql);
?>
<body>
	<?php /* if(empty($exportarWord)): ?>
		<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen"  style="border-bottom: 1px solid;" >
			<tr bgcolor="#ffffff">
				<td>
					<img src="../imagens/brasao.gif" width="50" height="50" border="0">
				</td>
				<td height="20" nowrap>
					SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
					Ministério da Educação / SE - Secretaria Executiva<br>
					DTI - Diretoria de Tecnologia da Informação<br>
				</td>
				<td height="20" align="right">
					Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
					Órgão: <?= $_SESSION['usuorgao']; ?><br>
					Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
				</td>
			</tr>
			<tr bgcolor="#ffffff">
				<td colspan="2">&nbsp;</td>
			</tr>
		</table>
		<div style="text-align: right;" class="no-print">
			<input type="button" onclick="window.print();" value="Imprimir" name="btnImprimir" class="botao">
			<input type="button" onclick="exportarWord();" value="Exportar para Word" name="btnExportarWord" class="botao">
		</div>
	<?php endif; */?>
	<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="titulo_pagina" >
				<div style="float:left" class="titulo_box" >PRONATEC<?=$tituloperiodo?> - <?=$titulo?> <span class="subtitulo_box bold" >(Referência: <?=date('d/m/Y')?>)</span></div>
				<div style="float:right;">
					<img src="/painel/images/logo_pronatec.png" style="vertical-align:middle;" />
				</div>
			</td>
		</tr>
	</table>
	<hr>
	<table border="1" align="center" width="100%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr>
			<td colspan="2">
				<?php
				$sql = "SELECT
							sum(dsh.dshqtde::integer) as matriculas,
							count(distinct dshcodmunicipio) as municipios
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2577)
						$periodo
						$where
						AND sh.sehstatus <> 'I'";
				$arrDados = $db->pegaLinha( $sql );
				?>
				<span class="titulo_box"><?=number_format($arrDados['matriculas'],0,",",".")?> matrículas em <?=number_format($arrDados['municipios'],0,",",".")?> municípios</span><br>
				<span class="subtitulo_box">Iniciativas: Bolsa-Formação, Acordo de Gratuidade, Expansão da Rede Federal, E-Tec Brasil e Brasil Profissionalizado.</span>
			</td>
		</tr>
		<tr>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Evolução anual das matrículas totais</div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				if($where){
					$sql = "SELECT
								dpe.dpeanoref AS descricao,
								sum(dsh.dshqtde::integer) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
							WHERE sh.indid IN (2577)
							AND sh.sehstatus <> 'I'
							$periodo
							$where
							GROUP BY descricao
							ORDER BY descricao";
					$arrDados = $db->carregar( $sql );
					$count=0;
					$total=0;
					if($arrDados){
						foreach($arrDados as $dado){
							$total = $total + $dado['valor'];
							$arrDados[$count]['valor'] = $total;
							$count++;
						}
						$formatoValores = array(
							'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
							'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
						);
						$config = array('dataLabels'=>'true');
						geraGraficoColuna($arrDados, 'graficoColunaMatriculas', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config, '#000');
					}
				}else{
					$sql = "SELECT descricao, valor, tipo
							FROM (
								SELECT
									dpe.dpeanoref AS descricao,
									sum(dsh.dshqtde::integer) AS valor,
									'Matrículas acumuladas' AS tipo
								FROM painel.seriehistorica sh
								INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
								INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
								INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
								WHERE sh.indid IN (2577)
								AND sh.sehstatus <> 'I'
								$periodo
								GROUP BY descricao
							UNION ALL
								SELECT dpe.dpeanoref AS descricao, 0 AS valor, 'Meta Acumulada'
								FROM painel.seriehistorica sh
								INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
								INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
								INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
								WHERE sh.indid IN (2577)
								AND sh.sehstatus <> 'I'
								$periodo
								GROUP BY descricao
							) AS FOO
							ORDER BY descricao, tipo DESC";
					$dados = $db->carregar( $sql );
					$count=0;
					$total=0;
					if($dados){
						foreach($dados as $dado){
							if($dados[$count]['tipo']!='Meta Acumulada'){
								$total = $total + $dado['valor'];
								$dados[$count]['valor'] = $total;
							}
							$count++;
						}
						//$dados[6]['valor'] = 7944775-$total;

						$dadosAgrupados = agruparDadosGrafico($dados, 'descricao', 'tipo', 'valor');

						$formatoValores = array(
							'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
							'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.percentage, 2, ',', '.') + '%</b>'; }"
						);
						geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaSituacao', null, null, $formatoValores, null, "", 1000, 600, null, 300, true, $cor='#000');
					}
				}
				?>
			</td>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Principais cursos <?= ($where ? ' - '.$titulo : ''); ?></div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?
				$sql = "SELECT
							'Técnico' AS tipo,
							tid1.tiddsc AS curso,
							SUM(dsh.dshqtde) AS valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2403)
						AND sh.sehstatus <> 'I'
						AND dsh.tidid2 IN (5331, 5330) --Técnico
						$periodo
						$where
						GROUP BY curso
						ORDER BY valor DESC, tipo, curso
						LIMIT 10";
				$arrDadosTecnico = $db->carregar( $sql );
				
				$sql = "SELECT
							'FIC' AS tipo,
							tid1.tiddsc AS curso,
							SUM(dsh.dshqtde) AS valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2403)
						AND sh.sehstatus <> 'I'
						AND dsh.tidid2 IN (5329) --FIC
						$periodo
						$where
						GROUP BY curso
						ORDER BY valor DESC, curso
						LIMIT 10";
				$arrDadosFic = $db->carregar( $sql );
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<th width="10%">&nbsp</th>
						<th>Cursos FIC</th>
						<th width="10%">&nbsp</th>
						<th>Cursos Técnicos</th>
					</tr>
					<?php
					$count = -1;
					$correArray = 0;
					while ($correArray < 10){
						$count++;
						?>
						<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?> height="30px">
							<td class="" ><?=$correArray+1 ?></td>
							<td class="" ><?=$arrDadosFic[$correArray]['curso'] ?></td>
							<td class="" ><?=$correArray+11 ?></td>
							<td class="" ><?=$arrDadosTecnico[$correArray]['curso'] ?></td>
						</tr>
					<?php
					$correArray++;
					} ?>
				</table>
			</td>
		</tr>
		<tr>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Matrículas por Estado</div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				$sql = "SELECT
							dsh.dshuf as descricao,
							sum(dsh.dshqtde::integer) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2577)
						AND sh.sehstatus <> 'I'
						$periodo
						GROUP BY descricao
						ORDER BY descricao";
				$arrDados = $db->carregar( $sql );
				if($arrDados){
					//$formatoValores = array(
					//	'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
					//	'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
					//);
					//$config = array('dataLabels'=>'true');
					//geraGraficoColuna($arrDados, 'graficoColunaUF', null, null, $formatoValores, null, "", 1000, 600, null, 300, $config, '#000');
                    $arrUF = array();
                    $somaUF = 0;
                    foreach($arrDados as $dado){
                        $arrUF[$dado['descricao']] = $dado['valor'];
                        $somaUF +=  $dado['valor'];
                    }
                    ?>
                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <th>UF</th>
                            <th>Total</th>
                            <th>UF</th>
                            <th>Total</th>
                        </tr>
                        <tr>
                            <td>AC</td>
                            <td class="numero"><?=number_format($arrUF['AC'], 0, ",", ".") ?></td>
                            <td>PB</td>
                            <td class="numero"><?=number_format($arrUF['PB'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>AL</td>
                            <td class="numero"><?=number_format($arrUF['AL'], 0, ",", ".") ?></td>
                            <td>PE</td>
                            <td class="numero"><?=number_format($arrUF['PE'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>AM</td>
                            <td class="numero"><?=number_format($arrUF['AM'], 0, ",", ".") ?></td>
                            <td>PI</td>
                            <td class="numero"><?=number_format($arrUF['PI'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>AP</td>
                            <td class="numero"><?=number_format($arrUF['AP'], 0, ",", ".") ?></td>
                            <td>PR</td>
                            <td class="numero"><?=number_format($arrUF['PR'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>BA</td>
                            <td class="numero"><?=number_format($arrUF['BA'], 0, ",", ".") ?></td>
                            <td>RJ</td>
                            <td class="numero"><?=number_format($arrUF['RJ'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>CE</td>
                            <td class="numero"><?=number_format($arrUF['CE'], 0, ",", ".") ?></td>
                            <td>RN</td>
                            <td class="numero"><?=number_format($arrUF['RN'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>DF</td>
                            <td class="numero"><?=number_format($arrUF['DF'], 0, ",", ".") ?></td>
                            <td>RO</td>
                            <td class="numero"><?=number_format($arrUF['RO'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>ES</td>
                            <td class="numero"><?=number_format($arrUF['ES'], 0, ",", ".") ?></td>
                            <td>RR</td>
                            <td class="numero"><?=number_format($arrUF['RR'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>GO</td>
                            <td class="numero"><?=number_format($arrUF['GO'], 0, ",", ".") ?></td>
                            <td>RS</td>
                            <td class="numero"><?=number_format($arrUF['RS'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>MA</td>
                            <td class="numero"><?=number_format($arrUF['MA'], 0, ",", ".") ?></td>
                            <td>SE</td>
                            <td class="numero"><?=number_format($arrUF['SE'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>MG</td>
                            <td class="numero"><?=number_format($arrUF['MG'], 0, ",", ".") ?></td>
                            <td>SC</td>
                            <td class="numero"><?=number_format($arrUF['SC'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>MS</td>
                            <td class="numero"><?=number_format($arrUF['MS'], 0, ",", ".") ?></td>
                            <td>SP</td>
                            <td class="numero"><?=number_format($arrUF['SP'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>MT</td>
                            <td class="numero"><?=number_format($arrUF['MT'], 0, ",", ".") ?></td>
                            <td>TO</td>
                            <td class="numero"><?=number_format($arrUF['TO'], 0, ",", ".") ?></td>
                        </tr>
                        <tr>
                            <td>PA</td>
                            <td class="numero"><?=number_format($arrUF['PA'], 0, ",", ".") ?></td>
                            <td>Brasil</td>
                            <td class="numero"><?=number_format($somaUF, 0, ",", ".") ?></td>
                        </tr>
                    </table>
                <?php
				}
				?>
			</td>
			<td width="50%">
				<?php
				if($muncod){
					$sql = "SELECT e.estuf, estdescricao AS titulo
							FROM territorios.municipio m
							INNER JOIN territorios.estado e ON e.estuf = m.estuf
							WHERE muncod = '{$muncod}'";
					$arrLinha = $db->pegaLinha( $sql );
					$estuf2 = $arrLinha['estuf'];
					if($estuf2 == 'AL' || $estuf2 == 'MG' || $estuf2 == 'PE' || $estuf2 == 'RO' || $estuf2 == 'RR' || $estuf2 == 'SC' || $estuf2 == 'SE' || $estuf2 == 'SP'){
						$preposicao = "de";
					}elseif($estuf2 == 'BA' || $estuf2 == 'PB'){
						$preposicao = "da";
					}else{
						$preposicao = "do";
					}
					$whereUF = "AND dsh.dshuf = '{$estuf2}' ";
					$titulo2 = 'Estado '.$preposicao.' '.$arrLinha['titulo'];
				}else{
					$titulo2 = $titulo;
				}
				?>
				<div>
					<div style="float:left" class="titulo_box" >Municípios com maior número de matrículas <?= ($where ? ' - '.$titulo2 : ''); ?></div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				$sql = "SELECT
							m.mundescricao AS municipio,
							SUM(dsh.dshqtde) AS valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.municipio m ON m.muncod = dsh.dshcodmunicipio
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2577)
						AND sh.sehstatus <> 'I'
						$periodo
						".($muncod ? $whereUF : $where)."
						GROUP BY municipio
						ORDER BY valor DESC, municipio
						LIMIT 20";
				$arrDados = $db->carregar( $sql );
				?>
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
					<tr>
						<th width="10%">&nbsp</th>
						<th>Município</th>
						<th width="10%">&nbsp</th>
						<th>Município</th>
					</tr>
					<?php
					$count = -1;
					$correArray = 0;
					while ($correArray < 10){
						$count++;
						?>
						<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?> height="30px">
							<td class="" ><?=$correArray+1 ?></td>
							<td class="" ><?=$arrDados[$correArray]['municipio'] ?></td>
							<td class="" ><?=$correArray+11 ?></td>
							<td class="" ><?=$arrDados[$correArray+10]['municipio'] ?></td>
						</tr>
					<?php
					$correArray++;
					} ?>
				</table>
			</td>
		</tr>
		<tr>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Matrículas por tipo de curso <?= ($where ? ' - '.$titulo : ''); ?></div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				$sql = "SELECT
							tid2.tiddsc AS descricao,
							sum(dsh.dshqtde::integer) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2577)
						AND sh.sehstatus <> 'I'
						$periodo
						$where
						GROUP BY descricao
						ORDER BY descricao";
                $arrDados = $db->carregar( $sql );
				if($arrDados){
                    geraGrafico($arrDados, "graficoPizzaMatriculas", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#000');

					//$dadosAgrupados = agruparDadosGrafico($dados, null, 'descricao', 'valor');
					//$formatoValores = array(
					//	'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
					//	'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.')  + '</b><br />' + 'Percentual: <b>'+ number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }",
					//	'plotOptions' => "SIM"
					//);
					//geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarraTipo', null, null, $formatoValores, null, "", 1000, 600, null, 300, '#000');
				}
				?>
			</td>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Matrículas por gênero <?= ($where ? ' - '.$titulo : ''); ?></div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				$sql = "SELECT
							tid1.tiddsc as descricao,
							sum(dsh.dshqtde::integer) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						AND sh.indid IN (2761, 2744)
			            AND dsh.tidid1 NOT IN (7448, 7442) --Não Informado
						AND sh.sehstatus <> 'I'
						$periodo
						$where
						GROUP BY descricao
						ORDER BY descricao";
                $arrDados = $db->carregar( $sql );
				if($arrDados){
                    geraGrafico($arrDados, "graficoPizzaMatriculasGenero", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#000');
					//$dadosAgrupados = agruparDadosGrafico($dados, null, 'descricao', 'valor');
					//$formatoValores = array(
					//	'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
					//	'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.')  + '</b><br />' + 'Percentual: <b>'+ number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }",
					//	'plotOptions' => "SIM"
					//);
					//geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarraGenero', null, null, $formatoValores, null, "", 1000, 600, null, 300, '#000');
				}
				?>
			</td>
		</tr>
		<tr>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Matrículas por rede ofertante <?= ($where ? ' - '.$titulo : ''); ?></div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				$sql = "SELECT
							tid1.tiddsc AS descricao,
							sum(dsh.dshqtde::integer) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2782)
						AND dsh.tidid1 NOT IN (7477) --Não categorizado
						AND sh.sehstatus <> 'I'
						$periodo
						$where
						GROUP BY descricao
						ORDER BY descricao";
                $arrDados = $db->carregar( $sql );
				if($arrDados){
                    geraGrafico($arrDados, "graficoPizzaMatriculasRede", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#000');
					//$dadosAgrupados = agruparDadosGrafico($dados, null, 'descricao', 'valor');
					//$formatoValores = array(
					//	'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
					//	'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.')  + '</b><br />' + 'Percentual: <b>'+ number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }",
					//	'plotOptions' => "SIM"
					//);
					//geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarraRede', null, null, $formatoValores, null, "", 1000, 600, null, 300, '#000');
				}
				?>
			</td>
			<td width="50%">
				<div>
					<div style="float:left" class="titulo_box" >Matrículas por faixa etária <?= ($where ? ' - '.$titulo : ''); ?></div>
				</div>
				<div class="clear"></div>
				<br><br>
				<?php
				$sql = "SELECT
							tid2.tiddsc as descricao,
							sum(dsh.dshqtde::integer) as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf
						WHERE sh.indid IN (2787)
						AND sh.sehstatus <> 'I'
						$periodo
						$where
						GROUP BY descricao
						ORDER BY descricao";
                $arrDados = $db->carregar( $sql );
				if($arrDados){
                    geraGrafico($arrDados, "graficoPizzaMatriculasEscolaridade", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#000');
					//$dadosAgrupados = agruparDadosGrafico($dados, null, 'descricao', 'valor');
					//$formatoValores = array(
					//	'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
					//	'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.')  + '</b><br />' + 'Percentual: <b>'+ number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }",
					//	'plotOptions' => "SIM"
					//);
					//geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarraEscolaridade', null, null, $formatoValores, null, "", 1000, 600, null, 300, '#000');
				}
				?>
			</td>
		</tr>
	</table>
</body>
</html>