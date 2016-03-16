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
		jQuery.noConflict();

		jQuery(function(){
			jQuery('.div_fotos_interno').cycle({
				fx: 'scrollDown'
			});

			jQuery('.scrollbar').perfectScrollbar({wheelPropagation:true});

			jQuery('.lista-noticias').totemticker({
				row_height  :   '100px',
				next        :   '#ticker-next',
				previous    :   '#ticker-previous',
				stop        :   '#stop',
				start       :   '#start',
				mousestop   :   true,
			});
		});
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/cockpit/fundo_caminhos_da_escola.jpg')}
        .div_fotos_padrao2{background-color:#152D56;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:400px;margin-bottom:2px}
	</style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_caminhos_escola&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Caminho da Escola</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td class="fundo_padrao" colspan=2>
			<div>
				<?php exibirTitulo('alvo', 'Metas e Execução', '<span class="link" onclick="abreIndicadorPopUp(1865);">Ônibus</span>, <span class="link" onclick="abreIndicadorPopUp(579);">Lanchas</span>, <span class="link" onclick="abreIndicadorPopUp(578);">Bicicletas</span>'); ?>
			</div>
			<?php
			$sql = "SELECT tipo, tipoveiculo, SUM(meta) AS meta, formaaquisicao, SUM(totalmunicipio) AS totalmunicipio, SUM(totalveiculo) AS totalveiculo, COALESCE(SUM(totalvalor),0) AS totalvalor
					FROM (
						SELECT
							1 AS ordem,
							'Ônibus'::VARCHAR AS tipo,
							tid1.tiddsc AS tipoveiculo,
							CASE
								WHEN dsh.tidid1 IN (4299) THEN 8000
								WHEN dsh.tidid1 IN (4300) THEN 2609
							END AS meta,
							CASE
								WHEN dsh.tidid2 IN (4301) THEN 'Recurso FNDE'
								WHEN dsh.tidid2 IN (4303) THEN 'Recurso Próprio'
								WHEN dsh.tidid2 IN (4302) THEN 'Recurso BNDES'
							END AS formaaquisicao,
							COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio,
							SUM(dsh.dshqtde) AS totalveiculo,
							SUM(dsh.dshvalor) AS totalvalor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
						WHERE sh.indid in (1865)
						AND sh.sehstatus <> 'I'
						--AND dpe.dpeanoref IN ('2011', '2012','2013')
						GROUP BY tipoveiculo, meta, formaaquisicao
					UNION ALL
						SELECT
							2 AS ordem,
							'Lanchas'::VARCHAR AS tipo,
							'Lanchas' AS tipoveiculo,
							2000 AS meta,
							CASE
								WHEN dsh.tidid1 IN (7609, 7610, 7606) THEN 'Recurso FNDE'
								WHEN dsh.tidid1 IN (7607) THEN 'Recurso Próprio'
								WHEN dsh.tidid1 IN (7608) THEN 'Recurso BNDES'
							END AS formaaquisicao,
							COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio,
							SUM(dsh.dshqtde) AS totalveiculo,
							SUM(dsh.dshvalor) AS totalvalor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
						WHERE sh.indid in (2826)
						AND sh.sehstatus <> 'I'
						--AND dpe.dpeanoref IN ('2010', '2011', '2012','2013')
						GROUP BY tipoveiculo, meta, formaaquisicao
					UNION ALL
						SELECT
							3 AS ordem,
							'Bicicletas'::VARCHAR AS tipo,
							'Bicicletas' AS tipoveiculo,
							180000 AS meta,
							CASE
								WHEN dsh.tidid1 IN (7873) THEN 'Recurso FNDE'
								WHEN dsh.tidid1 IN (7874) THEN 'Recurso Próprio'
								WHEN dsh.tidid1 IN (7872) THEN 'Recurso BNDES'
							END AS formaaquisicao,
							COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio,
							SUM(dsh.dshqtde) AS totalveiculo,
							SUM(dsh.dshvalor) AS totalvalor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
						WHERE sh.indid in (2888)
						AND sh.sehstatus <> 'I'
						--AND dpe.dpeanoref IN ('2011', '2012','2013')
						GROUP BY tipoveiculo, meta, formaaquisicao
					) AS FOO
					GROUP BY ordem, tipo, tipoveiculo, formaaquisicao
					ORDER BY ordem, tipo, tipoveiculo, formaaquisicao";
			$arrDados = $db->carregar($sql,null,3200);
			if($arrDados){
				foreach($arrDados as $dado){
					$arrTipo[$dado['tipoveiculo']][$dado['formaaquisicao']]['meta'][] = $dado['meta'];
					$arrTipo[$dado['tipoveiculo']][$dado['formaaquisicao']]['totalmunicipio'][] = $dado['totalmunicipio'];
					$arrTipo[$dado['tipoveiculo']][$dado['formaaquisicao']]['totalveiculo'][] = $dado['totalveiculo'];
					$arrTipo[$dado['tipoveiculo']][$dado['formaaquisicao']]['totalvalor'][] = $dado['totalvalor'];
				}
				foreach($arrTipo as $chave => $veiculo){
					if($chave!='Bicicletas' && $chave!='Lanchas'){
						$somaMeta += array_sum($veiculo['Recurso FNDE']['meta']);
						$somaMunicipioFNDE = 0;
						$somaVeiculoFNDE += array_sum($veiculo['Recurso FNDE']['totalveiculo']);
						$somaValorFNDE += array_sum($veiculo['Recurso FNDE']['totalvalor']);
						$somaMunicipioPROPRIO = 0;
						$somaVeiculoPROPRIO += array_sum($veiculo['Recurso Próprio']['totalveiculo']);
						$somaValorPROPRIO += array_sum($veiculo['Recurso Próprio']['totalvalor']);
						$somaMunicipioBNDES = 0;
						$somaVeiculoBNDES += array_sum($veiculo['Recurso BNDES']['totalveiculo']);
						$somaValorBNDES += array_sum($veiculo['Recurso BNDES']['totalvalor']);
					}
				}
				$somaVeiculoOnibus = $somaVeiculoFNDE + $somaVeiculoPROPRIO + $somaVeiculoBNDES;
				$somaValorOnibus = $somaValorFNDE + $somaValorPROPRIO + $somaValorBNDES;
			}
			$sql = "SELECT
						CASE
							WHEN dsh.tidid2 IN (4301) THEN 'Recurso FNDE'
							WHEN dsh.tidid2 IN (4303) THEN 'Recurso Próprio'
							WHEN dsh.tidid2 IN (4302) THEN 'Recurso BNDES'
						END AS formaaquisicao,
						COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
					WHERE sh.indid in (1865)
					AND sh.sehstatus <> 'I'
					--AND dpe.dpeanoref IN ('2011', '2012','2013')
					GROUP BY formaaquisicao";
			$arrDados = $db->carregar($sql,null,3200);
			if($arrDados){
				foreach($arrDados as $dado){
					$arrMun[$dado['formaaquisicao']]['totalmunicipio'] = $dado['totalmunicipio'];
				}
				$somaMunicipioFNDE = $arrMun['Recurso FNDE']['totalmunicipio'];
				$somaMunicipioPROPRIO = $arrMun['Recurso Próprio']['totalmunicipio'];
				$somaMunicipioBNDES = $arrMun['Recurso BNDES']['totalmunicipio'];
			}
			//SOMA MUNICÍPIOS DISTINTOS
			$sql = "SELECT
					COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
					WHERE sh.indid in (1865)
					AND sh.sehstatus <> 'I'
					--AND dpe.dpeanoref IN ('2011', '2012','2013')";
			$somaMunicipioOnibus = $db->pegaUm($sql,null,3200);
			
			$sql = "SELECT tipoveiculo, SUM(totalmunicipio) AS totalmunicipio
					FROM (
						SELECT
							tid1.tiddsc AS tipoveiculo,
							COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
						WHERE sh.indid in (1865)
						AND sh.sehstatus <> 'I'
						--AND dpe.dpeanoref IN ('2011', '2012','2013')
						GROUP BY tipoveiculo
					UNION ALL
						SELECT
							'Lanchas' AS tipoveiculo,
							COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
						WHERE sh.indid in (2826)
						AND sh.sehstatus <> 'I'
						--AND dpe.dpeanoref IN ('2010', '2011', '2012','2013')
						GROUP BY tipoveiculo
					UNION ALL
						SELECT
							'Bicicletas' AS tipoveiculo,
							COUNT(DISTINCT dshcodmunicipio) AS totalmunicipio
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON sh.dpeid = dpe.dpeid
						WHERE sh.indid in (2888)
						AND sh.sehstatus <> 'I'
						--AND dpe.dpeanoref IN ('2011', '2012','2013')
						GROUP BY tipoveiculo
					) AS FOO
					GROUP BY tipoveiculo
					ORDER BY tipoveiculo";
			$arrDados = $db->carregar($sql,null,3200);
			if($arrDados){
				foreach($arrDados as $dado){
					$arrMunDist[$dado['tipoveiculo']]['totalmunicipio'] = $dado['totalmunicipio'];
				}
				$somaMunicipioBicicletas = $arrMunDist['Bicicletas']['totalmunicipio'];
				$somaMunicipioRural = $arrMunDist['Escolar rural']['totalmunicipio'];
				$somaMunicipioUrbano = $arrMunDist['Urbano acessível']['totalmunicipio'];
				$somaMunicipioLanchas = $arrMunDist['Lanchas']['totalmunicipio'];
			}
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
				<tr>
                	<th class="center bold" rowspan="2">Iniciativa</th>
                	<th class="center bold" rowspan="2"><i>Meta até 2014</i></th>
                	<th class="center bold" colspan="3">Recurso FNDE</th>
                	<th class="center bold" colspan="3">Recurso Próprio</th>
                	<th class="center bold" colspan="3">Recurso BNDES</th>
                	<th class="center bold" colspan="3">Total</th>
				</tr>
				<tr>
                	<th class="center bold">Municípios</th>
                	<th class="center bold">Veículos</th>
                	<th class="center bold">Valor (R$)</th>
					<th class="center bold">Municípios</th>
                	<th class="center bold">Veículos</th>
                	<th class="center bold">Valor (R$)</th>
					<th class="center bold">Municípios</th>
                	<th class="center bold">Veículos</th>
                	<th class="center bold">Valor (R$)</th>
					<th class="center bold">Municípios</th>
                	<th class="center bold">Veículos</th>
                	<th class="center bold">Valor (R$)</th>
				</tr>
				<tr class="zebrado">
					<td class="">Ônibus</td>
					<td class="numero" ><i><?=number_format($somaMeta,0,",",".")?></i></td>
					<td class="numero" ><?=number_format($somaMunicipioFNDE,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaVeiculoFNDE,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaValorFNDE,2,",",".")?></td>
					<td class="numero" ><?=number_format($somaMunicipioPROPRIO,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaVeiculoPROPRIO,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaValorPROPRIO,2,",",".")?></td>
					<td class="numero" ><?=number_format($somaMunicipioBNDES,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaVeiculoBNDES,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaValorBNDES,2,",",".")?></td>
					<td class="numero" ><?=number_format($somaMunicipioOnibus,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaVeiculoOnibus,0,",",".")?></td>
					<td class="numero" ><?=number_format($somaValorOnibus,2,",",".")?></td>
				</tr>
				<?php
				$count = -1;
				foreach($arrTipo as $chave => $veiculo):
					$count++;
					?>
					<tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
						<?php if($chave!='Bicicletas' && $chave!='Lanchas'){ ?>
						<td class="" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $chave ?></td>
						<?php }elseif($chave=='Lanchas'){ ?>
						<td class=""><?php echo $chave ?></td>
						<?php }elseif($chave=='Bicicletas'){ ?>
						<td class=""><?php echo $chave ?></td>						
						<?php } ?>
						<td class="numero" ><i><?=(is_array($veiculo['Recurso FNDE']['meta'])?number_format(array_sum($veiculo['Recurso FNDE']['meta']),0,",","."):0)?></i></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso FNDE']['totalmunicipio'])?number_format(array_sum($veiculo['Recurso FNDE']['totalmunicipio']),0,",","."):0)?></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso FNDE']['totalveiculo'])?number_format(array_sum($veiculo['Recurso FNDE']['totalveiculo']),0,",","."):0)?></td>
						<?php if($chave=='Lanchas'){ ?>
							<td class="center" >Doação Marinha</td>
						<?php }else{ ?>
							<td class="numero" ><?=(is_array($veiculo['Recurso FNDE']['totalvalor'])?number_format(array_sum($veiculo['Recurso FNDE']['totalvalor']),2,",","."):0)?></td>
						<?php } ?>					
						<td class="numero" ><?=(is_array($veiculo['Recurso Próprio']['totalmunicipio'])?number_format(array_sum($veiculo['Recurso Próprio']['totalmunicipio']),0,",","."):"-")?></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso Próprio']['totalveiculo'])?number_format(array_sum($veiculo['Recurso Próprio']['totalveiculo']),0,",","."):"-")?></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso Próprio']['totalvalor'])?number_format(array_sum($veiculo['Recurso Próprio']['totalvalor']),2,",","."):"-")?></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso BNDES']['totalmunicipio'])?number_format(array_sum($veiculo['Recurso BNDES']['totalmunicipio']),0,",","."):"-")?></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso BNDES']['totalveiculo'])?number_format(array_sum($veiculo['Recurso BNDES']['totalveiculo']),0,",","."):"-")?></td>
						<td class="numero" ><?=(is_array($veiculo['Recurso BNDES']['totalvalor'])?number_format(array_sum($veiculo['Recurso BNDES']['totalvalor']),2,",","."):"-")?></td>
						<?php if($chave=='Escolar rural'){ ?>
							<td class="numero" ><?=number_format($somaMunicipioRural,0,",",".") ?></td>
						<?php }elseif($chave=='Urbano acessível'){ ?>
							<td class="numero" ><?=number_format($somaMunicipioUrbano,0,",",".") ?></td>
						<?php }elseif($chave=='Lanchas'){ ?>
							<td class="numero" ><?=number_format($somaMunicipioLanchas,0,",",".") ?></td>
						<?php }elseif($chave=='Bicicletas'){ ?>
							<td class="numero" ><?=number_format($somaMunicipioBicicletas,0,",",".") ?></td>
						<?php } ?>
						<td class="numero" ><?=number_format((is_array($veiculo['Recurso FNDE']['totalveiculo'])?array_sum($veiculo['Recurso FNDE']['totalveiculo']):0) + (is_array($veiculo['Recurso Próprio']['totalveiculo'])?array_sum($veiculo['Recurso Próprio']['totalveiculo']):0) + (is_array($veiculo['Recurso BNDES']['totalveiculo'])?array_sum($veiculo['Recurso BNDES']['totalveiculo']):0),0,",",".")?></td>
						<td class="numero" ><?=number_format((is_array($veiculo['Recurso FNDE']['totalvalor'])?array_sum($veiculo['Recurso FNDE']['totalvalor']):0) + (is_array($veiculo['Recurso Próprio']['totalvalor'])?array_sum($veiculo['Recurso Próprio']['totalvalor']):0) + (is_array($veiculo['Recurso BNDES']['totalvalor'])?array_sum($veiculo['Recurso BNDES']['totalvalor']):0),2,",",".") ?></td>
					</tr>
				<?php endforeach; ?>
			</table>
			<table>
				<tr>
					<td>* Execução apurada com base nas adesões ao registro de preços, exceto ônibus com recurso FNDE, apurada por meio de empenhos realizados, e lanchas, por meio de doações realizadas</td>
				</tr>
			</table>
		</td>
        <td class="fundo_azul_padrao" rowspan="2" >
            <div>
                <img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                <div style="float:left" class="titulo_box" >Ônibus escolares<br/></div>
            </div>
            <?php
            $sql = "select tid.tidid, tid.tiddsc, SUM(dsh.dshqtde) AS total, SUM(dsh.dshvalor) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
					inner join painel.detalhetipoindicador tdi on tid.tdiid = tdi.tdiid
					where i.indid in (135)
					and sehstatus <> 'I'
					and tid.tidid in (2580,2581,2582,2583,2584,2585,2586)
					group by tid.tidid, tid.tiddsc
					order by tid.tiddsc";
            $arrFotos = $db->carregar($sql,null,3200);
            ?>
            <div style="clear:both" id="div_fotos_padrao2" onclick="window.open('http://www.fnde.gov.br/portaldecompras/index.php/produtos/onibus-escolar-rural','FNDE','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');"  >
                <?php for($x=4;$x>=1;$x--): ?>
                    <div style="clear:both" class="div_fotos_interno" >
                        <?php for($i=$x;$i<=(2+$x);$i++): ?>
                            <div class="div_fotos_padrao2" >
                                <table border="0" >
                                    <tr>
                                        <td style="color:#FFFFFF" >
                                            <img src="../imagens/cockpit/onibus/<?php echo $arrFotos[$i]['tidid'] ?>.jpg" width="130px" height="80px"  />
                                        </td>
                                        <td style="color:#FFFFFF" >
                                            <b><?php echo substr($arrFotos[$i]['tiddsc'],0,60) ?></b><br />
                                            <b>Quantidade:</b> <?php echo  number_format($arrFotos[$i]['total'],0,",",".") ?><br/>
                                            <b>Valor:</b> <?php echo  number_format($arrFotos[$i]['valor'],0,",",".") ?>
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
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('doc', 'Veículos adquiridos (2008-2014)', '*Recursos do FNDE e Doação'); ?>
            </div>
            <?php
            $sql = "SELECT dpeanoref, SUM(TotalOnibus) AS TotalOnibus, SUM(TotalLanchas) AS TotalLanchas, SUM(TotalBicicletas) AS TotalBicicletas FROM (
						select dpe.dpeanoref, SUM(dsh.dshqtde) AS TotalOnibus, 0 as TotalLanchas, 0 as TotalBicicletas
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (1865)
						and sehstatus <> 'I'
						and dsh.tidid2 IN (4301)
						group by dpe.dpeanoref
					UNION ALL
						select dpe.dpeanoref, 0 AS TotalOnibus, SUM(dsh.dshqtde) AS TotalLanchas, 0 as TotalBicicletas
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (2826)
						and sehstatus <> 'I'
						and dsh.tidid1 IN (7609,7606,7610)
						group by dpe.dpeanoref
					UNION ALL
						select dpe.dpeanoref, 0 AS TotalOnibus, 0 as TotalLanchas, SUM(dsh.dshqtde) AS TotalBicicletas
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (2888)
						and sehstatus <> 'I'
						and dsh.tidid1 IN (7873)
						group by dpe.dpeanoref
					) AS FOO
					GROUP BY dpeanoref
					ORDER BY dpeanoref";
            $arrDados = $db->carregar($sql,null,3200);
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Ano</th>
                    <th class="center bold" style="cursor:pointer;" onclick="abreIndicadorPopUp(1865);">Ônibus</th>
                    <th class="center bold" style="cursor:pointer;" onclick="abreIndicadorPopUp(2826);">Lanchas</th>
                    <th class="center bold" style="cursor:pointer;" onclick="abreIndicadorPopUp(2888);">Bicicletas</th>
                </tr>
                <?php
                $count = -1;
                foreach($arrDados as $dado):
                    $count++;
                    ?>
                    <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                        <td class="center" ><?php echo $dado['dpeanoref'] ?></td>
                        <td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(1865);"><?php echo number_format($dado['totalonibus'],0,",",".") ?></td>
                        <td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(2826);"><?php echo number_format($dado['totallanchas'],0,",",".") ?></td>
                        <td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(2888);"><?php echo number_format($dado['totalbicicletas'],0,",",".") ?></td>
                    </tr>
                    <?php
                    $totalonibus = $totalonibus + $dado['totalonibus'];
                    $totallanchas = $totallanchas + $dado['totallanchas'];
                    $totalbicicletas = $totalbicicletas + $dado['totalbicicletas'];
                endforeach; ?>
                <tr>
                    <th class="center bold" >Total</th>
                    <th class="numero bold" style="cursor:pointer;" onclick="abreIndicadorPopUp(1865);"><?=number_format($totalonibus,0,"",".") ?></th>
                    <th class="numero bold" style="cursor:pointer;" onclick="abreIndicadorPopUp(2826);"><?=number_format($totallanchas,0,"",".") ?></th>
                    <th class="numero bold" style="cursor:pointer;" onclick="abreIndicadorPopUp(2888);"><?=number_format($totalbicicletas,0,"",".") ?></th>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2962);">
            <div>
                <?php exibirTitulo('doc', 'Viver sem limite -<br>Ônibus adquiridos (2012-2014)'); ?>
            </div>
            <?php
            $sql = "select dpe.dpeanoref, SUM(dsh.dshqtde) AS TotalOnibus
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
                    where i.indid in (2962)
                    and sehstatus <> 'I'
                    group by dpe.dpeanoref
                    order by dpe.dpeanoref";
            $arrDados = $db->carregar($sql,null,3200);
            $totalonibus = 0;
            if($arrDados){
            ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th>Ano</th>
                        <th>Ônibus</th>
                    </tr>
                    <?php
                    $count = -1;
                    foreach($arrDados as $dado):
                        $count++;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td class="center" ><?php echo $dado['dpeanoref'] ?></td>
                            <td class="numero"><?php echo number_format($dado['totalonibus'],0,",",".") ?></td>
                        </tr>
                        <?php
                        $totalonibus = $totalonibus + $dado['totalonibus'];
                    endforeach; ?>
                    <tr>
                        <th class="center bold">Total</th>
                        <th class="numero bold"><?=number_format($totalonibus,0,"",".") ?></th>
                    </tr>
                </table>
            <?php
            }
            ?>
        </td>
    </tr>
	<tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1865);" width="33%">
            <div>
                <?php exibirTitulo('mapas', 'Mapa de ônibus', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica where indid = 1865 and sehstatus != 'I';";
            $arrValores = $db->pegaLinha($sql);
            $sql = "select
                        count(distinct dshcodmunicipio)
                    from
                        painel.seriehistorica seh
                    inner join
                        painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    where
                        indid = 1865
                    and
                        seh.sehstatus != 'I';";
            $totalMun = $db->pegaUm($sql);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >ônibus</span>
                        <br /><span style="font-weight:bold;font-size:14px" >R$ <?php echo number_format($arrValores['valor'],2,",",".") ?> </span><span class="subtitulo_box" >reais</span>
                        <br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
                    </td>
                    <td class="link" valign="bottom" onclick="abreMapaMaisMedicos();">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2888);" width="34%">
            <div>
                <?php exibirTitulo('mapas', 'Mapa de bicicletas', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica where indid = 2888 and sehstatus != 'I';";
            $arrValores = $db->pegaLinha($sql);
            $sql = "select
                        count(distinct dshcodmunicipio)
                    from
                        painel.seriehistorica seh
                    inner join
                        painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    where
                        indid = 2888
                    and
                        seh.sehstatus != 'I';";
            $totalMun = $db->pegaUm($sql);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="center">
                <tr>
                    <td>
                        <br><br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >bicicletas</span>
                        <br /><span style="font-weight:bold;font-size:14px" >R$ <?php echo number_format($arrValores['valor'],2,",",".") ?> </span><span class="subtitulo_box" >reais</span>
                        <br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
                    </td>
                    <td class="link" valign="bottom" onclick="abreMapaMaisMedicos();">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2826);" width="33%">
            <div>
                <?php exibirTitulo('mapas', 'Mapa de lanchas', 'Clique no mapa para visualizar os detalhes.'); ?>
            </div>
            <?php
            $sql = "select sum(sehqtde) as qtde,sum(sehvalor) as valor from painel.seriehistorica where indid = 2826 and sehstatus != 'I';";
            $arrValores = $db->pegaLinha($sql);
            $sql = "select
                        count(distinct dshcodmunicipio)
                    from
                        painel.seriehistorica seh
                    inner join
                        painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
                    where
                        indid = 2826
                    and
                        seh.sehstatus != 'I';";
            $totalMun = $db->pegaUm($sql);
            ?>
            <table border=0 cellspacing="4" cellpadding="5" align="left">
                <tr>
                    <td>
                        <br><br><br><br><br><br><br>
                        <span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?>  </span><span class="subtitulo_box" >lanchas</span>
                        <br /><span style="font-weight:bold;font-size:14px" >R$ <?php echo number_format($arrValores['valor'],2,",",".") ?> </span><span class="subtitulo_box" >reais</span>
                        <br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($totalMun,0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
                    </td>
                    <td class="link" valign="bottom" onclick="abreMapaMaisMedicos();">
                        <img src="../imagens/cockpit/mapa_cockpit.png"  />
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1865);">
            <div>
                <?php exibirTitulo('indicador', 'Quantidade de ônibus adquiridos'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS rede,
                        'Ônibus' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (1865)
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
                        'tipo'=>'Ônibus Acumulados',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaOnibusAcumulado', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2888);">
            <div>
                <?php exibirTitulo('indicador', 'Quantidade de bicicletas adquiridas'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS rede,
                        'Bicicletas' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2888)
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
                        'tipo'=>'Bicicletas Acumuladas',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaBicicletasAcumulado', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2826);">
            <div>
                <?php exibirTitulo('indicador', 'Quantidade de lanchas adquiridas'); ?>
            </div>
            <?php
            $sql = "SELECT
                        dpeanoref AS rede,
                        'Lanchas' AS tipo,
                        SUM(dsh.dshqtde)::integer AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2826)
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
                        'tipo'=>'Lanchas Acumuladas',
                        'valor'=>$valorAcumulado,
                    );
                }
            }

            $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaLanchasAcumulado', null, null, $formatoValores, null, "", 1000, 600, null, 200, false);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(17);?>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',1,17,'','');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="3">
		    <div style="text-align:center;"  >
		    <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
		    <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
		    <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
		    </div>
	    </td>
	</tr>
</table>
</body>
</html>