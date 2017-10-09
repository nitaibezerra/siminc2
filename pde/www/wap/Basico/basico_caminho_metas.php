<?php
// carrega as funções gerais
include_once "config.inc";
include_once "../../_constantes.php";
include ("../../../../includes/funcoes.inc");
include ("../../../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

?>
<!DOCTYPE html> 
<html> 
	<head> 
		<title><?php echo SIGLA_SISTEMA; ?> - Mobile</title> 
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<link rel="stylesheet" href="/includes/mobile-simec/SIMEC.min.css" />
		<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile.structure-1.3.0-rc.1.min.css" /> 
		<script src="http://code.jquery.com/jquery-1.8.3.min.js"></script> 
		<script src="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile-1.3.0-rc.1.min.js"></script> 
	</head> 
<body>
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();

?>
<form name="formulario_mobile" method="post">
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
			<a href="../Basico" data-role="button" data-icon="arrow-r" data-ajax="false">Educação Básica </a>
		</div>
		<h1>Educação Básica</h1>
		<div data-role="navbar">
			<ul>
				<li><a class="ui-btn-active" data-theme="a" href="basico_caminho.php"  data-transition="slide">Caminho da Escola</a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="basico_caminho_metas_2012.php">Execução 2012</a>
				 </li>
				<li>
					<a data-transition="flip" href="basico_caminho_metas_2013.php">Execução 2013</a>
				 </li>
				<li>
					<a data-transition="flip" href="basico_caminho_metas.php">Execução 2012/2014</a>
				 </li>
					<tr>

						<td class="fundo_td" width="470" colspan=2 >
							<div>
				                <img style="float:left" src="../../../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
								<div style="float:left" class="titulo_box" ></br>Metas e Execução<br/></div>
							</div>
							<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
								<tr>
				                	<td class="center bold" style="background-color:#3B8550" rowspan=2>Iniciativa</td>
				                	<td class="center bold" style="background-color:#3B8550" rowspan=2><i>Meta até 2014</i></td>
				                	<td class="center bold" style="background-color:#3B8550" colspan=4>Executado 2012</td>
				                	<td class="center bold" style="background-color:#3B8550" colspan=4>Executado 2013</td>
									<td class="center bold" style="background-color:#3B8550" rowspan=2>Total</td>
								</tr>
								<tr height="30">
				                	<td class="center bold" style="background-color:#3B8550" >Recurso FNDE</td>
				                	<td class="center bold" style="background-color:#3B8550"  >Recurso Próprio</td>
				                	<td class="center bold" style="background-color:#3B8550"  >Recurso BNDES</td>
				                	<td class="center bold" style="background-color:#3B8550" >Subtotal</td>
								
				                	<td class="center bold" style="background-color:#3B8550" >Recurso FNDE</td>
				                	<td class="center bold" style="background-color:#3B8550" >Recurso Próprio</td>
				                	<td class="center bold" style="background-color:#3B8550" >Recurso BNDES</td>
				                	<td class="center bold" style="background-color:#3B8550" >Subtotal</td>
								</tr>
								<?php
								$sql = "select ano, tipoveiculo, sum(metaveiculo) as metaveiculo, sum(totalveiculofnde) as totalveiculofnde, sum(totalveiculoproprio) as totalveiculoproprio, sum(totalveiculobndes) as totalveiculobndes,
										(sum(totalveiculofnde)+sum(totalveiculoproprio)+sum(totalveiculobndes)) as totalonibus
										from(
											select	case tidid1
												when 3704 then 'urbano acessível'
												when 3705 then 'escolar rural'
												end as tipoveiculo,
												case tidid1
												when 3704 then 2609
												when 3705 then 8000
												end as metaveiculo,
												SUM(dsh.dshqtde) AS totalveiculofnde,
												0 as totalveiculoproprio,
												0 as totalveiculobndes,
												dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (1539)
											and sh.sehstatus <> 'I'
											and dpe.dpeanoref in ('2012','2013')
											group by tipoveiculo, metaveiculo, ano
										union all
											select	case
												when dsh.tidid2 in(1875,1876,1877,2586,2580,2581,2582,2583,2584,2585) then 'escolar rural'
												when dsh.tidid2 in(203,204,205,206,207,208,209,210,212,214,215,216,211,213,3259) then 'urbano acessível'
												end as tipoveiculo,
												0 as metaveiculo,
												0 as totalveiculofnde,
												SUM(dsh.dshqtde) AS totalveiculoproprio,
												0 as totalveiculobndes,
												dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (135)
											and sh.sehstatus <> 'I'
											and dsh.tidid1 in (18) --Recurso Próprio
											and dsh.tidid2 in (203,204,205,206,207,208,209,210,212,214,215,216,211,213,3259,1875,1876,1877,2586,2580,2581,2582,2583,2584,2585)
											and dpe.dpeanoref in ('2012','2013')
											group by tipoveiculo, ano
										union all
											select	case
												when dsh.tidid2 in(1875,1876,1877,2586,2580,2581,2582,2583,2584,2585) then 'escolar rural'
												when dsh.tidid2 in(203,204,205,206,207,208,209,210,212,214,215,216,211,213,3259) then 'urbano acessível'
												end as tipoveiculo,
												0 as metaveiculo,
												0 as totalveiculofnde,
												0 AS totalveiculoproprio,
												SUM(dsh.dshqtde) as totalveiculobndes,
												dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (135)
											and sh.sehstatus <> 'I'
											and dsh.tidid1 in (19) --Financiamento BNDES
											and dsh.tidid2 in (203,204,205,206,207,208,209,210,212,214,215,216,211,213,3259,1875,1876,1877,2586,2580,2581,2582,2583,2584,2585)
											and dpe.dpeanoref in ('2012','2013')
											group by tipoveiculo, ano
										) as foo
										group by tipoveiculo, ano
										order by ano, tipoveiculo";
								$arrMetas = $db->carregar($sql,null,3200);
								foreach($arrMetas as $meta){
									if($meta['ano']=='2012'){
										$totalonibusfnde2012 += $meta['totalveiculofnde'];
										$totalveiculoproprio2012 += $meta['totalveiculoproprio'];
										$totalveiculobndes2012 += $meta['totalveiculobndes'];
										$totalonibus2012 += $meta['totalonibus'];
									}else{
										$totalonibusfnde2013 += $meta['totalveiculofnde'];
										$totalveiculoproprio2013 += $meta['totalveiculoproprio'];
										$totalveiculobndes2013 += $meta['totalveiculobndes'];
										$totalonibus2013 += $meta['totalonibus'];
									}
								}
								if($arrMetas){
									foreach($arrMetas as $meta){
										$arrTipo[$meta['tipoveiculo']][$meta['ano']]['meta'][] = $meta['metaveiculo'];
										$arrTipo[$meta['tipoveiculo']][$meta['ano']]['fnde'][] = $meta['totalveiculofnde'];
										$arrTipo[$meta['tipoveiculo']][$meta['ano']]['proprio'][] = $meta['totalveiculoproprio'];
										$arrTipo[$meta['tipoveiculo']][$meta['ano']]['bndes'][] = $meta['totalveiculobndes'];
										$arrTipo[$meta['tipoveiculo']][$meta['ano']]['total'][] = $meta['totalonibus'];
									}
									$totalonibusgeral = $totalonibus2013 + $totalonibus2012;
								}
								?>
								<tr height="30">
									<td class="" style="background-color:#3B8550" >Ônibus</td>
									<td class="numero" style="background-color:#3B8550" ><i>10.609</i></td>
									<td class="numero" style="background-color:#3B8550" ><?=number_format($totalonibusfnde2012,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550" ><?=number_format($totalveiculoproprio2012,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550" ><?=number_format($totalveiculobndes2012,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550"><?=number_format($totalonibus2012,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550" ><?=number_format($totalonibusfnde2013,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550" ><?=number_format($totalveiculoproprio2013,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550" ><?=number_format($totalveiculobndes2013,0,",",".")?></td>
									<td class="numero" style="background-color:#3B8550"><?=number_format($totalonibus2013,0,",",".")?></td>
									<td class="numero bold" style="background-color:#3B8550"><?=number_format($totalonibusgeral,0,",",".")?></td>
								</tr height="30">
								<?php foreach($arrTipo as $chave => $onb): ?>
									<tr>
										<td class="" style="background-color:#3B8550">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $chave ?></td>
										<td class="numero" style="background-color:#3B8550"><i><?=number_format(array_sum($onb['2012']['meta']),0,",",".")?></i></td>
										<td class="numero" style="background-color:#3B8550"><?=number_format(array_sum($onb['2012']['fnde']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550"><?=number_format(array_sum($onb['2012']['proprio']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550"><?=number_format(array_sum($onb['2012']['bndes']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550"><?=number_format(array_sum($onb['2012']['total']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550" ><?=number_format(array_sum($onb['2013']['fnde']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550" ><?=number_format(array_sum($onb['2013']['proprio']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550" ><?=number_format(array_sum($onb['2013']['bndes']),0,",",".")?></td>
										<td class="numero" style="background-color:#3B8550"><?=number_format(array_sum($onb['2013']['total']),0,",",".")?></td>
										<?php
											$totallombgeral = array_sum($onb['2012']['total']) + array_sum($onb['2013']['total']);
										?>
										<td class="numero bold" style="background-color:#3B8550"><?=number_format($totallombgeral,0,",",".")?></td>
									</tr>
								<?php endforeach; ?>
								<?php
								$sql = "select ano, sum(totalfnde) as totalfnde, sum(totalproprio) as totalproprio, sum(totalbndes) as totalbndes,
										(sum(totalfnde)+sum(totalproprio)+sum(totalbndes)) as totallanchas
										from(
											select SUM(dsh.dshqtde) AS totalfnde,
											0 as totalproprio,
											0 as totalbndes,
											dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (579)
											and sh.sehstatus <> 'I'
											and dsh.tidid2 in (3360,3357) --Recurso FNDE e Doação
											and dpe.dpeanoref in ('2012','2013')
											group by ano
										union all
											select 0 as totalfnde,
											SUM(dsh.dshqtde) AS totalproprio,
											0 as totalbndes,
											dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (579)
											and sh.sehstatus <> 'I'
											and dsh.tidid2 = 3358 --Recurso Próprio
											and dpe.dpeanoref in ('2012','2013')
											group by ano
										union all
											select 0 as totalfnde,
											0 as totalproprio,
											SUM(dsh.dshqtde) AS totalbndes,
											dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (579)
											and sh.sehstatus <> 'I'
											and dsh.tidid2 = 3359 --Financiamento BNDES
											and dpe.dpeanoref in ('2012','2013')
											group by ano
										) as foo
										group by ano
										order by ano";
								$arrLanchas = $db->carregar($sql,null,3200);
								foreach($arrLanchas as $lac){
									if($lac['ano']=='2012'){
										$totalfnde2012 += $lac['totalfnde'];
										$totalproprio2012 += $lac['totalproprio'];
										$totalbndes2012 += $lac['totalbndes'];
										$totallanchas2012 += $lac['totallanchas'];
									}else{
										$totalfnde2013 += $lac['totalfnde'];
										$totalproprio2013 += $lac['totalproprio'];
										$totalbndes2013 += $lac['totalbndes'];
										$totallanchas2013 += $lac['totallanchas'];
									}
								}
								$totallanchasgeral = $totallanchas2012 + $totallanchas2013;
								?>
								<tr  height="30">
									<td class="" style="background-color:#3B8550">Lanchas</td>
									<td class="numero" style="background-color:#3B8550"><i>2.000</i></td>
									<td class="numero" style="background-color:#3B8550"><?php echo number_format($totalfnde2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalproprio2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalbndes2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totallanchas2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalfnde2013,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalproprio2013,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550"><?php echo number_format($totalbndes2013,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totallanchas2013,0,",",".") ?></td>
									<td class="numero bold" style="background-color:#3B8550" ><?=number_format($totallanchasgeral,0,",",".")?></td>
								</tr>
								<?php
								$sql = "select ano, sum(totalfnde) as totalfnde, sum(totalproprio) as totalproprio, sum(totalbndes) as totalbndes,
										(sum(totalfnde)+sum(totalproprio)+sum(totalbndes)) as totalbicicletas
										from(
											select SUM(dsh.dshqtde) AS totalfnde,
											0 as totalproprio,
											0 as totalbndes,
											dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (578)
											and sh.sehstatus <> 'I'
											and dsh.tidid1 in (2007,2467) --Recurso FNDE e Doação
											and dpe.dpeanoref in ('2012','2013')
											group by ano
										union all
											select 0 as totalfnde,
											SUM(dsh.dshqtde) AS totalproprio,
											0 as totalbndes,
											dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (578)
											and sh.sehstatus <> 'I'
											and dsh.tidid1 = 2006 --Recurso Próprio
											and dpe.dpeanoref in ('2012','2013')
											group by ano
										union all
											select 0 as totalfnde,
											0 as totalproprio,
											SUM(dsh.dshqtde) AS totalbndes,
											dpe.dpeanoref as ano
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (578)
											and sh.sehstatus <> 'I'
											and dsh.tidid1 = 3354 --Financiamento BNDES
											and dpe.dpeanoref in ('2012','2013')
											group by ano
										) as foo
										group by ano
										order by ano";
								$arrBicicletas = $db->carregar($sql,null,3200);
								foreach($arrBicicletas as $bic){
									if($bic['ano']=='2012'){
										$totalfnde2012 += $bic['totalfnde'];
										$totalproprio2012 += $bic['totalproprio'];
										$totalbndes2012 += $bic['totalbndes'];
										$totalbicicletas2012 += $bic['totalbicicletas'];
									}else{
										$totalfnde2013 += $bic['totalfnde'];
										$totalproprio2013 += $bic['totalproprio'];
										$totalbndes2013 += $bic['totalbndes'];
										$totalbicicletas2013 += $bic['totalbicicletas'];
									}
								}
								$totalbicicletasgeral = $totalbicicletas2012 + $totalbicicletas2013;
								?>
								<tr  height="30">
									<td class="" style="background-color:#3B8550" >Bicicletas</td>
									<td class="numero" style="background-color:#3B8550" style="background-color:#3B8550" ><i>180.000</i></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalfnde2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalproprio2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalbndes2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalbicicletas2012,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalfnde2013,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalproprio2013,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalbndes2013,0,",",".") ?></td>
									<td class="numero" style="background-color:#3B8550" ><?php echo number_format($totalbicicletas2013,0,",",".") ?></td>
									<td class="numero bold" style="background-color:#3B8550" ><?=number_format($totalbicicletasgeral,0,",",".")?></td>
								</tr>
							</table>
							<table>
								<tr>
									<td>* Execução apurada com base nas adesões ao registro de preços, exceto ônibus com recurso FNDE, apurada por meio de empenhos realizados, e lanchas, por meio de doações realizadas</td>
								</tr>
							</table>
						</td>
					</tr>


	</ul>
                     
	</div>
</body>
</html>


