<?php

//include APPRAIZ . 'includes/cabecalho.inc';
//print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

if ( $_REQUEST["formulario"] ) {
	try {
		$sql = sprintf( "update cte.proposicaoacao set ppadsc = '%s' where ppaid = %d", $_REQUEST["ppadsc"], $_REQUEST["ppaid"] );
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Erro ao editar guia." );
		}
		$sql = sprintf( "update cte.acaoindicador set acidsc = '%s' where ppaid = %d", $_REQUEST["ppadsc"], $_REQUEST["ppaid"] );
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Erro ao editar ação." );
		}
		$db->commit();
		$db->sucesso( "principal/configuracao/guia", "&acao=A" );
		exit();
	} catch ( Exception $erro ) {
		$db->rollback();
		?>
		<html>
			<head>
				<script type="text/javascript">
				alert( '<?= $erro->getMessage(); ?>' );
				location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>";
				</script>
			</head>
			<body/>
		</html>
		<?
	}
}

$sql = sprintf(
	"select
		it.itrdsc,
		d.dimcod, d.dimdsc,
		a.ardcod, a.arddsc,
		i.indcod, i.inddsc,
		c.ctrord, c.crtdsc,
		pa.ppadsc
	from cte.proposicaoacao pa
	inner join cte.criterio c on c.crtid = pa.crtid
	inner join cte.indicador i on i.indid = c.indid
	inner join cte.areadimensao a on a.ardid = i.ardid
	inner join cte.dimensao d on d.dimid = a.dimid
	inner join cte.instrumento it on it.itrid = d.itrid
	where pa.ppaid = %d",
	$_REQUEST["ppaid"]
);
$acao = $db->pegaLinha( $sql );

?>
<form method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="ppaid" value="<?= $_REQUEST["ppaid"] ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col style="width: 200px;"/>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td align='right' class="SubTituloDireita">Instrumento:</td>
				<td><?= $acao['itrdsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Dimensão:</td>
				<td><b><?= $acao['dimcod'] ?></b> <?= $acao['dimdsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Área:</td>
				<td><b><?= $acao['ardcod'] ?></b> <?= $acao['arddsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Indicador:</td>
				<td><b><?= $acao['indcod'] ?></b> <?= $acao['inddsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Critério:</td>
				<td><b><?= $acao['ctrord'] ?></b> <?= $acao['crtdsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Ação:</td>
				<td>
					<?php
					$ppadsc = $_REQUEST["ppadsc"] ? $_REQUEST["ppadsc"] : $acao["ppadsc"];
					echo campo_texto( 'ppadsc', 'S', 'S', '', 100, 200, '', '' );
					?>
				</td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td width="15%">&nbsp;</td>
				<td>
					<input type="button" class="botao" name="btalterar" value="Enviar" onclick="enviarFormulario();">
					<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="voltar();">
				</td>
			</tr>
		</tbody>
	</table>
</form>

<script type="text/javascript">
	
	function enviarFormulario(){
		if ( document.formulario.ppadsc.value == '' ) {
			alert( 'É necessário informar o nome da ação.' );
			return false;
		}
		document.formulario.submit();
	}
	
	function voltar(){
		window.location = '?modulo=principal/configuracao/guia&acao=A';
	}
	
</script>