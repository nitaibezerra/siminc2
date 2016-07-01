<?php
echo "<br>";
echo montarAbasArray( criaAbaNovasTurmas(), "proinfantil.php?modulo=novasturmas/questionarioAtendimento&acao=A&abas=questionarioTurmas&muncod={$_REQUEST['muncod']}" );

$obControle = new CampoExternoControle();
?>
<form name="formulario" id="formulario" method="post" action="" >			
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td><?//=$obControle->numAtualTurmasNT(); ?></td>
		</tr>
		<tr bgcolor="#cccccc" align="center">
			<td><input onclick="" value="Salvar" type="button" id="btn_qst_salvar"></td>
		</tr>
	</table>
</form>