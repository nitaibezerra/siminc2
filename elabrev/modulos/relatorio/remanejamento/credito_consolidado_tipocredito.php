<?php

$ano = $_SESSION['exercicio'];

$tcrid = (integer) $_REQUEST['tcrid'];
$where = '';
if ( $tcrid != '' )
{
	$where = " and tcrid = " . $tcrid . " ";
}

// captura os tipos de crédito
$sql = <<<EOT
	select
		*
	from elabrev.tipocredito
	where
		tcrstatus = 'A' and
		tcrano = '$ano'
		$where
	order by
		tcrcod
EOT;
$tcs = $db->carregar( $sql );
$tcs = $tcs ? $tcs : array();

$ppoid = pegar_proposta_ativa();

// para cada tipo de crédito exibe o resultado consolidado por unidade
$sql_base = <<<EOT
select
	unicod, unidsc,
	abs ( sum( sup ) ) as sup,
	abs ( sum( can ) ) as can,
	abs ( sum( acrescimo ) ) as acrescimo,
	abs ( sum( reducao ) ) as reducao,
	abs( sum( sup ) ) - abs( sum( can ) ) - abs( sum( acrescimo ) ) + abs( sum( reducao ) ) as diferenca
from
(
		select
			tc.tcrid, tc.tcrcod, u.unicod, u.unidsc,
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
			d.mcrid = $mcrid
		group by
			tc.tcrid, tc.tcrcod, u.unicod, u.unidsc
	union all
		select
			tc.tcrid, tc.tcrcod, u.unicod, u.unidsc,
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
			d.mcrid = $mcrid
		group by
			tc.tcrid, tc.tcrcod, u.unicod, u.unidsc
	union all
		select
			tc.tcrid, tc.tcrcod, u.unicod, u.unidsc,
			0 as sup,
			0 as can,
			sum( ru.rcuacrescimo ) as acrescimo,
			sum( ru.rcureducao ) as reducao
		from elabrev.receitaunidade ru
				inner join public.unidade u on u.unicod = ru.unicod and u.unitpocod = ru.unitpocod
				inner join elabrev.tipocredito tc on tc.tcrid = ru.tcrid
		where
			ru.rcuano = '$ano' and
			ru.mcrid = $mcrid
		group by
			tc.tcrid, tc.tcrcod, u.unicod, u.unidsc
) as a
where
	tcrid = %d
group by 
	tcrid, tcrcod, unicod, unidsc
order by
	unicod
