<?php 

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);

monta_titulo( $nmEntidadePar, '<strong>PAR For</strong>' );
echo '<br />';
$db->cria_aba( $abacod_tela, $url, '');

?>

<table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="5" align="center">
	<tr>
		<td width="20%">
			Nome do Professor:
			<br />
			<?php echo campo_texto('nome_professor', 'S', 'S', 'Nome do Professor', 40, 255, '', ''); ?>
		</td>
	</tr>
	<tr>
		<td colspan="3" align="center">
			<input type="button" value="Professor Censo" />
			<input type="button" value="Incluir" />
			<input type="button" value="Voltar" />
		</td>
	</tr>
</table>

<?

$sql = "SELECT
			'' as acao,
			'' as cpf,
			'' as nome_professor,
			'' as vinculo,
			'' as ultima_formacao,
			'' as nome_escola,
			'' as etapa,
			'' as curso,
			'' as professor_censo,
			'' as tipo_formacao
		FROM
			entidade.entidade";

//dbg($sql);
$cabecalho = array("Ação", "CPF", "Nome do Professor", "Vínculo", "Última Formação", "Nome da Escola", "Etapa", "Curso", "Professor Censo","Tipo de Formação");

$tamanho		= array( '5%', '5%','15%','10%','10%','15%','10%','10%','10%','10%');
$alinhamento	= array( 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center','center','center');

$db->monta_lista($sql, $cabecalho, 50, 10, 'S','center', 'S', '', $tamanho, $alinhamento);

?>