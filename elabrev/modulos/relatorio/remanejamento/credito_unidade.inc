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

$tcrid = $_REQUEST['tcrid'];
$unicod = $_REQUEST['unicod'];
$mcrid = (integer) $_REQUEST['mcrid'];
$ungcod = $_REQUEST['ungcod'];

$valor_unidade = $unicod ? " a.unicod = '" . $unicod . "' and " : " ";
$valor_momento = $mcrid ? " = " . $mcrid . " " : " is not null ";

$valor_unidade_gestora = $ungcod ? " d.ungcod = '" . $ungcod . "' and " : " ";

$uni_wherejoin = $unicod ? " a.unicod = '" . $unicod . "' and " : "";
$sql = <<<EOT
	select
		tmp.unicod, tmp.acacod, tmp.acadsc,
		tmp.loccod, tmp.prgcod, n.ndpcod,
		tmp.foncod, i.idocod,   tmp.iducod,tmp.plocod,
		sum( tmp.dot ) as dot,
		sum( tmp.sup ) as sup,
		sum( tmp.can ) as can,
		sum( tmp.supe ) as supe,
		sum( tmp.sups ) as sups,
		( sum( tmp.sup ) + sum( tmp.supe ) + sum( tmp.sups ) ) - sum( tmp.can ) as diferenca,
		( sum( tmp.dot ) + sum( tmp.sup ) + sum( tmp.supe ) + sum( tmp.sups ) ) - sum( tmp.can ) as saldo
	from (
			-- valor can
			select
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod,
				0 as dot,
				0 as sup,
				abs( sum( d.dpavalor ) ) as can,
				0 as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid = $tcrid and
				$valor_unidade
				$valor_unidade_gestora
				$uni_wherejoin
				a.prgano = '$ano' and
				d.dpavalor < 0 and
				(d.dpatipovalor in(1) or d.dpatipovalor is null) and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod
		union
			-- valor sup
			select
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod,
				0 as dot,
				sum( d.dpavalor ) as sup,
				0 as can,
				0 as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid = $tcrid and
				$valor_unidade
				$valor_unidade_gestora
				$uni_wherejoin
				a.prgano = '$ano' and
				d.dpavalor > 0 and
				(d.dpatipovalor in(2) or d.dpatipovalor is null) and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod
		union
			-- valor sup excesso
			select
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod,
				0 as dot,
				0 as sup,
				0 as can,
				sum( d.dpavalor ) as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid = $tcrid and
				$valor_unidade
				$valor_unidade_gestora
				$uni_wherejoin
				a.prgano = '$ano' and
				d.dpavalor > 0 and
				d.dpatipovalor = 3 and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod
		union
			-- valor sup superavit
			select
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod,
				0 as dot,
				0 as sup,
				0 as can,
				0 as supe,
				sum( d.dpavalor ) as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid = $tcrid and
				$valor_unidade
				$valor_unidade_gestora
				$uni_wherejoin
				a.prgano = '$ano' and
				d.dpavalor > 0 and
				d.dpatipovalor = 4 and
				d.ppoid = $ppoid and
				d.mcrid $valor_momento
			group by
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod
		union
			-- valor dot
			select
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod,
				sum( d.dpavalor ) as dot,
				0 as sup,
				0 as can,
				0 as supe,
				0 as sups
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid is null and
				$valor_unidade
				$valor_unidade_gestora
				$uni_wherejoin
				a.prgano = '$ano' and
				d.ppoid = $ppoid and
				d.mcrid is null
			group by
				a.unicod, a.acacod, a.acadsc,
				a.loccod, a.prgcod, d.ndpid,
				d.foncod, d.idoid, d.iducod, d.plocod
	
	) as tmp
		inner join public.naturezadespesa n on
			n.ndpid = tmp.ndpid
		inner join public.idoc i on
			i.idoid = tmp.idoid
	group by
		tmp.unicod, tmp.acacod, tmp.acadsc,
		tmp.loccod, tmp.prgcod, n.ndpcod,
		tmp.foncod, i.idocod,   tmp.iducod, tmp.plocod
	having
		sum( tmp.sup ) > 0 or sum( tmp.can ) > 0 or sum( tmp.supe ) > 0 or sum( tmp.sups ) > 0
	order by
		tmp.unicod, tmp.acacod, tmp.acadsc,
		tmp.loccod, tmp.prgcod, n.ndpcod,
		tmp.foncod, i.idocod,   tmp.iducod, tmp.plocod
EOT;

