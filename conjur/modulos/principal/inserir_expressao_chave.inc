<?
	$id_linha = ($_REQUEST["id_linha"] == "") ? 0 : $_REQUEST["id_linha"];
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top:none; border-bottom:none; width:100%;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;"> 
				Determine a Expressão Chave:
			</label>
		</td>
	</tr>
</table>
<br/>
<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tbody>
	<tr>
		<td width="20%" class="SubTituloDireita">Expressão:</td>
		<td width="80%"><input type="text" class="normal" id="expressao_chave" size="30" maxlength="20" value="<?=$_REQUEST["expressao_chave"]?>"></td>
	</tr>
	<tr>
		<td width="100%" colspan="2" class="SubTituloDireita" style="text-align:center;">
			<input type="button" value="Cadastrar" style="cursor: pointer" onclick="cadastraExpressao(<?=$id_linha?>);">
			<input type="button" value="Cancelar" style="cursor: pointer" onclick="self.close();">
		</td>
	</tr>
</tbody>
</table>
