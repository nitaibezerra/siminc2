<?php

define( 'TIPO_PROPOSTA', 1 );
define( 'TIPO_PROJETO_LEI', 2 );
define( 'TIPO_LOA', 3 );

require_once 'credito_funcoes.inc';

$ppoid = cre_pegar_proposta_ativa();
$ano = $_SESSION['exercicio'];

$tcrid = $_SESSION['creditos_adicionais_tcrid'];
$unicod = $_SESSION['creditos_adicionais_unicod'];

$unicod_permitidas = "'" . implode( "','", $db->usuarioUnidadesPermitidas() ) . "'";

$sql = "select unidsc from public.unidade where unicod = '" . $unicod . "'";
$unidsc = $db->pegaUm( $sql );

include APPRAIZ . 'includes/cabecalho.inc';

?>
<br/>
<?php $db->cria_aba( $abacod_tela, $url, '' ); ?>
<?php monta_titulo( $titulo_modulo, 'Relatório de Erros da Unidade ' . $unidsc ); ?>

<?php

$ano = $_SESSION['exercicio'];

$sql_momento = <<<EOT
	
	select
		mcrid as codigo,
		mcrdsc as descricao
	from 
		elabrev.momentocredito
	where
		mcrstatus = 'A' and mcrano = '$ano'
	order by
		mcrid desc
			
EOT;

// captura a quantidade somente os momentos atuais
$sql_qtd_momento = <<<EOT
	
	select
		count(*)
	from 
		elabrev.momentocredito
	where		
		mcrstatus = 'A' and mcrano = '$ano'
	
EOT;
$qtd_momento = $db->pegaUm( $sql_qtd_momento );

if(isset($_REQUEST["mcrid"])) {
		$mcrid = $_REQUEST["mcrid"];
}
else {
	$arrayMomento = $db->carregar($sql_momento);
	
	if(isset($arrayMomento[0]["codigo"]))
		$mcrid = $arrayMomento[0]["codigo"];
	else
		$mcrid = 'null';
}
	
?>
<form method="post" name="escolher_momento">	
	<table width="90%" class="tabela" cellspacing="1"  cellpadding="3" bgcolor="#f5f5f5" align="center">
		<?php if ( $qtd_momento ) : ?>
			<tr>
				<td width="20%" align="right" class="SubTituloDireita">
					Momento
				</td>
				<td width="80%">
					<?= $db->monta_combo( 'mcrid', $sql_momento, 'S', '', '', '', '', '', '', '' ) ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">
					&nbsp;
				</td>
				<td>
					<input type="submit" name="enviar" value="Avançar"/>
				</td>
			</tr>
		<?php else : ?>
			<tr>
				<td colspan="2" align="center" style="color: #bb3030;">
					Nenhum momento está disponível para a data atual
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>
<br />
<?php

// ----------  ----------  ----------  CRÉDITOS NEGATIVOS  ----------  ----------  ----------

// captura itens com saldo negativo
$sql = <<<EOT
	select
		-- campos para exibicao
		a.unicod,
		a.prgcod,
		a.acacod,
		a.loccod,
		n.ndpcod,
		d.foncod,
		i.idocod,
		d.iducod,
		-- campos para identificacao unica de cada item negativado
		d.acaidloa,
		d.ndpid,
		--d.foncod,
		d.idoid,
		--d.iducod,
		abs( sum( d.dpavalor ) ) as saldo
	from elabrev.despesaacao d
		inner join monitora.acao a on
			a.acaid = d.acaidloa
		inner join public.naturezadespesa n on
			n.ndpid = d.ndpid
		inner join public.idoc i on
			i.idoid = d.idoid
	where
 		a.unicod in ( $unicod_permitidas ) and
		a.prgano = '$ano'		
		-- momento de crédito
		and d.mcrid = $mcrid		
	group by
		a.unicod,
		a.prgcod,
		a.acacod,
		a.loccod,
		n.ndpcod,
		d.foncod,
		i.idocod,
		d.iducod,
		d.acaidloa,
		d.ndpid,
		d.idoid
	having
		sum( d.dpavalor ) < 0
EOT;
$negativos = $db->carregar( $sql );
$negativos = $negativos ? $negativos : array();

// captura as fontes onde o crédito está sendo utilizado
$sql_base = <<<EOT
	select
		t.tcrid,
		t.tcrcod
	from elabrev.despesaacao d
		inner join elabrev.tipocredito t on
			t.tcrid = d.tcrid
	where
		t.tcrano = '$ano' and
		d.acaidloa = %d and
		d.ndpid = %d and
		d.foncod = '%s' and
		d.idoid = %d and
		d.iducod = '%s'
