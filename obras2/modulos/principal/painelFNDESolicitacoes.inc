<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$titulo_modulo = 'Central de Solicitações';
monta_titulo($titulo_modulo, '');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" id="formulario" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			<center>
				<input name="btn_abrir" value="Abrir Painel Solicitações Desembolso" type="button" onclick="abrirPopUpDesembolso();">
				<input name="btn_abrir" value="Abrir Painel Solicitações Municipal" type="button" onclick="abrirPopUpMunicipal();">
			</center>
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">

function abrirPopUpDesembolso(){
	document.formulario.action = '../obras2/popPainelFNDESolicitacoes.php';

	var janela = window.open( '', 'modopop', 'width=780,height=465,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
	document.formulario.target = 'modopop';
	document.formulario.submit();
}

function abrirPopUpMunicipal(){
	document.formulario.action = '../obras2/popPainelFNDESolicitacoesMunicipal.php';

	var janela = window.open( '', 'modopop', 'width=780,height=465,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
	document.formulario.target = 'modopop';
	document.formulario.submit();
}

</script>
