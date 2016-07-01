<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
	Programador: Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
	Módulo: remanejamento.inc
	Finalidade: Remanejar despesas de uma ação (cacelamentos, remanejamentos e suplementações)
*/

/*

...&acao=C ---> CANCELAMENTO
...&acao=R ---> REMANEJAMENTO
...&acao=S ---> SUPLEMENTAÇÃO

*/

// ------------------------------- F U N Ç Õ E S -------------------------------

/**
 * Calcula o saldo de uma determinada despesa. Deve ser utilizada para verificar
 * se pode retirar valor ou simplesmente exibição do saldo.
 * 
 * @param integer $remid
 * @param integer $ppoid
 * @param integer $ppoid
 * @param integer $ppoid
 * @param string $iducod
 * @param string $foncod
 * @param string $idoid
 * @param integer $dpavalor
 * @return boolean
 */
function remAdicionarRemanejamento( $remid, $ppoid, $acaid, $ndpid, $iducod, $foncod, $idoid, $dpavalor )
{
	global $db;
	$remid = (integer) $remid;
	$ppoid = (integer) $ppoid;
	$acaid = (integer) $acaid;
	$ndpid = (integer) $ndpid;
	$iducod = addslashes( (string) $iducod );
	$foncod = addslashes( (string) $foncod );
	$idoid = (integer) $idoid;
	$dpavalor = (integer) $dpavalor;
	$sql = sprintf(
		' insert into elabrev.despesaacao ' .
			' ( remid, ppoid, acaid, ndpid, iducod, foncod, idoid, dpavalor ) ' .
			' values ( %s, %s, %s, %s, \'%s\', \'%s\', \'%s\', %s ) ',
		$remid,
		$ppoid,
		$acaid,
		$ndpid,
		$iducod,
		$foncod,
		$idoid,
		$dpavalor
	);
	return (boolean) $db->executar( $sql );
}

/**
 * Adicionar uma remessa. Esta função não manipula os remanejamentos, para isso
 * utilize a função {@link remAdicionarRemanejamento()} e
 * {@link remRemoverRemanejamentos()}.
 *
 * @param string $unicod
 * @param string $remresumo
 * @return boolean
 */
function remAdicionarRemessa( $unicod, $remresumo )
{
	global $db;
	$unicod = addslashes( (string) $unicod );
	$remresumo = addslashes( (string) $remresumo );
	$sql = "select nextval( 'elabrev.remessa_remid_seq' ) as remid";
	$remid = (integer) $db->pegaUm( $sql );
	$sql = sprintf(
		' insert into elabrev.remessa ( remid, unicod, remresumo, ppoid ) values ( %s, \'%s\', \'%s\', %s ) ',
		$remid,
		$unicod,
		$remresumo,
		remPegaPpoid()
	);
	$db->executar( $sql );
	return $remid;
}

/**
 * Adicionar uma remessa. Esta função não manipula os remanejamentos, para isso
 * utilize a função {@link remAdicionarRemanejamento()} e
 * {@link remRemoverRemanejamentos()}.
 *
 * @param integer $remid
 * @param string $unicod
 * @param string $remresumo
 * @return boolean
 */
function remAtualizaRemessa( $remid, $unicod, $remresumo )
{
	global $db;
	$remid = (integer) $remid;
	$unicod = addslashes( (string) $unicod );
	$remresumo = addslashes( (string) $remresumo );
	$sql = sprintf(
		' update elabrev.remessa set unicod = \'%s\', remresumo = \'%s\' where remid = %s and ppoid = %s ',
		$unicod,
		$remresumo,
		$remid,
		remPegaPpoid()
	);
	return (boolean) $db->executar( $sql );
}


/**
 * Captura entrades de retiradas e inserções. A função retorna no índice 'erro'
 * se alguma despesa estiver com erro. A despesa que contiver algum erro terá o
 * índice 'erro' setado com a mensagem de erro referente à ela.
 * 
 * @param string
 * @return string[]
 */
function remCapturarEntradaRemanejamentos( $tipo )
{
	$erro = false;
	$total = 0;
	$itens = array();
	if ( isset( $_REQUEST['remanejamento'][$tipo]['unidade'][0] ) == true )
	{
		$item = $_REQUEST['remanejamento'][$tipo];
		foreach ( array_keys( $item['unidade'] ) as $chave )
		{
			$novoItem = array(
				'unicod' => $item['unidade'][$chave],
				'prgcod' => $item['programa'][$chave],
				'acacod' => $item['acao'][$chave],
				'loccod' => $item['localizador'][$chave],
				'acaid' => remPegaAcaid(
					$item['unidade'][$chave],
					$item['programa'][$chave],
					$item['acao'][$chave],
					$item['localizador'][$chave]
				),
				'ndpcod' => $item['natureza'][$chave],
				'ndpid' => remPegaNdpid( $item['natureza'][$chave] ),
				'iducod' => $item['iduso'][$chave],
				'foncod' => $item['fonte'][$chave],
				'idocod' => $item['idoc'][$chave],
				'idoid' => pegaIdoid( $item['idoc'][$chave] ),
				'valor' => (integer) str_replace( '.', '', $item['valor'][$chave] )
			);
			if ( $tipo == 'retirada' )
			{
				$novoItem['valor'] *= -1;
				$saldo = remPegaSaldo(
					remPegaPpoid(),
					$novoItem['acaid'],
					$novoItem['ndpid'],
					$novoItem['iducod'],
					$novoItem['foncod'],
					$novoItem['idoid']
				);
				if ( $saldo + $novoItem['valor'] < 0 )
				{
					$erro = true;
					$novoItem['erro'] = 'O valor retirado é maior que o valor atual atribuído à despesa.';
				}
			}
			array_push( $itens, $novoItem );
			$total += $novoItem['valor'];
		}
	}
	return array(
		'total' => $total,
		'itens' => $itens,
		'erro' => $erro
	);
}

/**
 * Captura o acaid a partir do UPAL (unidade / programa / acao / localizador).
 * 
 * @param string $unicod
 * @param string $prgcod
 * @param string $acacod
 * @param string $loccod
 * @return integer
 */
function remPegaAcaid( $unicod, $prgcod, $acacod, $loccod )
{
	static $acaid = array();
	$unicod = (string) $unicod;
	$prgcod = (string) $prgcod;
	$acacod = (string) $acacod;
	$loccod = (string) $loccod;
	$chave = $unicod . $prgcod . $acacod . $loccod;
	if ( array_key_exists( $chave, $acaid ) == true )
	{
		return $acaid[$chave];
	}
	global $db;
	$sql = sprintf(
		" select acaid from elabrev.ppaacao_orcamento " .
			" where unicod = '%s' and prgcod = '%s' and acacod = '%s' and loccod = '%s' ",
		$unicod,
		$prgcod,
		$acacod,
		$loccod
	);
	$acaid[$chave] = (integer) $db->pegaUm( $sql );
	return $acaid[$chave];
}

/**
 * Captura o ndpid a partir do ndpcod.
 * 
 * @param string $unicod
 * @return integer
 */
function remPegaNdpid( $ndpcod )
{
	static $ndpid = array();
	$ndpcod = (string) $ndpcod;
	if ( array_key_exists( $ndpcod, $ndpid ) == true )
	{
		return $ndpid[$ndpcod];
	}
	global $db;
	$sql = sprintf(
		"select ndpid from public.naturezadespesa where ndpcod = '%s'",
		$ndpcod
	);
	$ndpid[$ndpcod] = $db->pegaUm( $sql );
	return $ndpid[$ndpcod];
}

function pegaIdoid( $idocod )
{
	static $idoid = array();
	$idocod = (string) $idocod;
	if ( array_key_exists( $idocod, $idoid ) == true )
	{
		return $idoid[$idocod];
	}
	global $db;
	$sql = sprintf(
		"select idoid from public.idoc where idocod = '%s'",
		$idocod
	);
	$idoid[$idocod] = $db->pegaUm( $sql );
	return $idoid[$idocod];
}

/**
 * Captura o ppoid da projeto de lei do ano vigente.
 * 
 * @return integer
 */
function remPegaPpoid()
{
	static $ppoid = null;
	global $db;
	if ( $ppoid != null )
	{
		return $ppoid;
	}
	$sql = "select ppoid from elabrev.propostaorcamento where ppoano = '" . $_SESSION['exercicio'] . "' and tppid = 2";
	$ppoid = (integer) $db->pegaUm( $sql );
	return $ppoid;
}

/**
 * Pegar itens da tabela despesaacao com remid indicado.
 * 
 * @param integer $remid
 * @return string[][]
 */
