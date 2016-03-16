<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
	Programador: Renan de Lima Barbosa (renandelima@gmail.com)
	Módulo: credito.inc
	Finalidade: avalia os remanejamentos
*/

// exibe cabecalho primeiro para mostrar animação de "carregando"
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );

define( 'TIPO_PROPOSTA', 1 );
define( 'TIPO_PROJETO_LEI', 2 );
define( 'TIPO_LOA', 3 );

require_once "credito_funcoes.inc";

function creval_pegar_tipos_credito()
{
	global $db;
	$sql = "select * from elabrev.tipocredito where tcrano = " . $_SESSION['exercicio'] . " order by tcrcod";
	$dados = $db->carregar( $sql );
	if ( !$dados )
	{
		return array();
	}
	$tipos = array();
	foreach ( $dados as $linha )
	{
		$tipo = $linha;
		$tipo['acaid'] = array();
		$tipo['gndcod'] = array();
		$tipo['gunid'] = array();
		$tipo['unicod'] = array();
		$tipo['unitpocod'] = array();
	}
	
	
	return $dados;
}

function creval_pegar_itens_saldo_negativo()
{
	global $db;
	$ppoid = cre_pegar_proposta_ativa();
	$sql = <<<EOF
		select
			d.acaidloa,
			d.idoid,
			d.ndpid,
			d.iducod,
			a.prgcod,
			a.acacod,
			u.unicod,
			a.loccod,
			n.ndpcod,
			d.foncod,
			i.idocod,
			abs( sum( coalesce( dpavalor, 0 ) ) ) as dpavalor
		from elabrev.despesaacao d
			inner join monitora.acao a on
				a.acaid = d.acaidloa
			inner join public.naturezadespesa n on
				n.ndpid = d.ndpid
			inner join public.idoc i on
				i.idoid = d.idoid
			inner join public.unidade u on
				u.unicod = a.unicod and
				u.unitpocod = a.unitpocod 
		where
			ppoid = $ppoid 
		group by
			d.acaidloa,
			d.idoid,
			d.ndpid,
			d.iducod,
			a.prgcod,
			a.acacod,
			u.unicod,
			a.loccod,
			n.ndpcod,
			d.foncod,
			i.idocod
		having
			sum( coalesce( dpavalor, 0 ) ) < 0
EOF;
	$dados = $db->carregar( $sql );
	if ( !$dados )
	{
		return array();
	}
	$despesas = array();
	foreach ( $dados as $linha )
	{
		$despesa = $linha;
		$despesa['tcrid'] = array();
		$sql =
			" select " .
				" tcrid " .
			" from elabrev.despesaacao " .
			" where " .
				" tcrid is not null and " .
				" foncod = '" . $linha['foncod'] . "' and " .
				" ndpid = " . $linha['ndpid'] . " and " .
				" acaidloa = " . $linha['acaidloa'] . " and " .
				" idoid = " . $linha['idoid'];
		$tipos = $db->carregar( $sql );
		if ( $tipos )
		{
			foreach ( $tipos as $linha )
			{
				array_push( $despesa['tcrid'], $linha['tcrid'] );
			}
		}
		array_push( $despesas, $despesa );
	}
	return $despesas;
}

function creval_pegar_itens_acao_invalido( $tcrid )
{
	global $db;
	$tcrid = (integer) $tcrid;
	$ppoid = cre_pegar_proposta_ativa();
	$sql = <<<EOF
		select
			a.prgcod,
			a.acacod,
			a.loccod,
			n.ndpcod,
			n.gndcod,
			d.foncod,
			d.iducod,
			i.idocod,
			u.unicod,
			g.gunid,
			g.gundsc
		from elabrev.despesaacao d
			inner join monitora.acao a on
				a.acaid = d.acaidloa
			inner join public.naturezadespesa n on
				n.ndpid = d.ndpid
			inner join public.idoc i on
				i.idoid = d.idoid
			inner join public.unidade u on
				u.unicod = a.unicod and
				u.unitpocod = a.unitpocod
			inner join public.grupounidade g on
				g.gunid = u.gunid
		where
			ppoid = $ppoid and
			d.tcrid = $tcrid and
			(
				( select count(*) from elabrev.tipocreditoacao where tcrid = $tcrid ) > 0 and
				d.acaidloa not in ( select acaid from elabrev.tipocreditoacao where tcrid = $tcrid )
			)
EOF;
	$despesas = $db->carregar( $sql );
	return $despesas ? $despesas : array();
}

