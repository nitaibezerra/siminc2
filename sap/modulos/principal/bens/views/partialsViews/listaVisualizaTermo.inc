<?php
$contasContabeis = $oBens->listaContaContabilBem($_GET['benid']);

if(is_array($contasContabeis) && count($contasContabeis) >= 1){
	$qtdRegistros = '';
	$totalGeral = '';
        $totalGeralDepreciado = '';
	foreach($contasContabeis as $key => $value){
?>
<br />
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td> <u>Conta Contábil <?php echo $value['ccbid'].': '.$value['ccbdsc']; ?></u> </td>
	</tr>
</table>
<?php 
		$result = $oBens->pegaDadosVisualizaTermo($_GET['benid'],$value['ccbid']);
		
		if(is_array($result) && count($result) >= 1){
			$subTotal = '';
                        $subTotalDepreciado = '';
                        
			$qtdRegistros = $qtdRegistros + count($result);
			echo "<table class=\"listagem\" cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"95%\">";
			echo "<thead>";
			echo "<tr>";
			echo "<td width=\"8%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>RGP</strong></td>";
			echo "<td width=\"15%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>N. de Série</strong></td>";
			echo "<td width=\"50%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Descrição</strong></td>";
			echo "<td width=\"7%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Garantia</strong></td>";
			echo "<td width=\"10%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Valor Historico</strong></td>";
                        echo "<td width=\"10%\" class=\"title\" bgcolor=\"\" align=\"center\" valign=\"top\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#c0c0c0';\" style=\"border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);\"><strong>Valor Depreciado</strong></td>";
			echo "</tr>";
			echo "</thead>";
			echo "<tbody>";
                       
			foreach($result as $key2 => $value2){
				$bmtvlrunitario = number_format($value2['bmtvlrunitario'],2,',','.');
				
				echo "<tr bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\">
					  	<td align=\"center\" title=\"RGP\">".$value2['rgpnum']."</td>
						<td align=\"center\" title=\"N. de Série\">".$value2['rgpnumserie']."</td>
						<td align=\"center\" title=\"Descrição\">".$value2['bmtdscit']."</td>
						<td align=\"center\" title=\"Garantia\">".$value2['bmtgarantia']."</td>
						<td align=\"center\" title=\"Valor Histórico\">R$ ".$bmtvlrunitario."<br></td>
                                                    <td align=\"center\" title=\"Valor Depreciado\">R$ ".$value2['valor_depreciado']."<br></td>
                                                    
					</tr>";	
				$soma = new Math($subTotal, $value2['bmtvlrunitario'],2);
				$subTotal = $soma->sum()->getResult();
                                
                                $soma2 = new Math($subTotalDepreciado, $value2['valor_depreciado'],2);
				$subTotalDepreciado = $soma2->sum()->getResult();
			}
                        
			$soma = new Math($totalGeral, $subTotal,2);
			$totalGeral = $soma->sum()->getResult();
                        
                        $soma2 = new Math($totalGeralDepreciado, $subTotalDepreciado,2);
			$totalGeralDepreciado = $soma2->sum()->getResult();
                        
                        
			echo "</tbody>";
			echo "<thead>
				      <tr>
						<td align=\"center\" title=\"RGP\"><b>Subtotal</b> </td>
						<td align=\"center\" title=\"N. de Série\"></td>
						<td align=\"center\" title=\"Descrição\"></td>
						<td align=\"center\" title=\"Garantia\"></td>
						<td align=\"center\" title=\"Valor Histórico\"><b>R$ ".number_format($subTotal,2,',','.')."</b></td>
                                                <td align=\"center\" title=\"Valor Depreciado\"><b>R$ ".number_format($subTotalDepreciado,2,',','.')."</b></td>    
					  </tr>
				  </thead>";
			echo "</table>";
		}
	}
?>
<table class="bordasimples" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
		<td width="10%"> <b>Total Geral:</b> </td>
		<td width="13%">&nbsp;</td>
		<td width="50%"> &nbsp;</td>
		<td width="7%">&nbsp;</td>
		<td width="10%" align="center"><b>R$ <?php echo formata_valor($totalGeral); ?></b></td>
                <td width="10%" align="center"><b>R$ <?php echo formata_valor($totalGeralDepreciado); ?></b></td>
	</tr>
	<tr>
		<td><b>Total de Registros: <?php echo $qtdRegistros; ?></b> </td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
                <td>&nbsp;</td>
	</tr>
</table>
<?php 
}
?>