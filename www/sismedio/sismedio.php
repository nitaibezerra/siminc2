<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

//if((date("w") == 1 || date("w") == 3 || date("w") == 5) && date("H:i") > "01:30" && date("H:i") < "05:00") {
if(date("H:i") > "01:30" && date("H:i") < "05:00") {

	include  APPRAIZ."includes/cabecalho.inc";

	echo "<br>";

	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="font-size:x-large;" align="center"><img src="../imagens/alerta_sistema.gif" border="0" align="absmiddle"> O SISMédio em Manutenção</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td style="font-size:large;" align="center">A manutenção no SISMédio deixará indisponível o sistema nos dias da semana : Segunda Quarta e Sexta (01:30 às 05:00)</td>
	</tr>
	</table>
	<?

	include APPRAIZ . "includes/rodape.inc";

} else {

	//Carrega as funções de controle de acesso
	include_once "controleAcesso.inc";

}
?>