function creval_pegar_itens_gnd_invalido( $tcrid )
{
	global $db;
	$tcrid = (integer) $tcrid;
	$ppoid = cre_pegar_proposta_ativa();
	$sql = <<<EOF
		select
			a.prgcod,
			a.acacod,
			a.loccod,
			n.ndpcod,
			n.gndcod,
			d.foncod,
			d.iducod,
			i.idocod,
			u.unicod,
			g.gunid,
			g.gundsc
		from elabrev.despesaacao d
			inner join monitora.acao a on
				a.acaid = d.acaidloa
			inner join public.naturezadespesa n on
				n.ndpid = d.ndpid
			inner join public.idoc i on
				i.idoid = d.idoid
			inner join public.unidade u on
				u.unicod = a.unicod and
				u.unitpocod = a.unitpocod
			inner join public.grupounidade g on
				g.gunid = u.gunid
		where
			ppoid = $ppoid and
			d.tcrid = $tcrid and
			(
				( select count(*) from elabrev.tipocreditognd where tcrid = $tcrid ) > 0 and
				n.gndcod not in ( select gndcod from elabrev.tipocreditognd where tcrid = $tcrid )
			)
EOF;
	$despesas = $db->carregar( $sql );
	return $despesas ? $despesas : array();
}

function creval_pegar_itens_grupounidade_invalido( $tcrid )
{
	global $db;
	$tcrid = (integer) $tcrid;
	$ppoid = cre_pegar_proposta_ativa();
	$sql = <<<EOF
		select
			a.prgcod,
			a.acacod,
			a.loccod,
			n.ndpcod,
			n.gndcod,
			d.foncod,
			d.iducod,
			i.idocod,
			u.unicod,
			g.gunid,
			g.gundsc
		from elabrev.despesaacao d
			inner join monitora.acao a on
				a.acaid = d.acaidloa
			inner join public.naturezadespesa n on
				n.ndpid = d.ndpid
			inner join public.idoc i on
				i.idoid = d.idoid
			inner join public.unidade u on
				u.unicod = a.unicod and
				u.unitpocod = a.unitpocod
			inner join public.grupounidade g on
				g.gunid = u.gunid
		where
			ppoid = $ppoid and
			d.tcrid = $tcrid and
			(
				( select count(*) from elabrev.tipocreditogrupounidade where tcrid = $tcrid ) > 0 and
				u.gunid not in ( select gunid from elabrev.tipocreditogrupounidade where tcrid = $tcrid )
			)
EOF;
	$retorno = $db->carregar( $sql );
	return $retorno ? $retorno : array();
}

function creval_pegar_itens_unidade_invalido( $tcrid )
{
	global $db;
	$tcrid = (integer) $tcrid;
	$ppoid = cre_pegar_proposta_ativa();
	$sql = <<<EOF
		select
			a.prgcod,
			a.acacod,
			a.loccod,
			n.ndpcod,
			n.gndcod,
			d.foncod,
			d.iducod,
			i.idocod,
			u.unicod,
			g.gunid,
			g.gundsc
		from elabrev.despesaacao d
			inner join monitora.acao a on
				a.acaid = d.acaidloa
			inner join public.naturezadespesa n on
				n.ndpid = d.ndpid
			inner join public.idoc i on
				i.idoid = d.idoid
			inner join public.unidade u on
				u.unicod = a.unicod and
				u.unitpocod = a.unitpocod
			inner join public.grupounidade g on
				g.gunid = u.gunid
		where
			ppoid = $ppoid and
			d.tcrid = $tcrid and
			(
				( select count(*) from elabrev.tipocreditounidade where tcrid = $tcrid ) > 0 and
				u.unicod not in ( select unicod from elabrev.tipocreditounidade where tcrid = $tcrid ) and
				u.unitpocod not in ( select unitpocod from elabrev.tipocreditounidade where tcrid = $tcrid ) 
			)
EOF;
	$despesas = $db->carregar( $sql );
	return $despesas ? $despesas : array();
}

