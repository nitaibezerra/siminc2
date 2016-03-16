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
				<?
				 
				$sql = "SELECT uu.unisigla||' - '||uu.uninome as universidade, g.gdedesc, 'Verba', o.orcvlrunitario, replace(o.orcdescricao,'\n','<br>')
						FROM sisindigena2.orcamento o
						INNER JOIN sisindigena2.grupodespesa g ON g.gdeid = o.gdeid
						INNER JOIN sisindigena2.nucleouniversidade u ON u.picid = o.picid
						INNER JOIN sisindigena2.universidade uu ON uu.uniid = u.uniid
						WHERE u.uncid='".$_SESSION['sisindigena2']['universidade']['uncid']."' AND o.orcstatus='A'
						ORDER BY g.gdedesc";
				
				$cabecalho = array("Núcleo","Grupo de Despesa","Unidade de Medida","Valor total (R$)","Detalhamento");
				$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
				
				
				?>
				</td>
			</tr>
		</table>
		
	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloCentro" colspan="2">Núcleos</td>
			</tr>
			<tr>
				<td colspan="2">
				<? carregarNucleosUniversidades(array('uncid'=>$_SESSION['sisindigena2']['universidade']['uncid'],'consulta' => true)); ?>
				</td>
			</tr>
		</table>
		
		
		
		</td>
		<td width="3%" valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sisindigena2']['universidade']['docid'], array('uncid' => $_SESSION['sisindigena2']['universidade']['uncid']) );
		?>
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center">
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=nucleos';"> 
		</td>
	</tr>
</table>
	