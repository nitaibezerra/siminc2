<?php
echo "<br>";
echo montarAbasArray( criaAbaNovasTurmas(), "proinfantil.php?modulo=novasturmas/questionarioAtendimento&acao=A&abas=questionario&muncod={$_REQUEST['muncod']}" );
?>
<form name="formulario" id="formulario" method="post" action="" >			
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tbody>
		<tr>
			<td class="SubTituloEsquerda">Questionário de Atendimento Novas Turmas</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda">O seu município/DF atende crianças da educação infantil em estabelecimentos educacionais públicos ou em instituições comunitárias, 
					confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público, em tempo parcial ou integral?</td>
		</tr>
		<tr>
			<td><input name="perg[3039]" value="4073" type="radio" checked="checked" onclick="javascript: quest.closeSubPerguntas(3039);"> Sim
				<input name="perg[3039]" value="4073" type="radio" onclick="javascript: quest.closeSubPerguntas(3039);"> Não</td>
		</tr>
		<tr bgcolor="#cccccc" align="center">
			<td><input onclick="" value="Salvar" type="button" id="btn_qst_salvar"></td>
		</tr>
		</tbody>
	</table>
</form>