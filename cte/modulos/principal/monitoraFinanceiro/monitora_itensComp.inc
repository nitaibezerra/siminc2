<?php
/************************* INCLUDES *************************************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'cte/classes/HistoricoConvItemComposicao.class.inc');
include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');

/************************* DECLARAÇÃO DE VARIAVEIS *********************************/
$scsid				 	= $_REQUEST['scsid'];
$statusItem				= $_REQUEST['statustexto'];
$sbaid 					= $_REQUEST['sbaid'];
$hmsid					= $_REQUEST['hmsid'];
$cosid					= $_REQUEST['cosid'];
$hieid					= $_REQUEST['hieid'];
$quantidade				= $_REQUEST['quant'];
$valor					= $_REQUEST['valor'];
$quantEmpenhada 		= $_REQUEST['hmsquantidadeempenhada'];
$quantliquidada 		= $_REQUEST['hmsquantidadeliquidada'];
$quantPaga				= $_REQUEST['hmsquantidadepaga'];
$valorUnitario 			= $_REQUEST['hmsvalorunitario'];
$micid					= $_REQUEST['micid'];
$ano		 			= $_SESSION['ano'];
$totqtdexecutado		= $_REQUEST['totqtdexecutado'];
$totvalorexecutado		= $_REQUEST['totvalorexecutado'];

//dbg($cosid);
$quantidade				= number_format($quantidade,0,'.','');

if($scsid == NAOINICIADAITEMCOMP){
	$hmsobs	= $_REQUEST['hmsobsi'];
}else{
	$hmsobs	= $_REQUEST['hmsobs'];
}

/************************* INSTANCIA CLASSES E OBJETOS ******************************/
$obdata 			= new Data();
$obHistItem			= new HistoricoConvItemComposicao();
$obDadosSubAcao 	= new HistoricoMonitoramentoConvSubac($hmsid);
$anoSubacoes		= $obDadosSubAcao->hmsano;
$hmcidSubacoes		= $obDadosSubAcao->hmcid;
/************************* CARREGA DADOS PARA MOSTRAR NA TELA E CARREGA COMBOS *****/
if( $_REQUEST['hciid'] ){// carrega dados do item de composição caso exista um monitoramento
	$obHistItem->carregarPorId( $_REQUEST['hciid'] );
}

if($anoSubacoes && $sbaid){
	$hmstravadoMontAnterior = $obDadosSubAcao->carregaSubacaoAnteriorPorID($sbaid, $anoSubacoes, $hmcidSubacoes);
	$idMonitoraSubPassado 	= $hmstravadoMontAnterior['hmsid'];
	if($idMonitoraSubPassado){
		$itemPassado = $obHistItem->carregaItemAnteriorPorID($cosid, $anoSubacoes, $idMonitoraSubPassado, $hieid);
		$MonitoraPassadoStatusItem = $itemPassado['scsid'];
		$MonitoraPassadoTravado = $itemPassado['hcitravado'];
	}
}

$acaind 					 	= $obHistItem->recuperaDadosPopupItensComp($sbaid, $cosid, $hieid); 	  // recupera dados de referência
//$listaHistorico 			 	= $obHistItem->recuperaHistoricoMonitoramentoPagItens($cosid,$obHistItem->hmsano ); // carrega lista de historico de monitoramento do item
if(!$hieid){
	$stWhere = " AND cosid = {$cosid} ";
}else{
	$stWhere = " AND hieid = {$hieid} ";
}
	


$listaHistorico = "SELECT   date_part('month',  c.hmcdatamonitoramento) || '/' || date_part('year',  c.hmcdatamonitoramento) as hmcdatamonitoramento ,
					sc.scsdesc,
					coalesce( i.hmsquantidadeempenhada,0) as hmsquantidadeempenhada, 
					coalesce(i.hmsquantidadeliquidada,0) as hmsquantidadeliquidada, 
					coalesce(i.hmsquantidadepaga,0) as hmsquantidadepaga, 
					coalesce(i.hmsvalorunitario,0) as hmsvalorunitario, 
					coalesce(i.hmsvalortotalempenhado,0) as hmsvalortotalempenhado,
					coalesce(i.hmsvalortotalliquidado,0) as hmsvalortotalliquidado , 
					coalesce(i.hmsvalortotalpago,0) as hmsvalortotalpago
				FROM cte.historicoconvitemcomposicao i
				INNER JOIN cte.historicomonitoramentoconvsubac s ON s.hmsid = i.hmsid
				INNER JOIN cte.historicomonitoramentoconvenio c ON s.hmcid = c.hmcid
				LEFT JOIN cte.statuscomposicaosubacao sc ON sc.scsid = i.scsid
				WHERE i.hmsano = ".$obDadosSubAcao->hmsano." {$stWhere} ORDER BY hciid asc";
//dbg($listaHistorico);
$selectStatusMotivoItensComp 	= $obHistItem->statusMotivoItensComp($obHistItem->micid);

// CARREGA STATUS DA SUBACAO
if( $hmsid ){// carrega dados do item de composição caso exista um monitoramento
	$obDadosSubAcao->carregarPorId( $hmsid );
	$statusSub = $obDadosSubAcao->ssuid;
}

