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

/**
 * Monta array com dados de ordenação de acordo com o $_REQUEST.
 * 
 * @return string[]
 */
function pegarOrdenacao()
{
	static $camposSql = array(
		'cod' => 'u.unicod',
		'dsc' => 'u.unidsc',
		'can' => 'can',
		'sup' => 'sup',
		'dif' => 'dif'
	);
	static $direcoesSql = array(
		'a' => ' ASC ',
		'd' => ' DESC '
	);
	$ordem = array(
		'campo' => '',
		'campoSql' => '',
		'direcao' => '',
		'direcaoSql' => '',
		'direcaoImagem' => '',
		'proximaDirecao' => ''
	);
	// verifica campo
	if ( array_key_exists( $_REQUEST['ordenacaoCampo'], $camposSql ) == true )
	{
		$ordem['campo'] = $_REQUEST['ordenacaoCampo'];
		$ordem['campoSql'] = $camposSql[$_REQUEST['ordenacaoCampo']];
	}
	else
	{
		$ordem['campo'] = 'cod';
		$ordem['campoSql'] = $camposSql['cod'];
	}
	// verifica direção
	if ( array_key_exists( $_REQUEST['ordenacaoDirecao'], $direcoesSql ) == true )
	{
		$ordem['direcao'] = $_REQUEST['ordenacaoDirecao'];
		$ordem['direcaoSql'] = $direcoesSql[$_REQUEST['ordenacaoDirecao']];
		$ordem['proximaDirecao'] = $ordem['direcao'] == 'a' ? 'd' : 'a';
	}
	else
	{
		$ordem['direcao'] = 'a';
		$ordem['direcaoSql'] = $direcoesSql['a'];
		$ordem['proximaDirecao'] = 'd';
	}
	$ordem['direcaoImagem'] = trim( $ordem['direcaoSql'] );
	return $ordem;
}

// seleciona os itens a serem exibidos no relatório
// exibe somente as unidades que o usuário possui privilégio
$joinUnidade = $db->usuarioJoinUnidadesPermitidas();
$ordem = pegarOrdenacao();
$sql = "
	select
		u.unicod,
		u.unidsc,
		sum( coalesce( can.dpavalor, 0 ) ) as can,
		sum( coalesce( sup.dpavalor, 0 ) ) as sup,
		sum( coalesce( can.dpavalor, 0 ) + coalesce( sup.dpavalor, 0 ) ) as dif
	from public.unidade u
		" . $joinUnidade . " and unijoin.unicod = u.unicod
		left join elabrev.remessa r on
			u.unicod = r.unicod
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
	group by
		u.unicod,
		u.unidsc
	order by
		" . $ordem['campoSql'] . " " . $ordem['direcaoSql'] . "";