function creval_pegar_itens_extrapolados( $tcrid, $tcrtipoporcento )
{
	global $db;
	$tcrid = (integer) $tcrid;
	$ppoid = cre_pegar_proposta_ativa();
	switch ( $tcrtipoporcento )
	{
		case 'L':
			$campo_cod = 'a.loccod';
			$campo_dsc = 'l.locdsc';
			break;
		case 'U':
		default:
			$campo_cod = 'a.unicod';
			$campo_dsc = 'u.unidsc';
			break;
	}
	$sql = <<<EOF
		select
			$campo_cod as cod,
			$campo_dsc as dsc,
			sum( d.dpavalor ) as dot,
			sum( abs( coalesce( can.dpavalor, 0 ) ) ) as can,
			sum( coalesce( sup.dpavalor, 0 ) ) as sup,
			tc.tcrporcentocancelamento as can_max,
			tc.tcrporcentosuplementacao as sup_max,

			case when sum( d.dpavalor ) > 0
				then ( sum( abs( coalesce( can.dpavalor, 0 ) ) ) / sum( d.dpavalor ) ) * 100
				else 0
			end as can_exec,

			case when sum( d.dpavalor ) > 0
				then ( sum( coalesce( sup.dpavalor, 0 ) ) / sum( d.dpavalor ) ) * 100
				else 0
			end as sup_exec

		from elabrev.despesaacao d
			inner join monitora.acao a on
				a.acaid = d.acaidloa
			inner join public.unidade u on
				u.unicod = a.unicod and
				u.unitpocod = a.unitpocod
			inner join public.localizador l on
				l.loccod = a.loccod
			left join elabrev.despesaacao can on
				can.foncod = d.foncod and
				can.ndpid = d.ndpid and
				can.acaidloa = d.acaidloa and
				can.idoid = d.idoid and
				can.dpavalor < 0 and
				can.tcrid = 14
			left join elabrev.despesaacao sup on
				sup.foncod = d.foncod and
				sup.ndpid = d.ndpid and
				sup.acaidloa = d.acaidloa and
				sup.idoid = d.idoid and
				sup.dpavalor > 0 and
				sup.tcrid = 14
			left join elabrev.tipocredito tc on
				tc.tcrid = can.tcrid or
				tc.tcrid = sup.tcrid
		where
			d.tcrid is null and
			d.ppoid = 158 
		group by
			$campo_cod,
			$campo_dsc,
			tc.tcrporcentocancelamento,
			tc.tcrporcentosuplementacao
		having
			sum( d.dpavalor ) > 0 and
			(	
				(
					tcrporcentocancelamento > 0
					and
					( sum( abs( coalesce( can.dpavalor, 0 ) ) ) / sum( d.dpavalor ) ) * 100 > tcrporcentocancelamento
				)
				or
				(
					tcrporcentosuplementacao > 0
					and
					( sum( abs( coalesce( sup.dpavalor, 0 ) ) ) / sum( d.dpavalor ) ) * 100 > tcrporcentosuplementacao
				)
			)
EOF;
	$extrapolados = $db->carregar( $sql );
	return $extrapolados ? $extrapolados : array();
}