EOT;
?>
<table align="center" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
		<tr bgcolor="#d0d0d0">
			<td rowspan="2" align="center" style="font-size:10px;"><b>Unidade Orçamentária</b></td>
			<td colspan="2" align="center" style="font-size:10px;"><b>Receita</b></td>
			<td colspan="2" align="center" style="font-size:10px;"><b>Créditos</b></td>
			<td rowspan="2" align="center" style="font-size:10px;"><b>Diferença</b></td>
		</tr>
		<tr bgcolor="#d0d0d0">
			<td align="center" width="100" style="font-size:10px;"><b>Acréscimo</b></td>
			<td align="center" width="100" style="font-size:10px;"><b>Redução</b></td>
			<td align="center" width="100" style="font-size:10px;"><b>Suplementação</b></td>
			<td align="center" width="100" style="font-size:10px;"><b>Cancelamento</b></td>
		</tr>
	</thead>
	<?php
	
	foreach ( $tcs as $tc )
	{
		// captura as unidade que efetuaram
		$sql = sprintf( $sql_base, $tc['tcrid'] );
		$unidades = $db->carregar( $sql );
		$unidades = $unidades ? $unidades : array();
		if ( count( $unidades ) == 0 )
		{
			continue;
		}
		?>
		<tr bgcolor="#e0e0e0">
			<td colspan="6" style="padding:0 0 0 10px;">
				<table border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td>
							<b>
								<a href="javascript:mostrarLancamentos( <?php echo $tc['tcrid']; ?>, '' );" style="font-size:14pt;">
									<?php echo $tc['tcrcod']; ?>
								</a>
							</b>
						</td>
						<td style="padding:10px;">
							<a href="javascript:mostrarLancamentos( <?php echo $tc['tcrid']; ?>, '' );" style="color:#505050;">
								<?php echo $tc['tcrdsc']; ?>
							</a>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<?php
		$acr = 0;
		$red = 0;
		$sup = 0;
		$can = 0;
		foreach ( $unidades as $unidade )
		{
			$acr += $unidade['acrescimo'];
			$red += $unidade['reducao'];
			$sup += $unidade['sup'];
			$can += $unidade['can'];
			$cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff';
			?>
				<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
					<td align="left" style="padding:0 0 0 20px;" title="Unidade Orçamentária">
						<a href="javascript:mostrarLancamentos( <?php echo $tc['tcrid']; ?>, '<?php echo $unidade['unicod']; ?>' );">
							<?php echo $unidade['unicod']; ?> - <?php echo $unidade['unidsc']; ?>
						</a>
					</td>
					<td align="right" style="color:#000050;" title="Acréscimo na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $tc['tcrcod']; ?>">
						<?php echo number_format( $unidade['acrescimo'], 0, ',', '.' ); ?>
					</td>
					<td align="right" style="color:#500000;" title="Redução na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $tc['tcrcod']; ?>">
						<?php echo number_format( $unidade['reducao'], 0, ',', '.' ); ?>
					</td>
					<td align="right" style="color:#000050;" title="Suplementação na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $tc['tcrcod']; ?>">
						<?php echo number_format( $unidade['sup'], 0, ',', '.' ); ?>
					</td>
					<td align="right" style="color:#500000;" title="Cancelamento na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $tc['tcrcod']; ?>">
						<?php echo number_format( $unidade['can'], 0, ',', '.' ); ?>
					</td>
					<td align="right" style="color:<?php echo $unidade['diferenca'] >= 0 ? '#000050' : '#500000'; ?>;"  title="Diferença na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $tc['tcrcod']; ?>">
						<?php echo number_format( $unidade['diferenca'], 0, ',', '.' ); ?>
					</td>
				</tr>
			<?php
		}
		$dif = $sup - $can - $acr + $red;
		?>
		<tr bgcolor="#f0f0f0" onmouseout="this.bgColor='#f0f0f0';" onmouseover="this.bgColor='#ffffcc';">
			<td align="left" style="padding:0 0 0 50px;" title="Unidade Orçamentária">
				&nbsp;
			</td>
			<td align="right" style="color:#000050;" title="Acréscimo no tipo de crédito <?php echo $tc['tcrcod']; ?>">
				<b><?php echo number_format( $acr, 0, ',', '.' ); ?></b>
			</td>
			<td align="right" style="color:#500000;" title="Redução no tipo de crédito <?php echo $tc['tcrcod']; ?>">
				<b><?php echo number_format( $red, 0, ',', '.' ); ?></b>
			</td>
			<td align="right" style="color:#000050;" title="Suplementação no tipo de crédito <?php echo $tc['tcrcod']; ?>">
				<b><?php echo number_format( $sup, 0, ',', '.' ); ?></b>
			</td>
			<td align="right" style="color:#500000;" title="Cancelamento no tipo de crédito <?php echo $tc['tcrcod']; ?>">
				<b><?php echo number_format( $can, 0, ',', '.' ); ?></b>
			</td>
			<td align="right" style="color:<?php echo $dif >= 0 ? '#000050' : '#500000'; ?>;"  title="Diferença no tipo de crédito <?php echo $tc['tcrcod']; ?>">
				<b><?php echo number_format( $dif, 0, ',', '.' ); ?></b>
			</td>
		</tr>
	<?php
	}
	?>
</table>