//ver( $sql );
//die;			
$creditos = $db->carregar( $sql );
$creditos = $creditos ? $creditos : array();

$uni_wherejoin = $unicod ? " r.unicod = '" . $unicod . "' and " : "";
$sql = <<<EOT
	select
		r.unicod,
		r.foncod,
		n.nrccod,
		n.nrcdsc,
		coalesce( rd.rcdvalor ) as dot,
		r.rcuacrescimo as acr,
		r.rcureducao as red
	from elabrev.receitaunidade r
		left join elabrev.receitaunidadedotacao rd on
			rd.rcdano = r.rcuano and
			rd.unicod = r.unicod and
			rd.unitpocod = r.unitpocod and
			rd.foncod = r.foncod and
			rd.nrcid = r.nrcid
		inner join public.naturezareceita n on
			n.nrcid = r.nrcid
	where
		r.tcrid = $tcrid and
		$uni_wherejoin
		r.rcuano = '$ano' and
		r.mcrid $valor_momento
	order by
		r.unicod,
		n.nrccod,
		r.foncod
EOT;
$receitas = $db->carregar( $sql );
$receitas = $receitas ? $receitas : array();

if ( $unicod )
{
	if ( $mcrid )
	{
		$sql = <<<EOT
			select
				*
			from elabrev.justificativacredito
			where
				unicod = '$unicod' and
				unitpocod = 'U' and
				tcrid = $tcrid and
				mcrid $valor_momento
EOT;
		$justificativa = $db->recuperar( $sql );
		$justificativa = $justificativa ? $justificativa : array();
	}
	$diferenca = array();
}
else
{
	$justificativa = array();
	
	$unicod_permitidas = "'" . implode( "','", $db->usuarioUnidadesPermitidas() ) . "'";
	$sql = <<<EOT
		select
			tcrid,
			tcrcod,
			abs ( sum( sup ) ) as sup,
			abs ( sum( can ) ) as can,
			abs ( sum( acrescimo ) ) as acrescimo,
			abs ( sum( reducao ) ) as reducao,
			abs ( sum( sup ) ) -
				abs ( sum( can ) ) -
				abs ( sum( acrescimo ) ) +
				abs ( sum( reducao ) ) as diferenca
		from
		(
			select
				tc.tcrid,	tc.tcrcod,
				u.unicod,	u.unidsc,
				sum( d.dpavalor ) as sup,
				0 as can,
				0 as acrescimo,
				0 as reducao
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
				inner join public.unidade u on
					u.unicod = a.unicod and
					u.unitpocod = a.unitpocod
				inner join elabrev.tipocredito tc on
					tc.tcrid = d.tcrid
			where
				d.tcrid is not null and
				d.dpavalor > 0 and
				a.prgano = '$ano' and
				a.unicod in ( $unicod_permitidas ) and
				d.mcrid $valor_momento
			group by
				tc.tcrid,	tc.tcrcod,
				u.unicod,	u.unidsc
		union all
			select
				tc.tcrid,	tc.tcrcod,
				u.unicod,	u.unidsc,
				0 as sup,
				sum( d.dpavalor ) as can,
				0 as acrescimo,
				0 as reducao
			from elabrev.despesaacao d
					inner join monitora.acao a on
						a.acaid = d.acaidloa
					inner join public.unidade u on
						u.unicod = a.unicod and
						u.unitpocod = a.unitpocod
					inner join elabrev.tipocredito tc on
						tc.tcrid = d.tcrid
				where
					d.tcrid is not null and
					d.dpavalor < 0 and
				a.prgano = '$ano' and
				a.unicod in ( $unicod_permitidas ) and
				d.mcrid $valor_momento
			group by
				tc.tcrid,	tc.tcrcod,
				u.unicod,	u.unidsc
		union all
			select
				tc.tcrid,	tc.tcrcod,
				u.unicod,	u.unidsc,
				0 as sup,
				0 as can,
				sum( ru.rcuacrescimo ) as acrescimo,
				sum( ru.rcureducao ) as reducao
			from elabrev.receitaunidade ru
					inner join public.unidade u on
						u.unicod = ru.unicod and
						u.unitpocod = ru.unitpocod
					inner join elabrev.tipocredito tc on
						tc.tcrid = ru.tcrid
				where
				ru.rcuano = '$ano' and
				u.unicod in ( $unicod_permitidas ) and
				ru.mcrid $valor_momento
			group by
				tc.tcrid,	tc.tcrcod,
				u.unicod,	u.unidsc
		) as a
		where
			tcrid = $tcrid
		group by
			tcrid,
			tcrcod
		having
			abs ( sum( sup ) ) -
				abs ( sum( can ) ) -
				abs ( sum( acrescimo ) ) +
				abs ( sum( reducao ) ) != 0
EOT;
	$diferenca = $db->recuperar( $sql );
	$diferenca = $diferenca ? $diferenca : array();
}

