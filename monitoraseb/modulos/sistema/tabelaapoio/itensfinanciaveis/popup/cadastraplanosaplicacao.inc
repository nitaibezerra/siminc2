<?php
// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';  

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Itens Finaciáveis','');

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

	<tr style="background-color: rgb(204, 204, 204);">
		<td align="left" colspan="2"><b>Itens Finaciáveis:</b></td>
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Grupo de Despesa:</td>
		<td><?=campo_texto('Grupo de Despesa','N','S','',100,255,'','','','','','id="grupoDespesa"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Item:</td>
		<td><?=campo_texto('Item','N','S','',100,255,'','','','','','id="item"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Unidade de Medida:</td>
		<td><?=campo_texto('Unidade de Medida','N','S','',100,255,'','','','','','id="unidadeMedida"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Valor Unitário:</td>
		<td><?=campo_texto('Valor Unitário','N','S','',100,255,'','','','','','id="valorUnitario"','',''); ?></td>	
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Observações:</td>
		<td><?=campo_textarea('Obesevações','N','S','', 104, 5, '', '', '', false, null, null); ?></td>	
	</tr>
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<input style="cursor:pointer;" type="submit" value="Pesquisar" />
			<input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="academico_verTodasUnidades();" />
		</td>
	</tr>
	
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
	
	
	<tr style="background-color: rgb(204, 204, 204);">
		<td align="left" colspan="2"><b>Planos de Aplicação:</b></td>
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Código / Aplicação:</td>
		<td>
			<?=campo_texto('Unidade de Medida','N','S','',15,255,'','','','','','id="unidadeMedida2"','',''); ?>
			<?=campo_texto('Unidade de Medida','N','S','',85,255,'','','','','','id="unidadeMedida2"','',''); ?>
		</td>	
	</tr>
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<input style="cursor:pointer;" type="submit" value="Pesquisar" />
			<input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="academico_verTodasUnidades();" />
		</td>
	</tr>
</table>