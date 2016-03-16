<?php

//error_reporting( E_ALL );

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
	Programador: Renan de Lima Barbosa (renandelima@gmail.com)
	Módulo: credito_escolher_filtro.inc
	Finalidade: escolher créditos a ser remanejado
*/

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, 'Suplementação e Cancelamento de Créditos Adicionais' );

/*** Verifica se perfil do usuário é de Super usuário, CGO / Coordenador de Orçamento ou CGO / Equipe Orçamentária ***/
$pflcods = array(SUPER_USUARIO,CGO_COORDENADOR_ORCAMENTO,CGO_EQUIPE_ORCAMENTARIA);


$arrUnicod_permitidas = $db->usuarioUnidadesPermitidas();
$arrUnicod_permitidas = $arrUnicod_permitidas ? $arrUnicod_permitidas : array();

if( possuiPerfil( $pflcods ) )
{
	$sql = "select 
						distinct
						u.unicod as codigo 
					from public.unidade u
						inner join monitora.acao a ON a.unicod = u.unicod
					where 
						unistatus = 'A' and a.prgano = '{$_SESSION['exercicio']}'
					order by 1";
	$arrCreditoUnidade = $db->carregarColuna( $sql );
	$arrCreditoUnidade = $arrCreditoUnidade ? $arrCreditoUnidade : array();
}
else{
	
	$sql = "SELECT unicod FROM elabrev.momentocreditounid WHERE mcrid = ".$_SESSION['credito_mcrid'];
	$arrCreditoUnidade = $db->carregarColuna( $sql );
	$arrCreditoUnidade = $arrCreditoUnidade ? $arrCreditoUnidade : array();
}

