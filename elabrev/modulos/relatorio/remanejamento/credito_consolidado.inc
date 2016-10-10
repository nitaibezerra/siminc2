<?php
$agrupadorCredito =
<<<EOF
	<table>
		<tr valign="middle">
			<td>
				<select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 140px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<!--
				<img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
				-->
				<img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
			</td>
			<td>
				<select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 140px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
				<img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
			</td>
		</tr>
	</table>
	<script type="text/javascript" language="javascript">
		limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
		limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
		{POVOAR_ORIGEM}
		{POVOAR_DESTINO}
		sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
	</script>
EOF;
include APPRAIZ . 'includes/Agrupador.php';

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, 'Relatório de Remanejamento' );

$unicod = (string) $_REQUEST['unicod'];
$tcrid = (integer) $_REQUEST['tcrid'];
$mcrid = (integer) $_REQUEST['mcrid'];
$agrupado = $_REQUEST['agrupado'];
if ( !$agrupado )
{
	$agrupado = array( 'cre', 'uni' );
}

// força identificador de momento
/*
if ( !$mcrid )
{
	$sql = "select mcrid from elabrev.momentocredito order by mcrdatainiciouo desc limit 1";
	$mcrid = (integer) $db->pegaUm( $sql );
}
*/

?>
<script type="text/javascript">
	
	function mostrarLancamentos( tcrid, unicod, mcrid )
	{
		var url =
			'?modulo=relatorio/remanejamento/credito_unidade&acao=A' +
				'&tcrid=' + tcrid +
				'&unicod=' + unicod + 
				'&mcrid=' + mcrid;
		window.open(
			url,
			'creditosAdicionaisDetalhesUnidade',
			'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=yes,width=650,height=550'
		);
	}
	
	function atualizarRelatorio()
	{
		if ( document.filtro.agrupado.options.length == 0 )
		{
			alert( 'Escolha ao menos um item para agrupar os remanejamentos.' );
			return;
		}
		selectAllOptions( document.filtro.agrupado );
		document.filtro.submit();
	}
	
