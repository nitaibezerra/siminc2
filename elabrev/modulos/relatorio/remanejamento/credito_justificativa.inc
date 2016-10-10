<?php


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, 'Relatório de Justificativa' );

$unicod = (string) $_REQUEST['unicod'];
$tcrid = (integer) $_REQUEST['tcrid'];
$mcrid = (integer) $_REQUEST['mcrid'];


?>
<script type="text/javascript">

	
	function atualizarRelatorio()
	{
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
						<td width="185" align="center">&nbsp;</td>
					</tr>
					<tr>
						<td colspan="2" align="center">&nbsp;</td>
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
						) as foo
					";
					$db->monta_combo( 'mcrid', $sql, 'S', '', '', '', '' );
				?>
			</td>
		</tr>
		<tr bgcolor="#e9e9e9">
			<td align="right" valign="middle">&nbsp;</td>
			<td align="left" valign="middle">&nbsp;</td>
			<td align="left" valign="middle">&nbsp;</td>
			<td align="left" valign="middle">&nbsp;</td>
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
	array_push( $where, " unidade = '" . $unicod . "' " );
}
if ( $tcrid )
{
	array_push( $where, " tc.tcrid = " . $tcrid . " " );
}
if ( $mcrid )
{
	array_push( $where, " mcrid = " . $mcrid . " " );
	$valor_momento = " = " . $mcrid . " ";
}
$where = implode( " and ", $where );

$sql = "
	SELECT
unidade, tipocredito,
jsccaracterizacao,
jscconsequencia,
jscreflexo,
jscrepercussao,
jscmemoria 
from
(
SELECT
a.unicod as unidade, tc.tcrcod as tipocredito, a.prgcod as programa, 
a.acacod as acao, a.loccod as localizador, d.foncod as fonte, 
i.idocod as idoc, d.iducod as iduso, a.irpcod as irpcod, d.dpavalor as suplementacao, 0 as cancelamento,
CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as natureza,
d.dpaid as sequencial, a.esfcod as esfera, a.funcod as funcao, a.sfucod as subfuncao
FROM elabrev.despesaacao d
INNER JOIN monitora.acao a on a.acaid = d.acaidloa                 
                                   and a.unicod like '26%'
                                   and a.prgano = '2011'
                                   and a.acasnrap = 'f'
INNER JOIN elabrev.tipocredito tc on tc.tcrid = d.tcrid                                         
                                              	and tc.tcrcod in ('100','101', '102', '103', '107', '110', '111', '112', '120', '150', '157', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
INNER JOIN public.idoc i on i.idoid = d.idoid
LEFT JOIN public.naturezadespesa n on n.ndpid = d.ndpid
LEFT JOIN public.naturezareceita nr on nr.nrcid = d.nrcid
WHERE 
d.dpavalor > 0 and d.mcrid = " . ( $mcrid ? $mcrid : "27" ) . "
and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )

union all

SELECT

a.unicod as unidade, tc.tcrcod as tipocredito, a.prgcod as programa, 
a.acacod as acao, a.loccod as localizador, d.foncod as fonte, 
i.idocod as idoc, d.iducod as iduso, a.irpcod as irpcod, 0 as suplementacao, d.dpavalor as cancelamento,
CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as natureza,
d.dpaid as sequencial, a.esfcod as esfera, a.funcod as funcao, a.sfucod as subfuncao
FROM elabrev.despesaacao d
INNER JOIN monitora.acao a on a.acaid = d.acaidloa                 
                                   and a.unicod like '26%'
                                   and a.prgano = '2011'
                                   and a.acasnrap = 'f'
INNER JOIN elabrev.tipocredito tc on tc.tcrid = d.tcrid                                        
                                              	and tc.tcrcod in ('100','101', '102', '103', '107', '110', '111', '112', '120', '150', '157', '152', '156', '160', '175', '176', '196', '200', '500', '600', '710', '910', '920')
INNER JOIN public.idoc i on i.idoid = d.idoid
LEFT JOIN public.naturezadespesa n on n.ndpid = d.ndpid
LEFT JOIN public.naturezareceita nr on nr.nrcid = d.nrcid

WHERE 

d.dpavalor < 0 and d.mcrid = " . ( $mcrid ? $mcrid : "27" ) . "
and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
) as foo
inner join ( select tcrid, tcrcod from elabrev.tipocredito where tcrano ='2011' and tcrstatus = 'A' ) tc ON tc.tcrcod = foo.tipocredito
inner join ( select tcrid, unicod,jsccaracterizacao, jscconsequencia, jscreflexo, jscrepercussao, jscmemoria  from elabrev.justificativacredito jc 
		where mcrid = " . ( $mcrid ? $mcrid : "27" ) . " ) jc ON jc.tcrid = tc.tcrid and jc.unicod = foo.unidade
	where " . ( $where ? $where : "1 = 1" ) . "
 group by  unidade, tipocredito 
,jsccaracterizacao, 
jscconsequencia, 
jscreflexo, 
jscrepercussao, 
jscmemoria 

order by tipocredito , unidade 
";
//dbg($sql,1);


$cabecalho = array( 'Unidade', 'Tipo de Crédito', 'Necessidade', 'Consequência', 'Impacto', 'Reflexos', 'Memória' );
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '' ,'' );

?>



