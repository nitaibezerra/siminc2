<?php

require_once 'credito_funcoes.inc';
$edicao_ativa = cre_edicao_ativa();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Caracterização e justificativa dos remanejamentos' );

if(!$_SESSION['creditos_adicionais_tcrid'] || !$_SESSION['creditos_adicionais_unicod']){
	echo "<script>
			alert('Sessão expirou!');
			location.href='elabrev.php?modulo=principal/remanejamento/credito&acao=A';
		 </script>";
	die();
}

$tcrid = $_SESSION['creditos_adicionais_tcrid'];
$unicod = $_SESSION['creditos_adicionais_unicod'];

$jscresumo = '';
$jsccaracterizacao = '';
$jscresultado = '';
$jscconsequencia = '';
$jscreflexo = '';
$jscrepercussao = '';
$jscaplicacao = '';
$jscmemoria = '';

if ( $edicao_ativa && isset( $_REQUEST['form'] ) ) 
{
	// grava os dados do formulário no banco
	$jscresumo = $_REQUEST['jscresumo'];
	$jsccaracterizacao = $_REQUEST['jsccaracterizacao'];
	$jscresultado = $_REQUEST['jscresultado'];
	$jscconsequencia = $_REQUEST['jscconsequencia'];
	$jscreflexo = $_REQUEST['jscreflexo'];
	$jscrepercussao = $_REQUEST['jscrepercussao'];
	$jscaplicacao = $_REQUEST['jscaplicacao'];
	$jscmemoria = $_REQUEST['jscmemoria'];
	
	// verifica se existe justificativa
	$sql =
		" select count(*) as existe " .
		" from elabrev.justificativacredito " .
		" where " .
			" tcrid = " . $tcrid . "  and " .
			" unicod = '" . $unicod . "' and " .
			" unitpocod = 'U' and " .
			" mcrid = " . $_SESSION['credito_mcrid'];
	$existe = (boolean) $db->pegaUm( $sql );
	
	if ( $existe )
	{
		$sql =
			" update elabrev.justificativacredito " .
			" set " .
				" jscresumo = '" . ( $jscresumo ) . "', " .
				" jsccaracterizacao = '" . ( $jsccaracterizacao ) . "', " .
				" jscresultado = '" . ( $jscresultado ) . "', " .
				" jscconsequencia = '" . ( $jscconsequencia ) . "', " .
				" jscreflexo = '" . ( $jscreflexo ) . "', " .
				" jscrepercussao = '" . ( $jscrepercussao ) . "', " .
				" jscaplicacao = '" . ( $jscaplicacao ) . "', " .
				" jscmemoria = '" . ( $jscmemoria ) . "' " .
			" where " .
				" tcrid = " . $tcrid . " and " .
				" unicod = '" . $unicod . "' and " .
				" unitpocod = 'U' and " .
				" mcrid = " . $_SESSION['credito_mcrid'];
	}
	else
	{
		$sql =
			" insert into elabrev.justificativacredito " .
			" ( " .
				" tcrid, unicod, unitpocod, " .
				" jscresumo, " .
				" jsccaracterizacao, " .
				" jscresultado, " .
				" jscconsequencia, " .
				" jscreflexo, " .
				" jscrepercussao, " .
				" jscaplicacao, " .
				" jscmemoria, " .
				" mcrid " .
			" ) " .
			" values " .
			" ( " .
				$tcrid . ", '" . $unicod . "', 'U', " .
				" '" . ( $jscresumo ) . "', " .
				" '" . ( $jsccaracterizacao ) . "', " .
				" '" . ( $jscresultado ) . "', " .
				" '" . ( $jscconsequencia ) . "', " .
				" '" . ( $jscreflexo ) . "', " .
				" '" . ( $jscrepercussao ) . "', " .
				" '" . ( $jscaplicacao ) . "', " .
				" '" . ( $jscmemoria ) . "', " .
				" " . ( $_SESSION['credito_mcrid'] ) . " " .
			" ) ";
	}
	$db->executar( $sql );
	$db->commit();
}
else
{
	// carregar dados do banco
	$sql =
		" select * " .
		" from elabrev.justificativacredito " .
		" where " .
			" tcrid = " . $tcrid . " and " . 
			" unicod = '" . $unicod . "' and " .
			" mcrid = " . $_SESSION['credito_mcrid'];
	$dados = $db->recuperar( $sql );
	if ( $dados )
	{
		$jscresumo = $dados['jscresumo'];
		$jsccaracterizacao = $dados['jsccaracterizacao'];
		$jscresultado = $dados['jscresultado'];
		$jscconsequencia = $dados['jscconsequencia'];
		$jscreflexo = $dados['jscreflexo'];
		$jscrepercussao = $dados['jscrepercussao'];
		$jscaplicacao = $dados['jscaplicacao'];
		$jscmemoria = $dados['jscmemoria'];
	}
}