</script>
<form action="" name="filtro" method="post">
	<table align="center" border="0" class="tabela" cellpadding="5" cellspacing="0" style="border-top: none;border-bottom: none; padding-top: 1px;">
		<tr bgcolor="#e9e9e9">
			<td align="center" style="width: 300px; padding-left: 10px;" rowspan="3">
				<table align="center" border="0" cellpadding="0" cellspacing="0">
					<tr>
						<td width="115">&nbsp;</td>
						<td width="185" align="center">Agrupar</td>
					</tr>
					<tr>
						<td colspan="2" align="center">
							<?php
								
								// inicia agrupador
								$agrupador = new Agrupador( 'filtro', $agrupadorCredito );
								$destino = array();
								$origem = array(
									'uni' => array(
										'codigo'    => 'uni',
										'descricao' => 'Unidade Orçamentária'
									),
									'cre' => array(
										'codigo'    => 'cre',
										'descricao' => 'Tipo de Crédito'
									)
								);
								
								// move os dados da origem par ao destino de acordo com os campo da variável $agrupado
								foreach ( $agrupado as $campo )
								{
									if ( array_key_exists( $campo, $origem ) )
									{
										array_push( $destino, $origem[$campo] );
										unset( $origem[$campo] );
									}
								}
								
								// exibe agrupador
								$agrupador->setOrigem( 'naoAgrupado', null, $origem );
								$agrupador->setDestino( 'agrupado', null, $destino );
								$agrupador->exibir();
								
							?>
						</td>
					</tr>
				</table>
			</td>
			<td align="right" valign="middle" width="140">
				Unidade Orçamentária
			</td>
			<td align="left" valign="middle" width="1">
				<?php
					$join = $db->usuarioJoinUnidadesPermitidas();
					$sql = "
						select
							'' as codigo,
							'Todas as Unidades Orçamentárias' as descricao
						union all
						(
							select
								u.unicod as codigo,
								u.unicod || ' - ' || SUBSTRING( u.unidsc, 0, 30 ) as descricao
							from public.unidade u
								" . $join . " and
								unijoin.unicod = u.unicod and
								unijoin.unitpocod = u.unitpocod
							where
								u.orgcod = '". CODIGO_ORGAO_SISTEMA. "' and
								u.unistatus = 'A'
							group by
								u.unicod, u.unidsc
							order by
								u.unicod
						)
					";

					//perfis que podem gerar relatórios utilizando todas as unidades
					$perfil = array('50', '52', '23', '55', '76');
					
					if( verificaPerfil($perfil) ){				
						
						$db->monta_combo( 'unicod', $sql, 'S', '', '', '', '' );
						
					}else{
						$sql_unicod = "select unicod from seguranca.usuario where usucpf='".$_SESSION['usucpf']."'";

						$unicod = $db->pegaUm($sql_unicod);

						$sql = "					
							select
								u.unicod as codigo,
								u.unicod || ' - ' || SUBSTRING( u.unidsc, 0, 30 ) as descricao
							from public.unidade u
								" . $join . " and
								unijoin.unicod = u.unicod and
								unijoin.unitpocod = u.unitpocod
							where
								u.orgcod = '". CODIGO_ORGAO_SISTEMA. "' and
								u.unistatus = 'A' and
								u.unicod = '".$unicod."'
							group by
								u.unicod, u.unidsc
							order by
								u.unicod
						";

					$db->monta_combo( 'unicod', $sql, 'S', '', '', '', '' );						
					}
					
				?>
			</td>
			<td align="center" valign="middle" rowspan="3">
				<input type="button" name="filtrar" value="Atualizar Relatório" onclick="atualizarRelatorio();"/>
			</td>
		</tr>
		<tr bgcolor="#e9e9e9">
			<td align="right" valign="middle">
				Tipo de crédito
			</td>
			<td align="left" valign="middle">
				<?php
					$sql = "
						select
							0 as codigo,
							'Todos os Tipos de Crédito' as descricao
						union all
						(
							select
								tcrid as codigo,
								tcrcod || ' - ' || SUBSTRING( tcrdsc, 0, 30 ) as descricao
							from elabrev.tipocredito
							where
								tcrstatus = 'A' and
								tcrano = '{$_SESSION['exercicio']}'
							order by
								tcrcod
						)
					";
					$db->monta_combo( 'tcrid', $sql, 'S', '', '', '', '' );
				?>
			</td>
		</tr>
		<tr bgcolor="#e9e9e9">
			<td align="right" valign="middle">
				Momento
			</td>
			<td align="left" valign="middle">
				<?php
					$sql = "
						select * from
						(
								select
									0 as codigo,
									'Todos os momentos de crédito' as descricao
							union
								select
									mcrid as codigo,
									mcrdsc as descricao
								from elabrev.momentocredito
								where mcrano = '{$_SESSION['exercicio']}'
								and mcrstatus = 'A'
						) as foo
					";
					$db->monta_combo( 'mcrid', $sql, 'S', '', '', '', '' );
				?>
			</td>
		</tr>
	</table>
</form>
<?php

$ano = $_SESSION['exercicio'];

// captura proposta ativa
$sql =
	" select " .
		" ppoid " .
	" from elabrev.propostaorcamento " .
	" where " .
		" ppoano = '" . $ano . "' and " .
		" tppid = 3 and " .
		" ppostatus = 'A'";
$ppoid = (integer) $db->pegaUm( $sql );

$where = array();
if ( $unicod )
{
	array_push( $where, " unicod = '" . $unicod . "' " );
}
if ( $tcrid )
{
	array_push( $where, " tcrid = " . $tcrid . " " );
}
if ( $mcrid )
{
	array_push( $where, " mcrid = " . $mcrid . " " );
	$valor_momento = " = " . $mcrid . " ";
}
else
{
	array_push( $where, " mcrid is not null " );
	$valor_momento = " is not null ";
}
$where = implode( " and ", $where );

