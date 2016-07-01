<?php
// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';  

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Itens Finaciáveis','');

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr style="background-color: rgb(204, 204, 204);">
		<td align="left" colspan="2"><b>Quantitativos:</b></td>
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Unidade de Medida:</td>
		<td><?=campo_texto('Unidade de Medida','N','S','',100,255,'','','','','','id="unidadeMedida2"','',''); ?></td>	
	</tr>
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<input style="cursor:pointer;" type="submit" value="Pesquisar" />
			<input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="academico_verTodasUnidades();" />
		</td>
	</tr>
</table>