// rotina de verificação de remanejamento
	
	/*
	 	tcrid => array(
			'acao'         => array(),
			'gnd'          => array(),
			'grupounidade' => array(),
			'unidade'      => array(),
			'can'          => array(),
			'sup'          => array()
		)
	*/
	$invalidos = array();
	$negativados = array();
	
	$tipos_credito = array();
	foreach ( creval_pegar_tipos_credito() as $tipo_credito )
	{
		$tipos_credito[$tipo_credito['tcrid']] = $tipo_credito;
	}
	
	// verifica saldo de cada item de credito
		
		$negativados = creval_pegar_itens_saldo_negativo();
		
	// para cada tipo de crédito
	// verifica se existe itens que não pertence à ação, gnd, grupo de unidade ou unidade daquele tipo
	// verifica limite de porcentagem de remanejamento para cada tipo de crédito

		$invalidos = array();
		reset( $tipos_credito );
		foreach ( $tipos_credito as $tipo_credito )
		{
			$tcrid = (integer) $tipo_credito['tcrid'];
			
			$invalido_itens = array();
			$invalido_item_base = array(
				'acao'         => false,
				'gnd'          => false,
				'grupounidade' => false,
				'unidade'      => false
			);
			
			// ações invalidas
			foreach ( creval_pegar_itens_acao_invalido( $tcrid ) as $item )
			{
				$chave = $item['acacod'] . $item['loccod'] . $item['ndpcod'] . $item['foncod'] . $item['idocod'];
				if ( !array_key_exists( $chave, $invalido_itens ) )
				{
					$invalido_itens[$chave] = $item;
					$invalido_itens[$chave]['invalido'] = $invalido_item_base;
				}
				$invalido_itens[$chave]['invalido']['acao'] = true;
			}
			
			// gnds inválidos
			foreach ( creval_pegar_itens_gnd_invalido( $tcrid ) as $item )
			{
				$chave = $item['acacod'] . $item['loccod'] . $item['ndpcod'] . $item['foncod'] . $item['idocod'];
				if ( !array_key_exists( $chave, $invalido_itens ) )
				{
					$invalido_itens[$chave] = $item;
					$invalido_itens[$chave]['invalido'] = $invalido_item_base;
				}
				$invalido_itens[$chave]['invalido']['gnd'] = true;
			}
			
			// grupo de unidades inválidos
			foreach ( creval_pegar_itens_grupounidade_invalido( $tcrid ) as $item )
			{
				$chave = $item['acacod'] . $item['loccod'] . $item['ndpcod'] . $item['foncod'] . $item['idocod'];
				if ( !array_key_exists( $chave, $invalido_itens ) )
				{
					$invalido_itens[$chave] = $item;
					$invalido_itens[$chave]['invalido'] = $invalido_item_base;
				}
				$invalido_itens[$chave]['invalido']['grupounidade'] = true;
			}
			
			// unidades inválidas
			foreach ( creval_pegar_itens_unidade_invalido( $tcrid ) as $item )
			{
				$chave = $item['acacod'] . $item['loccod'] . $item['ndpcod'] . $item['foncod'] . $item['idocod'];
				if ( !array_key_exists( $chave, $invalido_itens ) )
				{
					$invalido_itens[$chave] = $item;
					$invalido_itens[$chave]['invalido'] = $invalido_item_base;
				}
				$invalido_itens[$chave]['invalido']['unidade'] = true;
			}
			
			$extrapolado_itens = creval_pegar_itens_extrapolados( $tcrid, $tipos_credito[$tcrid]['tcrtipoporcento'] );
			
			if ( !count( $invalido_itens ) && !count( $extrapolado_itens ) )
			{
				continue;
			}
			
			if ( count( $invalido_itens ) || count( $extrapolado_itens ) )
			{
				$invalidos[$tcrid] = array(
					'invalidos'    => $invalido_itens,
					'extrapolados' => $extrapolado_itens
				);
			}
		}
		
		//$negativados = array();
		//$invalidos = array();
		
?>
<br/>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
	<thead>
		<tr bgcolor="#cfcfcf">
			<th>
				D E S P E S A S&nbsp;&nbsp;&nbsp;&nbsp;N E G A T I V A D A S
			</th>
		</tr>
	</thead>
