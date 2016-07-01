<script type="text/javascript">

function mudarPropriedade(tipoElement,targetInput){
	var input = $_(targetInput);
	input.value="";
	if(tipoElement.value == 0){
		new classCpf($_(targetInput));
	}else{
		new classCnpj($_(targetInput));
	}
	
}
</script>
<?php

include APPRAIZ . '/conjur/modulos/controller.inc';
?>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2" align="center"><b>Pesquisar Entidade</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF/CNPJ:</td>
			<td>
			
			<? 
			$obrdesc = $_POST["obrdesc"];
			echo "<input type='radio' name='stoid' value='0' onchange='mudarPropriedade(this,\"cpfcnpj\")'/> CPF";
			echo "<input type='radio' name='stoid' value='1'  onchange='mudarPropriedade(this,\"cpfcnpj\")'/> CNPJ ";
			?>
			<input name="cpfcnpj" id="cpfcnpj" size="20" value="" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align: left; width: 18ex;" title="" type="text">
			
			</td>
		</tr>		
		
</table>
</form>