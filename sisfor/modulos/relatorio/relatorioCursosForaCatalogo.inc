<?php 
if($_REQUEST['requisicao']=='excel'){
	relatorioCursosForaCatalogo('excel');
	exit();
}

require_once APPRAIZ . "includes/cabecalho.inc";

monta_titulo('Cursos Propostos pela IES – Cursos Fora do Catálogo MEC', '&nbsp;'); ?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	function gerarRelatorio(tipo){
		jQuery('#requisicao').val(tipo);
		jQuery('#formulario').submit();
	}
</script>
<form name="formulario" id="formulario" method="post">
	<input type="hidden"  name="requisicao" value="html" id="requisicao">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td align="center" colspan="2">
				<input type="submit" value="Gerar Excel" onclick="gerarRelatorio('excel');">
			</td>
		</tr>
	</table>
</form>

<?php relatorioCursosForaCatalogo('html'); ?>