EOT;
$tc = array();
foreach ( $negativos as &$despesa )
{
	$sql = sprintf(
		$sql_base,
			$despesa['acaidloa'],
			$despesa['ndpid'],
			$despesa['foncod'],
			$despesa['idoid'],
			$despesa['iducod']
	);
	$regs = $db->carregar( $sql );
	if ( !$regs )
	{
		$despesa['tc'] = array();
		continue;
	}
	$despesa['tc'] = array();
	foreach ( $regs as $reg )
	{
		$tc[$reg['tcrid']] = $reg['tcrcod'];
		array_push( $despesa['tc'], $reg['tcrcod'] );
	}
	asort( $despesa['tc'] );
}
$tc = array_unique( $tc );
asort( $tc );

// aqui está definido 2 variaveis
// $tc com tcrcod dos tipos de creditos utilizados pelos itens negativados
// $negativados com os dados de cada item negativao

?>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;border-bottom:0;" class="tabela">
	<thead>
		<tr bgcolor="#cfcfcf">
			<th>
				&nbsp;<br/>
				D E S P E S A S&nbsp;&nbsp;&nbsp;&nbsp;N E G A T I V A D A S
				<br/>&nbsp;
			</th>
		</tr>
	</thead>
</table>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;border-top:0;" class="tabela">
	<?php if ( count( $negativos ) ) : ?>
		<tr bgcolor="#e0e0e0">
			<td align="center" width="8%" rowspan="2"><b>Unidade</b></td>
			<td align="center" width="8%" rowspan="2"><b>Programa</b></td>
			<td align="center" width="8%" rowspan="2"><b>Ação</b></td>
			<td align="center" width="8%" rowspan="2"><b>Unidade</b></td>
			<td align="center" width="8%" rowspan="2"><b>Localizador</b></td>
			<td align="center" width="8%" rowspan="2"><b>Natureza</b></td>
			<td align="center" width="8%" rowspan="2"><b>Fonte</b></td>
			<td align="center" width="8%" rowspan="2"><b>IDOC</b></td>
			<td align="center" width="8%" rowspan="2"><b>IDUSO</b></td>
			<td align="center" colspan="<?php echo count( $tc ); ?>">Tipo de crédito</td>
			<td align="right" rowspan="2"><b>Saldo</b></td>
		</tr>
		<tr bgcolor="#e0e0e0">
			<?php reset( $tc ); ?>
			<?php foreach ( $tc as $tcrcod ) : ?>
				<td align="center"><b><?php echo $tcrcod; ?></b></td>
			<?php endforeach; ?>
		</tr>
		<?php $cor = '#f0f0f0'; ?>
		<?php foreach ( $negativos as $item ) : ?>
			<?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
			<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
				<td align="center"><?php echo $item['unicod']; ?></td>
				<td align="center"><?php echo $item['prgcod']; ?></td>
				<td align="center"><?php echo $item['acacod']; ?></td>
				<td align="center"><?php echo $item['unicod']; ?></td>
				<td align="center"><?php echo $item['loccod']; ?></td>
				<td align="center"><?php echo $item['ndpcod']; ?></td>
				<td align="center"><?php echo $item['foncod']; ?></td>
				<td align="center"><?php echo $item['idocod']; ?></td>
				<td align="center"><?php echo $item['iducod']; ?></td>
				<?php reset( $tc ); ?>
				<?php foreach ( $tc as $tcrid => $tcrcod ) : ?>
					<td align="center" width="5%">
						<?php if ( in_array( $tcrcod, $item['tc'] ) ) : ?>
							<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tcrid; ?>&unicod=<?php echo $item['unicod']; ?>">
								<img src="/imagens/check_p.gif" title="Editar crédito" border="0" style="margin:0;padding:0;"/>
							</a>
						<?php else : ?>
							&nbsp;
						<?php endif; ?>
					</td>
				<?php endforeach; ?>
				<td align="right" style="color:#ff0000;">
					-<?php echo number_format( $item['saldo'], '0', ',', '.' ); ?>
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
<?php

// ----------  ----------  ----------  FIM CRÉDITOS NEGATIVOS  ----------  ----------  ----------

?>
<?php

// ----------  ----------  ----------  RECEITAS NEGATIVAS  ----------  ----------  ----------