$tcrdsc = '';
$unidsc = '';

$sql = "select tcrdsc, tcrcod from elabrev.tipocredito where tcrid = " . $tcrid;
$dados = $db->recuperar( $sql );
$tcrcod = $dados['tcrcod'];
$tcrdsc = $dados['tcrdsc'];

$sql = "select unidsc from public.unidade where unicod = '" . $unicod . "'";
$unidsc = $db->pegaUm( $sql );

?>
<script type="text/javascript">
	
	function voltarEscolher()
	{
		document.escolher.escolher.value = '1';
		document.escolher.submit();
	}
	
</script>
<form action="?modulo=principal/remanejamento/credito&acao=A" method="post" name="escolher">
	<input type="hidden" name="escolher" value="1"/>
	<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: 0px;">
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Tipo de Crédito:
			</td>
			<td width="80%">
				<input type="hidden" name="tcrid" value="<?php echo $tcrid; ?>"/>
				<b><?= $tcrcod ?></b> - <?= $tcrdsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Unidade:
			</td>
			<td width="80%">
				<input type="hidden" name="unicod" value="<?php echo $unicod; ?>"/>
				<b><?= $unicod ?></b> - <?= $unidsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				&nbsp;
			</td>
			<td width="80%">
				<input type="button" onclick="voltarEscolher();" name="voltar" value="Voltar para tela inicial de Créditos Adicionais" />
			</td>
		</tr>
	</table>
</form>
<form name="questionario" action="" method="post">
	<input type="hidden" name="form" value="1"/>
	<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-top: 0px;">
<!--
		<tr>
			<td width="20%" align="right" class="SubTituloDireita" >
				<b>Resumo</b>
			</td>
			<td width="80%">
				<?php
					//print campo_textarea( 'jscresumo', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000 );
				?>
			</td>
		</tr>