/************************* SALVA DADOS ******************************/
if( $_REQUEST['salvar'] ){
	$sucesso = 0;
	if($hmsid){
		// Regra de négocios - cálculos dados e retira vírgulas para salvar certo no BD //
		// Tratamento dos valores
		$qtdEmpenhada 	   = str_replace(",", ".", $quantEmpenhada);
		$qtdliquidada 	   = str_replace(",", ".", $quantliquidada);
		$qtdPaga 	  	   = str_replace(",", ".", $quantPaga);
		$totqtdexecutado   = str_replace(",", ".", $totqtdexecutado);
		$totvalorexecutado = str_replace(",", ".", $totvalorexecutado);
		
//		$quantEmpenhada = str_replace(".", "", $quantEmpenhada);
//		$quantEmpenhada = str_replace(",", "", $quantEmpenhada);
//		$quantliquidada = str_replace(".", "", $quantliquidada);
//		$quantliquidada = str_replace(",", "", $quantliquidada);
//		$quantPaga 		= str_replace(".", "", $quantPaga);
//		$quantPaga 		= str_replace(",", "", $quantPaga);
		
		$valorUnitario 	= str_replace(".", "", $valorUnitario);
		$valorUnitario 	= str_replace(",", ".", $valorUnitario);
		
		$valorTotalEmpenhado 	= $qtdEmpenhada * $valorUnitario;
		$ValorTotalLiquidado 	= $qtdliquidada * $valorUnitario;
		$ValorTotalPago 		= $qtdPaga * $valorUnitario;
		// Fim Regra de négocios - cálculos //
		// salva dados //
		
		// Verifica se é novo item
		if(!$hieid){
			$arCampos = array(  "hciid","hmsid","cosid");	
		}else{
			$arCampos = array(  "hciid","hmsid","hieid");
		}
		
		
		
		$obHistItem->popularObjeto( $arCampos );
		$obHistItem->scsid = $scsid;
		
		$obHistItem->hmsquantidadeempenhada = null;
		$obHistItem->hmsquantidadeliquidada = null;
		$obHistItem->hmsquantidadepaga 		= null;
		$obHistItem->hmsvalorunitario 		= null;
		$obHistItem->hmsvalortotalempenhado = null;
		$obHistItem->hmsvalortotalliquidado = null;
		$obHistItem->hmsvalortotalpago 		= null;
		$obHistItem->hmsano 				= $anoSubacoes;
		$obHistItem->hmsobs 				= $hmsobs;
		
		if($scsid == EXECUTADOITEMCOMP || $scsid == CANCELADAITEMCOMP ){
			$obHistItem->hcitravado = 't';
		}else{
			$obHistItem->hcitravado = 'f';
		}
		
		if($scsid == EXECUTADOITEMCOMP || $scsid == EMEXECUÇÃOITEMCOMP){ //Se form 1
			$obHistItem->hmsquantidadeempenhada = $qtdEmpenhada;
			$obHistItem->hmsquantidadeliquidada = $qtdliquidada;
			$obHistItem->hmsquantidadepaga 		= $qtdPaga;
			$obHistItem->hmsvalorunitario 		= $valorUnitario;
			$obHistItem->hmsvalortotalempenhado = $valorTotalEmpenhado;
			$obHistItem->hmsvalortotalliquidado = $ValorTotalLiquidado;
			$obHistItem->hmsvalortotalpago 		= $ValorTotalPago;
			$obHistItem->hmsobs 				= null;
			$obHistItem->micid	 				= null;
			$arCamposNulo = array('hmsobs', 'micid');
		}else if($scsid == NAOINICIADAITEMCOMP){ 						//Se form 2
			$obHistItem->hmsobs 				= $hmsobs;
			$obHistItem->micid 					= $micid;
			$arCamposNulo = array(	'hmsquantidadeempenhada', 
									'hmsquantidadeliquidada',
									'hmsquantidadepaga',
									'hmsvalorunitario',
									'hmsvalortotalempenhado',
									'hmsvalortotalliquidado',
									'hmsvalortotalpago'
								);
			$sucesso = 1;
		}else{ 															//Se form 3
			$obHistItem->hmsobs 				= $hmsobs;
			$sucesso = 1;
			$arCamposNulo = array(	'hmsquantidadeempenhada', 
									'hmsquantidadeliquidada',
									'hmsquantidadepaga',
									'hmsvalorunitario',
									'hmsvalortotalempenhado',
									'hmsvalortotalliquidado',
									'hmsvalortotalpago'
								);
		}
		
		if($scsid == EXECUTADOITEMCOMP || $scsid == EMEXECUÇÃOITEMCOMP){
			if($qtdEmpenhada == "" || $qtdliquidada == "" || $qtdPaga == "" || $valorUnitario == "" ){
				echo "<script> 
				alert( 'Todos os campos são obrigatórios. ' ); 
		  	  </script>";
				$sucesso = 0;
				
			}else{
				$sucesso = 1;
			}
		}
		
		
		
		if($sucesso == 1){
			$obHistItem->salvar(true, true, $arCamposNulo);
			$obHistItem->commit();
			if(!$hieid){
				$cosid = $obHistItem->cosid;
			}else{
				$hieid = $obHistItem->hieid;	
			}
			$hciid = $obHistItem->hciid;			
			$hmcid = $obHistItem->recuperaHmcidPorItem($hciid);
			if($qtdPaga == NULL){
				$qtdPaga = 0;
			}
			if($valor == NULL){
					$valor = 0;
			}
			if($valorUnitario == NULL){
					$valorUnitario = 0;
			}
			if($ValorTotalPago == NULL){
					$ValorTotalPago = 0;
			}
			if($quantidade == NULL){
					$quantidade = 0;
			}
			
			$paramTotalExecutado = "{'qtdExecutadoNovo'    : '" . $qtdPaga . "', 
								     'valorExecutadoNovo'  : '" . $ValorTotalPago . "',
								     'qtdExecutadoAtual'   : '" . $totqtdexecutado . "',
								     'valorExecutadoAtual' : '" . $totvalorexecutado . "'}";
			if($hieid){
				$cosid = 0;	
			}
			
			if(!$hieid){
				$hieid = 0;
			}
			
		   echo '<script> 
					alert( "Operação realizada com sucesso. " );
					parent.closeItens('.$cosid.','.$sbaid.','.$hmsid.','.$hciid.','.$quantidade.','.$valor.', '.$qtdPaga.', '.$valorUnitario.', '.$ValorTotalPago.','.$obHistItem->hmsano.','.$hmcid.',\'' . $statusItem . '\', ' . $paramTotalExecutado . ', '.$hieid.');
			  	  </script>';
		}
    }else{
    	echo "<script> 
				alert( 'Não existe monitoramento aberto. Inicie um monitoramento para salvar os dados. ' ); 
		  	  </script>";
    }

}

