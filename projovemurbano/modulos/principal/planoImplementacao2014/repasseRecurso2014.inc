<?php 

if($_SESSION['projovemurbano']['muncod']) {
	$sugestaoampliacao = carregarSugestaoAmpliacao();
	$meta = carregarMeta($sugestaoampliacao);
} 

if($_SESSION['projovemurbano']['estuf']) {
	$sugestaoampliacao = carregarSugestaoAmpliacao();
	$meta = carregarMeta($sugestaoampliacao);
}

$cmemeta = $meta;
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarQualificacaoProfissional() {

	jQuery('#form').submit();
}

function visualizarFormula()
{
	var janela = window.open( 'projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&requisicao=popUpFormula2014', 'formula', 'width=800,height=400,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
    janela.focus();
}

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarQualificacaoProfissional">

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
					<td><?php echo number_format($cmemeta,0,',','.') ?></td>
					<?
					if( $_SESSION['projovemurbano']['ppuid'] == '3' ){
						$vlrpercapita = pegaValorPercapta();
					}else{
						if($_SESSION['projovemurbano']['muncod']) { 
							echo "<td>R$ 165,00</td>";
							$vlrpercapita = 165;
						}
						
						if($_SESSION['projovemurbano']['estuf']) {
							echo "<td>R$ 170,00</td>";
							$vlrpercapita = 170;
						}
					}
					?>
					<td><?=formata_valor($vlrpercapita) ?></td>
					<td>18</td>
					<td><?php echo number_format( ( $cmemeta*$vlrpercapita*18 ) ,2,',','.') ?></td>
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
					echo number_format($cmemeta*54,2,',','.');
					?>
					</td>
				</tr>
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Valor 1ª Parcela</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td>
						<?php
						 
						$per_capta = pegaValorPercapta();
						$m = 6;
						//Vr1 = EM1 x [(m x 94% x Vpc) + (18 x 1% x Vpc) + (12 x 5% x Vpc)] + EM1 x R$ 54,00
						if($_SESSION['projovemurbano']['muncod'] && $_SESSION['projovemurbano']['muncod'] != '5300108') {
							$valorTotal = $cmemeta*( ( $m*0.89*$per_capta ) + ( 18*0.01*$per_capta ) + ( 0.1*12*$per_capta ) ) + ($cmemeta*54);
						}
						//Vr1 = EM1 x [(m x 92,5% x Vpc) + (1,5% x 18x Vpc) + (1% x18x Vpc) + (12 x 5% x R$ Vpc)] + ( EM1 x R$ 54,00 )
						if($_SESSION['projovemurbano']['estuf']  || $_SESSION['projovemurbano']['muncod'] == '5300108' ) {
							$valorTotal = $cmemeta*( ( $m*0.875*$per_capta ) + ( 0.015*18*$per_capta ) + ( 0.01*18*$per_capta ) + ( 12*0.1*$per_capta ) ) + ($cmemeta*54);
						}
						
						echo number_format($valorTotal,2,',','.');
						 
						?> (valor demostrativo) <input type="button" onclick="visualizarFormula()" name="btn_visualizar_formula" value="Visualizar Fórmula" />
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<? 
// 	$cmddsc = $db->pegaUm("SELECT cmddsc FROM workflow.comentariodocumento WHERE docid='".$docid."' AND hstid IN(SELECT hstid FROM workflow.documento WHERE docid='".$docid."')");

// 	if($cmddsc) :
	?>
<!-- 	<tr> -->
	<!-- 	<td class="SubTituloDireita" style="background-color:red"> -->
<!-- 		<b>Observações sobre Plano de Implementação</b> -->
<!-- 		</td> -->
<!-- 		<td> -->
		<?
// 		=$cmddsc 
?>
<!-- 		</td> -->
<!-- 	</tr> -->
	<?
// 	 endif; 
?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?php echo $_GET['aba'] ?>&aba2=resumoFinanceiro';">
			<!-- 
			<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro';"></td>
			 -->
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>