</table>
<br/>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
	<?php if ( count( $negativados ) ) : ?>
		<tr bgcolor="#e0e0e0">
			<td align="center" width="10%" rowspan="2"><b>Programa</b></td>
			<td align="center" width="10%" rowspan="2"><b>Ação</b></td>
			<td align="center" width="10%" rowspan="2"><b>Unidade</b></td>
			<td align="center" width="10%" rowspan="2"><b>Localizador</b></td>
			<td align="center" width="10%" rowspan="2"><b>Natureza</b></td>
			<td align="center" width="10%" rowspan="2"><b>Fonte</b></td>
			<td align="center" width="10%" rowspan="2"><b>IDOC</b></td>
			<td align="center" width="10%" rowspan="2"><b>IDUSO</b></td>
			<td align="center" colspan="<?php echo count( $tipos_credito ); ?>">Tipo de crédito presente</td>
			<td align="right" rowspan="2"><b>Saldo</b></td>
		</tr>
		<tr bgcolor="#e0e0e0">
			<?php foreach ( $tipos_credito as $tipo_credito ) : ?>
				<td align="center"><b><?php echo $tipo_credito['tcrcod']; ?></b></td>
			<?php endforeach; ?>
		</tr>
		<?php $cor = '#f0f0f0'; ?>
		<?php foreach ( $negativados as $item ) : ?>
			<?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
			<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
				<td align="center"><?php echo $item['prgcod']; ?></td>
				<td align="center"><?php echo $item['acacod']; ?></td>
				<td align="center"><?php echo $item['unicod']; ?></td>
				<td align="center"><?php echo $item['loccod']; ?></td>
				<td align="center"><?php echo $item['ndpcod']; ?></td>
				<td align="center"><?php echo $item['foncod']; ?></td>
				<td align="center"><?php echo $item['idocod']; ?></td>
				<td align="center"><?php echo $item['iducod']; ?></td>
				<?php reset( $tipos_credito ); ?>
				<?php foreach ( $tipos_credito as $tc ) : ?>
					<td align="center"><?php echo in_array( $tc['tcrid'], $item['tcrid'] ) ? 'x' : '&nbsp;';?></td>
				<?php endforeach; ?>
				<td align="right" style="color:#ff0000;">
					-<?php echo number_format( $item['dpavalor'], '0', ',', '.' ); ?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr bgcolor="#ffffff" style="color:#0000ff;">
			<td align="center">
				Nenhum item negativado.
			</td>
		</tr>
	<?php endif; ?>
</table>
<br/>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
	<thead>
		<tr bgcolor="#cfcfcf">
			<th>
				E R R O S&nbsp;&nbsp;&nbsp;&nbsp;
				P O R&nbsp;&nbsp;&nbsp;&nbsp;
				T I P O&nbsp;&nbsp;&nbsp;&nbsp;
				D E&nbsp;&nbsp;&nbsp;&nbsp;
				C R É D I T O
			</th>
		</tr>
	</thead>