/************************* CARREGA VARIAVEIS E FORMATA DADOS PARA MOSTRAR NA TELA ******************************/
$hmsobs 	= $obHistItem->hmsobs;
$hmsobsi	= $obHistItem->hmsobs;
$scsid		= $obHistItem->scsid;

$comboStatusItensComposicao	= $obHistItem->carregaComboStatusItensComposicao($scsid, $cosid, $statusSub, $MonitoraPassadoStatusItem, $hieid);
$totalDoItem = $valor * $quantidade;

$valorTotalJaInformado = $obHistItem->recuperaValorTotalAcumuladoDoItem($cosid, $hieid);
$valorTotalJaInformado = $valorTotalJaInformado == NULL ? 0 : $valorTotalJaInformado;
$valor 		= number_format($valor, 2, ',', '.');

?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../../includes/funcoes.js"></script>

</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<input type=hidden name="salvar" value="false" />
	<input type=hidden name="hciid" value="<?php echo $obHistItem->hciid; ?>" />
	<input type=hidden name="cosid" value="<?php echo $cosid; ?>" />
	<input type=hidden name="hieid" value="<?php echo $hieid; ?>" />
	<input type=hidden name="ano" value="<?php echo $obDadosSubAcao->hmsano; ?>" />
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="4" >Histórico de Monitoramento do Item</th>
		</tr>
		<tr>
			<td colspan="3">
			<?php 
			/*
				if(count($listaHistorico) >= 1 ){//Formata data
					$valorEmpenhado = 0;
					$valorLiquidado = 0;
					$valorPago		= 0;
					for ($cont = 0; $cont < count($listaHistorico); $cont++){
						$mes = $obdata->formataData($listaHistorico[$cont]['hmcdatamonitoramento'],"mesTextual de yyyy","yyyy-mm-dd");
						$listaHistorico[$cont]['hmcdatamonitoramento'] = $mes;
						
						// calculos de valores
						$calculoEmpenhado = (integer)$listaHistorico[$cont]['hmsvalortotalempenhado'] * (integer)$listaHistorico[$cont]['hmsquantidadeempenhada'];
						$calculoLiquidado = (integer)$listaHistorico[$cont]['hmsvalortotalliquidado'] * (integer)$listaHistorico[$cont]['hmsquantidadeliquidada'];
						$calculoPago	  = (integer)$listaHistorico[$cont]['hmsvalortotalpago'] * (integer)$listaHistorico[$cont]['hmsquantidadepaga'];
						$valorEmpenhado   = $calculoEmpenhado + $valorEmpenhado;
						$valorLiquidado   = $calculoLiquidado + $valorLiquidado;
						$valorPago 		  = $calculoPago + $valorPago;
						
						$listaHistorico[$cont]['hmsvalortotalempenhado'] = $valorEmpenhado;
						$listaHistorico[$cont]['hmsvalortotalliquidado'] = $valorEmpenhado;
						$listaHistorico[$cont]['hmsvalortotalpago'] 	 = $valorEmpenhado;
					}
				}
			*/
			
				$obMontaListaAjax = new MontaListaAjax($db, true);   
				$registrosPorPagina = 10; 
				$cabecalho = array("Mês de Monitoramento", 'status', "Qtd.empenhada", "Qtd. liquidada", 
								   "Qtd. paga", "Vlr. unitário", "Vlr. empenhado", "Vlr. liquidado", "Vlr.pago");            
                $obMontaListaAjax->montaLista($listaHistorico, $cabecalho,$registrosPorPagina, 10, 'S', '', '', '', '', '', '', '' );        
				//$obHistItem->monta_lista($listaHistorico,$cabecalho,20, 10, 'N', '100%', '');				
			?>
			</td>
		</tr>
	</table>
	<br />
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%"; style="border-bottom: 0 !important;">
		<colgroup>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td class="tdDegradde02" style="padding: 0 15px 0 15px;">
					<div style="float: left; position: relative;">
						<h3 title="Monitora itens composição">
							Monitora item composição - <? echo $acaind['cosdsc']; ?>
						</h3>
					</div>
				</td>
			</tr>
		</tbody>
	</table>
	
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr><th colspan="2">Dados de referência</th></tr>
		<tr><td class="SubTituloDireita">Dimensão :</td><td><? echo $acaind['dimcod'].". ".$acaind['dimdsc']; ?></td></tr>
		<tr><td class="SubTituloDireita">Área :</td><td><? echo $acaind['ardcod'].". ".$acaind['arddsc']; ?></td></tr>
		<tr><td class="SubTituloDireita">Indicador :</td><td><? echo $acaind['indcod'].". ".$acaind['inddsc']; ?></td></tr>
		<tr><td class="SubTituloDireita">Descrição Subação :</td><td><? echo $acaind['sbadsc']; ?></td></tr>
		<tr><td class="SubTituloDireita">Quantidade do Item :</td><td><? echo $quantidade; ?></td></tr>
		<tr><td class="SubTituloDireita">Valor do Item :</td><td><? echo $valor;?></td></tr>
	</table>
		<br />
		
	<?php if( $MonitoraPassadoTravado == 'f' || $MonitoraPassadoTravado == NULL  ){ 	?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Selecione o status do item:</td>
			<td>
				<?=$comboStatusItensComposicao; ?>
				<input name="statustexto" id="statustexto" type="hidden">
			</td>
		</tr>
	</table>

	<table id="form1" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th>Execução</th>
			<th>Quantidades</th>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Quantidade empenhada</td>
			
			
			<td><?= campo_texto('hmsquantidadeempenhada', "S", "S", "", 30, 50, "", "", '', '', '', 'id="hmsquantidadeempenhada" onkeyup="/*this.value=mascaraglobal(\'[#],#\',this.value);*/ calculatotais(this, \'quantidadeempenhada\');" onchange="calculatotais(this, \'quantidadeempenhada\');"', '', $obHistItem->hmsquantidadeempenhada ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade liquidada</td>
			<td><?= campo_texto('hmsquantidadeliquidada', "S", "S", "", 30, 50, "", "", '', '', 0, 'id="hmsquantidadeliquidada" onkeyup="/*this.value=mascaraglobal(\'[#]\',this.value);*/ calculatotais(this, \'quantidadeliquidada\');" onchange="calculatotais(this, \'quantidadeliquidada\');"','',$obHistItem->hmsquantidadeliquidada ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade paga</td>
			<td>
				<?= campo_texto('hmsquantidadepaga', "S", "S", "", 30, 50, "", "", '', '', 0, 'id="hmsquantidadepaga" onkeyup="/*this.value=mascaraglobal(\'[#]\',this.value);*/ calculatotais(this, \'quantidadepaga\');" onchange="calculatotais(this, \'quantidadepaga\');"','',$obHistItem->hmsquantidadepaga ); ?>
				<input name="totqtdexecutado" id="totqtdexecutado" type="hidden" value="<?=$obHistItem->hmsquantidadepaga ?>">
			</td>
		</tr>
		<tr>
			<th></th>
			<th>Valores:</th>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor unitário contratado/executado</td>
			<td><?= campo_texto('hmsvalorunitario', "S", "S", "", 30, 50, "", "", '', '', 0, 'id="hmsvalorunitario" onkeyup="mascaraValor(this); calculatotais(this, \'valorunitario\');" onchange="calculatotais(this, \'valorunitario\');"','',$obHistItem->hmsvalorunitario );  ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor total empenhado</td>
			<td><?= campo_texto('hmsvalortotalempenhado', "N", "N", "", 30, 50, "", "", '', '', 0, 'id="hmsvalortotalempenhado"','',$obHistItem->hmsvalortotalempenhado );?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor total liquidado</td>
			<td><?= campo_texto('hmsvalortotalliquidado', "N", "N", "", 30, 50, "", "", '', '', 0, 'id="hmsvalortotalliquidado"','',$obHistItem->hmsvalortotalliquidado ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor total pago</td>
			<td>
				<?= campo_texto('hmsvalortotalpago', "N", "N", "", 30, 50, "", "", '', '', 0, 'id="hmsvalortotalpago"','',$obHistItem->hmsvalortotalpago ); ?>
				<input name="totvalorexecutado" id="totvalorexecutado" type="hidden" value="<?=$obHistItem->hmsvalortotalpago ?>">
			</td>
		</tr>	
		<tr>
			<td></td>
			<td><input type="button" name="salvar" value="salvar"  onclick="salvarDados();" /></td>
		</tr>
	</table>

	<table id="form2" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th colspan="2" >Porque não foi iniciado?</th>
		</tr>
		<tr>
			<td class="SubTituloDireita">Motivo:</td>
			<td><?=$selectStatusMotivoItensComp; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observação:</td>
			<td><?=campo_textarea('hmsobsi', 'N', 'S', '', 70, 3, 2000 ); ?>
			</td>
		</tr>
		<tr>
			<td></td>
			<td><input type="button" name="salvar" value="salvar"  onclick="salvarDados();" /></td>
		</tr>
	</table>

	<table id="form3" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th id="texto" colspan="2" ></th>
		</tr>
		<tr>
			<td class="SubTituloDireita">Motivo:</td>
			<td><?=campo_textarea('hmsobs', "S", 'S', '', 70, 3, 2000 ); ?></td>
		</tr>
		<tr>
			<td></td>
			<td><input type="button" name="salvar" value="salvar"  onclick="salvarDados();" /></td>
		</tr>
	</table>
	<?php }else{  
		if($scsid){
			$sql = "select scsdesc from cte.statuscomposicaosubacao where scsid = ".$scsid;
			$statusIemAnteriorAtual = $obHistItem->pegaUm($sql);
		}
			
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Status do item de composição:</td>
			<td><?php echo $statusIemAnteriorAtual; ?></td>
		</tr>
	</table>
	
	<?php } ?>
	
	<div style="text-align: center; margin-top: 20px;">
		<a style="cursor: pointer" onclick="return false;">
			<img src="/imagens/print.png" alt="Versão para impressão" onclick="janelaImpressao('<?php echo $_REQUEST['cosid']; ?>', '<?php echo $_REQUEST['sbaid']; ?>', '<?php echo $_REQUEST['hmsid']; ?>', '<?php echo $_REQUEST['hciid']; ?>', '<?php echo $_REQUEST['quant']; ?>', '<?php echo $_REQUEST['valor']; ?>')"> 
		</a>
	</div>
	
</form>
<?php
$valorUnitario 		= str_replace(",", ".", str_replace(".", "", $valor));
$totalEmpenhado 	= $quantidade * $valorUnitario;
$totalAcumulado 	= $valorTotalJaInformado;
$PegaQtdsAcumuladas	= $obHistItem->recuperaQtdTotalAcumuladoDoItem($cosid, $hieid, $obDadosSubAcao->hmsano, $_REQUEST['hciid']);
$qtdAcumuladaEmp	= $PegaQtdsAcumuladas[0]['hmsquantidadeempenhada'] ? $PegaQtdsAcumuladas[0]['hmsquantidadeempenhada'] : 0;
$qtdAcumuladaLiq	= $PegaQtdsAcumuladas[0]['hmsquantidadeliquidada'] ? $PegaQtdsAcumuladas[0]['hmsquantidadeliquidada'] : 0;
$qtdAcumuladaPag	= $PegaQtdsAcumuladas[0]['hmsquantidadepaga'] ? $PegaQtdsAcumuladas[0]['hmsquantidadepaga'] : 0;
?>
<script type="text/javascript">

	//mascaraValor( document.getElementById( 'hmsquantidadeempenhada' ) );
	//mascaraValor( document.getElementById( 'hmsquantidadeliquidada' ) );
	//mascaraValor( document.getElementById( 'hmsquantidadepaga' ) );
	mascaraValor( document.getElementById( 'hmsvalorunitario' ));
	mascaraValor( document.getElementById( 'hmsvalortotalempenhado' ));
	mascaraValor( document.getElementById( 'hmsvalortotalliquidado' ));
	mascaraValor( document.getElementById( 'hmsvalortotalpago' ));
	
	// Esconde todas os form da tela.
	if(document.getElementById('form1')){
		document.getElementById('form1').style.display = 'none';
	}
	if(document.getElementById('form2')){
		document.getElementById('form2').style.display = 'none';
	}
	if(document.getElementById('form3')){
		document.getElementById('form3').style.display = 'none';
	}
	if(document.getElementById('scsid')){
		var statusItensComposicao = document.getElementById('scsid').value;
	}
	
	function calculatotais(campo, calculoCampo){
		var quantEmpenhada 		= document.getElementById('hmsquantidadeempenhada').value;
		var quantidadeliquidada = document.getElementById('hmsquantidadeliquidada').value;
		var quantidadepaga 		= document.getElementById('hmsquantidadepaga').value;
		var valorunitario 		= document.getElementById('hmsvalorunitario').value;
		var valorunitarioMasc 	= document.getElementById('hmsvalorunitario').value;
		if(valorunitario == ""){
			valorunitario = '1,0';
		}
		
		if(calculoCampo == 'quantidadeempenhada'){ 
//			if(calculoCampo == 'valorunitario'){
				var qtdempe = replaceAll(campo.value, ",", ".");
				qtdempe 	= qtdempe > 0 ? qtdempe : 0;
				qtdempe 	= parseFloat( qtdempe );
				valorunitario = parseFloat(replaceAll(replaceAll(valorunitario, ".", ""), ",", "."));
				var qtde 	  = qtdempe * valorunitario;
				document.getElementById('hmsvalortotalempenhado').value = mascaraValorResultado(qtde);
//			}
		}
		if(calculoCampo == 'quantidadeliquidada'){
//			if(calculoCampo == 'valorunitario'){
				var qtdl   = replaceAll(campo.value, ",", ".");
				qtdl 	   = qtdl > 0 ? qtdl : 0;
				qtdl 	   = parseFloat( qtdl );
				valorunitarioa  = parseFloat(replaceAll(replaceAll(valorunitario, ".", ""), ",", "."));
				var qtdli 		= qtdl * valorunitarioa;
				document.getElementById('hmsvalortotalliquidado').value = mascaraValorResultado(qtdli);
				//var qtde = parseInt( mascaraglobal( "[#]", campo.value ) ) * parseInt( mascaraglobal( "[#]", valorunitario ) );
				//document.getElementById('hmsvalortotalliquidado').value = mascaraValorInterno(qtde, quantidadeliquidada,valorunitarioMasc );
//			}
		}
		if(calculoCampo == 'quantidadepaga'){
//			if(calculoCampo == 'valorunitario'){
				var qtdpaga    = replaceAll(campo.value, ",", ".");
				qtdpaga 	   = qtdpaga > 0 ? qtdpaga : 0;
				qtdpaga 	   = parseFloat( qtdpaga );
				valorunitariob = parseFloat(replaceAll(replaceAll(valorunitario, ".", ""), ",", "."));
				var qtdpagaa   = qtdpaga * valorunitariob;
				document.getElementById('hmsvalortotalpago').value = mascaraValorResultado(qtdpagaa);
				//var qtde = parseInt( mascaraglobal( "[#]", campo.value ) ) * parseInt( mascaraglobal( "[#]", valorunitario ) );
				//document.getElementById('hmsvalortotalpago').value 		= mascaraValorInterno(qtde, quantidadepaga,valorunitarioMasc );
//			}
		}
		if(calculoCampo == 'valorunitario'){
		
			if(quantEmpenhada){
				var quantEmpenhada 		= quantEmpenhada;
				//quantEmpenhada 			=  parseFloat(replaceAll(replaceAll(quantEmpenhada, ".", ""), ",", "."));
				quantEmpenhada 			=  parseFloat(replaceAll(quantEmpenhada, ",", "."));
				valorunitarioc   		=  parseFloat(replaceAll(replaceAll(valorunitario, ".", ""), ",", "."));
				var qtdEmp 		= quantEmpenhada * valorunitarioc;
				document.getElementById('hmsvalortotalempenhado').value = mascaraValorResultado(qtdEmp);
				//var qtde =  parseInt( mascaraglobal( "[#]", quantEmpenhada ) ) * parseInt( mascaraglobal( "[#]", valorunitario ) );
				//document.getElementById('hmsvalortotalempenhado').value = mascaraValorInterno(qtde, quantEmpenhada,valorunitarioMasc  );
			}
			
			if(quantidadeliquidada){
				var quantidadeliquidada 	= quantidadeliquidada;
				//quantidadeliquidada 		=  parseFloat(replaceAll(replaceAll(quantidadeliquidada, ".", ""), ",", "."));
				quantidadeliquidada 		=  parseFloat(replaceAll(quantidadeliquidada, ",", "."));
				valorunitariod   			=  parseFloat(replaceAll(replaceAll(valorunitario, ".", ""), ",", "."));
				var quantidadeliqu 			= quantidadeliquidada * valorunitariod;
				document.getElementById('hmsvalortotalliquidado').value = mascaraValorResultado(quantidadeliqu);
				//var qtde =  parseInt( mascaraglobal( "[#]", quantidadeliquidada ) ) * parseInt( mascaraglobal( "[#]", valorunitario ) );
				//document.getElementById('hmsvalortotalliquidado').value = mascaraValorInterno(qtde, quantidadeliquidada, valorunitarioMasc );
			}
			
			if(quantidadepaga){
			
				var quantidadepaga 		= quantidadepaga;
				//quantidadepaga 			=  parseFloat(replaceAll(replaceAll(quantidadepaga, ".", ""), ",", "."));
				quantidadepaga 			=  parseFloat(replaceAll(quantidadepaga, ",", "."));
				valorunitarioe   =  parseFloat(replaceAll(replaceAll(valorunitario, ".", ""), ",", "."));
				var qtdpaga 		= quantidadepaga * valorunitarioe;
				document.getElementById('hmsvalortotalpago').value = mascaraValorResultado(qtdpaga);
				//var qtde =  parseInt( mascaraglobal( "[#]", quantidadepaga ) ) * parseInt( mascaraglobal( "[#]", valorunitario ) );
				//document.getElementById('hmsvalortotalpago').value 	= mascaraValorInterno(qtde, quantidadepaga, valorunitarioMasc );
			}
		}
		
	}
	
	function mascaraValorResultado( obj ){
		var valor = new String();
		valor = obj.toString();
		var complemento = '';
		if( valor.charAt(0) == '-' ){
			complemento = '-';
		}
		obj = complemento+MascaraMonetario( valor );
		return obj;

	}

	function mascaraValorInterno( vlr ,obj, obj2 ){
		valor = vlr;
		var complemento = '';
		if(obj2 != ""){
			if( obj2.substr(0, 1) == '-' ){
				complemento = '-';
			}
		}
		if( obj.substr(0, 1) == '-' ){
			complemento = '-';
		}
		return complemento+mascaraglobal( '[###.]###,##', valor );
		
	}
	
	/**
	 * function mascaraComNegativo();
	 * descscricao 	: Mascara valores negativos ou não
	 * author 		: Thiago Tasca Barbosa
	 */
	function mascaraValor( obj ){
		if(obj){
			valor = obj.value;
			var complemento = '';
			
			if( valor.substr(0, 1) == '-' ){
				complemento = '-';
			}
			obj.value = complemento+mascaraglobal( '[###.]###,##', valor );
		}else{
			return false;
		}	
	}
		
	/**
	 * function salvarDados();
	 * descscricao 	: Salva dados do form no bd.
	 * author 		: Thiago Tasca Barbosa
	 */
	function salvarDados(){
		var statusItensComposicao = document.getElementById('scsid').value;
		var quantidade = <?=$quantidade;?>;
		var f = document.formulario;
		
		var totalEmpenhado 	= <?=$totalEmpenhado;?>;
		var totalAcumulado 	= <?=$totalAcumulado;?>;
		var atualEmpenhado 	= parseFloat(replaceAll(replaceAll(f.hmsvalortotalempenhado.value, ".", ""), ",", "."));
		var atualLiquidado 	= parseFloat(replaceAll(replaceAll(f.hmsvalortotalliquidado.value, ".", ""), ",", "."));
		var atualPago 		= parseFloat(replaceAll(replaceAll(f.hmsvalortotalpago.value, ".", ""), ",", "."));
		var vlrAtual 		= parseFloat(replaceAll(replaceAll(f.hmsvalorunitario.value, ".", ""), ",", "."));  	
		var totalAtual 		= atualEmpenhado + totalAcumulado; 
		var comboStatus 	= document.formulario.scsid.value;	
		var vlrUnitario 	= <?=$valorUnitario;?>;		
		var qtdEmpenhada 	= document.formulario.hmsquantidadeempenhada.value;
		var qtdLiquidada 	= document.formulario.hmsquantidadeliquidada.value;
		var qtdPaga 		= document.formulario.hmsquantidadepaga.value;
		var qtdAcumuladaEmp = <?=$qtdAcumuladaEmp;?>;
		var qtdAcumuladaLiq = <?=$qtdAcumuladaLiq;?>;
		var qtdAcumuladaPag = <?=$qtdAcumuladaPag;?>;
		var atualQtdEmp		= parseFloat(qtdEmpenhada) + parseFloat(qtdAcumuladaEmp);
		var atualQtdLiq		= parseFloat(qtdLiquidada) + parseFloat(qtdAcumuladaLiq);
		var atualQtdPag		= parseFloat(qtdPaga) + parseFloat(qtdAcumuladaPag); 

		if(comboStatus == ''){
			alert("Selecione o status do item de composição.");
			return false;
		}

		if(atualQtdEmp > quantidade){
			alert("A quantidade empenhada informada não pode ser maior que a quantidade do item de composição.");
			document.formulario.hmsquantidadeempenhada.focus();
			return false;
		}
		if(atualQtdLiq > quantidade){
			alert("A quantidade liquidada informada não pode ser maior que a quantidade do item de composição.");
			document.formulario.hmsquantidadeliquidada.focus();
			return false;
		}
		if(atualQtdPag > quantidade){
			alert("A quantidade paga informada não pode ser maior que a quantidade do item de composição.");
			document.formulario.hmsquantidadepaga.focus();
			return false;
		}
		
		if(qtdEmpenhada < 0){
			alert("A quantidade empenhada não pode ser negativa!");
			document.formulario.hmsquantidadeempenhada.focus();
			return false;
		}
		if(qtdLiquidada < 0){
			alert("A quantidade liquidada não pode ser negativa!");
			document.formulario.hmsquantidadeliquidada.focus();
			return false;
		}
		if(qtdPaga < 0){
			alert("A quantidade paga não pode ser negativa!");
			document.formulario.hmsquantidadepaga.focus();
			return false;
		}
		if(quantidade == 1){
			if(comboStatus == '2' ){
				alert('A quantidade do item é unico, mude o status para executado!');
				document.formulario.scsid.focus();
				return false;
			}
		}

		if( (totalAtual >= totalEmpenhado) && (atualQtdPag >= quantidade) ){
			if(comboStatus != '1' ){
				alert('Valor total do item atingido ou ultrapassado, mude o status para executado!');
				document.formulario.scsid.focus();
				return false;
			}
		}
		if(comboStatus == '1'){
			if(atualEmpenhado == 0 || atualLiquidado == 0 || atualPago == 0){
				alert("Para a subação estar como executado os valores não podem ser igual a zero!");				
				return false;
			}
			
			if(atualEmpenhado == '' || atualLiquidado == '' || atualPago == ''){
				alert("Para a subação estar como executado os valores não podem estar em branco!");				
				return false;
			}
		}

		if(vlrAtual > vlrUnitario){
			//alert("O valor unitário não pode ser maior que o estabelecido!");
			if(!confirm('O valor unitário não pode ser maior que o estabelecido. deseja continuar?')){
				document.formulario.hmsvalorunitario.focus();
				return false;
			}
		}
		
		if(totalAtual > totalEmpenhado){
			//alert('Valor total do item maior que o permitido!');
			if(!confirm('Valor do item superior ao programado. deseja continuar?')){
				return false;
			}
		}
		
		if(statusItensComposicao == <?php echo CANCELADAITEMCOMP; ?> || statusItensComposicao == <?php echo EMPLANEJAMENTOITEMCOMP; ?>  ){
			var observacao = document.getElementById("hmsobs").value;
			if(observacao == ''){
				alert("Obrigatório informar o motivo");
				return false;
			}
		}
		if(statusItensComposicao == <?php echo NAOINICIADAITEMCOMP; ?>){
			var motivo = document.getElementById("micid").value;
			if(!motivo){
				alert("Obrigatório informar o status");
				return false;
			}
		}

		if(statusItensComposicao == <?php echo EXECUTADOITEMCOMP; ?> || statusItensComposicao == <?php echo EMEXECUÇÃOITEMCOMP; ?>){
			var quantEmpenhada 	= document.getElementById('hmsquantidadeempenhada').value;
			var quantliquidada 	= document.getElementById('hmsquantidadeliquidada').value;
			var quantPaga		= document.getElementById('hmsquantidadepaga').value;
			var valorUnitario 	= document.getElementById('hmsvalorunitario').value;
			if(quantEmpenhada == '' || quantliquidada == '' || quantPaga == '' || valorUnitario == '' ){
				alert("Obrigatório informar todos os campos.");
				return false;
			}
			
			if(quantEmpenhada > quantidade || quantliquidada > quantidade || quantPaga > quantidade ){
				alert("A quantidade informada não pode ser maior que a quantidade do item de composição.");
				return false;
			}
			
			valorUnitario = parseInt(replaceAll(valorUnitario, ",", "."));
			var result = quantPaga * valorUnitario;
			var valorAtual = parseInt( result ) + parseInt('<? echo $valorTotalJaInformado; ?>');
			if(valorAtual > parseInt('<? echo $totalDoItem; ?>') ){
				if(!confirm('A valor/quantidade adquirida é maior que a planejada. deseja continuar?')){
					return false;
				}	
			}
		
		}
		document.formulario.salvar.value = true;
		document.formulario.submit();
	}
	
	/**
	 * function carregaEditarItensComposicao(idBtnEditar, valorSetado, sbaid, hmsid, hciid, cnvid);
	 * descscricao 	: Carrega o a tabela do form de acordo com o status.
	 * author 		: Thiago Tasca Barbosa
	 * parametros 	: scsid (status do item de composição)
	 */
	function carregaEditarItensComposicao(scsid){
		if(scsid == <?=EXECUTADOITEMCOMP;?> || scsid == <?=EMEXECUÇÃOITEMCOMP;?> ){
			document.getElementById('form1').style.display = '';
			document.getElementById('form2').style.display = 'none';
			document.getElementById('form3').style.display = 'none';
		}else if(scsid == <?=NAOINICIADAITEMCOMP;?> ){
			document.getElementById('form1').style.display = 'none';
			document.getElementById('form2').style.display = '';
			document.getElementById('form3').style.display = 'none';
		}else if(scsid == <?=CANCELADAITEMCOMP;?> || scsid == <?=EMPLANEJAMENTOITEMCOMP;?>){
			document.getElementById('form1').style.display = 'none';
			document.getElementById('form2').style.display = 'none';
			document.getElementById('form3').style.display = '';
			var texto = "Porque foi cancelada?";
			if(scsid == <?=EMPLANEJAMENTOITEMCOMP;?>){
				texto = "Porque está em planejamento?";
			}
			document.getElementById('texto').innerHTML = texto;
		}
		if ( document.getElementById('statustexto') ){
			if ( document.getElementById('scsid').value == '' )
				document.getElementById('statustexto').value = '';
			else
				document.getElementById('statustexto').value = document.getElementById('scsid').options[document.getElementById('scsid').selectedIndex].text;	
		}
	}
	
	/* Função para subustituir todos 
	* descscricao 	: 
	 * author 		: Alexandre Dourado
	 * parametros 	: 
	 */
	function replaceAll(str, de, para){
		if ( str == ''){ return str; }
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}
	
	function janelaImpressao(cosid, sbaid, hmsid, hciid, quant, valor){
		return window.open('cte.php?modulo=principal/monitoraFinanceiro/imprimir_monitora_itensComp&acao=A&cosid='+cosid+'&sbaid='+sbaid+'&hmsid='+hmsid+'&hciid='+hciid+'&quant='+quant+'&valor='+valor,'monitoraItens','height=500,width=800,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
	
	carregaEditarItensComposicao(statusItensComposicao);
</script>
</body>
</html>