// captura itens com saldo negativo
$sql = <<<EOT
	select
		r.rcuano,
		r.unicod,
		r.foncod,
		n.nrcid,
		n.nrccod,
		n.nrcdsc,
		coalesce( d.rcdvalor, 0 ) as dot,
		sum( r.rcuacrescimo ) as acr,
		sum( r.rcureducao ) as red,
		abs( coalesce( d.rcdvalor, 0 )
			+ sum( r.rcuacrescimo )
			- sum( r.rcureducao ) ) as saldo
	from elabrev.receitaunidade r
		left join elabrev.receitaunidadedotacao d on
			d.rcdano = r.rcuano and
			d.unicod = r.unicod and
			d.unitpocod = r.unitpocod and
			d.foncod = r.foncod and
			d.nrcid = r.nrcid
		inner join public.naturezareceita n on
			n.nrcid = r.nrcid
	where
	 	r.unicod in ( $unicod_permitidas ) and
		r.rcuano = '$ano'		
		-- momento de crédito
		and r.mcrid = $mcrid		
	group by
		r.rcuano,
		r.unicod,
		r.foncod,
		n.nrcid,
		n.nrccod,
		n.nrcdsc,
		d.rcdvalor
	having
		coalesce( d.rcdvalor, 0 ) + sum( r.rcuacrescimo ) - sum( r.rcureducao ) < 0
	order by
		r.unicod,
		r.foncod,
		n.nrccod
EOT;
$negativos = $db->carregar( $sql );
$negativos = $negativos ? $negativos : array();

// captura as fontes onde a receita está sendo utilizada
$sql_base = <<<EOT
	select
		t.tcrid,
		t.tcrcod
	from elabrev.receitaunidade r
		inner join elabrev.tipocredito t on
			t.tcrid = r.tcrid
	where
		t.tcrano = '$ano' and
		r.rcuano = '%s' and
		r.unicod = '%s' and
		r.foncod = '%s' and
		r.nrcid = %d		
EOT;
$tc = array();
foreach ( $negativos as &$receita )
{
	$sql = sprintf(
		$sql_base,
			$receita['rcuano'],
			$receita['unicod'],
			$receita['foncod'],
			$receita['nrcid']
	);
	$regs = $db->carregar( $sql );
	if ( !$regs )
	{
		$receita['tc'] = array();
		continue;
	}
	$receita['tc'] = array();
	foreach ( $regs as $reg )
	{
		$tc[$reg['tcrid']] = $reg['tcrcod'];
		array_push( $receita['tc'], $reg['tcrcod'] );
	}
	asort( $receita['tc'] );
}
$tc = array_unique( $tc );
asort( $tc );

// aqui está definido 2 variaveis
// $tc com tcrcod dos tipos de creditos utilizados pelos itens negativados
// $negativados com os dados de cada item negativao

?>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;border-bottom:0;" class="tabela">
	<thead>
		<tr bgcolor="#cfcfcf">
			<th>
				&nbsp;<br/>
				R E C E I T A S&nbsp;&nbsp;&nbsp;&nbsp;N E G A T I V A D A S
				<br/>&nbsp;
			</th>
		</tr>
	</thead>
</table>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;border-top:0;" class="tabela">
	<?php if ( count( $negativos ) ) : ?>
		<tr bgcolor="#e0e0e0">
			<td align="center" width="100" rowspan="2"><b>Unidade</b></td>
			<td align="left" rowspan="2"><b>Natureza de Receita</b></td>
			<td align="center" width="200" rowspan="2"><b>Fonte de Recurso</b></td>
			<td align="center" colspan="<?php echo count( $tc ); ?>">Tipo de crédito</td>
			<td align="right" rowspan="2"><b>Saldo</b></td>
		</tr>
		<tr bgcolor="#e0e0e0">
			<?php reset( $tc ); ?>
			<?php foreach ( $tc as $tcrcod ) : ?>
				<td align="center"><b><?php echo $tcrcod; ?></b></td>
			<?php endforeach; ?>
		</tr>
		<?php $cor = '#f0f0f0'; ?>
		<?php foreach ( $negativos as $item ) : ?>
			<?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
			<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
				<td align="center"><?php echo $item['unicod']; ?></td>
				<td align="left"><?php echo $item['nrccod'] . ' - ' . $item['nrcdsc']; ?></td>
				<td align="center"><?php echo $item['foncod']; ?></td>
				<?php reset( $tc ); ?>
				<?php foreach ( $tc as $tcrid => $tcrcod ) : ?>
					<td align="center" width="5%">
						<?php if ( in_array( $tcrcod, $item['tc'] ) ) : ?>
							<a href="?modulo=principal/remanejamento/credito_receita&acao=A&tcrid=<?php echo $tcrid; ?>&unicod=<?php echo $item['unicod']; ?>">
								<img src="/imagens/check_p.gif" title="Editar crédito" border="0" style="margin:0;padding:0;"/>
							</a>
						<?php else : ?>
							&nbsp;
						<?php endif; ?>
					</td>
				<?php endforeach; ?>
				<td align="right" style="color:#ff0000;">
					-<?php echo number_format( $item['saldo'], '0', ',', '.' ); ?>
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
<?php

// ----------  ----------  ----------  FIM RECEITAS NEGATIVAS  ----------  ----------  ----------

?>
<?php