$sql = "
	select
		tcrid, SUBSTRING( tcrdsc, 0, 70 ) || '...' as tcrdsc, tcrcod, unicod, unidsc,
		" . ( $mcrid ? " mcrid, " : " null as mcrid, " ) . "
		abs ( sum( sup ) ) as sup,
		abs ( sum( can ) ) as can,
		abs ( sum( acrescimo ) ) as acrescimo,
		abs ( sum( reducao ) ) as reducao,
		abs( sum( sup ) ) - abs( sum( can ) ) - abs( sum( acrescimo ) ) + abs( sum( reducao ) ) as diferenca
	from
	(
			select
				tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc, mcrid,
				sum( d.dpavalor ) as sup,
				0 as can,
				0 as acrescimo,
				0 as reducao
			from elabrev.despesaacao d
				inner join monitora.acao a on a.acaid = d.acaidloa
				inner join public.unidade u on u.unicod = a.unicod and u.unitpocod = a.unitpocod
				inner join elabrev.tipocredito tc on tc.tcrid = d.tcrid
			where
				d.tcrid is not null and
				d.dpavalor > 0 and
				a.prgano = '$ano' and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc, mcrid
		union all
			select
				tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc, mcrid,
				0 as sup,
				sum( d.dpavalor ) as can,
				0 as acrescimo,
				0 as reducao
			from elabrev.despesaacao d
					inner join monitora.acao a on a.acaid = d.acaidloa
					inner join public.unidade u on u.unicod = a.unicod and u.unitpocod = a.unitpocod
					inner join elabrev.tipocredito tc on tc.tcrid = d.tcrid
			where
				d.tcrid is not null and
				d.dpavalor < 0 and
				a.prgano = '$ano' and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc, mcrid
		union all
			select
				tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc, mcrid,
				0 as sup,
				0 as can,
				sum( ru.rcuacrescimo ) as acrescimo,
				sum( ru.rcureducao ) as reducao
			from elabrev.receitaunidade ru
					inner join public.unidade u on u.unicod = ru.unicod and u.unitpocod = ru.unitpocod
					inner join elabrev.tipocredito tc on tc.tcrid = ru.tcrid
			where
				ru.rcuano = '$ano' and
				ru.mcrid $valor_momento
			group by
				tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc, mcrid
	) as a
	where
		$where
	group by
		tcrid, tcrcod, tcrdsc, unicod, unidsc" . ( $mcrid ? ", mcrid " : "" ) . "
";
$itens_cru = $db->carregar( $sql );
$itens = array();

// prepara para agrupar
$agrupamento = array();
reset( $agrupado );
foreach ( $agrupado as $campo )
{
	if ( $campo == 'cre' )
	{
		array_push(
			$agrupamento,
			array(
				'cod' => 'tcrcod',
				'dsc' => 'tcrdsc'
			)
		);
	}
	else if ( $campo == 'uni' )
	{
		array_push(
			$agrupamento,
			array(
				'cod' => 'unicod',
				'dsc' => 'unidsc'
			)
		);
	}
}

// define função que realiza agrupamento
function agrupar( $itens, $agrupamento )
{
	$itens = $itens ? $itens : array();
	$agrupamento_atual = array_shift( $agrupamento );
	$campo_cod = $agrupamento_atual['cod'];
	$campo_dsc = $agrupamento_atual['dsc'];
	$retorno = array();
	// verifica se é último nível (mais baixo)
	if ( count( $agrupamento ) == 0 )
	{
		foreach ( $itens as &$item )
		{
			$item['cod'] = $item[$campo_cod];
			$item['dsc'] = $item[$campo_dsc];
			$retorno[$item[$campo_cod]] = $item;
		}
		ksort( $retorno );
		return $retorno;
	}
	$itens = $itens ? $itens : array();
	foreach ( $itens as $item )
	{
		$chave = $item[$campo_cod];
		if ( !array_key_exists( $chave, $retorno ) )
		{
			$retorno[$chave] = array(
				'cod'    => $item[$campo_cod],
				'dsc'    => $item[$campo_dsc],
				'mcrid'  => $item['mcrid'],
				'tcrid'  => '',
				'itens' => array()
			);
			if ( $campo_cod == 'tcrcod' )
			{
				$retorno[$chave]['tcrid'] = $item['tcrid'];
			}
		}
		array_push( $retorno[$chave]['itens'], $item );
	}
	foreach ( $retorno as &$item )
	{
		$item['itens'] = agrupar( $item['itens'], $agrupamento );
	}
	ksort( $retorno );
	return $retorno;
}

