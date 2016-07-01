<script>
jQuery(document).ready(function() {
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
		
		<? exibirCadastramentoCoordenadorAdjunto(array('picid' => $_SESSION['sisindigena']['coordenadoradjuntoies']['picid'], 'picsede' => $_SESSION['sisindigena']['coordenadoradjuntoies']['picsede'])); ?>
 	
		
		
		</td>
		<td width="3%" valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sisindigena']['coordenadoradjuntoies']['docid'], array('picid' => $_SESSION['sisindigena']['coordenadoradjuntoies']['picid']) );
		?>
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center">
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=definirequipe';"> 
		</td>
	</tr>
</table>
	