$itens = $db->carregar( $sql );
if ( !$itens )
{
	$itens = array();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Relatório de Remanejamentos' );

?>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td style="width:50px;" class="title" nowrap align="center">
				<strong>
					<label title="Ordenar por Código" onclick="ordenar( 'cod', '<?= $ordem['proximaDirecao'] ?>' );">
						Código
						<?php if ( $ordem['campo'] == 'cod' ) : ?>
							<img src="../imagens/seta_ordem<?= $ordem['direcaoImagem'] ?>.gif" width="11" height="13" align="middle"/>
						<?php endif; ?>
					</label>
				</strong>
			</td>
			<td class="title" nowrap align="center">
				<strong>
					<label title="Ordenar por Unidade Orçamentária" onclick="ordenar( 'dsc', '<?= $ordem['proximaDirecao'] ?>' );">
						Unidade Orçamentária
						<?php if ( $ordem['campo'] == 'dsc' ) : ?>
							<img src="../imagens/seta_ordem<?= $ordem['direcaoImagem'] ?>.gif" width="11" height="13" align="middle"/>
						<?php endif; ?>
					</label>
				</strong>
			</td>
			<td style="width:100px;" class="title" nowrap align="center">
				<strong>
					<label title="Ordenar por Cancelamento" onclick="ordenar( 'can', '<?= $ordem['proximaDirecao'] ?>' );">
						Cancelamento
						<?php if ( $ordem['campo'] == 'can' ) : ?>
							<img src="../imagens/seta_ordem<?= $ordem['direcaoImagem'] ?>.gif" width="11" height="13" align="middle"/>
						<?php endif; ?>
					</label>
				</strong>
			</td>
			<td style="width:100px;" class="title" nowrap align="center">
				<strong>
					<label title="Ordenar por Suplementação" onclick="ordenar( 'sup', '<?= $ordem['proximaDirecao'] ?>' );">
						Suplementação
						<?php if ( $ordem['campo'] == 'sup' ) : ?>
							<img src="../imagens/seta_ordem<?= $ordem['direcaoImagem'] ?>.gif" width="11" height="13" align="middle"/>
						<?php endif; ?>
					</label>
				</strong>
			</td>
			<td style="width:100px;" class="title" nowrap align="center">
				<strong>
					<label title="Ordenar por Suplementação" onclick="ordenar( 'dif', '<?= $ordem['proximaDirecao'] ?>' );">
						Diferenca
						<?php if ( $ordem['campo'] == 'dif' ) : ?>
							<img src="../imagens/seta_ordem<?= $ordem['direcaoImagem'] ?>.gif" width="11" height="13" align="middle"/>
						<?php endif; ?>
					</label>
				</strong>
			</td>
		</tr>
	</thead>
	<tbody>
		<?php foreach ( $itens as $chave => $item ) : ?>
			<?php $id = $item['unicod']; ?>
			<?php $cor = $chave % 2 == 0 ? '#f7f7f7' : '' ; ?>
			<tr bgcolor="<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?= $cor ?>';">
				<td align="center"><?= $item['unicod'] ?></td>
				<td>
					<a href="javascript:abrirFecharItem( '<?= $id ?>' );">
						<img src="../imagens/mais.gif" name="+" border="0" id="img<?= $id ?>">
						<?= $item['unidsc'] ?>
					</a>
				</td>
				<td align="right"><?= number_format( $item['can'], 0, ',', '.' ) ?></td>
				<td align="right"><?= number_format( $item['sup'], 0, ',', '.' ) ?></td>
				<td align="right"><?= number_format( $item['dif'], 0, ',', '.' ) ?></td>
			</tr>
			<tr bgcolor="<?= $cor ?>">
				<td id="td<?= $id ?>" colspan="5" style="padding-left:68px;" align="center"></td>
			</tr>
		<?php endforeach; ?>
	</tbody>
</table>
<form name="formulario" method="post">
	<input type="hidden" name="ordenacaoCampo" value="cod"/>
	<input type="hidden" name="ordenacaoDirecao" value="a"/>
</form>
<script type="text/javascript">
	
	/**
	 * Ordena os itens da tabela.
	 * 
	 * @param string
	 * @param string
	 * @return void
	 */
	function ordenar( campo, direcao )
	{
		var formulario = document.formulario;
		formulario.ordenacaoCampo.value = campo;
		formulario.ordenacaoDirecao.value = direcao;
		formulario.submit();
	}

	/**
	 * Abre ou fecha os subitens de um item da tabela.
	 * 
	 * @return void
	 */
	function abrirFecharItem( codigo )
	{
		var url = 'geral/subRemessa.php?unicod=' + codigo;
		abreconteudo( url, codigo );
	}
	
	/**
	 * Abre ou fecha os subitens de um item da tabela.
	 * 
	 * @return void
	 */
	function abrirFecharSubItem( codigo, subCodigo )
	{
		var url = 'geral/subSubRemessa.php?unicod=' + codigo + '&remid=' + subCodigo;
		abreconteudo( url, codigo + subCodigo );
	}
	
	function popupRemessa( remid )
	{
		var url = 'geral/popupRemessa.php?remid=' + remid;
		window.open( url, 'Combopopup', "height=500,width=700,top=50,left=200,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1" );
	}
	
</script>











