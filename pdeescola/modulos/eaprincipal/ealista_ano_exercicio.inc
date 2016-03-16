<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_REQUEST["submetido"]) {
	$_SESSION["exercicio"] = $_REQUEST["ano_exercicio"];
	
	echo "<script>
			location.href = 'pdeescola.php?modulo=eaprincipal/dados_escola&acao=A';
		  </script>";
	die;
}

include APPRAIZ . "includes/cabecalho.inc";

$titulo = "Escola Acessível";
$subtitulo = "Escolha o Ano de Exercício";

echo "<br />";
monta_titulo($titulo, $subtitulo);

?>

<form id="formListaAnoExercicio" method="post" action="">
<input type="hidden" name="submetido" value="1" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="8" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" width="40%" valign="top">Ano de Exercício:</td>
		<td>
		<?
			if($_SESSION["entid"]) {
				$sql = "SELECT
							eacanoreferencia as codigo,
							eacanoreferencia as descricao
						FROM
							pdeescola.eacescolaacessivel
						WHERE
							entid = ".$_SESSION["entid"]." AND 
							eacstatus = 'A'
						ORDER BY
							eacanoreferencia ASC";
				$db->monta_combo("ano_exercicio", $sql, 'S', "Selecione...", '', '', '', '265', 'S', '');
			} else {
				echo "<script>
						alert('Ocorreu um erro com a entidade(escola). Você será redirecionado para página inicial do sistema.');
						location.href = 'pdeescola.php?modulo=inicio&acao=C';
					  </script>";
				die;
			}
		?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0" colspan="2" align="center">
			<input type="button" id="bt_salvar" value="Prosseguir" onclick="salvar();" />
		</td>
	</tr>
</table>
</form>

<script type="text/javascript">

var btSalvar 	= document.getElementById("bt_salvar");
var form 		= document.getElementById("formListaAnoExercicio");
var ano			= document.getElementsByName("ano_exercicio")[0];

function salvar() {
	btSalvar.disabled = true;
	
	if(ano.value == "") {
		alert("O campo 'Ano de Exercício' deve ser informado.");
		ano.focus();
		btSalvar.disabled = false;
		return;
	}
	
	form.submit();
}

</script>