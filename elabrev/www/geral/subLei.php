<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Adonias Malosso (malosso@gmail.com)
	Programador: Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
	Módulo: remanejamentoRelatorioSubLei.inc
	Finalidade: Exibir os subitens da tela de relatório de lei
*/

// inicia sistema
header( 'Cache-Control: no-store, no-cache, must-revalidate' );
header( 'Cache-Control: post-check=0, pre-check=0', false );
header( 'Cache-control: private, no-cache' );
header( 'Last-Modified: ' . gmdate( 'D, d M Y H:i:s' ) . ' GMT' );
header( 'Pragma: no-cache' );
header( 'Content-Type: text/html; charset=iso-8859-1' );
include 'config.inc';
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/funcoes.inc';
$db = new cls_banco();

// seleciona os itens a serem exibidos no relatório
// exibe somente as unidades que o usuário possui privilégio
$joinUnidade = $db->usuarioJoinUnidadesPermitidas();
$codigo = $_REQUEST['unicod'];
$sql = <<<EOF
	select
		a.acacod,
		a.acadsc,
		sum( coalesce( lei.dpavalor, 0 ) ) as lei,
		sum( coalesce( can.dpavalor, 0 ) ) as can,
		sum( coalesce( sup.dpavalor, 0 ) ) as sup,
		sum( coalesce( lei.dpavalor, 0 ) + coalesce( can.dpavalor, 0 ) + coalesce( sup.dpavalor, 0 ) ) as sal
	from elabrev.ppaacao_orcamento a
		inner join (
			select sum( d.dpavalor ) as dpavalor, a.unicod, d.acaid
			from elabrev.despesaacao d
			inner join elabrev.ppaacao_orcamento a on
				d.acaid = a.acaid
			where remid is null
			group by a.unicod, d.acaid
		) as lei on
			lei.acaid = a.acaid
		left join (
			select sum( d.dpavalor ) as dpavalor, a.unicod, d.acaid
			from elabrev.despesaacao d
			inner join elabrev.ppaacao_orcamento a on
				d.acaid = a.acaid
			where remid is not null and dpavalor < 0
			group by a.unicod, d.acaid
		) as can on
			can.acaid = a.acaid
		left join (
			select sum( d.dpavalor ) as dpavalor, a.unicod, d.acaid
			from elabrev.despesaacao d
			inner join elabrev.ppaacao_orcamento a on
				d.acaid = a.acaid
			where remid is not null and dpavalor > 0
			group by a.unicod, d.acaid
		) as sup on
			sup.acaid = a.acaid
	where
		a.unicod = '$codigo'
	group by
		a.acacod,
		a.acadsc
EOF;
$itens = $db->carregar( $sql );
if ( !$itens )
{
	$itens = array();
}

?>
<?php if ( count( $itens ) == 0 ) : ?>
	<font color='red'>Não foram encontrados Registros</font>
<?php else : ?>
	<table width="100%" align="right" cellpadding="0" cellspacing="0" style="color:#003f7e;">
		<?php foreach ( $itens as $item ) : ?>
			<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
				<td valign="top" style="border-top: 1px solid #cccccc; width: 20px;">
					<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
				</td>
				<td align="center" style="border-top: 1px solid #cccccc; width: 50px;">
					<?= $item['acacod'] ?>
				</td>
				<td valign="top" style="border-top: 1px solid #cccccc;">
					<?= $item['acadsc'] ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 100px; padding:2px;">
					<?= number_format( $item['lei'] , 0, ',', '.' ) ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 100px; padding:2px;">
					<?= number_format( $item['can'] , 0, ',', '.' ) ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 100px; padding:2px;">
					<?= number_format( $item['sup'] , 0, ',', '.' ) ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 98px; padding:2px;">
					<?= number_format( $item['sal'] , 0, ',', '.' ) ?>
				</td>
			</tr>
		<?php endforeach; ?>
	</table>
<?php endif; ?>







