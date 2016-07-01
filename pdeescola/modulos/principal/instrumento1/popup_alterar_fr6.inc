<?php
pde_verificaSessao();
$prid = ($_REQUEST["priid"] == "") ? 0 : $_REQUEST["priid"];

?>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top:none; border-bottom:none; width:100%;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;"> 
				Altere o problema selecionado:
			</label>
		</td>
	</tr>
</table>
<br/>
<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tbody>
	<tr>
		<td width="20%" class="SubTituloDireita">Problema:</td>
		<td width="80%">
			<textarea class="CampoEstilo" id="alt_problema" cols="60" rows="4"><?=$_REQUEST["descproblema"]?></textarea>
		</td>
	</tr>
	<tr>
		<td width="100%" colspan="2" class="SubTituloDireita" style="text-align:center;">
			<input type="button" value="Alterar" style="cursor: pointer" onclick="alterarProblema();">
		</td>
	</tr>
</tbody>
</table>
<script type="text/javascript">
function alterarProblema() {
	linha = window.opener.document.getElementById('linha_<?=$_REQUEST["contador"]?>');
	
	descProblema = document.getElementById('alt_problema');
	
	linha.cells[0].innerHTML = "<img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Alterar' onclick='windowOpen(\"?modulo=principal/instrumento1/popup_alterar_fr6&acao=A&descproblema="+descProblema.value+"&contador=<?=$_REQUEST["contador"]?>\",\"blank\",\"height=200,width=450,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes\");'> " +
						       "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick='excluiProblema(this.parentNode.parentNode.rowIndex,<?=$prid?>);'>" +
					  	 	   "<input type='hidden' name='pridescricao[]' value='"+descProblema.value+"'>" +
						 	   "<input type='hidden' name='priid[]' value='<?=$_REQUEST["priid"]?>'>";
	
	linha.cells[1].innerHTML = descProblema.value;
	
	self.close();
}
</script>