function desenhaItem( $item, $profundidade = -1 )
{
	if ( count( $item ) === 5 ) // detecta se é agrupador
	{
		desenhaAgrupador( $item, $profundidade + 1 );
	}
	else
	{
		desenhaItemFinal( $item, $profundidade + 1 );
	}
}

function desenhaAgrupador( $item, $profundidade )
{
	global $cor;
	global $mcrid;
	$cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff';
	?>
	<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
		<td colspan="6" style="padding-left: <?php echo ( $profundidade * 10 ) + 4; ?>px;">
			<?php if ( $item['tcrid'] && $mcrid ) : ?>
				<a href="javascript:mostrarLancamentos( '<?php echo $item['tcrid']; ?>', '', '<?php echo $item['mcrid']; ?>' );">
			<?php endif; ?>
			<font size="3"><b><?php echo $item['cod']; ?></b></font>
			&nbsp;&nbsp;&nbsp;
			<font size="2"><?php echo $item['dsc']; ?></font>
			<?php if ( $item['tcrid'] && $mcrid ) : ?>
				</a>
			<?php endif; ?>
		</td>
	</tr>
	<?php
	foreach ( $item['itens'] as $subitem )
	{
		desenhaItem( $subitem, $profundidade + 1 );
	}
}

function desenhaItemFinal( $item, $profundidade )
{
	global $cor;
	global $mcrid;
	$cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff';
	?>
	<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
		<td style="padding-left: <?php echo ( $profundidade * 10 ) + 4; ?>px;">
			<a href="javascript:mostrarLancamentos( <?php echo $item['tcrid']; ?>, '<?php echo $item['unicod']; ?>', <?php echo $mcrid ? "'" . $mcrid . "'" : "null"; ?> );">
				<font size="2"><?php echo $item['cod']; ?></font>
				-
				<?php echo $item['dsc']; ?>
			</a>
		</td>
		<td align="right"style="color:#000050;">
			<?php echo number_format( $item['acrescimo'], 0, ',', '.' ); ?>
		</td>
		<td align="right"style="color:#500000;">
			<?php echo number_format( $item['reducao'], 0, ',', '.' ); ?>
		</td>
		<td align="right"style="color:#000050;">
			<?php echo number_format( $item['sup'], 0, ',', '.' ); ?>
		</td>
		<td align="right"style="color:#500000;">
			<?php echo number_format( $item['can'], 0, ',', '.' ); ?>
		</td>
		<td align="right"style="color:<?php echo $item['diferenca'] >= 0 ? '#000090' : '#900000'; ?>;">
			<b><?php echo number_format( $item['diferenca'], 0, ',', '.' ); ?></b>
		</td>
	</tr>
	<?php
}

$itens = agrupar( $itens_cru, $agrupamento );

?>
<table border="0" align="center" class="tabela" cellpadding="3" cellspacing="1">
	<?php if ( count ( $itens ) > 0 ) : ?>
		<thead>
			<tr bgcolor="#d0d0d0">
				<td rowspan="2" align="center" style="font-size:10px;"><b>&nbsp;</b></td>
				<td colspan="2" align="center" style="font-size:10px;"><b>Receita</b></td>
				<td colspan="2" align="center" style="font-size:10px;"><b>Créditos</b></td>
				<td rowspan="2" align="center" width="100" style="font-size:10px;"><b>Diferença</b></td>
			</tr>
			<tr bgcolor="#d0d0d0">
				<td align="center" width="100" style="font-size:10px;"><b>Acréscimo</b></td>
				<td align="center" width="100" style="font-size:10px;"><b>Redução</b></td>
				<td align="center" width="100" style="font-size:10px;"><b>Suplementação</b></td>
				<td align="center" width="100" style="font-size:10px;"><b>Cancelamento</b></td>
			</tr>
		</thead>
		<?php foreach ( $itens as $item ) : ?>
			<?php desenhaItem( $item ); ?>
		<?php endforeach; ?>
	<?php else : ?>
		<tr bgcolor="#f0f0f0">
			<td align="center" style="color: #202060; padding: 20px;">
				Nenhum crédito adicional foi efetuado com os filtros atuais
			</td>
		</tr>
	<?php endif; ?>
</table>



