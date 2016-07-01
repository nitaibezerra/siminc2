<?php 
$isXls = $_REQUEST['isXls'];
if( ! $isXls )
{
?>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
<?php 
}
?>
<style type="text/css" media="print">

.noPrint{ 
	display:none; 
} 

</style>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Fecha o relatório
	 * @name fechar
	 * @return void
	 */	
	function fechar(){
		this.close();
	}

	/**
	 * Chama a impressão
	 * @name imprimir
	 * @return void
	 */
	function imprimir(){
		window.print();
	}
</script>

<?php
if($isXls)
{
	$file_name="ExecucaoFiscal.xls";
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Content-Type: application/force-download");
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Disposition: attachment;filename=".$file_name);
	header("Content-Transfer-Encoding: binary ");
}

// incluindo as classes necessárias
$oPad = new Pad();
$oFornecedor = new Fornecedor();
$oContrato = new Contrato();
$oItemPad = new ItemPad();
$oCamp = new Campanha();

//verifica as datas
$_GET['dtinicial'] = $_GET['datainicial'];
$_GET['dtfinal'] = $_GET['datafinal'];
$_GET['ano'] = $_GET['ano'];
$erroData = $oPad->verifica_data($_GET);
if(!empty($erroData)){
	$msg = utf8_decode('A data informada é inválida!');
	alerta($msg);
	echo "<script>executarScriptPai('setaFocoData(\'".$erroData."\')');this.close();</script>";
}

//pega alguns dados necessários ao relatório
$dadosPad = $oPad->pegar_dados_execucao_fiscal($_GET);
if(count($dadosPad) < 1){
	alerta('Nenhum registro localizado.');
	echo "<script>executarScriptPai('setaFocoData(\'nenhumregistro\')');this.close();</script>";
}

//Pega qtd de pads e os ids das mesmas
$resultado = $oPad->pegar_qtd_execucao_fiscal($_GET);

//conta as pads recuperadas de cada agência
$aux = explode(',',$_GET['agencias']);
if(is_array($aux) && count($aux) >= 1){
	foreach($aux as $key => $value){
		if(!is_null($dadosPad[$value])){
			$dadosPad[$value]['qtd'] = count($resultado[$value]);	
		}
		else{
			$dadosPad[$value]['qtd'] = 0;
		}
	}
}


//através dos ids das pads, recupera os ids dos contratos
if(is_array($resultado) && count($resultado) >= 1){
	$cttids = array();
	$cttidsStr = '';
	foreach($resultado as $key => $value){
		foreach($value as $key2 => $value2){
			$dadosArr = $oPad->carrega_registro_por_id($value2['padid']);
			if(!in_array($dadosArr['cttid'],$cttids)){
				$cttids[] = $dadosArr['cttid'];
				$cttidsStr .= $dadosArr['cttid'].',';
			}
		}
	}
	$cttidsStr = substr($cttidsStr,0,strrpos($cttidsStr,','));
}

//soma os orçamentos dos contratos
$ctoValorTotal = $oContrato->pegar_total_valor_orcamento($cttidsStr,$_REQUEST['ano']);

//soma os valores das peças
$valores = $oItemPad->pegar_valores_execucao_fiscal($_GET);

//soma o valor total utilizado(aprovado+faturado+pago)
//soma também os valores: total aprovado, total faturado e total pago
$totalExecutado = 0;
$totalAprovado = 0;
$totalFaturado = 0;
$totalPago = 0;
if(is_array($valores) && count($valores) >= 1){
	foreach($valores as $key => $value){
		$soma = new Math($value['valorAprovado'], $value['valorFaturado'],2);
		$parcial = $soma->sum()->getResult();
		$soma = new Math($parcial, $value['valorPago'],2);
		$parcial = $soma->sum()->getResult();
		$soma = new Math($parcial, $value['valorFaturadoComGlosa'],2);
		$parcial = $soma->sum()->getResult();
		$soma = new Math($parcial, $value['valorAprovadoComGlosa'],2);
		$parcial = $soma->sum()->getResult();
		$soma = new Math($parcial, $totalExecutado,2);
		$totalExecutado = $soma->sum()->getResult();

		$soma = new Math($totalAprovado, $value['valorAprovado'],2);
		$totalAprovado = $soma->sum()->getResult();
		$soma = new Math($totalAprovado, $value['valorAprovadoComGlosa'],2);
		$totalAprovado = $soma->sum()->getResult();
		
		$soma = new Math($totalFaturado, $value['valorFaturado'],2);
		$totalFaturado = $soma->sum()->getResult();
		$soma = new Math($totalFaturado, $value['valorFaturadoComGlosa'],2);
		$totalFaturado = $soma->sum()->getResult();
		
		$soma = new Math($totalPago, $value['valorPago'],2);
		$totalPago = $soma->sum()->getResult();
	}
}

