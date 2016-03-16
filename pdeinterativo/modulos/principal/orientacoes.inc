<?php

if( !$_SESSION['pdeinterativo_vars']['usucpfdiretor'] )
{
	echo '<script>
			alert("Você deve ter perfil de Diretor(a) para acessar esta página.");
			window.location.href = "pdeinterativo.php?modulo=principal/principal&acao=A";
		  </script>';
	exit;
}

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);

cabecalhoPDEInterativo($arrDados);
echo monta_titulo("Primeiros Passos", "Orientações");

?>

<script type="text/javascript">

function abreJanela()
{
	var janela = window.open('pdeinterativo.php?modulo=principal/popOrientacoes&acao=A', 'popOrientacoes', 'width=500,height=280,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
	<tr>
		<td>
			<img border="0" src="/imagens/bussola.png" />
		</td>
		<td>
		<font style="color:#1719fd;">
		<p>Agora que já identificamos a escola e o(a) gestor(a) da escola, vamos executar os Primeiros Passos para o planejamento. Para tanto, é necessário que a escola e a Secretaria de Educação adotem algumas providências essenciais para que a elaboração do plano seja efetivamente democrática e participativa.</p> 
		<p>Esta etapa é muito importante e o seu cumprimento é obrigatório, pois ela organiza o ambiente institucional para a construção do planejamento. Para saber quais são os primeiros passos <a href="#" onclick="abreJanela();">clique aqui</a>..</p>
		</font>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px">
			O que deseja fazer agora?
		</td>
		<td>
			<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Galeria'" >
			<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/primeirospassos&acao=A&aba=primeiropasso'" >
		</td>
	</tr>
</table>