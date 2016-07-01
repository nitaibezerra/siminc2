<?php 
$meta = pegameta();
$montante = calcularMontante();
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarQualificacaoProfissional() {

	jQuery('#form').submit();
}

function visualizarFormula()
{
	var janela = window.open( 'projovemcampo.php?modulo=principal/planoImplementacao&acao=A&requisicao=popUpFormula', 'formula', 'width=800,height=400,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
    janela.focus();
}

</script>
<form id="form" name="form" method="POST">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color="blue">
				<?=$arrOrientacoes[10] ?>
			</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Total previsto a ser recebido por este Município, caso a meta prevista seja mantida até o final da execução.</td>
		<td>
			<table class="tabela" bgcolor="#d5d5d5" cellSpacing="1" cellPadding="3"	align="center">
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Meta</td>
					<td style="font-weight:bold" >PER CAPTA (R$)</td>
					<td style="font-weight:bold" >Meses de Referência</td>
					<td style="font-weight:bold" >Valor Total</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td><?php echo number_format($meta,0,',','.') ?></td>
					<?
							echo "<td>R$ 340,00</td>";
							$vlrpercapita = 340;
					?>
					<td><?='24' ?></td>
					<td><?php echo number_format( $montante ,2,',','.') ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor previsto para a 1ª parcela de recursos, com base no número de estudantes/meta.</td>
		<td>
			<table class="tabela" bgcolor="#d5d5d5" cellSpacing="1" cellPadding="3"	style="width:500px;" >
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Adicional - Provas</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td>
					<?php
					echo number_format($meta*54,2,',','.');
					?>
					</td>
				</tr>
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Valor 1ª Parcela</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td>
						<?php
						 
						$m = 6;
						
						$valorTotal = $meta*( ( $m*0.875*$vlrpercapita ) + ( 0.015*24*$vlrpercapita ) + ( 0.01*24*$vlrpercapita ) + ( 12*0.1*$vlrpercapita ) ) + ($meta*54);
						
						echo number_format($valorTotal,2,',','.');
						 
						?> (valor demostrativo) <input type="button" onclick="visualizarFormula()" name="btn_visualizar_formula" value="Visualizar Fórmula" />
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro';">
			<!-- 
			<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro';"></td>
			 -->
	</tr>
</table>
</form>
<?registarUltimoAcesso(); ?>