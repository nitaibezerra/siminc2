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
	<? carregarInformes(array('pflcoddestino' => PFL_PROFESSORALFABETIZADOR)); ?>
	</td>
</tr>
<? $perfis = pegaPerfilGeral(); ?>
<? if($_SESSION['sispacto']['professoralfabetizador']['iusdesativado']!='t') : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dados';">
		</td>
	</tr>
<? elseif($db->testa_superuser() || in_array(PFL_CONSULTAMEC,$perfis)) : ?>
	<tr>
		<td colspan="2">
			<p>Os perfis de Adminstrador e Consulta MEC podem acessar as informações dos professores clicando</p>
			<input type="button" value="Acessar Professor Desabilitado" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&requisicao=visualizarDesabilitado&vis=professoralfabetizador';">
		</td>
	</tr>
<? endif; ?>
</table>