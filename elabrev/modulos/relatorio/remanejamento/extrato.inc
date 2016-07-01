<?php

/*
	Sistema Simec
	Setor responsável: SPO-MEC
	Desenvolvedor: Equipe Consultores Simec
	Analista: Adonias Malosso (malosso@gmail.com)
	Programador: Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
	Módulo: remessa.inc
	Finalidade: Relatório agrupado por unidade/remessa/ação
*/

// seleciona os itens a serem exibidos no relatório
// exibe somente as unidades que o usuário possui privilégio
$joinUnidade = $db->usuarioJoinUnidadesPermitidas();
$sql = <<<EOF
	select
		r.remid,
		r.remresumo,
		r.remdata,
		r.unicod,
		unijoin.unidsc
	from elabrev.remessa r
	inner join public.unidade u on u.unicod = r.unicod
		$joinUnidade and unijoin.unicod = r.unicod
	order by
		r.remdata DESC
EOF;
$itens = $db->carregar( $sql );
if ( !$itens )
{
	$itens = array();
}

// query base para captura dos itens de remessa
$sqlBaseRemanejamentos = <<<EOF
	select
		a.unicod,	a.prgcod,
		a.acacod,	a.loccod,
		n.ndpcod,	d.iducod,
		d.foncod,	i.idocod,
		d.dpavalor
	from elabrev.despesaacao d
		inner join public.naturezadespesa n on
			n.ndpid = d.ndpid
		inner join public.idoc i on
			i.idoid = d.idoid
		inner join elabrev.ppaacao_orcamento a on
			a.acaid = d.acaid
	where
		d.remid = %s
	order by
		d.dpavalor asc
EOF;

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Extrato de Remanejamentos' );

?>
<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" class="tabela">
	<?php foreach ( $itens as $chave => $item ) : ?>
		<tr>
			<td bgcolor="#c0c0c0" align="right">Data</td>
			<td bgcolor="#c0c0c0" align="left"><?= $item['remdata'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Orçamentária</td>
			<td><?= $item['unicod'] ?> - <?= $item['unidsc'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Resumo</td>
			<td><?= $item['remresumo'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Remanejamentos</td>
			<td>
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<tr>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							Unidade
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							Programa
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							Ação
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							Localizador
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="12%">
							Natureza
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							IDUCOD
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							Fonte
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="10%">
							IDOCOD
						</td>
						<td class="SubTituloDireita" style="text-align: center;" width="18%">
							Valor
						</td>
					</tr>
					<?php
						$sqlRemanejamentos = sprintf( $sqlBaseRemanejamentos, $item['remid'] );
						$remanejamentos = $db->carregar( $sqlRemanejamentos );
						if ( !$remanejamentos )
						{
							$remanejamentos = array();
						}
					?>
					<?php if ( count( $remanejamentos ) > 0 ) : ?>
						<?php foreach( $remanejamentos as $chave => $itemRemanejamento ) : ?>
							<?php $cor = $chave % 2 == 0 ? '#f7f7f7' : '' ; ?>
							<tr bgcolor="<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?= $cor ?>';">
								<td align="center"><?= $itemRemanejamento['unicod'] ?></td>
								<td align="center"><?= $itemRemanejamento['prgcod'] ?></td>
								<td align="center"><?= $itemRemanejamento['acacod'] ?></td>
								<td align="center"><?= $itemRemanejamento['loccod'] ?></td>
								<td align="center"><?= $itemRemanejamento['ndpcod'] ?></td>
								<td align="center"><?= $itemRemanejamento['iducod'] ?></td>
								<td align="center"><?= $itemRemanejamento['foncod'] ?></td>
								<td align="center"><?= $itemRemanejamento['idocod'] ?></td>
								<td align="right" style="color: #003f7e;">
									<?= number_format( $itemRemanejamento['dpavalor'], 0, ',', '.' ) ?>
								</td>
							</tr>
						<?php endforeach; ?>
					<?php else : ?>
						<tr>
							<td align="center" colspan="9" style="color: #dd0000">
								Não foi efetuado nenhum remanejamento
							</td>
						</tr>
					<?php endif; ?>
				</table>
			</td>
		</tr>
	<?php endforeach; ?>
</table>