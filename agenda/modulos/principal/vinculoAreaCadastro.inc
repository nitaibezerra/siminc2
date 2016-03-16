<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita" width="200">Área:</td>
		<td>
		<?php
		$area = new AreaEnvolvida();
		$dados = $area->listaCombo( array('notVinculoEvento' => $eveid) );
		echo $db->monta_combo("aevid", $dados, 'S','Selecione...','carregaDirigente', '', '',240,'S','aevid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" valign="top" colspan="2">Dirigente(s) Vinculado(s) à Área</td>
	</tr>
	<tr>
		<td valign="top" id="td_lista_dirigente" colspan="2">
			<span>selecione uma área para visualizar seus dirigentes.</span>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
	   	<input type="button" name="btvoltar" value="Voltar" onclick="location.href='?modulo=principal/listaVinculoArea&acao=A';" class="botao">
	   </td>
	</tr> 	
</table>

