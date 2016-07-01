<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_fonte':

		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.desafioestrategico
								(dfesigla, dfedsc, dfestatus)
							VALUES
								('%s', '%s', '%s')",
							$_REQUEST['dfesigla'],
							$_REQUEST['dfedsc'],
							$_REQUEST['dfestatus']);
		}
		else
		{
			$sql = sprintf("UPDATE sisplan.desafioestrategico SET
								dfesigla = '%s',
								dfedsc = '%s',
								dfestatus = '%s'
							WHERE
								dfeid = %s",
							$_REQUEST['dfesigla'],
							$_REQUEST['dfedsc'],
							$_REQUEST['dfestatus'],
							$_REQUEST['dfeid']);
		}

		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );

		break;

	case 'excluir_fonte':

		$sql = sprintf("DELETE FROM
							sisplan.desafioestrategico
						WHERE
							dfeid = %s",
						$_REQUEST['dfeid']);

		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );

		break;

	case 'editar_fonte':
		$sql = "SELECT
					dfeid,
					dfesigla,
					dfedsc,
					dfestatus
				FROM
					sisplan.desafioestrategico
				WHERE
					dfeid = '{$_REQUEST['dfeid']}'";

		$dados = $db->carregar($sql);

		$dfeid     = $dados[0]['dfeid'];
		$dfesigla  = $dados[0]['dfesigla'];
		$dfedsc    = $dados[0]['dfedsc'];
		$dfestatus = $dados[0]['dfestatus'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Desafio Estratégico', 'Tabelas de Apoio - PPA' );
?>


<script language="javascript" type="text/javascript">

	function cadastrar_controle()
	{
		if ( validar_formulario_controle() )
		{
			document.controle.evento.value = 'cadastrar_fonte';
			document.controle.submit();
		}
	}

	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';

		if ( document.controle.dfedsc.value == '' )
		{
			mensagem += '\nDesafio Estratégico';
			validacao = false;
		}

		if ( document.controle.dfestatus.value == '' )
		{
			mensagem += '\nSituação';
			validacao = false;
		}

		if ( !validacao )
			alert( mensagem );

		return validacao;
	}

	function pesquisar_controle ()
	{
		document.controle.evento.value = 'pesquisar_fonte';
		document.controle.submit();
	}


	function limpar_controle ()
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=C"?>';
	}


	function excluir_controle( dfeid )
	{
		if ( confirm( 'Deseja excluir o Desafio Estratégico?' ) )
		{
			document.controle.dfeid.value = dfeid;
			document.controle.evento.value = 'excluir_fonte';
			document.controle.submit();
		}
	}

	function editar_controle ( dfeid )
	{
		document.controle.dfeid.value = dfeid;
		document.controle.evento.value = 'editar_fonte';
		document.controle.submit();
	}


	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}

	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}

	function trim( value ){
		return ltrim(rtrim(value));
	}

</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td>
			<!-- NOVO CONTROLE -->
				<form method="post" name="controle">
					<input type="hidden" name="evento" value=""/>
					<input type="hidden" name="dfeid" value="<?=$dfeid?>"/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Sigla:</td>
							<td>
								<?= campo_texto( 'dfesigla', 'N', 'S', '', 41, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Desafio Estratégico:</td>
							<td>
								<?= //campo_texto( 'dfedsc', 'S', 'S', '', 40, 400, '', '' );
									campo_textarea( 'dfedsc', 'S', 'S', '', 56, 5, 500 );
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="dfestatus" value="A" <?= $dfestatus == 'A' || $dfestatus == '' ? "checked='checked'" : ""; ?>  /> Ativo
								<input type="radio" id="I" name="dfestatus" value="I" <?= $dfestatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo

							</td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td>
							<input type="button" name="botao" value="Pesquisar" onclick="pesquisar_controle();"/>
							<input type="button" name="botao" value="Limpar" onclick="limpar_controle();"/>
							<input type="button" name="botao" value="Gravar" onclick="cadastrar_controle();"/>
							</td>
						</tr>
					</table>
				</form>
		</td>
	</tr>
</table>
<!-- LISTA -->
<?php

unset( $sql );

$cabecalho = array( 'Ações', 'Sigla', 'Desafio Estratégico', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Programa\" onclick=\"editar_controle(' || '\'' || dfe.dfeid || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Programa\" onclick=\"excluir_controle(' || '\'' || dfe.dfeid || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_fonte' )
{
	$where = "";

	if ( !empty($_REQUEST['dfesigla']) )
		$where .= "AND dfe.dfesigla = '{$_REQUEST['dfesigla']}'";

	if ( !empty($_REQUEST['dfedsc']) )
		$where .= "AND dfe.dfedsc ILIKE '%{$_REQUEST['dfedsc']}%'";

	if ( !empty($_REQUEST['dfestatus']) )
		$where .= "AND dfe.dfestatus = '{$_REQUEST['dfestatus']}'";
}

$sql = "SELECT
			'{$img}&nbsp;{$img_1}' AS acao,
			dfe.dfesigla,
			dfe.dfedsc,
			CASE
				WHEN dfe.dfestatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as itestatus
		FROM
			sisplan.desafioestrategico dfe
		WHERE
			1=1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>