// ----------  ----------  ----------  DIFERENÇA  ----------  ----------  ----------

$sql_base_diferenca = <<<EOT
	select
		u.unicod,
		u.unidsc,
		coalesce( dot.valor, 0 ) as dot,
		coalesce( sup.valor, 0 ) as sup,
		coalesce( can.valor, 0 ) as can,
		coalesce( rec_dot.valor, 0 ) as rec_dot,
		coalesce( rec_acr.valor, 0 ) as rec_acr,
		coalesce( rec_red.valor, 0 ) as rec_red,
		coalesce( sup.valor, 0 )
			- coalesce( can.valor, 0 )
			+ coalesce( rec_dot.valor, 0 )
			- coalesce( rec_acr.valor, 0 )
			+ coalesce( rec_red.valor, 0 ) as diferenca
	from elabrev.despesaacao d
		inner join monitora.acao a on
			a.acaid = d.acaidloa
		inner join public.unidade u on
			u.unicod = a.unicod and
			u.unitpocod = a.unitpocod
		left join
		(
			select
				a.unicod,
				d.tcrid,
				sum( d.dpavalor ) as valor
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid is null and
				d.ppoid = $ppoid
			group by
				a.unicod,
				d.tcrid
		) as dot on
			dot.unicod = a.unicod
		left join
		(
			select
				a.unicod,
				d.tcrid,
				sum( d.dpavalor ) as valor
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid is not null and
				d.ppoid = $ppoid and
				d.dpavalor > 0
			group by
				a.unicod,
				d.tcrid
		) as sup on
			sup.unicod = a.unicod
		left join
		(
			select
				a.unicod,
				d.tcrid,
				abs( sum( d.dpavalor ) ) as valor
			from elabrev.despesaacao d
				inner join monitora.acao a on
					a.acaid = d.acaidloa
			where
				d.tcrid is not null and
				d.ppoid = $ppoid and
				d.dpavalor < 0
			group by
				a.unicod,
				d.tcrid
		) as can on
			can.unicod = a.unicod
		left join
		(
			select
				unicod,
				tcrid,
				sum( rcuacrescimo ) as valor
			from elabrev.receitaunidade
			where
				rcuano = '$ano'				
			group by
				unicod,
				tcrid
		) as rec_acr on
			rec_acr.unicod = a.unicod
		left join
		(
			select
				unicod,
				tcrid,
				sum( rcureducao ) as valor
			from elabrev.receitaunidade
			where
				rcuano = '$ano'
			group by
				unicod,
				tcrid
		) as rec_red on
			rec_acr.unicod = a.unicod
		left join
		(
			select
				d.unicod,
				t.tcrid,
				sum( d.rcdvalor ) as valor
			from elabrev.receitaunidadedotacao d
				inner join elabrev.tipocredito t on
					t.tcrano = d.rcdano
			where
				d.rcdano = '$ano'
			group by
				d.unicod,
				t.tcrid
		) as rec_dot on
			rec_dot.unicod = a.unicod
	where
		d.ppoid = $ppoid and
 		u.unicod in ( $unicod_permitidas ) and
		sup.tcrid = %d and
		can.tcrid = %d and
		rec_dot.tcrid = %d and
		rec_acr.tcrid = %d and
		rec_red.tcrid = %d		
	group by
		sup.tcrid,
		u.unicod,
		u.unidsc,
		dot.valor,
		sup.valor,
		can.valor,
		rec_acr.valor,
		rec_red.valor,
		rec_dot.valor
	having
		coalesce( sup.valor, 0 )
			- coalesce( can.valor, 0 )
			+ coalesce( rec_dot.valor, 0 )
			- coalesce( rec_acr.valor, 0 )
			+ coalesce( rec_red.valor, 0 ) != 0
	order by
		u.unicod,
		u.unidsc
EOT;

?>
<style>
	.tabela_balanco td
	{
		font-size: 7.5pt;
	}
</style>
<?php

// ----------  ----------  ----------  FIM SALDO  ----------  ----------  ----------

?>
<?php

// ----------  ----------  ----------  VERIFICAÇÃO POR CRÉDITO  ----------  ----------  ----------

$sql = <<<EOT
	select
		*
	from elabrev.tipocredito
		where tcrstatus = 'A' and tcrano = '$ano' 
	order by
		tcrcod
