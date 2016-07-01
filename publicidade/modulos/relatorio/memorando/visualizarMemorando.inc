<?php 
$isXls = $_REQUEST['isXls'];
if( ! $isXls )
{
?>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
<?php 
}?>
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
if($isXls)
{
	$file_name="Memorando.xls";
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
$oParametro = new Parametro();
$oHistorico = new HistoricoDespachoMemorando();

//pega alguns dados do relatório
$dados = $oPad->pegar_dados_memorando($_GET['padnumero']);

if(empty( $dados['padnumsidoc'] ) )
{
	echo "<script> alert('Informe o No do Sidoc no cadastro da PAD na aba \'Cadastro PAD\'')</script>";
	echo "<script> self.close(); </script>";
}
if(empty( $dados['padptres'] ) )
{
	echo "<script> alert('Informe o campo \'Programa de Trabalho - PT\' no cadastro da PAD na aba \'Cadastro PAD\'')</script>";
	echo "<script> self.close(); </script>";
}

//pega os itens pad
$ipaids = explode(',',$_GET['ipaids'],-1);

//pega os dados dos itens
$dadosItens = $oItemPad->pegar_dados_memorando($ipaids);

//pega o fiscal do contrato
$fiscal = $oParametro->pegar_fiscal($_GET['fiscal']);
if($_GET['fiscal'] == 'S'){
	$tipoFiscal = 'Substituto';
}
else if($_GET['fiscal'] == 'T'){
	$tipoFiscal = '';
}

//pega o chefe ACS
$chefe = $oParametro->pegar_chefe($_GET['chefe']);
if($_GET['chefe'] == 'S'){
	$tipoChefe = 'Substituto';
}
else if($_GET['chefe'] == 'T'){
	$tipoChefe = '';
}

//grava o despacho no histórico
$oHistorico->salvar($_GET);

/*$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				    "<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				    "</td>" .
			        "</tr>
			        </table>";

echo $cabecalhoBrasao;


monta_titulo('Documento de Encaminhamento Para Pagamento','');*/

?>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
		<td align="left" bgcolor="#4D4D4D" width="5%" style="color: #FFFFFF;"><b>MEC</b></td>
		<td <?php if($isXls ){ ?> colspan="2" <?php } ?> >Minist&eacute;rio da Educa&ccedil;&atilde;o</td>
	</tr>
	<tr>
		<td align="left" bgcolor="#4D4D4D" width="5%" style="color: #FFFFFF;"><b>ACS</b></td>
		<td <?php if($isXls ){ ?> colspan="2" <?php } ?> >Assessoria de Comunica&ccedil;&atilde;o Social - GM</td>
	</tr>
</table>


<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tbody>
		<tr>
			<td rowspan="3" align="center" width="80%" bgcolor="#e9e9e9">
				<label class="TituloTela" style="color: rgb(0, 0, 0);">Documento de Encaminhamento Para Pagamento</label>
			</td>
		</tr>
		<tr>
			<td <?php if($isXls ){ ?> colspan="2" <?php } ?> align="center" bgcolor="#e9e9e9">N&uacute;mero/Ano</td>
		</tr>
		<tr>
			<td align="center" <?php if($isXls ){ ?> colspan="2" <?php } ?> ><?php echo $_GET['nummemorando'].' / '.date('Y'); ?></td>
		</tr>
	</tbody>
</table>

<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor='#4D4D4D' colspan='6' align='center' style="color: #FFFFFF;"> <b> Descri&ccedil;&atilde;o </b> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' width='30%' align='center'> <b> Processo </b> </td>
		<td bgcolor='#e9e9e9' width='30%' align='center'> <b> Programa de Trabalho </b> </td>
		<td bgcolor='#e9e9e9' width='20%' align='center'> <b> Natureza de Despesa  </b> </td>
		<td bgcolor='#e9e9e9' width='30%' align='center'> <b> PAD/SIREF </b> </td>
		<td bgcolor='#e9e9e9' width='20%' align='center'> <b> Exerc&iacute;cio </b> </td>
		<td bgcolor='#e9e9e9' width='30%' align='center'> <b> DATA </b> </td>
	</tr>
	<tr>
		<td align='center'> <?php echo $dados['padnumsidoc']; ?> </td>
		<td align='center'> <?php echo $dados['padptres']; ?> </td>
		<td align='center'> 333.90.39 </td>
		<td align='center'> <?php echo $_GET['padnumero']; ?> </td>
		<td align='center'> <?php $ano_exercicio = explode("/",$_GET['padnumero']); echo $ano_exercicio[0];  //echo $_SESSION['exercicio']//date('Y'); ?> </td>
		<td align='center'> <?php echo $_GET['dataemissao']; ?> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' width='50%' align='center' colspan='2' > <b> Favorecido </b> </td>
		<td bgcolor='#e9e9e9' width='50%' align='center' colspan='4'> <b> Contrato N&ordm; </b> </td>
	</tr>
	<tr>
		<td align=left colspan='2'> <?php echo utf8_decode($dados['fornome']); ?> </td>
		<td align='center' colspan='4'> <?php echo $dados['cttnumcontrato']; ?> </td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' width='50%' align='left' colspan='2'> <b> Nota(s) Fiscal(s) </b> </td>
		<td bgcolor='#e9e9e9' width='50%' align='center' colspan='4'> <b> Valor (R$) </b> </td>
	</tr>
	<?php 
	if(count($dadosItens) >= 1){
		foreach($dadosItens as $key => $value){
	?>
	<tr>
		<td align='left' colspan='2'> <?php echo $value['fipnumfaturaagencia']; ?> </td>
		<td align='center' colspan='4'> <?php echo formata_valor($value['fipvalortotal']); ?> </td>
	</tr>
	<?php 	
		}
	}
	?>
	<tr>
		<td bgcolor='#e9e9e9' colspan='6' width='50%' align='left'> <b> Objeto </b> </td>
	</tr>
	<tr>
		<td align='left' colspan='6'> <?php echo $_GET['objeto']; ?> </td>
	</tr>
	
	<tr>
		<td bgcolor='#e9e9e9' colspan='6' width='50%' align='center'> <b> Despacho </b> </td>
	</tr>
	<tr>
		<td align='left' colspan='6'> 
		<?php echo $_GET['despacho']; ?> <br /><br /><br /><br />
		
		<p style="text-align: center;">
		<b><?php echo $fiscal; ?></b><br />
		Fiscal do Contrato <?php echo $tipoFiscal; ?> <br />
		ACS/GM/MEC
		</p>
		</td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' colspan='6' width='50%' align='left'> <b> De Acordo </b> </td>
	</tr>
	<tr>
		<td align='center' colspan='6'> 
		<b><?php echo $chefe; ?></b><br />
		Chefe da Assessoria de Comunica&ccedil;&atilde;o Social <?php echo $tipoChefe; ?> - ACS/GM/MEC
		</td>
	</tr>
	<tr>
		<td bgcolor='#e9e9e9' colspan='6' width='50%' align='left'> <b> Observa&ccedil;&atilde;o </b> </td>
	</tr>
	<tr>
		<td align='center' colspan='6'> <?php echo $_GET['obs']; ?> </td>
	</tr>
	<tr class='noPrint'>
		<td align='center' colspan='6'>
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