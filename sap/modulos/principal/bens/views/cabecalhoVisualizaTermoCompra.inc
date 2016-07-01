<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>

<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Tipo de Entrada:</b> </td>
		<td class="campo" width="25%"><?=$tesdsc;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data de Entrada:</b> </td>
		<td class="campo" width="25%"><?=$bendtentrada;?></td>
	</tr>
	
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Número do Termo:</b> </td>
		<td class="campo" width="25%"><?=$ternum;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data do Termo:</b> </td>
		<td class="campo" width="25%"><?=$terdata;?></td>
	</tr>
	
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Número do Processo:</b> </td>
		<td class="campo" width="25%"><?=$bemnumproc;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data do Processo:</b> </td>
		<td class="campo" width="25%"><?=$bendtproc; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Número do Empenho:</b> </td>
		<td class="campo" width="25%"><?=$empnumero; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data do Empenho:</b> </td>
		<td class="campo" width="25%"><?=$empdata; ?></td>
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
		<td class="SubtituloDireita" width="25%"> <b>CNPJ do Fornecedor:</b> </td>
		<td class="campo" width="25%"><?=$forcpfcnpj; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
		<td class="campo" width="25%"><?=$benobs; ?></td>
		<td class="campo" width="25%"></td>
		<td class="campo" width="25%"></td>
	</tr>
	<?php 
	if(!empty($ternumanterior)){
	?>
	<tr>
		<td colspan='4'> <b>Este documento substitui o Termo de Entrada de Bens <?php echo $ternumanterior; ?> emitido por este ministério.</b> </td>
	</tr>
	<?php 	
	}
	?>
</table>