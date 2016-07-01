<script language="javascript" type="text/javascript" src="../includes/open_flash_chart/swfobject.js"></script>
<script>
	swfobject.embedSWF("../includes/open_flash_chart/open-flash-chart.swf", "grafico", "600px", "400px", "9.0.0", "expressInstall.swf", {"data-file":"../../cte/monitoraFinanceiro/geraGrafico.php?tipo=linha;<?php echo $_REQUEST['prsid']?>;<?php echo tratarDataMesAno($_REQUEST['hmcdatamonitoramentoinicio']) ?>;<?php echo tratarDataMesAno($_REQUEST['hmcdatamonitoramentofim']) ?>", "loading":"Carregando gráfico..."} );
</script>
<?php 

function tratarDataMesAno( $data ){
	if( !$data ) return '';
	return substr($data, 3, 4).substr($data, 0, 2);
}

$dados = array();
$prsid 	= $_REQUEST['prsid'];

if( isset($_REQUEST['hmcdatamonitoramentoinicio']) ){
	
	$stWhere = $_REQUEST['hmcdatamonitoramentoinicio'] ? " AND to_char(hmcdatamonitoramento, 'YYYYMM') >= '" . tratarDataMesAno( $_REQUEST['hmcdatamonitoramentoinicio'] ) . "'" : '';
	$stWhere .= $_REQUEST['hmcdatamonitoramentofim'] ? " AND to_char(hmcdatamonitoramento, 'YYYYMM') <= '" . tratarDataMesAno( $_REQUEST['hmcdatamonitoramentofim'] ) . "'" : '';
	
	$sql = "SELECT  p.prsvalorconvenio AS total,
			to_char(hmcdatamonitoramento, 'MM') as mes,
			to_char(hmcdatamonitoramento, 'YYYY')  as ano,
			hmc.hmcid,
			coalesce( ( SELECT 
				sum(hici.hmsvalortotalpago) AS gasto
				FROM cte.historicomonitoramentoconvenio hmci 
				INNER JOIN cte.historicomonitoramentoconvsubac hmsi ON hmci.hmcid = hmsi.hmcid
				INNER JOIN cte.historicoconvitemcomposicao hici ON hici.hmsid = hmsi.hmsid
				INNER JOIN cte.projetosape pi ON pi.prsid = hmci.prsid
				WHERE hmci.prsid = ".$prsid."  and hmci.hmcid = hmc.hmcid
				AND hici.scsid in (1,2)
				GROUP BY hmci.hmcid, to_char(hmcdatamonitoramento, 'MM'),to_char(hmcdatamonitoramento, 'YYYY')
			) , 0 ) AS gasto
		FROM cte.historicomonitoramentoconvenio hmc 
		INNER JOIN cte.historicomonitoramentoconvsubac hms ON hmc.hmcid = hms.hmcid
		INNER JOIN cte.historicoconvitemcomposicao hic ON hic.hmsid = hic.hmsid
		INNER JOIN cte.projetosape p ON p.prsid = hmc.prsid
		WHERE hmc.prsid = ".$prsid."
		AND hic.scsid in (1,2)
		$stWhere
		GROUP BY hmc.hmcid, p.prsvalorconvenio, mes, ano
		ORDER BY ano,mes";

	$dados 	= $db->carregar($sql);
	$soma = 0;
	if(is_array($dados)){
		foreach($dados as $dado){
			$arrMes[] = $dado['mes']."/".$dado['ano'];
			$arrGasto[] = round($dado['gasto']);
			$arrAcum[] = $soma;
		}
	}
	else{
	?>
	<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
			<tr>
				<td class="SubTituloCentro tdDegradde01" >Não existem meses monitorados até o momento.</td>
			</tr>
		</table>
	</body>
	</html>
	<?php 
	die();
	}
} 

?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript">
		function pesquisarRelatorio(){
			document.getElementById('formulario').submit();
		}
	</script>
