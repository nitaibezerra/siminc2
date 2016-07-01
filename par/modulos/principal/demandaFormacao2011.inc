<?php 

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);

monta_titulo( $nmEntidadePar, 'PAR For' );
echo '<br />';
$db->cria_aba( $abacod_tela, $url, '');

?>

<!--<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
    	<td class="SubTituloDireita" colspan="2" style="text-align:center;font-weight:bold;">Informações</td>
    </tr>
    <tr>
    	<td width="25%" align='right' class="SubTituloDireita">Ano:</td>
        <td></td>
    </tr>
    <tr>
    	<td width="25%" align='right' class="SubTituloDireita">Entidade:</td>
        <td></td>
    </tr>
    <tr>
    	<td width="25%" align='right' class="SubTituloDireita">Nível / Modalidade de Ensino:</td>
        <td></td>
    </tr>
</table>-->
<table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="5" align="center">
	<tr>
		<td width="20%">
			CPF:
			<br />
			<?php echo campo_texto('cpf', 'S', 'S', 'CPF', 20, 15, '###.###.###-##', ''); ?>
		</td>
		<td width="20%">
			Nome do Professor:
			<br />
			<?php echo campo_texto('nome_professor', 'S', 'S', 'Nome do Professor', 40, 255, '', ''); ?>
		</td>
		<td width="60%">
			Vínculo:
			<br />
			<?php echo campo_texto('vinculo', 'S', 'S', 'Vínculo', 30, 255, '', ''); ?>
		</td>
	</tr>
	<tr>
		<td>
			Última formação:
			<br />
			<?php $sql = "SELECT tfoid as codigo,tfodsc as descricao FROM public.tipoformacao WHERE tfostatus = 'A'"; ?>
			<?php $db->monta_combo('ultima_formacao', $sql, 'S', 'Selecione...', '', '', '', 200); ?>
		</td>
		<td>
			Escola (Cód. INEP):
			<br />
			<?php $sql = "SELECT entid as codigo,entcodent as descricao FROM entidade.entidade WHERE entstatus = 'A' and entcodent is not null LIMIT 5"; ?>
			<?php $db->monta_combo('cod_inep', $sql, 'S', 'Selecione...', '', '', '', 200); ?>
		</td>
		<td>
			Nome da Escola:
			<br />
			<?php echo campo_texto('nome_escola', 'S', 'S', 'Nome da Escola', 30, 255, '', ''); ?>
		</td>
	</tr>
	<tr>
		<td>
			Etapa:
			<br />
			<?php 
				$sql = "SELECT 
							'F' as codigo,
							'Fundamental' as descricao
						UNION ALL
						SELECT 
							'M' as codigo,
							'Médio' as descricao
						UNION ALL
						SELECT 
							'S' as codigo,
							'Superior' as descricao"; 
			?>
			<?php $db->monta_combo('etapa', $sql, 'S', 'Selecione...', '', '', '', 200); ?>
		</td>
		<td>
			Curso Desejado:
			<br />
			<?php echo campo_texto('curso_desejado', 'S', 'S', 'Curso Desejado', 30, 255, '', ''); ?>
		</td>
		<td>
			Tipo Formação desejado:
			<br />
			<?php 
				$sql = "SELECT 
							'C' as codigo,
							'Continuada' as descricao
						UNION ALL
						SELECT 
							'I' as codigo,
							'Inicial' as descricao
						UNION ALL
						SELECT 
							'E' as codigo,
							'Especializada' as descricao"; 
			?>
			<?php $db->monta_combo('tipo_formacao', $sql, 'S', 'Selecione...', '', '', '', 200); ?>
		</td>
	</tr>
	<tr>
		<td colspan="3">
			Disciplina de atuação:
			<br />
			<?php echo campo_texto('disciplina_atuacao', 'S', 'S', 'Disciplina de atuação', 30, 255, '', ''); ?>
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