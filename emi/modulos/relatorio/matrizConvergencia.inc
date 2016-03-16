<?php
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo("Matriz de Convergência", "");

?>

<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="2" cellpadding="4" align="center" style="border-collapse: collapse;">
	<tr>
		<td></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="5">
			<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
		</td>
	</tr>
</table>