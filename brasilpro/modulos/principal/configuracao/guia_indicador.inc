<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

if ( $_REQUEST["formulario"] ) {
	try {
		$sql = sprintf( "delete from cte.indicadorunidademedida where indid = %d", $_REQUEST["indid"] );
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Erro eu excluir unidades de medida." );
		}
		$sql = sprintf(
			"update cte.indicador set indqtdporescola = '%s' where indid = %d",
			$_REQUEST['indqtdporescola'],
			$_REQUEST["indid"]
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Erro atualizar dados do indicador." );
		}
		foreach ( (array) $_REQUEST['undid'] as $undid ) {
			if ( !$undid ) {
				continue;
			}
			$sql = sprintf(
				"insert into cte.indicadorunidademedida ( indid, undid ) values ( %d, %d )",
				$_REQUEST['indid'],
				$undid
			);
			if ( !$db->executar( $sql ) ) {
				throw new Exception( "Erro ao inserir unidades de medida." );
			}
		}
		$db->commit();
		$db->sucesso( $_REQUEST['modulo'], "&acao=A&indid={$_REQUEST['indid']}" );
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
		it.itrid, it.itrdsc,
		d.dimcod, d.dimdsc,
		a.ardcod, a.arddsc,
		i.*
	from cte.indicador i
	inner join cte.areadimensao a on a.ardid = i.ardid
	inner join cte.dimensao d on d.dimid = a.dimid
	inner join cte.instrumento it on it.itrid = d.itrid 
	where i.indid = %d
	",
	$_REQUEST["indid"]
);
$indicador = $db->pegaLinha( $sql );

?>
<form method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="indid" value="<?= $_REQUEST["indid"] ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col style="width: 200px;"/>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td align='right' class="SubTituloDireita">Instrumento:</td>
				<td><b><?= $indicador['itrdsc'] ?></b></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Dimensão:</td>
				<td><b><?= $indicador['dimcod'] ?></b> <?= $indicador['dimdsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Área:</td>
				<td><b><?= $indicador['ardcod'] ?></b> <?= $indicador['arddsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Indicador:</td>
				<td><b><?= $indicador['indcod'] ?></b> <?= $indicador['inddsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Quantitativo Físico</td>
				<td>
					<?php
					$indqtdporescola = $_REQUEST['indqtdporescola'] ? $_REQUEST['indqtdporescola'] : $indicador['indqtdporescola'];
					?>
					<label style="cursor: pointer;">
						<input type="radio" name="indqtdporescola" value="f" <?= $indqtdporescola == 'f' ? 'checked="checked"' : '' ?>/>
						Global
					</label>
					<br/>
					<label style="cursor: pointer;">
						<input type="radio" name="indqtdporescola" value="t" <?= $indqtdporescola == 't' ? 'checked="checked"' : '' ?>/>
						Por Escola
					</label>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Unidades de Medida:</td>
				<td>
				<?php
				# identifica os valores selecionados anteriormente
				$sql = sprintf(
					"select u.undid as codigo, u.unddsc as descricao
					from cte.indicadorunidademedida iu
					inner join cte.unidademedida u on u.undid = iu.undid
					where iu.indid = %d",
					$_REQUEST['indid']
				);
				$undid = $db->carregar( $sql );
				# captura os valores possíveis
				$undtipo = $indicador['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? 'E' : 'M';
				$sql = sprintf(
					"select undid as codigo, unddsc as descricao
					from cte.unidademedida
					where undtipo = '%s'
					order by unddsc",
					$undtipo
				);
				# exibe campo
				combo_popup( "undid", $sql, "Unidade de Medida", "400x400", 0, array(), "", "S", false, false, 5, 400 );
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
		selectAllOptions( document.getElementById( 'undid' ) );
		document.formulario.submit();
	}
	
	function voltar(){
		window.location = '?modulo=principal/configuracao/guia&acao=A';
	}
	
	function selectAllOptions(obj) {
		for (var i=0; i<obj.options.length; i++) {
			obj.options[i].selected = true;
		}
	}
	
</script>