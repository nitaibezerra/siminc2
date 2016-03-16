<?php

verifica_sessao();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo_cte( $titulo_modulo, '&nbsp;' );

$dimid = (integer) $_REQUEST['dimid'];
$restricao = $dimid ? " and d.dimid = {$dimid} " : "";
$sql = sprintf(
	"select p.*, r.*
	from cte.resposta r
	inner join cte.pergunta p on p.prgid = r.prgid
	inner join cte.dimensao d on d.dimid = p.dimid and d.dimstatus = 'A' and itrid = %d %s
	where r.inuid = %d",
	$_SESSION['itrid'],
	$restricao,
	$_SESSION['inuid']
);
$respostas = $db->carregar( $sql );

?>
<script type="text/javascript">
	function selecionarDimensao( dimid ){
		window.location = '?modulo=relatorio/questoes_pontuais&acao=A&dimid=' + dimid;
	}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<colgroup>
		<col style="width: 25%;"/>
		<col style="width: 75%;"/>
	</colgroup>
	<tbody>
		<tr>
			<td class="SubTituloDireita">Dimensão:</td>
			<td>
			<?php
			$sql = sprintf(
				"select d.dimid as codigo, d.dimdsc as descricao
				from cte.dimensao d
				where d.dimstatus = 'A' and d.itrid = %d
				and exists ( select p.prgid from cte.pergunta p where p.dimid = d.dimid )
				order by d.dimcod",
				$_SESSION['itrid']
			);
			$db->monta_combo( "dimid", $sql, 'S', ' - selecione - ', 'selecionarDimensao', '' );
			?>
			</td>
		</tr>
	</tbody>
</table>
<?php if( count( $respostas ) > 0 ): ?>
	<?php foreach( $respostas as $resposta ): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tbody>
			<tr>
				<td class="SubTituloEsquerda" style="padding: 10px;"><?= $resposta['prgdsc'] ?></td>
			</tr>
			<tr>
				<td style="padding: 20px;"><?= $resposta['rspdsc'] ?></td>
			</tr>
			</tbody>
	</table>
	<?php endforeach; ?>
<?php endif; ?>