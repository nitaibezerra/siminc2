<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">
	this._closeWindows = false;
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

	<tr style="background-color: rgb(204, 204, 204);">
		<td align="left" colspan="2"><b>Itens Finaciáveis:</b></td>
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Grupo de Despesa:</td>
		<td><?=campo_texto('Grupo de Despesa','S','S','',100,255,'','','','','','id="grupoDespesa"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Item:</td>
		<td><?=campo_texto('Item','S','S','',100,255,'','','','','','id="item"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Unidade de Medida:</td>
		<td><?=campo_texto('Unidade de Medida','S','S','',100,255,'','','','','','id="unidadeMedida"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Valor Unitário:</td>
		<td><?=campo_texto('Valor Unitário','S','S','',100,255,'','','','','','id="valorUnitario"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Observações:</td>
		<td><?=campo_textarea('Obesevações','S','S','', 104, 5, '', '', '', false, null, null); ?></td>	
	</tr>
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<input style="cursor:pointer;" type="submit" value="Salvar" />
			<input style="cursor:pointer;" type="button" value="Cancelar"/>
		</td>
	</tr>
</table>