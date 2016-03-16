<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Título da página abaixo da aba
monta_titulo("Relatório Analítico");
?>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Opção Analítica: </td>
			<td class="campo">
				Data de início <?=campo_data2('bendtentrada','N','S','Informe a Data da Entrada','S','','',$bendtentrada);?>
				Data de fim <?=campo_data2('bendtsaida','N','S','Informe a Data da Saída','S','','',$bendsaida);?>
            </td>
		</tr>	
		<tr>
			<td class="SubtituloDireita"> Tipo: </td>
			<td class="campo">
				<input type="radio" name="relatorio" value="entrada" /> Entrada
				<input type="radio" name="relatorio" value="saida" /> Saída
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2' class="SubTituloCentro" align="center">
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Gerar Relatório' onclick="filtrar('filtrar')" />
				<input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar Campos' />
		    </td>
		</tr>
	</table>
</form>