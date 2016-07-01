<?php


$ano = $_SESSION['exercicio'];

$unicod = (string) $_REQUEST['unicod'];
$where = '';
if ( $unicod != '' )
{
	$where = " where unicod = '" . $unicod . "' ";
}

$ppoid = pegar_proposta_ativa();

// para cada unidade o resultado consolidado por tipo de credito
$sql = <<<EOT
select
	tcrid, SUBSTRING( tcrdsc, 0, 70 ) || '...' as tcrdsc, tcrcod, unicod, unidsc,
	abs ( sum( sup ) ) as sup,
	abs ( sum( can ) ) as can,
	abs ( sum( acrescimo ) ) as acrescimo,
	abs ( sum( reducao ) ) as reducao,
	abs( sum( sup ) ) - abs( sum( can ) ) - abs( sum( acrescimo ) ) + abs( sum( reducao ) ) as diferenca
from
(
		select
			tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc,
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
			tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc
	union all
		select
			tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc,
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
			tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc
	union all
		select
			tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc,
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
			tc.tcrid, tc.tcrdsc, tc.tcrcod, u.unicod, u.unidsc
) as a
$where
group by
	tcrid, tcrcod, tcrdsc, unicod, unidsc
order by
	unicod, unidsc, tcrcod, tcrdsc
EOT;

$unidades = $db->carregar( $sql );
$unidades = $unidades ? $unidades : array();

$unidsc = $db->pegaUm( "select unidsc from public.unidade where unicod = '" . $unicod . "'" );

?>
<table align="center" class="tabela" cellpadding="3" cellspacing="1">
<?php
if ( count ( $unidades ) > 0 )
{
	
	?>
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
	
	$ultimo_unicod = '';
	foreach ( $unidades as $unidade )
	{
		if ( $unidade['unicod'] != $ultimo_unicod )
		{
			?>
			<tr bgcolor="#e0e0e0">
				<td colspan="6" style="padding:0 0 0 10px;">
					<table border="0" cellspacing="0" cellpadding="0">
						<tr>
							<td>
								<b>
									<a href="javascript:mostrarLancamentos( '', '<?php echo $unidade['unicod']; ?>' );" style="font-size:14pt;">
										<?php echo $unidade['unicod']; ?>
									</a>
								</b>
							</td>
							<td style="padding:10px;">
								<a href="javascript:mostrarLancamentos( '', '<?php echo $unidade['unicod']; ?>' );" style="color:#505050;">
									<?php echo $unidade['unidsc']; ?>
								</a>
							</td>
						</tr>
					</table>
				</td>
			</tr>
			<?php
			$ultimo_unicod = $unidade['unicod'];
			$acr = 0;
			$red = 0;
			$sup = 0;
			$can = 0;
		}
		$acr += $unidade['acrescimo'];
		$red += $unidade['reducao'];
		$sup += $unidade['sup'];
		$can += $unidade['can'];
		$cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff';
		?>
		<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
			<td align="left" style="padding:0 0 0 20px;" title="Tipo de Crédito" nowrap="nowrap">
				<a href="javascript:mostrarLancamentos( <?php echo $unidade['tcrid']; ?>, '<?php echo $unidade['unicod']; ?>' );">
					<?php echo $unidade['tcrcod']; ?> - <?php echo $unidade['tcrdsc']; ?>
				</a>
			</td>
			<td align="right" style="color:#000050;" title="Acréscimo na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
				<?php echo number_format( $unidade['acrescimo'], 0, ',', '.' ); ?>
			</td>
			<td align="right" style="color:#500000;" title="Redução na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
				<?php echo number_format( $unidade['reducao'], 0, ',', '.' ); ?>
			</td>
			<td align="right" style="color:#000050;" title="Suplementação na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
				<?php echo number_format( $unidade['sup'], 0, ',', '.' ); ?>
			</td>
			<td align="right" style="color:#500000;" title="Cancelamento na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
				<?php echo number_format( $unidade['can'], 0, ',', '.' ); ?>
			</td>
			<td align="right" style="color:<?php echo $unidade['diferenca'] >= 0 ? '#000050' : '#500000'; ?>;"  title="Diferença na Unidade <?php echo $unidade['unicod']; ?> no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
				<?php echo number_format( $unidade['diferenca'], 0, ',', '.' ); ?>
			</td>
		</tr>
		<?php
	}
	$dif = $sup - $can - $acr + $red;
	?>
	<tr bgcolor="#f7f7f7" onmouseout="this.bgColor='#f0f0f0';" onmouseover="this.bgColor='#ffffcc';">
		<td align="left" style="padding:0 0 0 50px;" title="Unidade Orçamentária">
			&nbsp;
		</td>
		<td align="right" style="color:#000050;" title="Acréscimo no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
			<b><?php echo number_format( $acr, 0, ',', '.' ); ?></b>
		</td>
		<td align="right" style="color:#500000;" title="Redução no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
			<b><?php echo number_format( $red, 0, ',', '.' ); ?></b>
		</td>
		<td align="right" style="color:#000050;" title="Suplementação no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
			<b><?php echo number_format( $sup, 0, ',', '.' ); ?></b>
		</td>
		<td align="right" style="color:#500000;" title="Cancelamento no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
			<b><?php echo number_format( $can, 0, ',', '.' ); ?></b>
		</td>
		<td align="right" style="color:<?php echo $dif >= 0 ? '#000050' : '#500000'; ?>;"  title="Diferença no tipo de crédito <?php echo $unidade['tcrcod']; ?>">
			<b><?php echo number_format( $dif, 0, ',', '.' ); ?></b>
		</td>
	</tr>
<?php
}
else
{
?>
	<tr bgcolor="#f0f0f0">
		<td align="center" style="color: #202060; padding: 20px;">
			Nenhum crédito adicional foi efetuado na Unidade Orçamentária<br/>
			<?php echo $unicod; ?> - <?php echo $unidsc; ?>. 
		</td>
	</tr>
<?php
}
?>
</table>