EOT;
$tipos_credito = $db->carregar( $sql );
$tipos_credito = $tipos_credito ? $tipos_credito : array();
foreach ( $tipos_credito as &$tc )
{
	$id = $tc['tcrid'];
	$tc['acaid'] = array();
	$tc['gndcod'] = array();
	$tc['gunid'] = array();
	$tc['unicod'] = array();
	$sql = <<<EOT
		select
			acaid
		from elabrev.tipocreditoacao
		where
			tcrid = $id
EOT;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach( $linhas as $linha )
	{
		array_push( $tc['acaid'], $linha['acaid'] );
	}
	$sql = <<<EOT
		select
			gndcod
		from elabrev.tipocreditognd
		where
			tcrid = $id
EOT;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach( $linhas as $linha )
	{
		array_push( $tc['gndcod'], $linha['gndcod'] );
	}
	$sql = <<<EOT
		select
			gunid
		from elabrev.tipocreditogrupounidade
		where
			tcrid = $id
EOT;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach( $linhas as $linha )
	{
		array_push( $tc['gunid'], $linha['gunid'] );
	}
	$sql = <<<EOT
		select
			unicod
		from elabrev.tipocreditounidade
		where
			tcrid = $id
EOT;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach( $linhas as $linha )
	{
		array_push( $tc['unicod'], $linha['unicod'] );
	}
}


function pegar_extrapolados( $tcrid, $tipo, $tcrtipoporcento )
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
	if ( $tipo == 'can' )
	{
		$inner = <<<EOT
			case when sum( d.dpavalor ) > 0
				then ( sum( abs( coalesce( can.dpavalor, 0 ) ) ) / sum( d.dpavalor ) ) * 100
				else 0
			end as can_exec
EOT;
		$having = <<<EOT
			tcrporcentocancelamento > 0
				and
				( sum( abs( coalesce( can.dpavalor, 0 ) ) ) / sum( d.dpavalor ) ) * 100 > tcrporcentocancelamento
EOT;
	}
	else
	{
		$inner = <<<EOT
			case when sum( d.dpavalor ) > 0
				then ( sum( coalesce( sup.dpavalor, 0 ) ) / sum( d.dpavalor ) ) * 100
				else 0
			end as sup_exec
EOT;
		$having = <<<EOT
			tcrporcentosuplementacao > 0
				and
				( sum( abs( coalesce( sup.dpavalor, 0 ) ) ) / sum( d.dpavalor ) ) * 100 > tcrporcentosuplementacao
EOT;
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
			$inner
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
			( $having )
EOF;
	$extrapolados = $db->carregar( $sql );
	return $extrapolados ? $extrapolados : array();
}

?>
<script type="text/javascript">
	
	function mostrarLancamentos( tcrid, unicod )
	{
		var url =
			'?modulo=relatorio/remanejamento/credito_unidade&acao=A' +
				'&tcrid=' + tcrid +
				'&unicod=' + unicod;
		window.open(
			url,
			'creditosAdicionaisDetalhesUnidade',
			'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=yes,width=650,height=550'
		);
	}
	
</script>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="color:101010;border-top:0;border-bottom:0;" class="tabela">
	<thead>
		<tr bgcolor="#cfcfcf">
			<th>
				&nbsp;<br/>
				V E R I F I C A Ç Ã O&nbsp;&nbsp;&nbsp;&nbsp;P O R&nbsp;&nbsp;&nbsp;&nbsp;T I P O&nbsp;&nbsp;&nbsp;&nbsp;D E&nbsp;&nbsp;&nbsp;&nbsp;C R É D I T O
				<br/>&nbsp; 
			</th>
		</tr>
		<tr bgcolor="#cfcfcf">
			<td style="padding: 10px;">
				<b>Cancelamento</b>:&nbsp;&nbsp;Itens que extrapolaram o limite de porcentagem de cancelamento.
				<br/>
				<b>Suplementação</b>:&nbsp;&nbsp;Itens que extrapolaram o limite de porcentagem de cancelamento.
				<br/>
				<b>Ação</b>:&nbsp;&nbsp;Itens que possuem alguma Ação não permitida pelo tipo de crédito.
				<br/>
				<b>GND</b>:&nbsp;&nbsp;Itens que possuem algum GND não permitido pelo tipo de crédito.
				<br/>
				<b>Tipo de Unidade</b>:&nbsp;&nbsp;Itens que possuem algum Tipo de Unidade não permitido pelo tipo de crédito.
				<br/>
				<b>Unidade</b>:&nbsp;&nbsp;Itens que possuem alguma Unidade não permitida pelo tipo de crédito.
				<br/>
				<b>Diferença</b>:&nbsp;&nbsp;Itens que a soma entre os valores de alteração de créditos e receitas não resultam em valor zero.
			</td>
		</tr>
	</thead>
