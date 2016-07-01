<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
	
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Tipo de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$tipo;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$mvbdata;?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Novo Responsável:</b> </td>
		<td class="campo" width="25%"><?=$no_servidor; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Matrícula SIAPE:</b> </td>
		<td class="campo" width="25%"><?=$nu_matricula_siape; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
		<td class="campo" colspan="3"><?=$mvbobservacao; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Responsável pela devolução:</b> </td>
		<td class="campo" width="25%"><?=$nomerespdevolucao; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Matrícula SIAPE:</b> </td>
		<td class="campo" width="25%"><?=$matricularespdevolucao; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Responsável pelo recebimento:</b> </td>
		<td class="campo" width="25%"><?=$nomeresprecebimento; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Matrícula SIAPE:</b> </td>
		<td class="campo" width="25%"><?=$matricularesprecebimento; ?></td>
	</tr>
</table>