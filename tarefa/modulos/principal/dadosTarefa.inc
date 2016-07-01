<tr>
	<td align="left" colspan="2"><b>Dados da Tarefa</b></td>
</tr>
<tr>
	<td class="SubTituloDireita" >
			Título:
	</td>
	<td><?$tartitulo = $obTarefa->tartitulo; ?>
		<?= campo_texto( 'tartitulo', 'S', $permissao_formulario, 'Título', 60, 255, '', '','','','','id="tartitulo"'); ?>
		<?$barras = array(" \", " /", "\ ", "/ ""); //Função implementada para correção de erro. Desenvolvedor: Afonso Alves 
		  $tartitulo = str_replace($barras,"/",$obTarefa)?> 
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" >
			Modalidade:
	</td>
	<td>
	<?php
	$sql = "select tmdid, tmddescricao from tarefa.tipomodalidade";
	$arTipoModalidade = $db->carregar($sql);
	$arTipoModalidade = (is_array($arTipoModalidade)) ? $arTipoModalidade : array();
	foreach($arTipoModalidade as $modalidade){
		if($obTarefa->tmdid == $modalidade['tmdid']){
			$ckecked = "checked=\"checked\"";
		}
		echo "<input type=\"radio\" $ckecked id=\"{$modalidade['tmddescricao']}\" title=\"Modalidade\" name=\"tmdid\" value=\"{$modalidade['tmdid']}\" align=\"bottom\"><label for=\"{$modalidade['tmddescricao']}\">{$modalidade['tmddescricao']}</label>";
		$ckecked = "";
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" >
			Tema:
	</td>
	<td><?$tartema = $obTarefa->tartema; ?>
		<?= campo_texto( 'tartema', 'S', $permissao_formulario, 'Tema', 60, 50, '', '','','','','id="tartema"'); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">
			Descrição:
	</td>
	<td><?$tardsc = $obTarefa->tardsc; ?>
		<?= campo_textarea( 'tardsc', 'N', 'S', 'Descrição ', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição' ); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Data do Recebimento: </td>
<td>  
<?php 
$tardatarecebimento = $obTarefa->tardatarecebimento;
?>
<?= campo_data( 'tardatarecebimento','S', 'S', 'Data do Recebimento', 'S' ); ?>
</td> 
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Instituição Avaliada:</td>
	<td align="left">
	</td>
</tr>