</table>
<table align="center" border="0" cellspacing="1" cellpadding="2" style="border-top:0;" class="tabela">
	<tr bgcolor="#d0d0d0">
		<td align="center" width="110"><b>Tipo de Crédito</b></td>
		<td align="center" width="170"><b>Item Avaliado</b></td>
		<td align="center" width="60"><b>Status</b></td>
		<td align="left"><b>Detalhes</b></td>
	</tr>
	<?php foreach ( $tipos_credito as $num_item => $tc ) : ?>
		<?php $id = $tc['tcrid']; ?>
		<tr>
			<td bgcolor="#e0e0e0" align="center" rowspan="7">
				<font style="font-size:10pt;font-weight:bold;"><?php echo $tc['tcrcod']; ?></font>
			</td>
			<?php
				// ----------  ----------  CANCELAMENTO ----------  ----------
			?>
			<?php $extrapolados = pegar_extrapolados( $tc['tcrid'], 'can', $tc['tcrtipoporcento'] ); ?>
			<td bgcolor="#f0f0f0" align="center">
				Cancelamento
			</td>
			<?php if ( count( $extrapolados ) == 0 ) : ?>
				<td style="color:#0000ff;" align="center">OK</td>
				<td align="left">Nenhum item extrapolou o limite de cancelamento.</td>
			<?php else : ?>
				<td align="center"><font style="color:#FF0000;">ERRO</font></td>
				<td>
					<table cellspacing="0" cellpadding="0" border="0">
						<?php foreach ( $invalidos as $invalido ) : ?>
							<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<td>
									<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tc['tcrid']; ?>&unicod=<?php echo $invalido['uni']; ?>">
										<?php echo $invalido['uni']; ?>.<b><?php echo $invalido['cod']; ?></b> - <?php echo $invalido['dsc']; ?>
									</a>
								</td>
							</tr>
						<?php endforeach; ?>
					</table>
				</td>
			<?php endif; ?>
		</tr>
		<tr>
			<?php
				// ----------  ----------  SUPLEMENTAÇÂO ----------  ----------
			?>
			<?php $extrapolados = pegar_extrapolados( $tc['tcrid'], 'sup', $tc['tcrtipoporcento'] ); ?>
			<td bgcolor="#f0f0f0" align="center">
				Suplementação
			</td>
			<?php if ( count( $extrapolados ) == 0 ) : ?>
				<td style="color:#0000ff;" align="center">OK</td>
				<td align="left">Nenhum item extrapolou o limite de suplementação.</td>
			<?php else : ?>
				<td align="center"><font style="color:#FF0000;">ERRO</font></td>
				<td>
					<table cellspacing="0" cellpadding="0" border="0">
						<?php foreach ( $invalidos as $invalido ) : ?>
							<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<td>
									<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tc['tcrid']; ?>&unicod=<?php echo $invalido['uni']; ?>">
										<?php echo $invalido['uni']; ?>.<b><?php echo $invalido['cod']; ?></b> - <?php echo $invalido['dsc']; ?>
									</a>
								</td>
							</tr>
						<?php endforeach; ?>
					</table>
				</td>
			<?php endif; ?>
		</tr>
		<tr>
			<?php
				// ----------  ----------  ACAO  ----------  ----------
			?>
			<td bgcolor="#f0f0f0" align="center">
				Ação
			</td>
			<?php if ( count( $tc['acaid'] ) == 0 ) : ?>
				<td align="center"><font style="color:#0000ff;">OK</font></td>
				<td>Este tipo de crédito aceita todas as Ações.</td>
			<?php else : ?>
				<?php
						$cods = implode( ",",  $tc['acaid'] );
						$sql = <<<EOT
							select
								a.acacod as cod,
								a.acadsc as dsc,
								a.unicod as uni
							from elabrev.despesaacao d
								inner join monitora.acao a on
									a.acaid = d.acaidloa
							where
								d.tcrid = $id and
								a.acaid not in ( $cods ) and
 								a.unicod in ( $unicod_permitidas ) 								
							group by
								a.acacod,
								a.acadsc,
								a.unicod
EOT;
						$invalidos = $db->carregar( $sql );
						$invalidos = $invalidos ? $invalidos : array();
					?>
				<?php if ( count( $invalidos ) == 0 ) : ?>
					<td align="center"><font style="color:#0000ff;">OK</font></td>
					<td>Nenhuma Ação inválida foi encontrada neste tipo de crédito.</td>
				<?php else : ?>
					<td align="center"><font style="color:#FF0000;">ERRO</font></td>
					<td>
						<table cellspacing="0" cellpadding="0" border="0">
							<?php foreach ( $invalidos as $invalido ) : ?>
								<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td>
										<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tc['tcrid']; ?>&unicod=<?php echo $invalido['uni']; ?>">
											<?php echo $invalido['uni']; ?>.<b><?php echo $invalido['cod']; ?></b> - <?php echo $invalido['dsc']; ?>
										</a>
									</td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
				<?php endif; ?>
			<?php endif; ?>
		</tr>
		<tr>
			<?php
				// ----------  ----------  GND  ----------  ----------
			?>
			<td bgcolor="#f0f0f0" align="center">
				GND
			</td>
			<?php if ( count( $tc['gndcod'] ) == 0 ) : ?>
				<td align="center"><font style="color:#0000ff;">OK</font></td>
				<td>Este tipo de crédito aceita todos os GNDs.</td>
			<?php else : ?>
				<?php
					$cods = "'" . implode( "','",  $tc['gndcod'] ) . "'";
					$sql = <<<EOT
						select
							g.gndcod as cod,
							g.gnddsc as dsc,
							a.unicod as uni
						from elabrev.despesaacao d
							inner join public.naturezadespesa n on
								n.ndpid = d.ndpid
							inner join public.gnd g on
								g.gndcod = n.gndcod
							inner join monitora.acao a on
									a.acaid = d.acaidloa
						where
							d.tcrid = $id and
							g.gndcod not in ( $cods ) and
 							a.unicod in ( $unicod_permitidas )
						group by
							g.gndcod,
							g.gnddsc,
							a.unicod