function remPegaRemanejamentos( $remid )
{
	global $db;
	$remid = (integer) $remid;
	$sql = sprintf(
		' select a.acaid, a.unicod, a.prgcod, a.acacod, a.loccod, d.ndpid, n.ndpcod, d.idoid, i.idocod, d.foncod, d.iducod, d.dpavalor as valor ' .
			' from elabrev.despesaacao d inner join elabrev.ppaacao_orcamento a on a.acaid = d.acaid ' .
			' inner join public.naturezadespesa n on n.ndpid = d.ndpid '.
			' inner join public.idoc i on i.idoid = d.idoid ' .
			' where d.remid = %s',
		$remid
	);
	$registros = $db->carregar( $sql );
	if ( !$registros )
	{
		return array();
	}
	return $registros;
}

/**
 * Captura os dados de uma remessa. Para capturar os itens de remessa utilize a
 * função {@link remPegaRemanejamentos()}.
 * 
 * @param integer
 * @return boolean
 */
function remPegaRemessa( $remid )
{
	global $db;
	$remid = (integer) $remid;
	$sql = sprintf(
		'select * from elabrev.remessa where remid = %s',
		$remid
	);
	$registros = $db->recuperar( $sql );
	if ( !$registros )
	{
		return array();
	}
	return $registros;
}

/**
 * Calcula o saldo de uma determinada despesa. Deve ser utilizada para verificar
 * se pode retirar valor ou simplesmente exibição do saldo.
 * 
 * @param integer $ppoid
 * @param integer $ppoid
 * @param integer $ppoid
 * @param string $iducod
 * @param string $foncod
 * @param integer $idoid
 * @return integer
 */
function remPegaSaldo( $ppoid, $acaid, $ndpid, $iducod, $foncod, $idoid )
{
	static $saldo = array();
	$chave = $ppoid . $acaid . $ndpid . $iducod . $foncod . $idoid;
	if ( array_key_exists( $chave, $saldo ) == true )
	{
		return $saldo[$chave];
	}
	global $db;
	$ppoid = (integer) $ppoid;
	$acaid = (integer) $acaid;
	$ndpid = (integer) $ndpid;
	$iducod = addslashes( (string) $iducod );
	$foncod = addslashes( (string) $foncod );
	$idoid = (integer) $idoid;
	$sql = sprintf(
		' select coalesce( sum( dpavalor ), 0 ) as dpavalor from elabrev.despesaacao where ' .
			' ppoid = %s and acaid = %s and ndpid = %s and ' .
			' iducod = \'%s\' and foncod = \'%s\' and idoid = \'%s\' ' .
			' group by ppoid, acaid, ndpid, iducod, foncod, idoid ',
		$ppoid,
		$acaid,
		$ndpid,
		$iducod,
		$foncod,
		$idoid
	);
	$saldo[$chave] = (integer) $db->pegaUm( $sql );
	return $saldo[$chave];
}

/**
 * Remove todos os remanejamentos de uma remessa.
 *
 * @param integer $remid
 * @return boolean
 */
function remRemoverRemanejamentos( $remid )
{
	global $db;
	$remid = (integer) $remid;
	$sql = sprintf(
		'delete from elabrev.despesaacao where remid = %s',
		$remid
	);
	return (boolean) $db->executar( $sql );
}

/**
 * Verifica se $pppid é do tipo lei.
 * 
 * @param integer
 * @return boolean
 */
function remVerificaLei( $tppid )
{
	return ( (integer) $tppid ) == 3;
}

/**
 * Verifica se $remstatus é do tipo finalizada.
 * 
 * @param integer
 * @return boolean
 */
function remVerificaRemessaFinalizada( $remstatus )
{
	return strtoupper( (string) $remstatus ) == 'F';
}

// FIM ----------------------------- F U N Ç Õ E S -----------------------------

// ------------------- M A N I P U L A   R E Q U I S I Ç Ã O -------------------

/*
...&acao=C ---> CANCELAMENTO
...&acao=R ---> REMANEJAMENTO
...&acao=S ---> SUPLEMENTAÇÃO
*/
$acao = strtoupper( $_REQUEST['acao'] );

$remid = '';
$remresumo = '';
$unicod = '';
$remanejamentos = array();

// indicador de que houve erro
$erro = false;

// ---------- ESCOLHA DE UNIDADE

if ( isset( $_REQUEST['escolha_unidade'] ) == true )
{
	$unicod = $_REQUEST['unicod'];
	$remresumo = $_REQUEST['remresumo'];
}

// ---------- INCLUI/EDITA REMESSA E REMANEJAMENTOS

else if ( isset( $_REQUEST['remanejar'] ) )
{
	
	$unicod = (string) $_REQUEST['unicod'];
	$remresumo = (string) $_REQUEST['remresumo'];
	
	// cria remanjemanto
	if ( !$_REQUEST['remid'] )
	{
		$remid = remAdicionarRemessa( $unicod, $remresumo );
	}
	// atualiza remanejamento
	else
	{
		$remid = (integer) $_REQUEST['remid'];
		remAtualizaRemessa( $remid, $unicod, $remresumo );
		remRemoverRemanejamentos( $remid );
	}
	
	// verifica se os dados da remessa foram inserida/editada
	if ( !$remid )
	{
		$erro = true;
	}
	
	// continua inserção em caso de sucesso na edição/inserção de dados gerais da remessa
	if ( $erro == false )
	{
		// captura remanejamentos indicados pelo usuário
		$dadosRetiradas = remCapturarEntradaRemanejamentos( 'retirada' );
		$dadosInsercoes = remCapturarEntradaRemanejamentos( 'insercao' );
		// captura remanejamentos (utilizado pra atualização/edição e exibição na tela)
		$remanejamentos = array_merge( $dadosRetiradas['itens'], $dadosInsercoes['itens'] );
		if ( $dadosRetiradas['erro'] == false && $dadosInsercoes['erro'] == false )
		{
			// percorre remanejamentos para realizar a inserção
			foreach ( $remanejamentos as $chave => $item )
			{
				// adiciona o remanejamento
				$erro = !remAdicionarRemanejamento(
					$remid,
					remPegaPpoid(),
					$item['acaid'],
					$item['ndpid'],
					$item['iducod'],
					$item['foncod'],
					$item['idoid'],
					$item['valor']
				);
				// verifica sucesso da operação item-a-item
				if ( $erro == true )
				{
					break;
				}
			}
		}
		else
		{
			$erro = true;
		}
	}
	$erro == false ? $db->commit() : $db->rollback();
}

// ---------- CARREGA REMESSA E REMANEJAMENTOS

else
{
	// carrega remessa
	if ( isset( $_REQUEST['remid'] ) == true )
	{
		$remessa = remPegaRemessa( $_REQUEST['remid'] );
		// realiza verificações
		if ( !$remessa || count( $remessa ) == 0 )
		{
			$db->insucesso( 'Remessa inexistente' );
		}
		if ( remVerificaRemessaFinalizada( $remessa['remstatus'] ) == true )
		{
			$db->insucesso( 'A remessa indicada já foi finalizada' );
		}
		$remid = $remessa['remid'];
		$unicod = $remessa['unicod'];
		$remresumo = $remessa['remresumo'];
		// inicia remanejamentos de uma determinada remessa
		$remanejamentos = remPegaRemanejamentos( $remid );
	}
}