//método criado 02/04/2012 para atender solicitação do gestor
//ele pediu que o relatório apresentasse o valor de um item
//aprovado mesmo que este esteja faturado, se o período 
//informado no filtro abranger a data de cadastro do item mas não abranger a data de faturamento
$x = $oItemPad->pegar_valores_execucao_fiscal_retroativo($_GET);
if(is_array($x) && count($x) >= 1){
	foreach($x as $key => $value){
		
		$soma = new Math($totalExecutado, $value['valorAprovado'],2);
		$totalExecutado = $soma->sum()->getResult();
		$soma = new Math($totalExecutado, $value['valorFaturado'],2);
		$totalExecutado = $soma->sum()->getResult();
		
		$soma = new Math($totalAprovado, $value['valorAprovado'],2);
		$totalAprovado = $soma->sum()->getResult();
		
		$soma = new Math($totalFaturado, $value['valorFaturado'],2);
		$totalFaturado = $soma->sum()->getResult();
	}
}

//faz a diferença entre o total(verba) menos o total executado
$subtrai = new Math($ctoValorTotal, $totalExecutado,2);
$saldo = $subtrai->sub()->getResult();

//pega campanha com total dos itens por campanha
$campanhas = $oCamp->pegar_dados_execucao_fiscal($_GET);

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				    "<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				    "</td>" .
			        "</tr>
			        </table>";


if(! $isXls)
{
	echo $cabecalhoBrasao;
}

monta_titulo('Acompanhamento de Execu&ccedil;&atilde;o Financeira','');
?>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor='#e9e9e9' colspan='3' align='left'> <b> Per&iacute;odo </b> </td>
	</tr>
	<tr>
		<td align='center' colspan='3'> <?php echo $_GET['datainicial'].' a '.$_GET['datafinal']; ?> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' align='left'> <b> Ag&ecirc;ncia </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Total de PAD's Ativas </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> PAD's </b> </td>
	</tr>
	<?php 
	if(is_array($dadosPad) && count($dadosPad) >= 1){
		foreach($dadosPad as $key => $value){
			if($value['qtd'] >= 1){
	?>
	<tr>
		<td align='left'> <?php echo $value['fornome']; ?> </td>
		<td align='center'> <?php echo $value['qtd']; ?> </td>
		<td align='center'> <?php echo $value['padnumeroinicial'].' a '.$value['padnumerofinal']; ?> </td>
	</tr>
	<?php 
			}
		}
	}
	?>
	<tr>
		<td bgcolor='#e9e9e9' colspan='2' align='center'> <b> Situa&ccedil;&atilde;o </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> R$ </b> </td>
	</tr>
	<tr>
		<td colspan='2' align='center'> <b> Verba </b> </td>
		<td align='center'> <?php echo number_format($ctoValorTotal,2,',','.'); ?> </td>
	</tr>
	<tr>
		<td colspan='2' align='left'> Aprovados </td>
		<td align='center'> <?php echo number_format($totalAprovado,2,',','.'); ?> </td>
	</tr>
	<tr>
		<td colspan='2' align='left'> Faturados (A pagar) </td>
		<td align='center'> <?php echo number_format($totalFaturado,2,',','.'); ?> </td>
	</tr>
	<tr>
		<td colspan='2' align='left'> Pagos </td>
		<td align='center'> <?php echo number_format($totalPago,2,',','.'); ?> </td>
	</tr>
	<tr>
		<td colspan='2' align='right'> <b> Total Executado </b> </td>
		<td align='center'> <b> <?php echo number_format($totalExecutado,2,',','.'); ?> </b> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' colspan='2' align='center'> <b> Saldo </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> <?php echo number_format($saldo,2,',','.'); ?> </b> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' colspan='2' align='center'> <b> Detalhamento Executado por Campanha </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> R$ </b> </td>
	</tr>
	<?php 
	if(is_array($campanhas) && count($campanhas) >= 1){
		foreach($campanhas as $key => $value){
	?>
	<tr>
		<td colspan='2' align='left'> <?php echo $value['camtitulo']; ?> </td>
		<td align='center'> <?php echo number_format($value['valor'],2,',','.'); ?> </td>
	</tr>
	<?php 
		}
	}
	?>
	<tr>
		<td bgcolor='#e9e9e9' colspan='2' align='center'> <b> Total </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> <?php echo number_format($totalExecutado,2,',','.'); ?> </b> </td>
	</tr>
	<tr class='noPrint'>
		<td align='center' colspan='3'>
		<input type='button' class="botao" name='btnImprimir' id='btnImprimir' value='Imprimir' onclick="javascript:imprimir();">
		<input type='button' class="botao" name='btnFechar' id='btnFechar' value='Fechar' onclick="javascript:fechar();">
		</td>
	</tr>
	
</table>
<?php 
	if($isXls)
	{
		exit();
	}
?>