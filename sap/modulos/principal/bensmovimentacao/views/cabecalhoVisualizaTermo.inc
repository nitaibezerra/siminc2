<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>

<?php 
if($mvbtipo == 'S'){
?>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Tipo de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$tipo;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$mvbdata;?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Número do Termo:</b> </td>
		<td class="campo" width="25%"><?=$ternum;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data do Termo:</b> </td>
		<td class="campo" width="25%"><?=$teremissaodt; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Antigo Responsável:</b> </td>
		<td class="campo" width="25%"><?=$no_servidorantigo; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Matrícula SIAPE:</b> </td>
		<td class="campo" width="25%"><?=$nu_matricula_siapeantigo; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Novo Responsável:</b> </td>
		<td class="campo" width="25%"><?=$no_servidoratual; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Matrícula SIAPE:</b> </td>
		<td class="campo" width="25%"><?=$nu_matricula_siapeatual; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
		<td class="campo" colspan="3"><?=$mvbobservacao; ?></td>
	</tr>
	<?php 
	if(!empty($ternumanterior)){
	?>
	<tr>
		<td colspan='4'> <b>Este documento substitui o Termo de Movimentação de Bens <?php echo $ternumanterior; ?> emitido por este ministério.</b> </td>
	</tr>
	<?php 	
	}
	?>
</table>
<?php 
}
else if($mvbtipo == 'L'){
?>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Tipo de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$tipo;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$mvbdata;?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Antiga Unidade:</b> </td>
		<td class="campo" width="25%"><?=$uornoantigo;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Antigo Endereço:</b> </td>
		<td class="campo" width="25%"><?php echo $endlogantigo, ', ', $endcomantigo, ', ', $enadescricaoantigo, ', ', $salaantigo; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Nova Unidade:</b> </td>
		<td class="campo" width="25%"><?=$uornoatual;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Novo Endereço:</b> </td>
		<td class="campo" width="25%"><?php echo $endlogatual, ', ', $endcomatual, ', ', $enadescricaoatual, ', ', $salaatual; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Número do Termo:</b> </td>
		<td class="campo" width="25%"><?=$ternum;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data do Termo:</b> </td>
		<td class="campo" width="25%"><?=$teremissaodt; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
		<td class="campo" colspan="3"><?=$mvbobservacao; ?></td>
	</tr>
	<?php 
	if(!empty($ternumanterior)){
	?>
	<tr>
		<td colspan='4'> <b>Este documento substitui o Termo de Movimentação de Bens <?php echo $ternumanterior; ?> emitido por este ministério.</b> </td>
	</tr>
	<?php 	
	}
	?>
</table>
<?php 	
}
else if($mvbtipo == 'G'){
?>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Tipo de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$tipo;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data de Movimentação:</b> </td>
		<td class="campo" width="25%"><?=$mvbdata;?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Unidade:</b> </td>
		<td class="campo" width="25%"><?=$uornoatual;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Endereço:</b> </td>
		<td class="campo" width="25%"><?php echo $endlogatual, ', ', $endcomatual, ', ', $enadescricaoatual, ', ', $salaatual; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Número do Termo:</b> </td>
		<td class="campo" width="25%"><?=$ternum;?></td>
		<td class="SubtituloDireita" width="25%"> <b>Data do Termo:</b> </td>
		<td class="campo" width="25%"><?=$teremissaodt; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Novo Responsável:</b> </td>
		<td class="campo" width="25%"><?=$no_servidoratual; ?></td>
		<td class="SubtituloDireita" width="25%"> <b>Matrícula SIAPE:</b> </td>
		<td class="campo" width="25%"><?=$nu_matricula_siapeatual; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%"> <b>Observação:</b> </td>
		<td class="campo" colspan="3"><?=$mvbobservacao; ?></td>
	</tr>
	<?php 
	if(!empty($ternumanterior)){
	?>
	<tr>
		<td colspan='4'> <b>Este documento substitui o Termo de Movimentação de Bens <?php echo $ternumanterior; ?> emitido por este ministério.</b> </td>
	</tr>
	<?php 	
	}
	?>
</table>
<?php 
}
?>