-->
		<tr>
			<td width="20%" align="right" class="SubTituloDireita" >
				<!--<b>Caracterização</b> do problema e suas causas -->
				Indique a necessidade de alteração orçamentária. - Aonde serão aplicados os recursos e quais as razões que deram origem a insuficiência dos recursos iniciais ?
			</td>
			<td width="80%">
				<?php
					print campo_textarea( 'jsccaracterizacao', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Descrever a situação atual, ou situação-problema, com as razões que deram origem a insuficiência de dotação orçamentária detectada, incluindo a variação dos parâmetros atuais em relação àqueles originalmente utilizados, se for o caso.',
						true
					);
				?>
			</td>
		</tr>
		<!--<tr>
			<td align="right" class="SubTituloDireita" >
				<b>Resultados</b> esperados com a alteração solicitada
			</td>
			<td>
				<?php
				/*	print campo_textarea( 'jscresultado', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Descrever os resultados esperados com a aplicação dos recursos solicitados e os indicadores que demonstrarem seus efeitos na alteração do quadro descrito na situação-problema, evidenciando o incremento qualitativo ou quantitativo dos níveis de serviços ou ações.',
						false
					);
					*/
				?>
			</td>
		</tr> -->
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Reflexos</b> nos cancelamentos sobre a programação prevista e o impacto no plano pluruanual PPA 2003-2007 -->
				Descreva o impacto do cancelamento de dotação. - Haverá impacto nas contas da unidade com o cancelamento (se for o caso) de dotação? - A unidade poderá arcar com seus custos ?
			</td>
			<td>
				<?php
					print campo_textarea( 'jscreflexo', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Demonstrar quais os efeitos dos cancelamentos das dotações propostas na execução da programação e o impacto no PPA 2008-2010, inclusive que não haverá pedido de recursos adicionais nestas dotações.',
						false
					);
				?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Consequências</b> do não atendimento do pleito -->
				Quais as consequências do não atendimento do pleito ? - Caso não atendido o pedido de crédito, quais problemas acarretarão para a unidade ?
			</td>
			<td>
				<?php
					print campo_textarea( 'jscconsequencia', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Apresentar as repercusões negativas no desenvolvimento das ações do órgão/unidade caso a alteração solicitada não seja atendida ou a seja parcialmente.',
						false
					);
				?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Repercusão</b> no nível de gastos fixos decorrente da alteração solicitada -->
				Quais os reflexos do atendimento da demanda sobre o nível dos gastos de custeio do órgão e/ou da unidade orçamentária? - Haverá aumento sobre os gastos com custeio da unidade?
				- Caso sim, quais e quanto? - A unidade está preparada para esse aumento (caso haja)?
			</td>
			<td>
				<?php
					print campo_textarea( 'jscrepercussao', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Demonstrar o efeito do atendimento da solicitação em relação ao nível de gasto fixo, indicando física e financeiramente o acréscimo.',
						false
					);
				?>
			</td>
		</tr>
		<!--<tr>
			<td align="right" class="SubTituloDireita" >
				"Como" e "em que" serão <b>aplicados</b> os recursos solicitados
			</td>
			<td>
				<?php
				/*	print campo_textarea( 'jscaplicacao', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Descrever pormenorizadamente *como* e *em que* serão aplicados os recursos. No caso de despesa de capital, especificar detalhadamente as aquisições, indicando os custos unitários ou totais. No caso de terceirizado, indicar a natureza do serviço e o respectivo custo.',
						false
					);
					*/
				?>
			</td>
		</tr> -->
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Memórias</b> de cálculo não incluídas nos itens precedentes-->
				Memória de cálculo e outras informações consideradas relevantes.
			</td>
			<td>
				<?php
					print campo_textarea( 'jscmemoria', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Crédito', 80, 4, 7000, '', 0,
						'Apresentar as memórias de cálculo não incluídas nos itens anteriores necessárias à demonstração dos valores propostos, seja na suplementação seja no cancelamento.',
						false
					);
				?>
			</td>
		</tr>
		<?php if ( $edicao_ativa ) : ?>
			<tr>
				<td align="right" class="SubTituloDireita">&nbsp;</td>
				<td>
					<input type="button" value="Gravar" name="gravar" onclick="enviarQuestionario();"/>
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>
<script type="text/javascript">

	function enviarQuestionario()
	{
		var form = document.questionario;
		/*
		if ( !form.jscresumo.value )
		{
			alert( 'Preencha o resumo.' );
			form.jscresumo.focus();
			return;
		}
		if ( !form.jsccaracterizacao.value )
		{
			alert( 'Preencha a caracterização.' );
			form.jsccaracterizacao.focus();
			return;
		}
		if ( !form.jscresultado.value )
		{
			alert( 'Preencha o resultado.' );
			form.jscresultado.focus();
			return;
		}
		if ( !form.jscconsequencia.value )
		{
			alert( 'Preencha a consequência.' );
			form.jscconsequencia.focus();
			return;
		}
		if ( !form.jscreflexo.value )
		{
			alert( 'Preencha o reflexo.' );
			form.jscreflexo.focus();
			return;
		}
		if ( !form.jscrepercussao.value )
		{
			alert( 'Preencha a repercussão.' );
			form.jscrepercussao.focus();
			return;
		}
		if ( !form.jscaplicacao.value )
		{
			alert( 'Preencha a aplicação.' );
			form.jscaplicacao.focus();
			return;
		}
		if ( !form.jscmemoria.value )
		{
			alert( 'Preencha a memória.' );
			form.jscmemoria.focus();
			return;
		}
		*/
		form.submit();
	}

</script>




