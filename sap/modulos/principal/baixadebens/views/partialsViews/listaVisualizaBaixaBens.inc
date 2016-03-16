<?php
$contasContabeis = $oBensBaixa->listaContaContabilBaixa($_GET['bebid']);

if(is_array($contasContabeis) && count($contasContabeis) >= 1){
	$qtdRegistros = '';
	$totalGeral = '';
	foreach($contasContabeis as $key => $value){
?>
<br />
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td> <u>Conta Contábil <?php echo $value['ccbid'].': '.$value['ccbdsc']; ?></u> </td>
	</tr>
</table>
<?php 
		$result = $oBensBaixa->pegaDadosVisualizaTermo($_GET['bebid'],$value['ccbid']);
		
		if(is_array($result) && count($result) >= 1){
			$subTotal = '';
			$qtdRegistros = $qtdRegistros + count($result);
			echo "<table class=\"listagem\" cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"95%\">";
			echo "<thead>";
			echo "<tr>";
			echo "<td width=\"8%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>RGP</strong></td>";
			echo "<td width=\"15%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>N. de Série</strong></td>";
			echo "<td width=\"60%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Descrição</strong></td>";
			echo "<td width=\"9%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>E. Conservação</strong></td>";
			echo "<td width=\"8%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Valor</strong></td>";
			echo "</tr>";
			echo "</thead>";
			echo "<tbody>";
			foreach($result as $key2 => $value2){
				$bmtvlrunitario = number_format($value2['bmtvlrunitario'],2,',','.');
				
				echo "<tr bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\">
					  	<td align=\"center\" title=\"RGP\">".$value2['rgpnum']."</td>
						<td align=\"center\" title=\"N. de Série\">".$value2['rgpnumserie']."</td>
						<td align=\"center\" title=\"Descrição\">".$value2['bmtdscit']."</td>
						<td align=\"center\" title=\"E. Conservação\">".$value2['ecodsc']."</td>
						<td align=\"center\" title=\"Valor\">R$ ".$bmtvlrunitario."<br></td>
					</tr>";	
				$soma = new Math($subTotal, $value2['bmtvlrunitario'],2);
				$subTotal = $soma->sum()->getResult();
			}
			$soma = new Math($totalGeral, $subTotal,2);
			$totalGeral = $soma->sum()->getResult();
			echo "</tbody>";
			echo "<thead>
				      <tr>
						<td align=\"center\" title=\"RGP\"><b>Subtotal</b> </td>
						<td align=\"center\" title=\"N. de Série\"></td>
						<td align=\"center\" title=\"Descrição\"></td>
						<td align=\"center\" title=\"E. Conservação\"></td>
						<td align=\"center\" title=\"Valor\"><b>R$ ".number_format($subTotal,2,',','.')."</b></td>
					  </tr>
				  </thead>";
			echo "</table>";
		}
	}
?>
<br />
<table class="bordasimples" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td> <b>Total de Registros:</b> </td>
		<td>&nbsp;</td>
		<td width="60%">&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td align="center"><b><?php echo $qtdRegistros; ?></b></td>
	</tr>
	<tr>
		<td> <b>Total Geral:</b> </td>
		<td>&nbsp;</td>
		<td width="60%">&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td align="center"><b>R$ <?php echo formata_valor($totalGeral); ?></b></td>
	</tr>
	
</table>
<?php 
}
?>