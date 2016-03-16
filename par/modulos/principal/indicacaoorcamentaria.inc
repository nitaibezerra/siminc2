<?php
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<form method="post" name="formulario" id="formulario">
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td class="textosTabela"></td>
			<td class="camposTabela"></td>
			<td class="textosTabela"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
	<tr>
		<td class="SubTituloDireita">PTRES:</td>
		<td colspan="3"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Plano Interno (PI):</td>
		<td colspan="3"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Natureza de Despesa:</td>
		<td colspan="3"></td>
	</tr>
	<tr>
		<td></td>
		<td colspan="3">
			<input class="botao" type="submit" name="Salvar" value="Salvar" />
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/indicacaoorcamentaria&acao=A';" />
		</td>
	</tr>
	</tbody>
</table>
</form>