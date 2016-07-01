<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
<style type="text/css" media="print">

.noPrint{ 
	display:none; 
} 

</style>

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

if($_GET['isXLS'] == 'Excel')
{
	$isXls = true;
}
else
{
	$isXls = false;
}

if($isXls)
{
	$file_name="levantamento_servicos_PAD.xls";
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Content-Type: application/force-download");
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Disposition: attachment;filename=".$file_name);
	header("Content-Transfer-Encoding: binary ");
}

$oPad = new Pad();
$oItemPad = new ItemPad();

//pega dados da pad
$dados = $oPad->pegar_dados_espelho_pad($_GET);

if(empty( $dados['padnumsidoc'] ) )
{
	echo "<script> alert('Informe o No do Sidoc na aba \'Cadastro PAD\'')</script>";
	echo "<script> self.close(); </script>";
}
if(empty( $dados['padptres'] ) )
{
	echo "<script> alert('Informe o campo \'Programa de Trabalho - PT\' na aba \'Cadastro PAD\'')</script>";
	echo "<script> self.close(); </script>";
}

//pega dados de item
$dadosItem = $oItemPad->pega_itens_por_pad_espelho_pad($_GET);

//pega a soma do valor total
$totValor = $oItemPad->pega_qtd_itens_por_pad_espelho_pad($_GET);

if(!$isXls)
{
	$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
	$cabecalhoBrasao .= "<tr>" .
						"<td colspan=\"100\">" .			
						monta_cabecalho_relatorio('100') .
						"</td>" .
						"</tr>
						</table>";

	echo $cabecalhoBrasao;
}

monta_titulo('Levantamento dos Servi&ccedil;os da PAD','');
?>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor='#e9e9e9' width='50%' align='left' colspan='6'> <b> Dados da PAD: </b> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' align='center'> <b> N&uacute;mero </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Data </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Processo </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> N&ordm; Contrato </b> </td>
		<td bgcolor='#e9e9e9' align='center' colspan="2"> <b> &Oacute;rg&atilde;o Solicitante </b> </td>
	</tr>
	<tr>
		<td align='center'> <?php echo $dados['padnumero']; ?> </td>
		<td align='center'> <?php echo $dados['pademissao']; ?> </td>
		<td align='center'>&nbsp;
            <?php
                $padnumsidoc = str_replace( array( '.', '/', '-'), '', $dados['padnumsidoc'] );
                if( !empty( $padnumsidoc ) )
                {
                    echo $dados['padnumsidoc'];
                }
				
            ?>
        </td>
		<td align='center'> <?php echo $dados['cttnumcontrato']; ?> </td>
		<td align='center' colspan="2" > <?php echo $dados['orgdsc']; ?> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' align='center' colspan='3'> <b> Ag&ecirc;ncia </b> </td>
		<td bgcolor='#e9e9e9' align='center' colspan='2'> <b> Campanha </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Programa de Trabalho - PT </b> </td>
		
	</tr>
	<tr>
		<td align='center' colspan='3'> <?php echo $dados['fornome']; ?> </td>
		<td align='center' colspan='2'> <?php echo $dados['camtitulo']; ?> </td>
		<td align='center'> <?php echo $dados['padptres']; ?></td>
		<?php if($isXls)
		{
		?>
		<td align='center'> </td>
		<?php } ?>
	</tr>
	
</table>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor='#e9e9e9' width='50%' align='left' colspan='7'> <b> Itens da PAD: </b> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' align='center'> <b> N&uacute;mero </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Descri&ccedil;&atilde;o </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> Fornecedor </b> </td>
		
		<td bgcolor='#e9e9e9' align='center'> <b> Valor Total </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> N&ordm; da Fatura da Ag&ecirc;ncia </b> </td>
		<td bgcolor='#e9e9e9' align='center'> <b> N&ordm; da OB </b> </td>
	</tr>
	<?php 
	if(is_array($dadosItem) && count($dadosItem) >= 1){
		foreach($dadosItem as $key => $value){
	?>
	<tr>
		<td align='center'> <?php echo $value['ipanumitempad']; ?> </td>
		<td align='left'> <?php echo $value['ipadsc']; ?> </td>
		<td align='left'> <?php echo $value['fornome']; ?> </td>
		<td align='right'> <?php echo number_format($value['ipavalortotal'],2,',','.'); ?> </td>
		<td align='center'> <?php echo $value['fipnumfaturaagencia']; ?> </td>
		<td align='center'> <?php echo $value['pipnumordembancaria']; ?> </td>
	</tr>
	<?php 
		}
	}
	?>
	<tr>
		<td bgcolor='#e9e9e9' align='center' colspan='7'> <b> Total da PAD &nbsp;&nbsp;&nbsp;<?php echo number_format($totValor,2,',','.'); ?> </b> </td>
	</tr>
	<tr class='noPrint'>
		<td align='center' colspan='7'>
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