if( $arrCreditoUnidade ){
	$unicod_permitidas = array();
	
	foreach ($arrUnicod_permitidas as $unicod) {
		if( in_array( $unicod, $arrCreditoUnidade ) ){
			array_push( $unicod_permitidas, $unicod );
		}
	}
	//array_push( $unicod_permitidas, '74902' );
	$unicod_permitidas = "'" . implode( "','", $unicod_permitidas ) . "'";


$ano = $_SESSION['exercicio'];
$sql_tcrid = <<<EOF
	select
		tc.tcrid as codigo,
		tc.tcrcod as descricao
	from elabrev.tipocredito tc
		left join elabrev.tipocreditounidade tcu on
			tcu.tcrid = tc.tcrid
		left join elabrev.tipocreditogrupounidade tcg on
			tcg.tcrid = tc.tcrid
		left join public.unidade u on
			u.gunid = tcg.gunid
	where
		tc.tcrstatus = 'A' and
		tc.tcrano = '$ano'
		and
		(
			tcu.tcrid is null or
			tcu.unicod in ( $unicod_permitidas )
		)
 		and
		(
			tcg.tcrid is null or
			u.unicod in ( $unicod_permitidas )
		)
	group by
		tc.tcrid,
		tc.tcrcod
	order by
		tc.tcrcod
EOF;




/*** Se for exibe todas as Ug's ***/ 
if( possuiPerfil( $pflcods ) )
{	
	$sql_ungcod = "
		select 
			ungcod as codigo, 
			ungcod || ' - ' || ungdsc as descricao  
		from 
			public.unidadegestora 
		where 
			ungstatus = 'A' and unicod = '26101'
			and podelancarcredito = 't' 
		order by
			ungdsc
	";
	$sql_unicod = "	select 
						distinct
						u.unicod as codigo, 
						u.unicod || ' - ' || u.unidsc as descricao  
					from public.unidade u
						inner join monitora.acao a ON a.unicod = u.unicod
					where 
						unistatus = 'A' and a.prgano = '{$_SESSION['exercicio']}'
					order by 1";
}
/*** Se não for, exibe somente as Ug's associadas ***/
else
{
	$ungcodUser = $db->carregarColuna("SELECT ungcod FROM elabrev.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND ungcod is not null");

	if( $ungcodUser )
	{
		$sql_ungcod = "
			select 
				ungcod as codigo, 
				ungcod || ' - ' || ungdsc as descricao  
			from 
				public.unidadegestora 
			where 
				ungstatus = 'A'
				AND ungcod in ('".implode("','", $ungcodUser)."')
				and podelancarcredito = 't' 
			order by
				ungdsc
		";
		
	}
	
	// exibe somente as unidades permitidas
	$sql_unicod = "select 
		unicod as codigo, 
		unicod || ' - ' || unidsc as descricao  
	from public.unidade 
	where 
		unistatus = 'A' and
		(
			unicod in ( $unicod_permitidas )
		)
	order by 1";
}



// lista créditos adicionais iniciados
$sql = <<<EOT

select
	tcrid,
	tcrcod,
	--coalesce(ungcod,'') as ungcod,
	unicod,
	unidsc,
	abs ( sum( sup ) ) as sup,
	abs ( sum( can ) ) as can,
	abs ( sum( acrescimo ) ) as acrescimo,
	abs ( sum( reducao ) ) as reducao,
	abs ( sum( sup ) ) - abs ( sum( can ) ) - abs ( sum( acrescimo ) ) + abs ( sum( reducao ) ) as diferenca
from
(
	select
		tc.tcrid,
		tc.tcrcod,
		--d.ungcod,
		u.unicod,
		u.unidsc,
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
		d.mcrid = {$_SESSION['credito_mcrid']}
	group by
		tc.tcrid,
		tc.tcrcod,
		--d.ungcod,
		u.unicod,
		u.unidsc
union all
	select
		tc.tcrid,
		tc.tcrcod,
	--	d.ungcod,
		u.unicod,
		u.unidsc,
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
		d.mcrid = {$_SESSION['credito_mcrid']}
	group by
		tc.tcrid,
		tc.tcrcod,
		--d.ungcod,
		u.unicod,
		u.unidsc
union all
	select
		tc.tcrid,
		tc.tcrcod,
		--'' as ungcod,
		u.unicod,
		u.unidsc,
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
		ru.mcrid = {$_SESSION['credito_mcrid']}
	group by
		tc.tcrid,
		tc.tcrcod,
		u.unicod,
		u.unidsc
union all

select 		tc.tcrid,
		tc.tcrcod,
		--'' as ungcod,
		u.unicod,
		u.unidsc,
		0 as sup,
		0 as can,
		0 as acrescimo,
		0 as reducao
 from 
elabrev.justificativacredito jc
			inner join public.unidade u on
				u.unicod = jc.unicod and
				u.unitpocod = jc.unitpocod
			inner join elabrev.tipocredito tc on
				tc.tcrid = jc.tcrid
where mcrid = {$_SESSION['credito_mcrid']}  and
		u.unicod in ( $unicod_permitidas )
) as a
group by 
	tcrid,
	tcrcod,
	unicod,
	--coalesce(ungcod,''),
	unidsc
order by
	unicod,
	unidsc,
	tcrcod

EOT;
/*
 * Retirado o UNGCOD da listagem
 * 
 */
//dbg($sql, 1);

$creditos_iniciados = $db->carregar( $sql );
$creditos_iniciados = $creditos_iniciados ? $creditos_iniciados : array();

// captura nome do momento de crédito
$sql = "select mcrdsc from elabrev.momentocredito where mcrid = " . $_SESSION['credito_mcrid'];
$mcrdsc = $db->pegaUm( $sql );

?>
<form action="" method="post" name="escolher">
	<input type="hidden" name="escolher" value="0"/>
	<table width="90%" class="tabela" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" >
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Momento:
			</td>
			<td width="80%">
				<?= $mcrdsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Tipo de Crédito:
			</td>
			<td width="80%">
                            
				<?php
                                    // Regra para atender a demanda #224117
                                    if($_SESSION['credito_mcrid'] === 45 || $_SESSION['credito_mcrid'] === '45'){
                                        $arr = array(array('codigo'=>'209', 'descricao'=>'101'), 
                                                     array('codigo'=>'212', 'descricao'=>'112'), 
                                                     array('codigo'=>'217', 'descricao'=>'157'));
                                        $db->monta_combo( 'tcrid', $arr, 'S', 'Tipo de Crédito', '', '', '', '', '', '' ); 
                                    }else{
                                        $db->monta_combo( 'tcrid', $sql_tcrid, 'S', 'Tipo de Crédito', '', '', '', '', '', '' ); 
                                    }
                                ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Unidade:
			</td>
			<td width="80%">
				<?php
				$funcaoUnicod = ($sql_ungcod) ? 'habilitaComboUG(this)' : '';
				?>
				<?= $db->monta_combo( 'unicod', $sql_unicod, 'S', 'Unidade', $funcaoUnicod, '', '', '', '', '' ) ?>
			</td>
		</tr>
		<?php
		if( ($_REQUEST['ungcod'] && $_REQUEST['unicod'] == '26101') || $unicod_permitidas == "'26101'" )
		{
			$display = 'table-row';
			if(is_array($ungcodUser)){
				$ungcod = $_REQUEST['ungcod'] ? $_REQUEST['ungcod'] : $ungcodUser[0];
			}else{
				$ungcod = $_REQUEST['ungcod'];
			}
		}
		else
		{
			$display = 'none';
		}
		?>
		<tr id="tr_unidade_gestora" style="display:<?=$display?>;">
			<td width="20%" align="right" class="SubTituloDireita">
				UG:
			</td>
			<td width="80%">
				<?php
				if($sql_ungcod)
				{
					echo $db->monta_combo( 'ungcod', $sql_ungcod, 'S', 'Unidade Gestora', '', '', '', '', 'S', '' );
				}
				?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				&nbsp;
			</td>
			<td width="80%">
				<input type="button" onclick="avancar();" name="gravar" value="Avançar" />
			</td>
		</tr>
	</table>
	<?php if ( $creditos_iniciados ) : ?>
		<table align="center" class="listagem" width="95%" cellspacing="0" cellpadding="2">
			<thead>
				<tr bgcolor="#e0e0e0">
					<td rowspan="2" align="center" width="50"><b>Editar</b></td>
					<td rowspan="2" align="center" width="100"><b>Tipo de Crédito</b></td>
					<td rowspan="2" align="center"><b>Unidade Orçamentária</b></td>
					<td colspan="2" align="center"><b>Receita</b></td>
					<td colspan="2" align="center"><b>Créditos</b></td>
					<td rowspan="2" align="center"><b>Diferença</b></td>
				</tr>
				<tr bgcolor="#e0e0e0">
					<td align="center" width="100"><b>Acréscimo</b></td>
					<td align="center" width="100"><b>Redução</b></td>
					<td align="center" width="100"><b>Suplementação</b></td>
					<td align="center" width="100"><b>Cancelamento</b></td>
				</tr>				
			</thead>
			<tbody>
				<?php $cor = '#ffffff'; ?>
				<?php foreach ( $creditos_iniciados as $iniciado ) : ?>
					<?php $cor = $cor == '#ffffff' ? '#f7f7f7' : '#ffffff'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';">
						<td align="center">
						<?php if( $iniciado['unicod'] != '26101' ){ ?>
							<a href="?modulo=principal/remanejamento/credito&acao=A&tcrid=<?php echo $iniciado['tcrid']; ?>&unicod=<?php echo $iniciado['unicod']; ?>&ungcod=<?php echo $iniciado['ungcod']; ?>">
								<img src="/imagens/check_p.gif" border="0" style="margin:0;padding:0;"/>
							</a>
						<?php } //inserido por solicitação do gerente para garantia de sempre salvar o ungcod da ultima que mexer.  ?>
						</td>
						<td align="center"><?php echo $iniciado['tcrcod']; ?></td>
						<td align="left">
							<?php echo $iniciado['unicod'] . ' - ' . $iniciado['unidsc']; ?>
							<?php 
								if($iniciado['ungcod']){
									echo " - UG: ". $iniciado['ungcod'] ." - ". $db->pegaUm("select ungdsc from public.unidadegestora where ungcod = '".$iniciado['ungcod']."'");
								}
							?>
						</td>
						<td align="right" style="color: #000050;"><?php echo number_format( $iniciado['acrescimo'], 0, ',', '.' ); ?></td>
						<td align="right" style="color: #500000;"><?php echo number_format( $iniciado['reducao'], 0, ',', '.' ); ?></td>
						<td align="right" style="color: #000050;"><?php echo number_format( $iniciado['sup'], 0, ',', '.' ); ?></td>
						<td align="right" style="color: #500000;"><?php echo number_format( $iniciado['can'], 0, ',', '.' ); ?></td>
						<td align="right" style="color: #500000;"><?php echo number_format( $iniciado['diferenca'], 0, ',', '.' ); ?></td>
					</tr>
				<?php endforeach; ?>
			</tbody>
		</table>
	<?php endif; ?>
</form>
<?
} else {
	?>
		<table width="90%" class="tabela" cellspacing="1"  cellpadding="3" bgcolor="#f5f5f5" align="center">
		<tr>
				<td colspan="2" align="center" style="color: #bb3030;">
					Nenhum momento está disponível para a data atual
				</td>
			</tr>
			</table>
		<br/>
	<?php
}
?>
<script type="text/javascript">
	
	function avancar()
	{
		if( document.escolher.unicod.value == '26101' )
		{
			if( document.escolher.ungcod.value == '' )
			{
				alert("O campo 'UG' deve ser informado.");
				document.escolher.ungcod.focus();
				return;
			}
		}
		
		document.escolher.escolher.value = '0';
		document.escolher.submit();
	}

	function habilitaComboUG(obj)
	{
		if( obj.value == '26101' )
		{
			document.getElementById('tr_unidade_gestora').style.display = 'table-row';
		}
		else
		{
			document.getElementById('tr_unidade_gestora').style.display = 'none';
		}
	}
	
</script>