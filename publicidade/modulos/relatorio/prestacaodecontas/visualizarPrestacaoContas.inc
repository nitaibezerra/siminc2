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
	$file_name="PrestacaoContas.xls";
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
$oTipoServico = new TipoServico();

//verifica as datas
$_GET['dtinicial'] = $_GET['datainicial'];
$_GET['dtfinal'] = $_GET['datafinal'];
$erroData = $oPad->verifica_data($_GET);
if(!empty($erroData)){
	$msg = utf8_decode('A data informada é inválida!');
	alerta($msg);
	echo "<script>executarScriptPai('setaFocoData(\'".$erroData."\')');this.close();</script>";
}

//pega os dados das agências com seus contratos
$dadosAgencia = $oFornecedor->pegar_agencias_prestacao_contas($_GET);
if(count($dadosAgencia) < 1){
	alerta('Nenhum registro localizado.');
	echo "<script>executarScriptPai('setaFocoData(\'nenhumregistro\')');this.close();</script>";
}

//pega os tipos de serviço, as categorias e os valores pagos
$dadosTipoServico = $oTipoServico->pegar_tipos_prestacao_contas($_GET);

//pega a qtd de categorias para cada tipo de serviço
//retornados pela query para ser usado no rowspan
if(is_array($dadosTipoServico) && count($dadosTipoServico) >= 1){
	$tipoServico = '';
	$tipoQtd = array();
	$i = 0;
	$totalValor = '';
	foreach($dadosTipoServico as $key => $value){
		if($value['tsedsc'] != $tipoServico){
			if($i > 0){
				$tipoQtd[$tipoServico] = $i;
			}
			$tipoServico = $value['tsedsc'];
			$i = 0;
			$i++;
		}
		else{
			$i++;
		}
		$soma = new Math($totalValor,$value['valor'],2);
		$totalValor = $soma->sum()->getResult();
	}
	$tipoQtd[$tipoServico] = $i;
}

//pega os tipos de serviço e os fornecedores
$dadosTipoServicoFornec = $oTipoServico->pegar_tipos_fornec_prestacao_contas($_GET);

//pega a qtd de fornecedores para cada tipo de serviço
//retornados pela query para ser usado no rowspan
if(is_array($dadosTipoServicoFornec) && count($dadosTipoServicoFornec) >= 1){
	$tipoServico = '';
	$tipoFornecQtd = array();
	$i = 0;
	foreach($dadosTipoServicoFornec as $key => $value){
		if($value['tsedsc'] != $tipoServico){
			if($i > 0){
				$tipoFornecQtd[$tipoServico] = $i;
			}
			$tipoServico = $value['tsedsc'];
			$i = 0;
			$i++;
		}
		else{
			$i++;
		}
	}
	$tipoFornecQtd[$tipoServico] = $i;
}

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				    "<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				    "</td>" .
			        "</tr>
			        </table>";

if( ! $isXls )
{
	echo $cabecalhoBrasao;
}


monta_titulo('Presta&ccedil;&atilde;o de Contas - Valores Pagos (Conforme disp&otilde;e Art. 16 da Lei 12.232/2010)','');
?>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
            <td bgcolor='#e9e9e9' align='left' width='50%' colspan="2"> <b> Per&iacute;odo de Pagamento </b> </td>
		<!--<td bgcolor='#e9e9e9' align='center' width='50%' <?php //if($isXls){ echo 'colspan="2"';} ?> > <b> Exerc&iacute;cio </b> </td>-->
	</tr>
	<tr>
		<td align='center' colspan="2"> <?php echo $_GET['dtinicial'].' a '.$_GET['dtfinal']; ?> </td>
		<!--<td align='center' <?php // if($isXls){ echo 'colspan="2"';} ?> > <?php echo date('Y');  ?> </td>-->
	</tr>
	<tr>
            <td bgcolor='#e9e9e9' align='left' style="width: 50%"> <b> Ag&ecirc;ncia </b> </td>
		<td bgcolor='#e9e9e9' align='center' <?php if($isXls){ echo 'colspan="2"';} ?>> <b> Contrato </b> </td>
	</tr>
	<?php 
	if(is_array($dadosAgencia) && count($dadosAgencia) >= 1){
		foreach($dadosAgencia as $key => $value){	
	?>
	<tr>
		<td align='center'> <?php echo $value['fornome']; ?> </td>
		<td align='center'> <?php echo $value['cttnumcontrato']; ?> </td>
	</tr>
	<?php 
		}
	}
	?>
</table>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor='#e9e9e9' align='center'> <b> Tipo de Servi&ccedil;o </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Categoria </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Valor (R$) </b> </td>
	</tr>
	<?php 
	if(is_array($dadosTipoServico) && count($dadosTipoServico) >= 1){
		$tipoServico = '';
		foreach($dadosTipoServico as $key => $value){
			 if($tipoServico != $value['tsedsc']){
			 	echo "<tr>
			 		      <td align='center' rowspan='".$tipoQtd[$value['tsedsc']]."'>".$value['tsedsc']."</td>
			 		      <td align='center'>".$value['catdsc']."</td>
			 		      <td align='center'>".number_format($value['valor'],2,',','.')."</td>
			 		  </tr>";
			 	$tipoServico = $value['tsedsc'];
			 }
			 else{
			 	echo "<tr>
			 		      <td align='center'>".$value['catdsc']."</td>
			 		      <td align='center'>".number_format($value['valor'],2,',','.')."</td>	
			 		  </tr>";
			 }
		}
	}
	?>
	<tr>
		<td bgcolor='#e9e9e9' align='center' colspan='2'> <b> Total </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> <?php echo number_format($totalValor,2,',','.'); ?> </b> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' align='center'> <b> Tipo de Servi&ccedil;o </b> </td>
		<td bgcolor='#e9e9e9' align='center' colspan='2'> <b> Fornecedor </b> </td>
	</tr>
	<?php 
	if(is_array($dadosTipoServicoFornec) && count($dadosTipoServicoFornec) >= 1){
		$tipoServico = '';
		foreach($dadosTipoServicoFornec as $key => $value){
			if($tipoServico != $value['tsedsc']){
				echo "<tr>
					      <td align='center' rowspan='".$tipoFornecQtd[$value['tsedsc']]."'>".$value['tsedsc']."</td>
					      <td align='left' colspan='2'>".$value['fornome']."</td>	
					  </tr>";
				$tipoServico = $value['tsedsc'];
			}
			else{
				echo "<tr>
					      <td align='left' colspan='2'>".$value['fornome']."</td>	
					  </tr>";
			}
		}
	}
	?>
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