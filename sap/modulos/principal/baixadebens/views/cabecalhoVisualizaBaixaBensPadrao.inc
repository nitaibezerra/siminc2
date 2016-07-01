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
			<td class="SubtituloDireita" width="25%"> <b>Motivo da Baixa:</b> </td>
			<td class="campo" width="25%"><?=$tesdsc;?></td>
			<td class="SubtituloDireita" width="25%"> <b>Data de Baixa:</b> </td>
			<td class="campo" width="25%"><?=$bebdata;?></td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Número do Processo:</b> </td>
			<td class="campo" width="25%"><?=$bebnumprocesso;?></td>
			<td class="SubtituloDireita" width="25%"> <b>Data do Processo:</b> </td>
			<td class="campo" width="25%"><?=$bebdataprocesso; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Recebedor:</b> </td>
			<td class="campo" width="25%"><?=$bebnomerecebedor; ?></td>
			<td class="SubtituloDireita" width="25%"> <b>CNPJ do Recebedor:</b> </td>
			<td class="campo" width="25%"><?=$bebcnpjrecebedor; ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
			<td class="campo" width="25%" colspan="3"><?=$bebobs; ?></td>
		</tr>
		
	</table>
	
</form>