EOT;
					$invalidos = $db->carregar( $sql );
					$invalidos = $invalidos ? $invalidos : array();
				?>
				<?php if ( count( $invalidos ) == 0 ) : ?>
					<td align="center"><font style="color:#0000ff;">OK</font></td>
					<td>Nenhum GND inválido foi encontrada neste tipo de crédito.</td>
				<?php else : ?>
					<td align="center"><font style="color:#FF0000;">ERRO</font></td>
					<td>
						<table cellspacing="0" cellpadding="0" border="0">
							<?php foreach ( $invalidos as $invalido ) : ?>
								<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td>
										<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tc['tcrid']; ?>&unicod=<?php echo $invalido['uni']; ?>">
											<?php echo $invalido['uni']; ?>.<b><?php echo $invalido['cod']; ?></b> - <?php echo $invalido['dsc']; ?>
										</a>
									</td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
				<?php endif; ?>
			<?php endif; ?>
		</tr>
		<tr>
			<?php
				// ----------  ----------  GRUPO DE UNIDADE  ----------  ----------
			?>
			<td bgcolor="#f0f0f0" align="center">
				Tipo de Unidade
			</td>
			<?php if ( count( $tc['gunid'] ) == 0 ) : ?>
				<td align="center"><font style="color:#0000ff;">OK</font></td>
				<td>Este tipo de crédito aceita todos os Tipos de Unidade.</td>
			<?php else : ?>
				<?php
					$cods = implode( ",",  $tc['gunid'] );
					$sql = <<<EOT
						select
							gu.gunid as cod,
							gu.gundsc as dsc,
							a.unicod as uni
						from elabrev.despesaacao d
							inner join monitora.acao a on
								a.acaid = d.acaidloa
							inner join public.unidade u on
								u.unicod = a.unicod and
								u.unitpocod = a.unitpocod
							inner join public.grupounidade gu on
								gu.gunid = u.gunid
						where
							d.tcrid = $id and
							gu.gunid not in ( $cods ) and
 							a.unicod in ( $unicod_permitidas )
						group by
							gu.gunid,
							gu.gundsc,
							a.unicod
EOT;
					$invalidos = $db->carregar( $sql );
					$invalidos = $invalidos ? $invalidos : array();
				?>
				<?php if ( count( $invalidos ) == 0 ) : ?>
					<td align="center"><font style="color:#0000ff;">OK</font></td>
					<td>Nenhum Tipo de Unidade inválido foi encontrada neste tipo de crédito.</td>
				<?php else : ?>
					<td align="center"><font style="color:#FF0000;">ERRO</font></td>
					<td>
						<table cellspacing="0" cellpadding="0" border="0">
							<?php foreach ( $invalidos as $invalido ) : ?>
								<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td>
										<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tc['tcrid']; ?>&unicod=<?php echo $invalido['uni']; ?>">
											<?php echo $invalido['uni']; ?>.<b><?php echo $invalido['cod']; ?></b> - <?php echo $invalido['dsc']; ?>
										</a>
									</td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
				<?php endif; ?>
			<?php endif; ?>
		</tr>
		<tr>
			<?php
				// ----------  ----------  UNIDADE  ----------  ----------
			?>
			<td bgcolor="#f0f0f0" align="center">
				Unidade
			</td>
			<?php if ( count( $tc['unicod'] ) == 0 ) : ?>
				<td align="center"><font style="color:#0000ff;">OK</font></td>
				<td>Este tipo de crédito aceita todas as Unidade.</td>
			<?php else : ?>
				<?php
					$cods = "'" . implode( "','",  $tc['unicod'] ) . "'";
					$sql = <<<EOT
						select
							u.unicod as cod,
							u.unidsc as dsc,
							a.unicod as uni
						from elabrev.despesaacao d
							inner join monitora.acao a on
								a.acaid = d.acaidloa
							inner join public.unidade u on
								u.unicod = a.unicod and
								u.unitpocod = a.unitpocod
							where
								d.tcrid = $id and
								u.unicod not in ( $cods ) and
 								a.unicod in ( $unicod_permitidas )
							group by
								u.unicod,
								u.unidsc,
								a.unicod
