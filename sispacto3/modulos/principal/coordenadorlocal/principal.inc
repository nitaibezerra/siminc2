<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	
	<? carregarInformes(array('pflcoddestino' => PFL_COORDENADORLOCAL)); ?>
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?></td>
</tr>
</table>
</form>