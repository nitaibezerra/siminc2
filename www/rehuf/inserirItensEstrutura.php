<?

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

?>

<html>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form name='formulario'>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">Linha :</td>
	<td><? echo campo_texto('itmdsc', "S", "S", "Descrição do item", 50, 50, "", "", '', '', 0, '' ); ?></td>
</tr>
<tr>
	<td><input type='button' value='Gravar'></td>
</tr>
</table>
</form>
</html>