</head>
<body>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
		<tr>
			<td colspan="2" class="SubTituloCentro tdDegradde01" >Filtro</td>
		</tr>
		<tr>
			<td colspan="2" >
				<form method="post" name="formulario" id="formulario">
					<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="width: 100%;">
						<tr>
							<td class="SubTituloDireita" valign="top" width="20%">
								Período (MM/YYYY):
							</td>
							<td width="80%">
								<label for="hmcdatamonitoramentoinicio">de </label><?php echo campo_texto('hmcdatamonitoramentoinicio', "N", "S", '', 10, 25 , '##/####', '', 'left', '', 0, 'id="hmcdatamonitoramentoinicio"', null, $_REQUEST['hmcdatamonitoramentoinicio']);?>
								<label for="hmcdatamonitoramentofim">até </label><?php echo campo_texto('hmcdatamonitoramentofim', "N", "S", '', 10, 25 , '##/####', '', 'left', '', 0, 'id="hmcdatamonitoramentofim"', null, $_REQUEST['hmcdatamonitoramentofim']);?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" valign="top" width="20%">
								Tipo de Relatório:
							</td>
							<td width="80%">
								<?php
									$arDados[0]['codigo'] = 'completo';	 
									$arDados[0]['descricao'] = 'Completo';
									$arDados[1]['codigo'] = 'grafico';	 
									$arDados[1]['descricao'] = 'Somente Gráfico';
									$arDados[2]['codigo'] = 'dados';	 
									$arDados[2]['descricao'] = 'Somente Dados';
										 
									echo $db->monta_combo( 'tipoRelatorio', $arDados, 'S', '', '', '', '', '', '', '', '', $_REQUEST['tiporelatorio'] ); 
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2" class="SubTituloDireita" style="text-align:left;">
								<input type="button" value="Pesquisar" onclick="pesquisarRelatorio();" />
							</td>
						</tr>
					</table>
				</form>
			</td>
		</tr>
		<?php if( isset( $_REQUEST['tiporelatorio'] ) ){ ?>
			<tr>
				<td colspan="2" class="SubTituloCentro tdDegradde01" >Relatório Geral do Monitoramento </td>
			</tr>
			<tr>
				<td width="120px" class="SubTituloDireita" >Total do Convênio: </td>
				<td class="SubTituloEsquerda" >R$<?php echo number_format($dados[0]['total'], 2, ',', '.'); ?></td>
			</tr>
			<?php if( $_REQUEST['tiporelatorio'] == 'completo' || $_REQUEST['tiporelatorio'] == 'grafico' ){ ?>
				<tr>
					<td colspan="2"  class="SubTituloCentro"  >
						<div id="grafico"></div>
					</td>
				</tr>
			<?php } ?>
			<?php if( $_REQUEST['tiporelatorio'] == 'completo' || $_REQUEST['tiporelatorio'] == 'dados' ){ ?>
				<tr>
					<td width="120px" class="SubTituloDireita tdDegradde02" style="font-weight: bold;" >Mês/Ano</td>
					<td class="SubTituloEsquerda tdDegradde02" >Valor executado no mês</td>
				</tr>
				<?php 
				foreach($dados as $dado){ 
					$sql =" SELECT 
							to_char(hmcdatamonitoramento, 'MM')  || '/' || to_char(hmcdatamonitoramento, 'YYYY')  as data,
							sb.sbadsc,
							cs.cosdsc,
							sb.sbaid,
							hmsi.hmsid,
							CASE WHEN hici.hmsvalortotalempenhado::character varying = '0.00' THEN 'Executado anteriormente.' 
								ELSE hici.hmsvalortotalempenhado::character varying END AS valor,
							scs.scsdesc
							FROM cte.historicomonitoramentoconvenio hmci 
							INNER JOIN cte.historicomonitoramentoconvsubac hmsi ON hmci.hmcid = hmsi.hmcid
							INNER JOIN cte.historicoconvitemcomposicao hici ON hici.hmsid = hmsi.hmsid
							INNER JOIN cte.composicaosubacao cs ON cs.cosid = hici.cosid
							INNER JOIN cte.subacaoindicador sb ON sb.sbaid = hmsi.sbaid and cs.sbaid = sb.sbaid  
							INNER JOIN cte.projetosape pi ON pi.prsid = hmci.prsid
							INNER JOIN cte.statuscomposicaosubacao scs ON scs.scsid = hici.scsid
							WHERE hmci.prsid = ".$prsid."  and hmci.hmcid = ".$dado['hmcid']."
							AND hici.scsid in (1,2) ORDER BY sbaid";
					$itens 	= $db->carregar($sql);
				?>
				<tr>
					<td width="120px" class="SubTituloDireita tdDegradde02" ><?php echo $dado['mes']."/".$dado['ano']; ?> </td>
					<td class="SubTituloEsquerda tdDegradde02" >R$<?php echo number_format($dado['gasto'], 2, ',', '.'); ?></td>
				</tr>
				<?php  if(is_array($itens)){ ?>
				<tr>
					<td colspan="2"  class="SubTituloCentro" > 
						<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="95%">
							<tr>
								<td colspan="4" class="SubTituloCentro tdDegradde04" >Itens Em Execução ou Executados no mês </td>
							</tr>
							<tr>
								<td width="300px" class="SubTituloDireita tdDegradde04" style="font-weight:bold;" >Subação </td>
								<td class="SubTituloEsquerda tdDegradde04" >Item</td>
								<td class="SubTituloEsquerda tdDegradde04" >Status</td>
								<td class="SubTituloEsquerda tdDegradde04" >Executado no mês</td>
							</tr>
							<?php  
								foreach($itens as $item){ 
									if($item['valor'] == "Executado anteriormente."){
										$resul = $item['valor'];
									}else{
										$resul = "R$".number_format($item['valor'], 2, ',', '.');
									}
							?>
								<tr>
									<td width="120px" class="SubTituloDireita" > <?php echo $item['sbadsc']; ?> </td>
									<td class="SubTituloEsquerda" style="font-weight:normal;"  ><?php echo $item['cosdsc']; ?></td>
									<td class="SubTituloEsquerda" style="font-weight:normal;"  ><?php echo $item['scsdesc']; ?></td>
									<td class="SubTituloEsquerda" style="font-weight:normal;"  ><?php echo $resul; ?></td>
								</tr>
							<?php } ?>
						</table>
					</td>
				</tr>
				<?php }else{ ?>
					<tr>
					<td colspan="2" class="SubTituloCentro " >Nenhum item foi monitorado neste mês </td>
				</tr>
				
				<?php } } ?>
			<?php } ?>
			<tr>
				<td colspan="2" style="text-align: center;">
					<input type="button" value="imprimir" onClick="javascript:window.print()" id="botao" />
				</td>
			</tr>
		<?php } ?>
	</table>
</body>
</html>

				