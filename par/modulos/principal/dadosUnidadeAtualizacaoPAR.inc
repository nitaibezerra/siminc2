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

$_SESSION['par']['atualizacao'] = true;

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
monta_titulo( 'Atualização PAR - Passo 01', 'Atualização dos Dados da Unidade' );

$testeDados = verificaPreenchimentoDadosUnidadePorData();

?>
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
<tr>
	<td>
		<?php
			if( (string)$testeDados == 'Aguardando' ){
				echo '<BR><CENTER><h1>Aguardando Validação dos Dados da Unidade</h1></CENTER><BR>';
			} else {
				echo '<BR><CENTER><h1>Dados da Unidade atualizados</h1></CENTER><BR>';
			}
		?>
	</td>
</tr>
</table>
<?php unset($_SESSION['par']['atualizacao']); ?>
		</td>
	</tr>
</table>
