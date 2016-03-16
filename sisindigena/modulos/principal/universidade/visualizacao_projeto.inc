<script>

jQuery(document).ready(function() {
	jQuery("[id^='img2_']").each(function() {
		jQuery(this).click();
	});

	jQuery("[id^='img_']").each(function() {
		jQuery(this).click();
	});
});

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Visualização do Plano de Trabalho</td>
	</tr>
	<tr>
		<td>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloCentro" colspan="2">Orçamento</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
				<td>
				<? carregarListaCustos(array("consulta"=>true,"uncid"=>$_SESSION['sisindigena']['universidade']['uncid'])); ?>
				</td>
			</tr>
		</table>
		
	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloCentro" colspan="2">Núcleos</td>
			</tr>
			<tr>
				<td colspan="2">
				<? carregarNucleosUniversidades(array('uncid'=>$_SESSION['sisindigena']['universidade']['uncid'],'consulta' => true)); ?>
				</td>
			</tr>
		</table>
		
		
		
		</td>
		<td width="3%" valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sisindigena']['universidade']['docid'], array('uncid' => $_SESSION['sisindigena']['universidade']['uncid']) );
		?>
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center">
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=nucleos';"> 
		</td>
	</tr>
</table>
	