// FIM ----------------- M A N I P U L A   R E Q U I S I Ç Ã O -----------------

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );
?>
<form name="formulario" method="post" style="margin:0px;">
	<input type="hidden" name="remid" value="<?= $remid ?>" />
	
	<!-- ----- UNIDADE ORÇAMENTÁRIA ---------------------------------------- -->
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-bottom: none;">
		<tr>
			<td width="20%" align="right" class="SubTituloDireita" >Unidade</td>
			<td width="80%">
				<?php if ( !$unicod ) : ?>
					<?php
						$sqlCombo = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.reporcfin r inner join public.unidade u on r.unicod = u.unicod where r.rofano = '" . $_SESSION['exercicio'] . "' group by u.unicod, u.unidsc order by u.unicod, u.unidsc";
						$db->monta_combo( 'unicod', $sqlCombo, 'S', 'Unidade', '', '' );
					?>
				<?php else : ?>
					<input type="hidden" name="unicod" value="<?= $unicod ?>" />
					<?php
						$sqlUnidade = "select u.unicod || ' - ' || u.unidsc as descricao from public.unidade u where u.unicod = '" . $unicod . "'";
						print $db->pegaUm( $sqlUnidade );
					?>
				<?php endif; ?>
			</td>
		</tr>
	</table>
	<!-- ------------------------------------------------------------------- -->
	
	<?php if ( $unicod ) : ?>
		
		<!-- ----- ABAS DADOS_GERAIS/REMANEJAMENTOS ------------------------ -->
		<table class="tabela" cellspacing="1" cellpadding="3" align="center" style="border-top:none;border-bottom:none;">
			<tr>
				<td width="50%" class="SubTituloDireita" style="text-align:center;">
					<a id="span_dados_gerais" href="javascript:alterarAba( 'dados_gerais' );">
						Dados Gerais
					</a>
				</td>
				<td width="50%" class="SubTituloDireita" style="text-align:center;">
					<a id="span_remanejamentos" href="javascript:alterarAba( 'remanejamentos' );">
						Remanejamentos
					</a>
				</td>
			</tr>
		</table>
		<!-- ------------------------------------------------------------------- -->
		
		<!-- ----- DADOS GERAIS DA REMESSA ------------------------------------- -->
		<div id="div_dados_gerais" style="display:none;">
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-top: none;border-bottom: none;">
				<tr>
					<td width="20%" align="right" class="SubTituloDireita" >Status</td>
					<td width="80%">
						<select name="remstatus" id="remstatus" class="CampoEstilo">
							<option value="C" <?= $remstatus == 'C' || !$remstatus  ? ' selected="selected" ' : '' ; ?>>Construção</option>
							<option value="A" <?= $remstatus == 'A' ? ' selected="selected" ' : '' ; ?>>Ativa</option>
						</select>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Resumo
					</td>
					<td>
						<?php
							print campo_textarea( 'remresumo', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000 );
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Caracterização do problema e suas causas
					</td>
					<td>
						<?php
							print campo_textarea( 'remcaracterizacao', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Descrever a situação atual, ou situação-problema, com as razões que deram origem a insuficiência de dotação orçamentária detectada, incluindo a variação dos parâmetros atuais em relação àqueles originalmente utilizados, se for o caso.',
								true
							);
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Resultados esperados com a alteração solicitada
					</td>
					<td>
						<?php
							print campo_textarea( 'remresultado', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Descrever os resultados esperados com a aplicação dos recursos solicitados e os indicadores que demonstrarem seus efeitos na alteração do quadro descrito na situação-problema, evidenciando o incremento qualitativo ou quantitativo dos níveis de serviços ou ações.',
								true
							);
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Consequências do não atendimento do pleito
					</td>
					<td>
						<?php
							print campo_textarea( 'remconsequencia', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Apresentar as repercusões negativas no desenvolvimento das ações do órgão/unidade caso a alteração solicitada não seja atendida ou a seja parcialmente.',
								true
							);
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Reflexos nos cancelamentos sobre a programação prevista e o impacto no plano pluruanual PPA 2003-2007
					</td>
					<td>
						<?php
							print campo_textarea( 'remreflexo', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Demonstrar quais os efeitos dos cancelamentos das dotações propostas na execução da programação e o impacto no PPA 2003-2007, inclusive que não haverá pedido de recursos adicionais nestas dotações.',
								true
							);
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Repercusão no nível de gastos fixos decorrente da alteração solicitada
					</td>
					<td>
						<?php
							print campo_textarea( 'remrepercussao', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Demonstrar o efeito do atendimento da solicitação em relação ao nível de gasto fixo, indicando física e financeiramente o acréscimo.',
								true
							);
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						"Como" e "em que" serão aplicados os recursos solicitados
					</td>
					<td>
						<?php
							print campo_textarea( 'remaplicacao', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Descrever pormenorizadamente *como* e *em que* serão aplicados os recursos. No caso de despesa de capital, especificar detalhadamente as aquisições, indicando os custos unitários ou totais. No caso de terceirizado, indicar a natureza do serviço e o respectivo custo.',
								true
							);
						?>
					</td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita" >
						Memórias de cálculo não incluídas nos itens precedentes
					</td>
					<td>
						<?php
							print campo_textarea( 'remmemoria', 'S', 'S', 'Resumo do Crédito', 80, 4, 2000, '', 0,
								'Apresentar as memórias de cálculo não incluídas nos itens anteriores necessárias à demonstração dos valores propostos, seja na suplementação seja no cancelamento.',
								true
							);
						?>
					</td>
				</tr>
			</table>
		</div>
		<!-- ------------------------------------------------------------------- -->
		
		<div id="div_remanejamentos" style="display:none;">
		
			<?php if ( $acao == 'C' || $acao == 'R' ) : ?>
			
				<!-- ----- RETIRADAS ----------------------------------------------- -->
				<table class="tabela" id="tabela_retiradas" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-bottom: none; border-top: none;">
					<tr>
						<td colspan="11" class="SubTituloDireita" style="text-align: center;">
							C A N C E L A M E N T O S
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="text-align: center;" colspan="4">
							Funcional 
							<?php
								$sqlUpal = "select a.unicod || '.' || a.prgcod || '.' || a.acacod || '.' || a.loccod as codigo, a.prgcod || '.' || a.acacod || '.' || a.loccod as descricao from elabrev.ppaacao_orcamento a inner join elabrev.despesaacao d on d.acaid = a.acaid where a.prgano = '" . $_SESSION['exercicio'] . "' and a.funcod = '12' and a.unicod = '" . $unicod . "' group by a.unicod, a.prgcod, a.acacod, a.loccod order by a.unicod, a.prgcod, a.acacod, a.loccod";
								$db->monta_combo( 'pal_retirada', $sqlUpal, 'S', 'Escolha', 'remRetiradaAlteraUpal', '', '', '', '', 'select_upal_retirada' );
							?>
						</td>
						<td class="SubTituloDireita" style="text-align: center; width: 100px;" rowspan="2">Natureza</td>
						<td class="SubTituloDireita" style="text-align: center; width: 75px;" rowspan="2">IDUSO</td>
						<td class="SubTituloDireita" style="text-align: center; width: 75px;" rowspan="2">Fonte</td>
						<td class="SubTituloDireita" style="text-align: center; width: 75px;" rowspan="2">IDOC</td>
						<td class="SubTituloDireita" style="text-align: center; width: 100px;" rowspan="2">Saldo</td>
						<td class="SubTituloDireita" style="text-align: center; width: 130px;" rowspan="2">Valor</td>
						<td class="SubTituloDireita" style="text-align: center;" rowspan="2">Ações</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Unidade</td>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Programa</td>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Ação</td>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Localizador</td>
					</tr>
					<tr>
						<td style="text-align: center;">
							<input type="hidden" name="retirada_input_unidade" id="retirada_input_unidade" value=""/>
							<span id="retirada_span_unidade">-</span>
						</td>
						<td style="text-align: center;">
							<input type="hidden" name="retirada_input_programa" id="retirada_input_programa" value=""/>
							<span id="retirada_span_programa">-</span>
						</td>
						<td style="text-align: center;">
							<input type="hidden" name="retirada_input_acao" id="retirada_input_acao" value=""/>
							<span id="retirada_span_acao">-</span>
						</td>
						<td style="text-align: center;">
							<input type="hidden" name="retirada_input_localizador" id="retirada_input_localizador" value=""/>
							<span id="retirada_span_localizador">-</span>
						</td>
						<?php
							
							// seleciona somente as naturezas de despesa que estão
							// presentes em alguma despesa de alguma ação onde a unidade
							// é a escolhida
							$sqlNatureza =
								" select n.ndpcod as codigo, n.ndpdsc as descricao from public.naturezadespesa n " .
									" inner join elabrev.despesaacao d on n.ndpid = d.ndpid " .
									" inner join elabrev.ppaacao_orcamento a on d.acaid = a.acaid " .
								" where a.unicod = '" . $unicod . "' " .
								" group by n.ndpcod, n.ndpdsc ";
							
							// seleciona somente os IDUSOs que estão presentes em alguma
							// despesa de alguma ação onde a unidade é a escolhida
							$sqlIduso =
								" select i.iducod as codigo, i.idudsc as descricao from public.identifuso i " .
									" inner join elabrev.despesaacao d on i.iducod = d.iducod " .
									" inner join elabrev.ppaacao_orcamento a on d.acaid = a.acaid " .
								" where a.unicod = '" . $unicod . "' " .
								" group by i.iducod, i.idudsc ";
							
							// seleciona somente os fontes que estão presentes em alguma
							// despesa de alguma ação onde a unidade é a escolhida
							$sqlFonte =
								" select f.foncod as codigo, f.fondsc as descricao from public.fonterecurso f " .
									" inner join elabrev.despesaacao d on f.foncod = d.foncod " .
									" inner join elabrev.ppaacao_orcamento a on d.acaid = a.acaid " .
								" where a.unicod = '" . $unicod . "' " .
								" group by f.foncod, f.fondsc";
							
							// seleciona somente os fontes que estão presentes em alguma
							// despesa de alguma ação onde a unidade é a escolhida
							$sqlIdoc =
								" select i.idocod as codigo, i.idodsc as descricao from public.idoc i " .
									" inner join elabrev.despesaacao d on i.idoid = d.idoid " .
									" inner join elabrev.ppaacao_orcamento a on d.acaid = a.acaid " .
								" where a.unicod = '" . $unicod . "' " .
								" group by i.idocod, i.idodsc";
						
						?>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remRetiradaAlterarCampoFocado( event, \'natureza\' );" '; ?>
							<? texto_popup( 'retirada_input_natureza', $sqlNatureza, 'Natureza da Despesa', 8, 13, '########', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remRetiradaAlterarCampoFocado( event, \'iduso\' );" '; ?>
							<? texto_popup( 'retirada_input_iduso', $sqlIduso, 'Ident. de Uso', 1, 7, '#', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remRetiradaAlterarCampoFocado( event, \'fonte\' );" '; ?>
							<? texto_popup( 'retirada_input_fonte', $sqlFonte, 'Fonte de Recursos', 3, 7, '###', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remRetiradaAlterarCampoFocado( event, \'idoc\' );" '; ?>
							<? texto_popup( 'retirada_input_idoc', $sqlIdoc, 'Ident. Op. de Crédito', 4, 7, '####', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<span id="retirada_span_saldo"></span>
						</td>
						<td style="text-align: right;">
							<? $complemento = ''; ?>
							<?= campo_texto( 'retirada_input_valor', 'N', 'S', '', 18, 14, '##.###.###.###', '', 'right', '', 0, ' id="retirada_input_valor" onblur="MouseBlur(this);" ' . $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<span id="retirada_span_acoes_desligado" style="display: block;">
								<a href="javascript:
										if ( remRetiradaHabilitarValor( true ) == false )
										{
											alert( 'Preencha todos os campos para poder continuar a inserção de uma retirada.' );
										}
									">
								Cont
								</a>
							</span>
							<span id="retirada_span_acoes_ligado" style="display: none;">
								<a href="javascript:remRetiradaAdicionarDoFormulario();">Ok</a>
								&nbsp;&nbsp;
								<a href="javascript:remRetiradaCancelarInsercao();">Can</a>
							</span>
						</td>
					</tr>
					<tr>
						<td colspan="9" style="text-align: right;">&nbsp;</td>
						<td style="text-align: right;">
							<b><span id="retiradas_span_total">0</span></b>
						</td>
						<td style="text-align: center;">&nbsp;</td>
					</tr>
				</table>
				<!-- --------------------------------------------------------------- -->
			
			<?php endif; ?>
			
			<?php if ( $acao == 'S' || $acao == 'R' ) : ?>
			
				<!-- ----- INSERÇÕES ------------------------------------------------ -->
				<table class="tabela" id="tabela_insercoes" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-bottom: none; border-top: none;">
					<tr>
						<td colspan="10" class="SubTituloDireita" style="text-align: center;">
							S U P L E M E N T A Ç Õ E S
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="text-align: center;" colspan="4">
							Funcional 
							<?php
								$sqlUpal = "select a.unicod || '.' || a.prgcod || '.' || a.acacod || '.' || a.loccod as codigo, a.prgcod || '.' || a.acacod || '.' || a.loccod as descricao from elabrev.ppaacao_orcamento a inner join elabrev.despesaacao d on d.acaid = a.acaid where a.prgano = '" . $_SESSION['exercicio'] . "' and a.funcod = '12' and a.unicod = '" . $unicod . "' group by a.unicod, a.prgcod, a.acacod, a.loccod order by a.unicod, a.prgcod, a.acacod, a.loccod";
								$db->monta_combo( 'pal_insercao', $sqlUpal, 'S', 'Escolha', 'remInsercaoAlteraUpal', '', '', '', '', 'select_upal_insercao' );
							?>
						</td>
						<td class="SubTituloDireita" style="text-align: center; width: 100px;" rowspan="2">Natureza</td>
						<td class="SubTituloDireita" style="text-align: center; width: 75px;" rowspan="2">IDUSO</td>
						<td class="SubTituloDireita" style="text-align: center; width: 75px;" rowspan="2">Fonte</td>
						<td class="SubTituloDireita" style="text-align: center; width: 75px;" rowspan="2">IDOC</td>
						<td class="SubTituloDireita" style="text-align: center; width: 237px;" rowspan="2">Valor</td>
						<td class="SubTituloDireita" style="text-align: center;" rowspan="2">Ações</td>
					<tr>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Unidade</td>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Programa</td>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Ação</td>
						<td class="SubTituloDireita" style="text-align: center; width: 65px;">Localizador</td>
					</tr>
					<tr>
						<td style="text-align: center;">
							<input type="hidden" name="insercao_input_unidade" id="insercao_input_unidade" value=""/>
							<span id="insercao_span_unidade">-</span>
						</td>
						<td style="text-align: center;">
							<input type="hidden" name="insercao_input_programa" id="insercao_input_programa" value=""/>
							<span id="insercao_span_programa">-</span>
						</td>
						<td style="text-align: center;">
							<input type="hidden" name="insercao_input_acao" id="insercao_input_acao" value=""/>
							<span id="insercao_span_acao">-</span>
						</td>
						<td style="text-align: center;">
							<input type="hidden" name="insercao_input_localizador" id="insercao_input_localizador" value=""/>
							<span id="insercao_span_localizador">-</span>
						</td>
						<?php
							
							// seleciona somente as naturezas de despesa ativas
							$sqlNatureza =
								" select ndpcod as codigo, ndpdsc as descricao " .
								" from naturezadespesa " .
								" where edpcod != '00' and ndpstatus = 'A' " .
								" order by ndpcod, ndpdsc";
							
							// seleciona somente os IDUSOs ativos
							$sqlIduso =
								" select iducod as codigo, idudsc as descricao " .
								" from identifuso " .
								" where idustatus = 'A' " .
								" order by iducod, idudsc";
							
							// seleciona somente as fontes em que a unidade possui algum limite
							$sqlFonte =
								" select foncod as codigo, fondsc as descricao " .
								" from fonterecurso " .
								" where fonstatus = 'A' " .
								" order by foncod, fondsc";
							
							// seleciona somente os IDOCs ativos
							$sqlIdoc =
								" select idocod as codigo, idodsc as descricao " .
								" from idoc " .
								" where idostatus = 'A' " .
								" order by idocod, idodsc";
							
						?>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remInsercaoAlterarCampoFocado( event, \'natureza\' );" '; ?>
							<? texto_popup( 'insercao_input_natureza', $sqlNatureza, 'Natureza da Despesa', 8, 13, '########', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remInsercaoAlterarCampoFocado( event, \'iduso\' );" '; ?>
							<? texto_popup( 'insercao_input_iduso', $sqlIduso, 'Ident. de Uso', 1, 7, '#', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remInsercaoAlterarCampoFocado( event, \'fonte\' );" '; ?>
							<? texto_popup( 'insercao_input_fonte', $sqlFonte, 'Fonte de Recursos', 3, 7, '###', '', $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<? $complemento = ' onkeypress="remInsercaoAlterarCampoFocado( event, \'idoc\' );" '; ?>
							<? texto_popup( 'insercao_input_idoc', $sqlIdoc, 'Ident. Op. de Crédito', 4, 7, '####', '', $complemento ); ?>
						</td>
						<td style="text-align: right;">
							<? $complemento = ''; ?>
							<?= campo_texto( 'insercao_input_valor', 'N', 'S', '', 18, 14, '##.###.###.###', '', 'right', '', 0, ' id="insercao_input_valor" onblur="MouseBlur(this);" ' . $complemento ); ?>
						</td>
						<td style="text-align: center;">
							<a href="javascript:remInsercaoAdicionarDoFormulario()">Ok</a>
						</td>
					</tr>
					<tr>
						<td colspan="8" style="text-align: right;">&nbsp;</td>
						<td style="text-align: right;">
							<b><span id="insercoes_span_total">0</span></b>
						</td>
						<td style="text-align: center;">&nbsp;</td>
					</tr>
				</table>
				<!-- --------------------------------------------------------------- -->
			
			<?php endif; ?>
		</div>
		
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-top: none;" style="border-top: none;">
			<tr>
				<td class="SubTituloDireita" style="text-align: left;">
					<input type="hidden" name="remanejar" value="1"/>
					<input type="button" name="botao_remanejar" value="Submeter" onclick="submeterFormulario();"/>
				</td>
			</tr>
		</table>
		
	<?php else : ?>
		
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-top: none;" >
			<tr>
				<td class="SubTituloDireita" style="text-align: left;">
					<input type="hidden" name="escolha_unidade" value="1"/>
					<input type="button" name="botao_escolha_unidade" value="Finalizar Escolha de Unidade" onclick="submeterFormulario();"/>
				</td>
			</tr>
		</table>
		
	<?php endif; ?>
</form>

<script type="text/javascript">

	// ----- FUNÇÕES E VARIÁVEIS GERAIS ----------------------------------------
	
	/**
	 * Armazena as naturezas, IDUSOs, fontes e IDOCs possíveis de serem
	 * utilizados em um array de duas dimensões. O primeiro nível agrupa pelo
	 * tipo, o segundo armazena em suas chaves os valores possíveis para o tipo
	 * especificado.
	 * 
	 * @var string[][]
	 */
	var rem_Campos = new Array();
	
		// alimenta as naturezas possíveis
		rem_Campos['natureza'] = new Array();
		<?php
		$sqlCampo = "select ndpcod as cod from naturezadespesa where edpcod != '00' and ndpstatus = 'A'";
		foreach ( $db->carregar( $sqlCampo ) as $item )
		{
			?> rem_Campos['natureza']['<?= $item['cod'] ?>'] = true; <?php
		}
		?>
		
		// alimenta os IDUSOs possíveis
		rem_Campos['iduso'] = new Array();
		<?php
		$sqlCampo = "select iducod as cod from identifuso where idustatus = 'A'";
		foreach ( $db->carregar( $sqlCampo ) as $item )
		{
			?> rem_Campos['iduso']['<?= $item['cod'] ?>'] = true; <?php
		}
		?>
		
		// alimenta as fontes possíveis
		rem_Campos['fonte'] = new Array();
		<?php
		$sqlCampo = "select foncod as cod from fonterecurso where fonstatus = 'A'";
		foreach ( $db->carregar( $sqlCampo ) as $item )
		{
			?> rem_Campos['fonte']['<?= $item['cod'] ?>'] = true; <?php
		}
		?>
		
		// alimenta os IDOCs possíveis
		rem_Campos['idoc'] = new Array();
		<?php
		$sqlCampo = "select idocod as cod from idoc where idostatus = 'A'";
		foreach ( $db->carregar( $sqlCampo ) as $item )
		{
			?> rem_Campos['idoc']['<?= $item['cod'] ?>'] = true; <?php
		}
		?>
	
	/**
	 * Altera o UPAL ( Unidade Programa Ação Localizador ). O unicod, prgcod,
	 * acacod e loccod devem ser passados separados pelo caracter ".". Os
	 * valores são colocados nos campos: retirada_input_unidade,
	 * retirada_input_programa, retirada_input_acao e
	 * retirada_input_localizador.
	 * 
	 * @param string
	 * @return void
	 */
	function rem_AlteraUpal( tipo, valor )
	{
		valor = valor.split( '.' );
		
		// força valores serem strings
		if ( valor.length != 4 )
		{
			valor[0] = '';
			valor[1] = '';
			valor[2] = '';
			valor[3] = '';
		}
		
		var spanUnidade = document.getElementById( tipo + '_span_unidade' );
		var inputUnidade = document.getElementById( tipo + '_input_unidade' );
		spanUnidade.innerHTML = valor[0];
		inputUnidade.value = valor[0];
		
		var spanPrograma = document.getElementById( tipo + '_span_programa' );
		var inputPrograma = document.getElementById( tipo + '_input_programa' );
		spanPrograma.innerHTML = valor[1];
		inputPrograma.value = valor[1];
		
		var spanAcao = document.getElementById( tipo + '_span_acao' );
		var inputAcao = document.getElementById( tipo + '_input_acao' );
		spanAcao.innerHTML = valor[2];
		inputAcao.value = valor[2];
		
		var spanLocalizador = document.getElementById( tipo + '_span_localizador' );
		var inputLocalizador = document.getElementById( tipo + '_input_localizador' );
		spanLocalizador.innerHTML = valor[3];
		inputLocalizador.value = valor[3];
		
		// desabilita campo valor, caso seja do tipo retirada
		if ( tipo == 'retirada' )
		{
			remRetiradaHabilitarValor( false );
		}
	}
	
	/**
	 * Atualiza as cores de uma tabela (cor_sim/cor_nao) a partir da linha
	 * indicada pelo segundo parâmetro.
	 * 
	 * @param object
	 * @param integer
	 * @return object
	 */
	function rem_AtualizarCor( tipo, posicaoInicial )
	{
		var tabela = document.getElementById( 'tabela_' + tipo );
		var quantidade_tr = tabela.rows.length;
		var cor = '';
		for ( var i = posicaoInicial; i < quantidade_tr; i++ )
		{
			var tr = tabela.rows[i];
			cor = cor == '' ? '#e9e9e9' : '' ;
			tr.style.backgroundColor = cor;
			tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
		}
		rem_AtualizarValor( tipo );
	}
	
	/**
	 * Atualiza valor total de retirada ou inserção.
	 * 
	 * @param string
	 * @return void
	 */
	function rem_AtualizarValor( tipo )
	{
		var span = document.getElementById( tipo + '_span_total' );
		var valor = tipo == 'retiradas' ? remRetiradaPegarValor() : remInsercaoPegarValor();
		valor = MascaraMonetario( valor + '' );
		span.innerHTML = valor.substr( 0, valor.length - 3 ); // retira o ,00 do final da string
	}
	
	/**
	 * Verifica se valor de cancelamento é igual ao valor de suplementação.
	 * 
	 * @return boolean
	 */
	function rem_ChecaValor()
	{
		return remRetiradaPegarValor() == remInsercaoPegarValor();
	}
	
	/**
	 * Cria uma célula em uma determinada linha.
	 * 
	 * @param object
	 * @param string
	 * @param string
	 * @param string
	 * @param integer
	 * @return object
	 */
	function rem_CriarCelulaItem( tr, tipo, nome, valor, align, colspan )
	{
		var celula = tr.insertCell( tr.cells.length );
		celula.colSpan = colspan;
		celula.style.textAlign = align;
		celula.appendChild( document.createTextNode( valor ) );
		celula.appendChild( rem_CriarHidden( 'remanejamento[' + tipo + '][' + nome + '][]', valor ) );
	}
	
	/**
	 * Cria um input do tipo hidden com o valor determinado.
	 * 
	 * @param string
	 * @param string
	 * @return object
	 */
	function rem_CriarHidden( nome, valor )
	{
		var input = document.createElement( 'input' );
		input.type = 'hidden';
		input.name = nome;
		input.value = valor;
		return input;
	}
	
	/**
	 * Habilita ou desabilita um campo do tipo natureza, idoc, fonte ou iduso.
	 * 
	 * @param boolean
	 * @param string
	 * @param string
	 * @return void
	 */
	function rem_HabilitarCampo( habilitar, tipo, nome )
	{
		document.getElementById( tipo + '_input_' + nome ).disabled = habilitar;
		document.getElementById( 'img_texto_popup_' + tipo + '_input_' + nome ).style.display = habilitar ? 'none' : 'inline';
	}
	
	/**
	 * Lê dados do formulário de inserção e retiradas e agrupa os dados em um
	 * array.
	 * 
	 * @param string
	 * @return string[]
	 */
	function rem_LerFormulario( tipo )
	{
		var dados = new Array();
		dados['unidade'] = document.getElementById( tipo + '_input_unidade' ).value;
		dados['programa'] = document.getElementById( tipo + '_input_programa' ).value;
		dados['acao'] = document.getElementById( tipo + '_input_acao' ).value;
		dados['localizador'] = document.getElementById( tipo + '_input_localizador' ).value;
		dados['natureza'] = document.getElementById( tipo + '_input_natureza' ).value;
		dados['iduso'] = document.getElementById( tipo + '_input_iduso' ).value;
		dados['fonte'] = document.getElementById( tipo + '_input_fonte' ).value;
		dados['idoc'] = document.getElementById( tipo + '_input_idoc' ).value;
		dados['valor'] = document.getElementById( tipo + '_input_valor' ).value;
		return dados;
	}
	
	/**
	 * Limpa os dados do formulário. Os dados de UPAL não são alterados.
	 * 
	 * @param string
	 * @return void
	 */
	function rem_LimparFormulario( tipo )
	{
		document.getElementById( tipo + '_input_natureza' ).value = '';
		document.getElementById( tipo + '_input_iduso' ).value = '';
		document.getElementById( tipo + '_input_fonte' ).value = '';
		document.getElementById( tipo + '_input_idoc' ).value = '';
		document.getElementById( tipo + '_input_valor' ).value = '';
	}
	
	/**
	 * Remove a linha de uma tabela a partir de seu identificador.
	 * 
	 * @param string
	 * @return void
	 */
	function rem_RemoverItem( id )
	{
		var tr = document.getElementById( id );
		if ( tr )
		{
			tr.parentNode.removeChild( tr );
		}
	}
	
	/**
	 * Verifica se uma natureza, iduso, fonte ou idoc é válido. Os dados que
	 * indicam existência ou não são armazenados na variável global
	 * {@link rem_Campos}.
	 * 
	 * @param string
	 * @param string
	 * @return boolean
	 */
	function rem_VerificaCampo( campo, valor )
	{
		return rem_Campos[campo][valor] == true;
	}
	
	// -------------------------------------------------------------------------
	
	// ----- FUNÇÕES E VARIÁVEIS DE RETIRADA -----------------------------------
	
	/**
	 * Armazena os valores de retirada. Os índices indicam as despesas.
	 * 
	 * @var integer[]
	 */
	var remRetiradaValor = new Array();
	
	/**
	 * Adiciona um item de retirada à tabela de itens retirados.
	 * 
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @return void
	 */
	function remRetiradaAdicionar( unidade, programa, acao, localizador, natureza, iduso, fonte, idoc, valor, erro )
	{
		var tabela = document.getElementById( 'tabela_retiradas' );
		
		// cria linha e armazena o valor adiconar à lista geral de valores retirados
		var tr = tabela.insertRow( tabela.rows.length - 1 );  // -1 porque a última linha é do total
		var id =
			unidade + '.' +
			programa + '.' +
			acao + '.' +
			localizador + '.' +
			natureza + '.' +
			iduso + '.' +
			fonte + '.' +
			idoc;
		remRetiradaValor[id] = valor;
		tr.id = 'retirada_item_' + id;
		
		// cria celula a célula de cada campo
		rem_CriarCelulaItem( tr, 'retirada', 'unidade', unidade, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'programa', programa, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'acao', acao, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'localizador', localizador, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'natureza', natureza, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'iduso', iduso, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'fonte', fonte, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'idoc', idoc, 'center', 1 );
		rem_CriarCelulaItem( tr, 'retirada', 'valor', valor, 'right', 2 );
		
		// cria celula de ações do item
		var linkDeletar = document.createElement( 'a' );
		    linkDeletar.href = 'javascript:remRetiradaRemoverItem( \'' + id + '\' );';
		    linkDeletar.title = 'del';
		    linkDeletar.innerHTML = 'del';
		var linkEditar = document.createElement( 'a' );
		    linkEditar.href = 'javascript:remRetiradaEditarItem( \'' + id + '\' );';
		    linkEditar.title = 'edt';
		    linkEditar.innerHTML = 'edt';
		var celulaAcaoes = tr.insertCell( tr.cells.length );
			celulaAcaoes.style.textAlign = 'center';
			celulaAcaoes.appendChild( linkEditar );
			celulaAcaoes.appendChild( document.createTextNode( ' ' ) );
			celulaAcaoes.appendChild( linkDeletar );
		
		if ( erro != '' )
		{
			var linkErro = document.createElement( 'a' );
				linkErro.innerHTML = 'erro';
				linkErro.title = erro;
				linkErro.href = '#';
				linkErro.onclick = Function( "alert( '" + erro + "' )" );
				linkErro.style.color = '#ff0000';
			celulaAcaoes.appendChild( document.createElement( 'br' ) );
			celulaAcaoes.appendChild( linkErro );
		}
		
		// altualiza a cor_sim/cor_nao da tabela ( a partir da terceira linha )
		rem_AtualizarCor( 'retiradas', 2 );
	}
	
	/**
	 * Verifica os campos do item a ser retirado e, em caso de sucesso, insere o
	 * item na tabela de itens retirados, caso contrário o item não é adicionado
	 * à tabela.
	 * 
	 * @return void
	 */
	function remRetiradaAdicionarDoFormulario()
	{
		var inputRetirada = document.getElementById( 'retirada_input_valor' );
		
		// verifica se existe saldo
		var spanSaldo = document.getElementById( 'retirada_span_saldo' );
		var saldo = parseInt( spanSaldo.innerHTML.replace( /\./g, '' ) );
		if ( saldo <= 0 )
		{
			alert( 'A despesa não possui saldo para a realização dessa operação.' );
			inputRetirada.focus();
			inputRetirada.select();
			return;
		}
		
		// verifica se retirada é válida
		var dados = rem_LerFormulario( 'retirada' );
		var valorRetirado = parseInt( dados['valor'].replace( /\./g, '' ) );
		if ( isNaN( valorRetirado ) || valorRetirado <= 0 )
		{
			alert( 'Erro! O valor retirado não é válido.' );
			inputRetirada.focus();
			inputRetirada.select();
			return;
		}
		
		// verifica se retirada é maior que saldo
		if ( valorRetirado > saldo )
		{
			alert( 'Erro! O valor retirado é maior que o saldo disponível na despesa.' );
			inputRetirada.focus();
			inputRetirada.select();
			return;
		}
		
		// adiciona de fato o item à tabela de itens retirados
		remRetiradaAdicionar(
			dados['unidade'], dados['programa'],
			dados['acao'], dados['localizador'],
			dados['natureza'], dados['iduso'],
			dados['fonte'], dados['idoc'],
			dados['valor'], ''
		);
		
		// limpa fomulário e desabilita valor para nova inserção
		rem_LimparFormulario( 'retirada' );
		remRetiradaHabilitarValor( false );
	}
	
	/**
	 * Verifica o evento ocorrido aos campos de cadastro de retiradas. Em caso
	 * do caracter 'enter' ter sido digitado o cursor é movido para o próximo
	 * campo, caso todos estejam preenchidos o saldo é exibido e o campo valor é
	 * acionado.
	 * 
	 * @param object
	 * @param string
	 * @return void
	 */
	function remRetiradaAlterarCampoFocado( event, campo )
	{
		if ( event.keyCode != 13 )
		{
			return;
		}
		if ( campo == 'idoc' )
		{
			var input = document.getElementById( 'retirada_input_idoc' );
			input.blur();
		}
		var habilitarValor = remRetiradaHabilitarValor( campo == 'idoc' );
		switch ( campo )
		{
			case 'natureza':
				var input = document.getElementById( 'retirada_input_iduso' );
				break;
			case 'iduso':
				var input = document.getElementById( 'retirada_input_fonte' );
				break;
			case 'fonte':
				var input = document.getElementById( 'retirada_input_idoc' );
				break;
			case 'idoc':
				if ( !habilitarValor )
				{
					return;
				}
				var input = document.getElementById( 'retirada_input_valor' );
				break;
			default:
				return;
		}
		input.focus();
		input.select();
	}
	
	/**
	 * Altera o UPAL ( Unidade Programa Ação Localizador ). Para mais detalhes
	 * ler documentação da função {@link rem_AlteraUpal()}
	 * 
	 * @param string
	 * @return void
	 */
	function remRetiradaAlteraUpal( valor )
	{
		rem_AlteraUpal( 'retirada', valor );
	}
	
	/**
	 * Consulta o saldo de uma despesa e atualiza o valor do saldo em tela. A
	 * despesa verifica é a retornada pela função {@link rem_LerFormulario()}.
	 * 
	 * @return void
	 */
	function remRetiradaAtualizaSaldo()
	{
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/geral/saldoDespesa.php';
		var dados = rem_LerFormulario( 'retirada' );
		var requisicao = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
		requisicao.open(
			'GET',
			url +
				'?proposta=<?= remPegaPpoid() ?>' +
				'&remid=<?= $remid ?>' +
				'&unidade=' + dados['unidade'] +
				'&programa=' + dados['programa'] +
				'&acao=' + dados['acao'] +
				'&localizador=' + dados['localizador'] +
				'&natureza=' + dados['natureza'] +
				'&iduso=' + dados['iduso'] +
				'&fonte=' + dados['fonte'] +
				'&idoc=' + dados['idoc'],
			true
		);
		requisicao.onreadystatechange = function(){ remRetiradaCapturaSaldo( requisicao ) };
		requisicao.send( null );
	}
	
	/**
	 * Cancela a inserção de uma retirada.
	 * 
	 * @return void
	 */
	function remRetiradaCancelarInsercao()
	{
		// desabilita campo valor
		remRetiradaHabilitarValor( false );
	}
	
	/**
	 * Ouve a requisição de um pedido de consulta à algum saldo de alguma
	 * despesa. Quando o saldo é retornado o valor é colocado no elemento com id
	 * "retirada_span_saldo".
	 * 
	 * @param object
	 * @return void
	 */
	function remRetiradaCapturaSaldo( requisicao )
	{
		if ( requisicao.readyState != 4 )
		{
			return;
		}
		if ( requisicao.status == 200 )
		{
			var spanSaldo = document.getElementById( 'retirada_span_saldo' );
			spanSaldo.innerHTML = requisicao.responseText;
		}
		else
		{
			alert( 'Não foi possível capturar o saldo da despesa ' );
		}
		requisicao = null;
	}
	
	/**
	 * Aciona a edição de um item de retirada.
	 * 
	 * @param string
	 * @return void
	 */
	function remRetiradaEditarItem( id )
	{
		// separa os dados do id ( unidade, programa, ação, localizador, natureza, iduso, fonte, idoc )
		var dados = id.split( '.' );
		
		// monta upal ( unidade / programa / ação / localizador )
		var upal = dados[0] + '.' + dados[1] + '.' + dados[2] + '.' + dados[3]; 
		
		// remove linha da tabela
		// antes armazena o valor da retirada pra ser colocado no formulário
		dados[8] = remRetiradaValor[id];
		remRetiradaRemoverItem( id );
		
		// verifica se UPAL existe
		var selectUpal = document.getElementById( 'select_upal_retirada' );
		var tamanho = selectUpal.options.length;
		var existeUpal = false;
		for( var i = 0; i < tamanho; i++ )
		{
			if ( selectUpal.options[i].value == upal )
			{
				selectUpal.options[i].selected = true;
				existeUpal = true;
				break;
			}
		}
		if ( existeUpal == false )
		{
			return;
		}
		
		// povoa formulário com os dados preenchidos
		remRetiradaAlteraUpal( upal );
		document.getElementById( 'retirada_input_natureza' ).value = dados[4];
		document.getElementById( 'retirada_input_iduso' ).value = dados[5];
		document.getElementById( 'retirada_input_fonte' ).value = dados[6];
		document.getElementById( 'retirada_input_idoc' ).value = dados[7];
		document.getElementById( 'retirada_input_valor' ).value = dados[8];
		
		// habilita campo valor
		remRetiradaHabilitarValor( true );
	}
	
	/**
	 * Habilita ou desabilita o preenchimento do campo valor de uma retirada. A
	 * chamada dessa função deve ser considerada uma tentativa de habilitação ou
	 * desabilitação, para verificar se a ação foi realizada com sucesso utilize
	 * o retorno dessa função que indica o status atual de habilitação do campo
	 * valor.
	 * 
	 * @param boolean
	 * @return boolean
	 */
	function remRetiradaHabilitarValor( habilitar )
	{
		// captura o input que define o UPAL
		// utilizado somente para efeito de bloquei o ou não do campo
		// os valores devem ser pegos nos inputs referenciados acima
		var selectUpal = document.getElementById( 'select_upal_retirada' );
		
		// captura elementos utilizados para finalização de inserção de uma retirada
		var spanSaldo = document.getElementById( 'retirada_span_saldo' );
		var inputValor = document.getElementById( 'retirada_input_valor' );
		var spanAcoesDesligado = document.getElementById( 'retirada_span_acoes_desligado' );
		var spanAcoesLigado = document.getElementById( 'retirada_span_acoes_ligado' );
		
		// caso a ação seja de habilitar, verifica se todos os campos estão preenchidos
		var dados = rem_LerFormulario( 'retirada' );
		habilitar =
			habilitar &&
			dados['unidade'] != '' && dados['programa'] != '' &&
			dados['acao'] != '' && dados['localizador'] != '' &&
			dados['natureza'] != '' && dados['iduso'] != '' &&
			dados['fonte'] != '' && dados['idoc'] != '';
		
		// em caso de habilitação verifica se os dados digitados são válidos
		if ( habilitar == true )
		{
			if ( rem_VerificaCampo( 'natureza', dados['natureza'] ) == false )
			{
				alert( 'Natureza inválida' );
				habilitar = false;
			}
			else if ( rem_VerificaCampo( 'iduso', dados['iduso'] ) == false )
			{
				alert( 'IDUSO inválido' );
				habilitar = false;
			}
			else if ( rem_VerificaCampo( 'fonte', dados['fonte'] ) == false )
			{
				alert( 'Fonte inválida' );
				habilitar = false;
			}
			else if ( rem_VerificaCampo( 'idoc', dados['idoc'] ) == false )
			{
				alert( 'IDOC inválida' );
				habilitar = false;
			}
		}
		
		if ( habilitar )
		{
			// atualiza o saldo a ser visualizado e verificado posteriormente
			remRetiradaAtualizaSaldo();
			
			// desabilita os campos de preenchimento da despesa
			selectUpal.disabled = true;
			rem_HabilitarCampo( true, 'retirada', 'natureza' );
			rem_HabilitarCampo( true, 'retirada', 'iduso' );
			rem_HabilitarCampo( true, 'retirada', 'fonte' );
			rem_HabilitarCampo( true, 'retirada', 'idoc' );
			
			// habilita as ações de inserção e cancelamento de inserção de retirada
			spanAcoesLigado.style.display = 'block';
			spanAcoesDesligado.style.display = 'none';
			
			// habilita campo de valor
			inputValor.disabled = false;
			
			// verifica se despesa já foi utilizada, em caso de sim o valor da
			// tabela é preenchido no campo valor e a linha é removida da tabela
			dados = rem_LerFormulario( 'retirada' );
			var id =
				dados['unidade'] + '.' +
				dados['programa'] + '.' +
				dados['acao'] + '.' +
				dados['localizador'] + '.' +
				dados['natureza'] + '.' +
				dados['iduso'] + '.' +
				dados['fonte'] + '.' +
				dados['idoc'];
			if ( remRetiradaValor[id] && remRetiradaValor[id] != 0 )
			{
				inputValor.value = remRetiradaValor[id];
				remRetiradaValor[id] = 0;
				rem_RemoverItem( 'retirada_item_' + id );
				// altualiza a cor_sim/cor_nao da tabela ( a partir da terceira linha )
				rem_AtualizarCor( 'retiradas', 2 );
			}
			
			// direciona o foco so campo valor
			inputValor.focus();
			inputValor.select();
		}
		else
		{
			// desabilita os campos de preenchimento da despesa
			selectUpal.disabled = false;
			rem_HabilitarCampo( false, 'retirada', 'natureza' );
			rem_HabilitarCampo( false, 'retirada', 'iduso' );
			rem_HabilitarCampo( false, 'retirada', 'fonte' );
			rem_HabilitarCampo( false, 'retirada', 'idoc' );
			
			// limpa saldo visualizado
			spanSaldo.innerHTML = '';
			
			// desabilita as ações de inserção e cancelamento de inserção de retirada
			spanAcoesLigado.style.display = 'none';
			spanAcoesDesligado.style.display = 'block';
			
			// desabilita campo de valor, altera seu valor pra vazio
			// direciona o foco ao campo natureza
			inputValor.disabled = true;
			inputValor.value = '';
			var inputNatureza = document.getElementById( 'retirada_input_natureza' );
			inputNatureza.focus();
			inputNatureza.select();
		}
		
		// retorna status atual de habilitação
		return habilitar;
	}
	
	/**
	 * Calcula o valor total de cancelamento.
	 * 
	 * @return integer
	 */
	function remRetiradaPegarValor()
	{
		var chave;
		var valor = 0;
		for ( chave in remRetiradaValor )
		{
			if ( typeof remRetiradaValor[chave] != 'string' )
			{
				continue;
			}
			valor += parseInt( remRetiradaValor[chave].replace( /\./g, '' ) );
		}
		return valor;
	}
	
	/**
	 * Cancela uma retirada a partir do seu identificador.
	 * 
	 * @param string
	 * @return void
	 */
	function remRetiradaRemoverItem( id )
	{
		// zera o valor retirada
		remRetiradaValor[id] = 0;
		// remove o item da tabela
		rem_RemoverItem( 'retirada_item_' + id );
		// altualiza a cor_sim/cor_nao da tabela ( a partir da terceira linha )
		rem_AtualizarCor( 'retiradas', 2 );
	}
	
	// -------------------------------------------------------------------------
	
	// ----- FUNÇÕES E VARIÁVEIS DE INSERÇÃO -----------------------------------
	
	/**
	 * Armazena os valores de inserção. Os índices indicam as despesas.
	 * 
	 * @var integer[]
	 */
	var remInsercaoValor = new Array();
	
	/**
	 * Verifica o evento ocorrido aos campos de cadastro de inserção. Em caso
	 * do caracter 'enter' ter sido digitado o cursor é movido para o próximo
	 * campo.
	 * 
	 * @param object
	 * @param string
	 * @return void
	 */
	function remInsercaoAlterarCampoFocado( event, campo )
	{
		if ( event.keyCode != 13 )
		{
			return;
		}
		switch ( campo )
		{
			case 'natureza':
				var input = document.getElementById( 'insercao_input_iduso' );
				break;
			case 'iduso':
				var input = document.getElementById( 'insercao_input_fonte' );
				break;
			case 'fonte':
				var input = document.getElementById( 'insercao_input_idoc' );
				break;
			case 'idoc':
				var input = document.getElementById( 'insercao_input_valor' );
				break;
			default:
				return;
		}
		input.focus();
		input.select();
	}
	
	/**
	 * Adiciona um item de inserção à tabela de itens inseridos.
	 * 
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @param string
	 * @return void
	 */
	function remInsercaoAdicionar( unidade, programa, acao, localizador, natureza, iduso, fonte, idoc, valor, erro )
	{
		var tabela = document.getElementById( 'tabela_insercoes' );
		
		// cria identificador do item de despesa
		var id =
			unidade + '.' +
			programa + '.' +
			acao + '.' +
			localizador + '.' +
			natureza + '.' +
			iduso + '.' +
			fonte + '.' +
			idoc;
		
		// remove o item de inserção
		// evita que exista despesas repetidas
		remInsercaoRemoverItem( id );
		
		// armazena o valor adiconar à lista geral de valores inseridos
		remInsercaoValor[id] = valor;
		
		// cria linha
		var tr = tabela.insertRow( tabela.rows.length - 1 );  // -1 porque a última linha é do total
		tr.id = 'insercao_item_' + id;
		
		// cria celula a célula de cada campo
		rem_CriarCelulaItem( tr, 'insercao', 'unidade', unidade, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'programa', programa, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'acao', acao, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'localizador', localizador, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'natureza', natureza, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'iduso', iduso, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'fonte', fonte, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'idoc', idoc, 'center', 1 );
		rem_CriarCelulaItem( tr, 'insercao', 'valor', valor, 'right', 1 );
		
		// cria celula de ações do item
		var linkDeletar = document.createElement( 'a' );
		    linkDeletar.href = 'javascript:remInsercaoRemoverItem( \'' + id + '\' );';
		    linkDeletar.title = 'del';
		    linkDeletar.innerHTML = 'del';
		var linkEditar = document.createElement( 'a' );
		    linkEditar.href = 'javascript:remInsercaoEditarItem( \'' + id + '\' );';
		    linkEditar.title = 'edt';
		    linkEditar.innerHTML = 'edt';
		var celulaAcaoes = tr.insertCell( tr.cells.length );
			celulaAcaoes.style.textAlign = 'center';
			celulaAcaoes.appendChild( linkEditar );
			celulaAcaoes.appendChild( document.createTextNode( ' ' ) );
			celulaAcaoes.appendChild( linkDeletar );
		
		if ( erro != '' )
		{
			var linkErro = document.createElement( 'a' );
				linkErro.innerHTML = 'erro';
				linkErro.title = erro;
				linkErro.href = '#';
				linkErro.onclick = Function( "alert( '" + erro + "' )" );
				linkErro.style.color = '#ff0000';
			celulaAcaoes.appendChild( document.createElement( 'br' ) );
			celulaAcaoes.appendChild( linkErro );
		}
		
		// altualiza a cor_sim/cor_nao da tabela ( a partir da terceira linha )
		rem_AtualizarCor( 'insercoes', 2 );
	}
	
	/**
	 * Verifica os campos do item a ser inserido e, em caso de sucesso, insere o
	 * item na tabela de itens retirados, caso contrário o item não é adicionado
	 * à tabela.
	 * 
	 * @return void
	 */
	function remInsercaoAdicionarDoFormulario()
	{
		// verifica se todos os campos estão preenchidos
		var dados = rem_LerFormulario( 'insercao' );
		var podeInserir =
			dados['unidade'] != '' && dados['programa'] != '' &&
			dados['acao'] != '' && dados['localizador'] != '' &&
			dados['natureza'] != '' && dados['iduso'] != '' &&
			dados['fonte'] != '' && dados['idoc'] != '' &&
			dados['valor'] != '';
		if ( podeInserir == false )
		{
			alert( 'Preencha todos os campos para o cadastro de inserção.' );
			return;
		}
		
		// verifica se valor é válido
		var valorRetirado = parseInt( dados['valor'].replace( /\./g, '' ) );
		if ( valorRetirado <= 0 || valorRetirado == '' || isNaN( valorRetirado ) == true )
		{
			alert( 'Insira um valor de inserção válido.' );
			var inputRetirada = document.getElementById( 'insercao_input_valor' );
			inputRetirada.focus();
			inputRetirada.select();
			return;
		}
		
		// verifica se os dados digitados são válidos
		if ( rem_VerificaCampo( 'natureza', dados['natureza'] ) == false )
		{
			alert( 'Natureza inválida' );
			return;
		}
		if ( rem_VerificaCampo( 'iduso', dados['iduso'] ) == false )
		{
			alert( 'IDUSO inválido' );
			return;
		}
		if ( rem_VerificaCampo( 'fonte', dados['fonte'] ) == false )
		{
			alert( 'Fonte inválida' );
			return;
		}
		if ( rem_VerificaCampo( 'idoc', dados['idoc'] ) == false )
		{
			alert( 'IDOC inválida' );
			return;
		}
		
		// adiciona de fato o item à tabela de itens retirados
		remInsercaoAdicionar(
			dados['unidade'], dados['programa'],
			dados['acao'], dados['localizador'],
			dados['natureza'], dados['iduso'],
			dados['fonte'], dados['idoc'],
			dados['valor'], ''
		);
		
		// limpa fomulário e desabilita valor para nova inserção
		rem_LimparFormulario( 'insercao' );
	}
	
	/**
	 * Altera o UPAL ( Unidade Programa Ação Localizador ). Para mais detalhes
	 * ler documentação da função {@link rem_AlteraUpal()}
	 * 
	 * @param string
	 * @return void
	 */
	function remInsercaoAlteraUpal( valor )
	{
		rem_AlteraUpal( 'insercao', valor );
	}
	
	
	/**
	 * Aciona a edição de um item de inserção.
	 * 
	 * @param string
	 * @return void
	 */
	function remInsercaoEditarItem( id )
	{
		// separa os dados do id ( unidade, programa, ação, localizador, natureza, iduso, fonte, idoc )
		var dados = id.split( '.' );
		
		// monta upal ( unidade / programa / ação / localizador )
		var upal = dados[0] + '.' + dados[1] + '.' + dados[2] + '.' + dados[3]; 
		
		// remove linha da tabela
		// antes armazena o valor da retirada pra ser colocado no formulário
		dados[8] = remInsercaoValor[id];
		remInsercaoRemoverItem( id );
		
		// verifica se UPAL existe
		var selectUpal = document.getElementById( 'select_upal_insercao' );
		var tamanho = selectUpal.options.length;
		var existeUpal = false;
		for( var i = 0; i < tamanho; i++ )
		{
			if ( selectUpal.options[i].value == upal )
			{
				selectUpal.options[i].selected = true;
				existeUpal = true;
				break;
			}
		}
		if ( existeUpal == false )
		{
			return;
		}
		
		// povoa formulário com os dados preenchidos e direciona o foco no valor
		remInsercaoAlteraUpal( upal );
		document.getElementById( 'insercao_input_natureza' ).value = dados[4];
		document.getElementById( 'insercao_input_iduso' ).value = dados[5];
		document.getElementById( 'insercao_input_fonte' ).value = dados[6];
		document.getElementById( 'insercao_input_idoc' ).value = dados[7];
		var inputValor = document.getElementById( 'insercao_input_valor' );
		inputValor.value = dados[8];
		inputValor.focus();
		inputValor.select();
	}
	
	/**
	 * Calcula o valor total de suplementação.
	 * 
	 * @return integer
	 */
	function remInsercaoPegarValor()
	{
		var chave;
		var valor = 0;
		for ( chave in remInsercaoValor )
		{
			if ( typeof remInsercaoValor[chave] != 'string' )
			{
				continue;
			}
			valor += parseInt( remInsercaoValor[chave].replace( /\./g, '' ) );
		}
		return valor;
	}
	
	/**
	 * Cancela uma inserção a partir do seu identificador.
	 * 
	 * @param string
	 * @return void
	 */
	function remInsercaoRemoverItem( id )
	{
		// zera o valor retirada
		remInsercaoValor[id] = 0;
		// remove o item da tabela
		rem_RemoverItem( 'insercao_item_' + id );
		// altualiza a cor_sim/cor_nao da tabela ( a partir da terceira linha )
		rem_AtualizarCor( 'insercoes', 2 );
	}
	
	// -------------------------------------------------------------------------
	
	function submeterFormulario()
	{
		<?php if ( $acao == 'R' ) : ?>
			if ( rem_ChecaValor() == false )
			{
				alert( 'Os valores de cancelamento e suplementação devem ser iguais.' );
				return;
			}
		<?php endif; ?>
		document.formulario.submit();
	}
	
	function alterarAba( aba )
	{
		linkDadosGerais = document.getElementById( 'span_dados_gerais' );
		divDadosGerais = document.getElementById( 'div_dados_gerais' );
		linkRemanejamentos = document.getElementById( 'span_remanejamentos' );
		divRemanejamentos = document.getElementById( 'div_remanejamentos' );
		if ( aba == 'dados_gerais' )
		{
			linkDadosGerais.style.fontWeight = 'bold';
			divDadosGerais.style.display = 'inline';
			linkRemanejamentos.style.fontWeight = '';
			divRemanejamentos.style.display = 'none';
		}
		else
		{
			linkDadosGerais.style.fontWeight = '';
			divDadosGerais.style.display = 'none';
			linkRemanejamentos.style.fontWeight = 'bold';
			divRemanejamentos.style.display = 'inline';
		}
	}
	
	<?php if ( $unicod ) : ?>
		alterarAba( 'dados_gerais' );
	<?php endif; ?>
	
	<?php if ( $acao != 'S' ) : ?>
		remRetiradaHabilitarValor( false );
	<?php endif; ?>
	
	<?php foreach ( $remanejamentos as $item ) : ?>
		// ( unidade, programa, acao, localizador, natureza, iduso, fonte, idoc, valor )
		<?php $funcao = $item['valor'] > 0 ? 'remInsercaoAdicionar' : 'remRetiradaAdicionar'; ?>
		<?= $funcao ?>(
			'<?= $item['unicod'] ?>',
			'<?= $item['prgcod'] ?>',
			'<?= $item['acacod'] ?>',
			'<?= $item['loccod'] ?>',
			'<?= $item['ndpcod'] ?>',
			'<?= $item['iducod'] ?>',
			'<?= $item['foncod'] ?>',
			'<?= $item['idocod'] ?>',
			'<?= abs( $item['valor'] ) ?>',
			'<?= $item['erro'] ?>'
		);
	<?php endforeach; ?>
	
</script>


