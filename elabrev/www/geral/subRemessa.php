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
		r.remid,
		r.remdata,
		r.remresumo,
		sum( coalesce( can.dpavalor, 0 ) ) as can,
		sum( coalesce( sup.dpavalor, 0 ) ) as sup,
		sum( coalesce( can.dpavalor, 0 ) + coalesce( sup.dpavalor, 0 ) ) as dif
	from elabrev.remessa r
		left join (
			select remid, sum( dpavalor ) as dpavalor
			from elabrev.despesaacao
			where remid is not null and dpavalor < 0
			group by remid
		) as can on
			can.remid = r.remid
		left join (
			select remid, sum( dpavalor ) as dpavalor
			from elabrev.despesaacao
			where remid is not null and dpavalor > 0
			group by remid
		) as sup on
			sup.remid = r.remid
	where
		r.unicod = '$codigo'
	group by
		r.remid,
		r.remdata,
		r.remresumo
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
			<?php $id = $codigo . $item['remid']; ?>
			<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
				<td valign="top" style="border-top: 1px solid #cccccc; width: 20px;">
					<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
				</td>
				<td valign="top" style="border-top: 1px solid #cccccc;">
					<a href="javascript:abrirFecharSubItem( '<?= $codigo ?>', '<?= $item['remid'] ?>' );"><img src="../imagens/mais.gif" name="+" border="0" id="img<?= $id ?>"></a>
					<a href="javascript:popupRemessa( '<?= $item['remid'] ?>' );"><img src="../imagens/report.gif" border="0"></a>
					<?php $data = explode( '-', $item['remdata'] ); ?>
					<?= $data[2] . '/' . $data[1] . '/' . $data[0] ?>
					<?php $resumo = strlen( $item['remresumo'] ) > 70 ? substr( $item['remresumo'], 0, 67 ) . '...' : $item['remresumo'] ?>
					<?= $resumo ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 100px; padding:2px;">
					<?= number_format( $item['can'] , 0, ',', '.' ) ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 100px; padding:2px;">
					<?= number_format( $item['sup'] , 0, ',', '.' ) ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; width: 98px; padding:2px;">
					<?= number_format( $item['sup'] + $item['can'], 0, ',', '.' ) ?>
				</td>
			</tr>
			<tr>
				<td id="td<?= $id ?>" colspan="5" style="padding-left:23px;" align="center"></td>
			</tr>
		<?php endforeach; ?>
	</table>
<?php endif; ?>