$sql = <<<EOT
	select *
	from elabrev.tipocredito
	where tcrid = $tcrid 
EOT;
$dados_tc = $db->recuperar( $sql );

if ( $unicod )
{
	$sql = <<<EOT
		select *
		from public.unidade
		where
			unicod = '$unicod' and
			unitpocod = 'U' 
EOT;
	$dados_uni = $db->recuperar( $sql );
}
else
{
	$dados_uni = array();
}

$dados_momento = array();
if ( $mcrid )
{
	$sql_momento = "
		select *
		from elabrev.momentocredito
		where
			mcrstatus = 'A' and
			mcrid = $mcrid 
	";
	
	$dados_momento = $db->recuperar( $sql_momento );
}

?>
<html>
	<head>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">
			
			window.focus();
			
			function mostrarLancamentos( tcrid, unicod )
			{
				var url =
					'?modulo=relatorio/remanejamento/credito_unidade&acao=A' +
						'&tcrid=' + tcrid +
						'&unicod=' + unicod;
				document.location.href = url;
			}
			
		</script>
	</head>
	<body>
		<span class="notprint">
			<table align="center" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td align="center" bgcolor="#ffffff">
						&nbsp;<br/>
						<input type="button" name="imprimir" value="Imprimir" onclick="window.print();"/>
						&nbsp;&nbsp;&nbsp;
						<input type="button" name="fechar" value="Fechar" onclick="window.close();"/>
						<br/>&nbsp;
					</td>
				</tr>
			</table>
		</span>
		<table align="center" class="tabela" cellpadding="3" cellspacing="1">
			<tr bgcolor="#c0c0c0">
				<td align="center" style="font-size: 12pt;">
					<?php if ( $mcrid ) : ?>
						<?php echo $dados_momento['mcrdsc']; ?>
					<?php else : ?>
						Todos os momentos de crédito
					<?php endif; ?>
				</td>
			</tr>
		</table>
		<table align="center" class="tabela" cellpadding="3" cellspacing="1">
			<tr bgcolor="#c0c0c0">
				<td align="center">
					<a href="javascript:mostrarLancamentos( <?php echo $tcrid; ?>, '' );" style="font-size:12pt;"><b><?php echo $dados_tc['tcrcod']; ?></b></a>
					<br/>
					Tipo de Crédito
					<br/></br>
					<?php echo $dados_tc['tcrdsc']; ?>
				</td>
			</tr>
			<?php if ( count( $dados_uni ) ) : ?>
				<tr bgcolor="#c0c0c0">
					<td align="center">
						<font style="font-size:12pt;"><b><?php echo $dados_uni['unicod']; ?></b></font>
						<br/>
						<?php echo $dados_uni['unidsc']; ?>
					</td>
				</tr>
			<?php endif; ?>
			<?php if( $ungcod ): ?>
			<?php $ungdsc = $db->pegaUm("SELECT ungdsc FROM public.unidadegestora WHERE ungcod = '".$ungcod."'"); ?>
			<!-- 
			<tr bgcolor="#c0c0c0">
				<td align="center">
					<font style="font-size:12pt;"><b><?php //echo $ungcod; ?></b></font>
					<br/>
					<?php //echo $ungdsc; ?>
				</td>
			</tr>
			 -->
			<?php endif;?>
		</table>
		<?php if ( count( $creditos ) ) : ?>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<tr bgcolor="#d0d0d0">
					<td align="center">
						<b>C R É D I T O S</b>
					</td>
				</tr>
			</table>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<thead>
					<tr bgcolor="#e0e0e0">
						<?php if ( !$unicod ) : ?>
							<td align="center" width="30" style="font-size:7pt;"><b>Uni.</b></td>
						<?php endif; ?>
						<?php if ( $ungcod ) : ?>
							<td align="center" width="30" style="font-size:7pt;"><b>UG.</b></td>
						<?php endif; ?>
						<td align="center" width="30" style="font-size:7pt;"><b>Prog.</b></td>
						<td align="center" width="25" style="font-size:7pt;"><b>Ação</b></td>
						<td align="center" width="30" style="font-size:7pt;"><b>Loc.</b></td>
						<td align="center" width="30" style="font-size:7pt;"><b>PO</b></td>
						<td align="center" style="font-size:7pt;"><b>Natureza</b></td>
						<td align="center" width="30" style="font-size:7pt;"><b>Fonte</b></td>
						<td align="center" width="30" style="font-size:7pt;"><b>IDOC</b></td>
						<td align="center" width="30" style="font-size:7pt;"><b>IDUSO</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Dotação</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Sup.</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Can.</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Sup. Excesso</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Sup. Superávit</b></td>
					</tr>
				</thead>
				<?php $cor = '#ffffff'; ?>
				<?php $dot = 0; ?>
				<?php $sup = 0; ?>
				<?php $can = 0; ?>
				<?php $supe = 0; ?>
				<?php $sups = 0; ?>
				<?php foreach ( $creditos as $credito ) : ?>
					<?php $cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff'; ?>
					<?php $dot += $credito['dot']; ?>
					<?php $sup += $credito['sup']; ?>
					<?php $can += $credito['can']; ?>
					<?php $supe += $credito['supe']; ?>
					<?php $sups += $credito['sups']; ?>			
					<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
						<?php if ( !$unicod ) : ?>
							<td align="center" style="font-size:7pt;">
								<a href="javascript:mostrarLancamentos( <?php echo $tcrid; ?>, '<?php echo $credito['unicod']; ?>' );">
									<?php echo $credito['unicod']; ?>
								</a>
							</td>
						<?php endif; ?>
						<?php if ( $ungcod ) : ?>
							<td align="center" style="font-size:7pt;">
								<?php echo $ungcod ?>
							</td>
						<?php endif; ?>
						<td align="center" style="font-size:7pt;"><?php echo $credito['prgcod']; ?></td>
						<td align="left" style="font-size:7pt;" title="<?php echo $credito['acadsc']; ?>">
							<?php echo $credito['acacod']; ?>
						</td>
						<td align="center" style="font-size:7pt;"><?php echo $credito['loccod']; ?></td>
						<?php
						$sql = "select  po.titulo as titulo from elabrev.ws_planosorcamentariosdto as po
													join elabrev.ws_acaodto as a on a.identificadorunico = po.identificadorunicoacao 
													where a.codigoacao = '".$credito['acacod']."'
													and a.codigoorgao = '".$credito['unicod']."'
													and po.planoorcamentario = '".$credito['plocod']."'";
										
						$plotitulo = $db->pegaUm( $sql );
										
						?>
						<td align="center" title="<?php echo $plotitulo; ?>" style="font-size:7pt;"><?php echo $credito['plocod']; ?></td>
						<td align="center" style="font-size:7pt;"><?php echo $credito['ndpcod']; ?></td>
						<td align="center" style="font-size:7pt;"><?php echo $credito['foncod']; ?></td>
						<td align="center" style="font-size:7pt;"><?php echo $credito['idocod']; ?></td>
						<td align="center" style="font-size:7pt;"><?php echo $credito['iducod']; ?></td>
						<td align="right" style="font-size:7pt;">
							<?php echo number_format( $credito['dot'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color:#000050;font-size:7pt;">
							<?php echo number_format( $credito['sup'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color:#500000;font-size:7pt;">
							<?php echo number_format( $credito['can'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color:#000050;font-size:7pt;">
							<?php echo number_format( $credito['supe'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color:#000050;font-size:7pt;">
							<?php echo number_format( $credito['sups'], 0, ',', '.' ); ?>
						</td>
					</tr>
				<?php endforeach; ?>
				<?php $cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff'; ?>
				<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
					<td align="center" style="font-size:7pt;" colspan="<?php echo $unicod ? 8 : 9; ?>">&nbsp;</td>
					<?php if ( $ungcod ) : ?>
						<td align="center" style="font-size:7pt;">
							&nbsp;
						</td>
					<?php endif; ?>
					<td align="right" style="font-size:7pt;">
						<b><?php echo number_format( $dot, 0, ',', '.' ); ?></b>
					</td>
					<td align="right" style="color:#000050;font-size:7pt;">
						<b><?php echo number_format( $sup, 0, ',', '.' ); ?></b>
					</td>
					<td align="right" style="color:#500000;font-size:7pt;">
						<b><?php echo number_format( $can, 0, ',', '.' ); ?></b>
					</td>
					<td align="right" style="color:#000050;font-size:7pt;">
						<b><?php echo number_format( $supe, 0, ',', '.' ); ?></b>
					</td>
					<td align="right" style="color:#000050;font-size:7pt;">
						<b><?php echo number_format( $sups, 0, ',', '.' ); ?></b>
					</td>
				</tr>
			</table>
		<?php endif; ?>
		<?php if ( count( $receitas ) ) : ?>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<tr bgcolor="#d0d0d0">
					<td align="center">
						<b>R E C E I T A S</b>
					</td>
				</tr>
			</table>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<thead>
					<tr bgcolor="#e0e0e0">
						<?php if ( !$unicod ) : ?>
							<td align="center" width="30" style="font-size:7pt;"><b>Uni.</b></td>
						<?php endif; ?>
						<td align="center" style="font-size:7pt;"><b>Natureza de Receita</b></td>
						<td align="center" width="110" style="font-size:7pt;"><b>Fonte de Recurso</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Dotação</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Acréscimo</b></td>
						<td align="right" width="70" style="font-size:7pt;"><b>Redução</b></td>
					</tr>
				</thead>
				<?php $cor = '#ffffff'; ?>
				<?php $dot = 0; ?>
				<?php $acr = 0; ?>
				<?php $red = 0; ?>
				<?php foreach ( $receitas as $receita ) : ?>
					<?php $cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff'; ?>
					<?php $dot += $receita['dot']; ?>
					<?php $acr += $receita['acr']; ?>
					<?php $red += $receita['red']; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
						<?php if ( !$unicod ) : ?>
							<td align="center" style="font-size:7pt;">
								<a href="javascript:mostrarLancamentos( <?php echo $tcrid; ?>, '<?php echo $credito['unicod']; ?>' );">
									<?php echo $receita['unicod']; ?>
								</a>
							</td>
						<?php endif; ?>
						<td align="center" style="font-size:7pt;"><?php echo $receita['nrccod']; ?></td>
						<td align="center" style="font-size:7pt;"><?php echo $receita['foncod']; ?></td>
						<td align="right" style="font-size:7pt;">
							<?php echo number_format( $receita['dot'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color:#000050;font-size:7pt;">
							<?php echo number_format( $receita['acr'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color:#500000;font-size:7pt;">
							<?php echo number_format( $receita['red'], 0, ',', '.' ); ?>
						</td>
					</tr>
				<?php endforeach; ?>
				<?php $cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff'; ?>
				<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
					<td align="center" style="font-size:7pt;" colspan="<?php echo $unicod ? 2 : 3; ?>">&nbsp;</td>
					<td align="right" style="font-size:7pt;">
						<b><?php echo number_format( $dot, 0, ',', '.' ); ?></b>
					</td>
					<td align="right" style="color:#000050;font-size:7pt;">
						<b><?php echo number_format( $acr, 0, ',', '.' ); ?></b>
					</td>
					<td align="right" style="color:#500000;font-size:7pt;">
						<b><?php echo number_format( $red, 0, ',', '.' ); ?></b>
					</td>
				</tr>
			</table>
		<?php endif; ?>
		<?php if ( count( $diferenca ) ) : ?>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<tr bgcolor="#d0d0d0">
					<td align="center">
						<b>D I F E R E N Ç A</b>
					</td>
				</tr>
			</table>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<thead>
					<tr bgcolor="#e0e0e0">
						<td colspan="2" align="center" style="font-size:7pt; font-weight:bold;">Receita</td>
						<td colspan="2" align="center" style="font-size:7pt; font-weight:bold;">Créditos</td>
						<td rowspan="2" align="right" style="font-size:7pt; font-weight:bold;" width="80">Diferença</td>
					</tr>
					<tr bgcolor="#e0e0e0">
						<td align="center" style="font-size:7pt; font-weight:bold;" width="80">Acréscimo</td>
						<td align="center" style="font-size:7pt; font-weight:bold;" width="80">Redução</td>
						<td align="center" style="font-size:7pt; font-weight:bold;" width="80">Suplementação</td>
						<td align="center" style="font-size:7pt; font-weight:bold;" width="80">Cancelamento</td>
					</tr>				
				</thead>
				<tbody>
					<tr bgcolor="#ffffff">
						<td align="right" style="color: #000050; font-size:7pt;">
							<?php echo number_format( $diferenca['acrescimo'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color: #500000; font-size:7pt;">
							<?php echo number_format( $diferenca['reducao'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color: #000050; font-size:7pt;">
							<?php echo number_format( $diferenca['sup'], 0, ',', '.' ); ?>
						</td>
						<td align="right" style="color: #500000; font-size:7pt;">
							<?php echo number_format( $diferenca['can'], 0, ',', '.' ); ?>
						</td>
						<?php $dif = $diferenca['diferenca']; ?>
						<td align="right" style="color: <?php echo $dif < 0 ? '#500000' : '#000050'; ?>; font-size:7pt;">
							<b>
								<?php echo $dif < 0 ? '-' : '+'; ?><?php echo number_format( abs( $dif ), 0, ',', '.' ); ?>
							</b>
						</td>
					</tr>
				</tbody>
			</table>
		<?php endif; ?>
		<?php if ( count( $justificativa ) ) : ?>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<tr bgcolor="#d0d0d0">
					<td align="center">
						<b>J U S T I F I C A T I V A</b>
					</td>
				</tr>
			</table>
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<!--<tr>
					<td width="20%" align="right" class="SubTituloDireita">Resumo</td>
					<?php //$txt = $justificativa['jscresumo']; ?>
					<td width="80%" bgcolor="#ffffff"><?php //echo $txt ? $txt : '-'; ?></td>
				</tr> -->
				<tr>
				<!--	<td width="20%" align="right" class="SubTituloDireita">Caracterização</td> -->
				<td width="20%" align="right" class="SubTituloDireita">Indique a necessidade de alteração orçamentária.</td> 
					<?php $txt = $justificativa['jsccaracterizacao']; ?>
					<td width="80%" bgcolor="#f7f7f7"><?php echo $txt ? $txt : '-'; ?></td>
				</tr>
				<!--<tr>
					<td width="20%" align="right" class="SubTituloDireita">Resultado</td>
					<?php //$txt = $justificativa['jscresultado']; ?>
					<td width="80%" bgcolor="#ffffff"><?php //echo $txt ? $txt : '-'; ?></td>
				</tr>-->
				<tr>
					<!--<td width="20%" align="right" class="SubTituloDireita">Reflexo</td>-->
					<td width="20%" align="right" class="SubTituloDireita">Descreva o impacto do cancelamento de dotação.</td>
					<?php $txt = $justificativa['jscreflexo']; ?>
					<td width="80%" bgcolor="#ffffff"><?php echo $txt ? $txt : '-'; ?></td>
				</tr>
				<tr>
					<!--<td width="20%" align="right" class="SubTituloDireita">Consequência</td>-->
					<td width="20%" align="right" class="SubTituloDireita">Quais as consequências do não atendimento do pleito ?</td>
					<?php $txt = $justificativa['jscconsequencia']; ?>
					<td width="80%" bgcolor="#f7f7f7"><?php echo $txt ? $txt : '-'; ?></td>
				</tr>
				<tr>
					<!--<td width="20%" align="right" class="SubTituloDireita">Repercusão</td>-->
					<td width="20%" align="right" class="SubTituloDireita">Quais os reflexos do atendimento da demanda sobre o nível dos gastos de custeio do órgão e/ou da unidade orçamentária?</td>
					<?php $txt = $justificativa['jscrepercussao']; ?>
					<td width="80%" bgcolor="#f7f7f7"><?php echo $txt ? $txt : '-'; ?></td>
				</tr>
				<!--<tr>
					<td width="20%" align="right" class="SubTituloDireita">Aplicação</td>
					<?php //$txt = $justificativa['jscaplicacao']; ?>
					<td width="80%" bgcolor="#ffffff"><?php //echo $txt ? $txt : '-'; ?></td>
				</tr>-->
				<tr>
					<!--<td width="20%" align="right" class="SubTituloDireita">Memória</td>-->
					<td width="20%" align="right" class="SubTituloDireita">Memória de cálculo e outras informações consideradas relevantes.</td>
					<?php $txt = $justificativa['jscmemoria']; ?>
					<td width="80%" bgcolor="#f7f7f7"><?php echo $txt ? $txt : '-'; ?></td>
				</tr>
			</table>
		<?php endif; ?>
		<span class="notprint">
			<table align="center" class="tabela" cellpadding="3" cellspacing="1" style="border-top:none;">
				<tr>
					<td align="center" bgcolor="#ffffff">
						&nbsp;<br/>
						<input type="button" name="imprimir" value="Imprimir" onclick="window.print();"/>
						&nbsp;&nbsp;&nbsp;
						<input type="button" name="fechar" value="Fechar" onclick="window.close();"/>
						<br/>&nbsp;
					</td>
				</tr>
			</table>
		</span>
	</body>
</html>
