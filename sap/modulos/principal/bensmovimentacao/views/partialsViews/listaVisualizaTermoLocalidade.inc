<?php 
$contasContabeis = $oMovimentacao->listaContaContabilBem($_GET['mvbid']);

if(is_array($contasContabeis) && count($contasContabeis) >= 1){
	$qtdRegistros = '';
	$totalGeral = '';
	foreach($contasContabeis as $key => $value){
		$subTotalQtd = 0;
		$i = 1;
?>
<br />
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td> <u>Conta Contábil <?php echo $value['ccbid'].': '.$value['ccbdsc']; ?></u> </td>
	</tr>
</table>
<?php 
		$result = $oMovimentacao->pegaDadosVisualizaTermo($_GET['mvbid'],$value['ccbid']);
		
		if(is_array($result) && count($result) >= 1){
			$subTotal = '';
			$qtdRegistros = $qtdRegistros + count($result);
			echo "<table class=\"listagem\" cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"95%\">";
			echo "<thead>";
			echo "<tr>";
			echo "<td width=\"4%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Item</strong></td>";
			echo "<td width=\"8%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>RGP</strong></td>";
			echo "<td width=\"15%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>N. de Série</strong></td>";
			echo "<td width=\"65%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Descrição</strong></td>";
			//echo "<td width=\"20%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Antigo Responsável</strong></td>";
			echo "<td width=\"8%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Valor</strong></td>";
			echo "</tr>";
			echo "</thead>";
			echo "<tbody>";
			foreach($result as $key2 => $value2){
				$bmtvlrunitario = number_format($value2['bmtvlrunitario'],2,',','.');
				
				echo "<tr bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\">";
				echo "<td align=\"center\" title=\"Item\">".$i."</td>";
				echo "<td align=\"center\" title=\"RGP\">".$value2['rgpnum']."</td>";
				echo "<td align=\"center\" title=\"N. de Série\">".$value2['rgpnumserie']."</td>";
				echo "<td align=\"center\" title=\"Descrição\">".$value2['bmtdscit']."</td>";
				//echo "<td align=\"center\" title=\"Garantia\">".$value2['no_servidor']."</td>";
				echo "<td align=\"center\" title=\"Valor\">R$ ".$bmtvlrunitario."<br></td>";
				echo "</tr>";	
				$soma = new Math($subTotal, $value2['bmtvlrunitario'],2);
				$subTotal = $soma->sum()->getResult();
				$i++;
				$subTotalQtd++;
			}
			$soma = new Math($totalGeral, $subTotal,2);
			$totalGeral = $soma->sum()->getResult();
			echo "</tbody>";
			echo "<tr>
					<td align=\"center\">&nbsp;</td>
			      	<td align=\"center\">&nbsp;</td>
					<td align=\"center\">&nbsp;</td>
					<td align=\"center\">&nbsp;</td>
					<td align=\"center\">&nbsp;</td>
				  </tr>";
			echo "<tr>
					<td align=\"center\"><b>Subtotal</b> </td>
				    <td align=\"center\"></td>
					<td align=\"center\"></td>
					<td align=\"center\"></td>
					<td align=\"center\"></td>
				  </tr>";
			echo "<thead>
				      <tr>
						<td align=\"center\"><b>".$subTotalQtd."</b> </td>
				      	<td align=\"center\"></td>
						<td align=\"center\"></td>
						<td align=\"center\"></td>
						<td align=\"center\"><b>R$ ".number_format($subTotal,2,',','.')."</b></td>
					  </tr>
				  </thead>";
			echo "</table>";
		}
	}
?>
<br />
<table class="bordasimples" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td> <b>Total de Itens:</b> </td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td align="center" width="8%"><b><?php echo $qtdRegistros; ?></b></td>
	</tr>
	<tr>
		<td> <b>Total Geral:</b> </td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td align="center" width="8%"><b>R$ <?php echo formata_valor($totalGeral); ?></b></td>
	</tr>
	<tr>
		<td colspan='6'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro que recebi e conferi os bens constantes, no anexo do presente Termo de Responsabilidade e assumo os seguintes compromissos: 1) zelar
						pela boa conservação dos bens nele constantes e diligenciar no sentido da recuperação daqueles que se avariarem; 2) assumir a efetiva
						responsabilidade pela guarda e uso dos mesmos; 3) que nenhum equipamento ou material permanente será movimentado sem comunicação
						expressa à  Divisão de Patrimônio deste ministério; 4) comunicar à  Divisão de Patrimônio, independente de levantamento, qualquer irregularidade de
						funcionamento ou danificação nos materiais sob minha responsabilidade; e 5) comunicar, à  autoridade superior, qualquer irregularidade ocorrida com
						o material sob meus cuidados.<br />
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Estou ciente, ainda, de que todo servidor público poderá ser chamado à  responsabilidade pelo desaparecimento do material que lhe for confiado,
						para guarda ou uso, bem como pelo dano que, dolosa ou culposamente, causar a qualquer material, esteja ou não sob sua guarda, conforme
						preceitua o art. 90 do Decreto-Lei no 200/67; c/c art. 39 de Decreto no 93872/86; e Instrução Normativa SEDAP/PR 205/88.
		</td>
	</tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td>&nbsp;</td>
		<td width="60%">&nbsp;</td>
	</tr>
	<tr>
		<td><div><p style="text-align: center">Local e Data:__________________________________________, __/__/____</p></div></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td><div><p style="text-align: center">_________________________________________</p><p style="text-align: center"><?php echo $no_servidoratual; ?></p></div></td>
		<td>&nbsp;</td>
	</tr>
</table>
<?php 
}
?>