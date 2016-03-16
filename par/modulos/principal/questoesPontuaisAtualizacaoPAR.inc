<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if( !$_SESSION['par']['inuid'] ){
	echo '<script>
				alert("Faltam dados da sessão!");
				window.location="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
			</script>';
	die;
}

$mnuid = verificaAbasAtualizacao( $_SESSION['par']['inuid'] );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//$db->cria_aba( $abacod_tela, $url, '' );
echo cabecalho();
echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">';
				echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);
			echo '</a>';

$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
monta_titulo( 'Atualização PAR - Passo 02', 'Validação das Questões Pontuais' );

$testeQuestoesPontuais = verificaValidacaoQuestoesPontuais();
?>
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
<tr>
	<td>
		<?php
			if( (string)$testeQuestoesPontuais == 'Aguardando'){
				echo '<BR><CENTER><h1>Aguardando Validação das Questões Pontuais</h1></CENTER><BR>';
			} else {
				echo '<BR><CENTER><h1>Questões Pontuais Validadas</h1></CENTER><BR>';
			}
		?>
	</td>
</tr>
</table>
		</td>
	</tr>
</table>