EOT;
					$invalidos = $db->carregar( $sql );
					$invalidos = $invalidos ? $invalidos : array();
				?>
				<?php if ( count( $invalidos ) == 0 ) : ?>
					<td align="center"><font style="color:#0000ff;">OK</font></td>
					<td>Nenhuma Unidade inválida foi encontrada neste tipo de crédito.</td>
				<?php else : ?>
					<td align="center"><font style="color:#FF0000;">ERRO</font></td>
					<td>
						<table cellspacing="0" cellpadding="0" border="0">
							<?php foreach ( $invalidos as $invalido ) : ?>
								<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td>
										<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $tc['tcrid']; ?>&unicod=<?php echo $invalido['uni']; ?>">
											<?php echo $invalido['uni']; ?>.<b><?php echo $invalido['cod']; ?></b> - <?php echo $invalido['dsc']; ?>
										</a>
									</td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
				<?php endif; ?>
			<?php endif; ?>
		</tr>
		<tr>
			<?php
				// ----------  ----------  SALDO NEGATIVO (DESPESAS E RECEITAS)  ----------  ----------
				
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
							a.unicod in ( $unicod_permitidas )
							-- momento de crédito
							and d.mcrid = $mcrid		
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
							a.unicod in ( $unicod_permitidas )
							-- momento de crédito
							and d.mcrid = $mcrid		
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
							u.unicod in ( $unicod_permitidas )
							-- momento de crédito
							and ru.mcrid = $mcrid		
						group by
							tc.tcrid,	tc.tcrcod,
							u.unicod,	u.unidsc
					) as a
					where
						tcrid = {$tc['tcrid']}
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
			?>
			<td bgcolor="#f0f0f0" align="center">
				Diferença
			</td>
			<?php if ( count( $diferenca ) == 0 ) : ?>
				<td align="center"><font style="color:#0000ff;">OK</font></td>
				<td>Este tipo de crédito não possui itens com diferenças entre receitas e créditos.</td>
			<?php else : ?>
				<td align="center"><font style="color:#FF0000;">ERRO</font></td>
				<td>
					<table align="left" cellspacing="2" cellpadding="2">
						<thead>
							<tr bgcolor="#f5f5f5">
								<td colspan="2" align="center" style="font-size:9px;">Receita</td>
								<td colspan="2" align="center" style="font-size:9px;">Créditos</td>
								<td rowspan="2" align="right" style="font-size:9px;" width="80">Diferença</td>
							</tr>
							<tr bgcolor="#f5f5f5">
								<td align="center" style="font-size:9px;" width="80">Acréscimo</td>
								<td align="center" style="font-size:9px;" width="80">Redução</td>
								<td align="center" style="font-size:9px;" width="80">Suplementação</td>
								<td align="center" style="font-size:9px;" width="80">Cancelamento</td>
							</tr>				
						</thead>
						<tbody>
							<tr bgcolor="#fafafa">
								<td align="right" style="color: #000050;font-size:9px;">
									<?php echo number_format( $diferenca['acrescimo'], 0, ',', '.' ); ?>
								</td>
								<td align="right" style="color: #500000;font-size:9px;">
									<?php echo number_format( $diferenca['reducao'], 0, ',', '.' ); ?>
								</td>
								<td align="right" style="color: #000050;font-size:9px;">
									<?php echo number_format( $diferenca['sup'], 0, ',', '.' ); ?>
								</td>
								<td align="right" style="color: #500000;font-size:9px;">
									<?php echo number_format( $diferenca['can'], 0, ',', '.' ); ?>
								</td>
								<?php $dif = $diferenca['diferenca']; ?>
								<td align="right" style="color: <?php echo $dif < 0 ? '#500000' : '#000050'; ?>;font-size:9px;">
									<b>
										<a href="javascript:mostrarLancamentos( <?php echo $tc['tcrid']; ?>, '' )">
											<?php echo $dif < 0 ? '-' : '+'; ?><?php echo number_format( abs( $dif ), 0, ',', '.' ); ?>
										</a>
									</b>
								</td>
							</tr>
						</tbody>
					</table>
				</td>
			<?php endif; ?>
		</tr>
		<tr bgcolor="#d0d0d0">
			<td colspan="4" height="3"></td>
		</tr>
	<?php endforeach; ?>
</table>
<?php

// ----------  ----------  ----------  FIM VERIFICAÇÃO POR CRÉDITO  ----------  ----------  ----------

?>



