</table>
<br/>
<?php if ( count( $invalidos ) ) : ?>
	<?php foreach ( $invalidos as $tcrid => $item ) : ?>
		<?php $tc = $tipos_credito[$tcrid]; ?>
		<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
			<tr bgcolor="#cfcfcf">
				<td align="center">
					<b>
						Erros no tipo de crédito <?php echo $tc['tcrcod']; ?>
					</b>
				</td>
			</tr>
		</table>
		<br/>
		<!-- Ações, Unidades, GNDs e Grupo de Unidades -->
		<?php if ( count( $item['invalidos'] ) ) : ?>
			<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
				<tr bgcolor="#e0e0e0">
					<td align="center" width="70" colspan="9">Despesas do tipo de crédito <?php echo $tc['tcrcod']; ?> com Ação, GND, Grupo de Unidade ou Unidades inválidas</td>
				</tr>
				<tr bgcolor="#e0e0e0">
					<td align="center" width="10%">Programa</td>
					<td align="center" width="10%"><b>Ação</b></td>
					<td align="center" width="10%"><b>Unidade</b></td>
					<td align="center" width="10%">Localizador</td>
					<td align="center" width="10%"><b>Natureza</b></td>
					<td align="center" width="10%">Fonte</td>
					<td align="center" width="10%">IDOC</td>
					<td align="center" width="10%">IDUSO</td>
					<td align="center"><b>Grupo de Unidade</b></td>
				</tr>
				<?php $cor = '#f0f0f0'; ?>
				<?php foreach ( $item['invalidos'] as $item_invalido ) : ?>
					<?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td align="center"><?php echo $item_invalido['prgcod']; ?></td>
						<td align="center" style="color:<?php echo $item_invalido['invalido']['acao'] ? '#ff0000' : '#0000ff';?>;">
							<?php echo $item_invalido['acacod']; ?>
						</td>
						<td align="center" style="color:<?php echo $item_invalido['invalido']['unidade'] ? '#ff0000' : '#0000ff';?>;">
							<?php echo $item_invalido['unicod']; ?>
						</td>
						<td align="center"><?php echo $item_invalido['loccod']; ?></td>
						<td align="center">
							<?php $ndpcod = $item_invalido['ndpcod']; ?>
							<?php $cor_gnd = $item_invalido['invalido']['gnd'] ? '#ff0000' : '#0000ff'; ?>
							<?php echo $ndpcod{0} . '<font color="' . $cor_gnd . '">' . $ndpcod{1} . '</font>' . substr( $ndpcod, 2 ); ?>
						</td>
						<td align="center"><?php echo $item_invalido['foncod']; ?></td>
						<td align="center"><?php echo $item_invalido['idocod']; ?></td>
						<td align="center"><?php echo $item_invalido['iducod']; ?></td>
						<td align="center" style="color:<?php echo $item_invalido['invalido']['grupounidade'] ? '#ff0000' : '#0000ff';?>;">
							<?php echo $item_invalido['gundsc']; ?>
						</td>
					</tr>
				<?php endforeach; ?>
			</table>
		<?php endif; ?>
		<!-- Extrapolação de cancelamento e suplementação -->
		<?php if ( count( $item['extrapolados'] ) ) : ?>
			<?php $label = $tc['tcrtipoporcento'] == 'L' ? 'Localizador' : 'Unidade Orçamentária'; ?>
			<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
				<tr bgcolor="#e0e0e0">
					<td align="center" width="70" colspan="4">
						Cancelamento da despesa <?php echo $tc['tcrcod']; ?> extrapolam o limite definido para cada <?php echo $label; ?>
						<br/>
						Cancelamento Máximo <b><?php echo $tipos_credito[$tcrid]['tcrporcentocancelamento']; ?>.00</b> %
						<br/>
						Suplementação Máximo <b><?php echo $tipos_credito[$tcrid]['tcrporcentosuplementacao']; ?>.00</b> %
					</td>
				</tr>
				<tr bgcolor="#e0e0e0">
					<td align="center" width="10%">Código</td>
					<td align="center"><?php echo $label; ?></td>
					<td align="center" width="20%"><b>Cancelado (%)</b></td>
					<td align="center" width="20%"><b>Suplementado (%)</b></td>
				</tr>
				<?php $cor = '#f0f0f0'; ?>
				<?php foreach ( $item['extrapolados'] as $item_extrapolado ) : ?>
					<?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td align="center"><?php echo $item_extrapolado['cod']; ?></td>
						<td align="left"><?php echo $item_extrapolado['dsc']; ?></td>
						<td align="center">
							<?php echo number_format( $item_extrapolado['can_exec'], 2, ',', '.' ); ?>
						</td>
						<td align="center">
							<?php echo number_format( $item_extrapolado['dsc_exec'], 2, ',', '.' ); ?>
							</td>
					</tr>
				<?php endforeach; ?>
			</table>
		<?php endif; ?>
		<br/>
	<?php endforeach; ?>
<?php else : ?>
	<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;" class="tabela">
		<tr bgcolor="#ffffff" style="color:#0000ff;">
			<td align="center">
				Nenhum crédito adicional possui Ação, GND, Grupo de Unidades ou Unidade fora dos permitidos para cada tipo de crédito.
				<br/><br/>
				Nenhum crédito adicional extrapolou os limites de cancelamento ou suplementação. 
			</td>
		</tr>
	</table>
<?php endif; ?>

