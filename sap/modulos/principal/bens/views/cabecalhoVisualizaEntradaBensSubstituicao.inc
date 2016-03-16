<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>
</script>
<form name="formularioCadastro" id="formularioCadastro" method="post">
	
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Tipo de Entrada:</b> </td>
			<td class="campo" width="25%"><?=$tesdsc;?></td>
			<td class="SubtituloDireita" width="25%"> <b>Data de Entrada:</b> </td>
			<td class="campo" width="25%"><?=$bendtentrada;?></td>
		</tr>
				
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Número do Processo:</b> </td>
			<td class="campo" width="25%"><?=$bemnumproc;?></td>
			<td class="SubtituloDireita" width="25%"> <b>Data do Processo:</b> </td>
			<td class="campo" width="25%"><?=$bendtproc; ?></td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Número da Nota Fiscal:</b> </td>
			<td class="campo" width="25%"><?=$bennumdoc; ?></td>
			<td class="SubtituloDireita" width="25%"> <b>Data da Nota Fiscal:</b> </td>
			<td class="campo" width="25%"><?=$bendtdoc; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Fornecedor:</b> </td>
			<td class="campo" width="25%"><?=$forrazaosocial; ?></td>
			<td class="SubtituloDireita" width="25%"> <b>Documento do Fornecedor:</b> </td>
			<td class="campo" width="25%"><?=$forcpfcnpj; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
			<td class="campo" width="25%" colspan="3"><?=$benobs; ?></td>
		</tr>
		
	</table>
	
</form>