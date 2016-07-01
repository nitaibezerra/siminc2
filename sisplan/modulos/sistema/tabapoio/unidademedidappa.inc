<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_unidade':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.unidademedida 
								(unmcod, unmnome, unmabrev, unmstatus) 
							VALUES 
								(%d, '%s', '%s', '%s')",
							$_REQUEST['unmcod'],
							$_REQUEST['unmnome'],
							$_REQUEST['unmabrev'],
							$_REQUEST['unmstatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.unidademedida SET
								unmnome = '%s', 
								unmabrev = '%s', 
								unmstatus = '%s'
							WHERE
								unmcod = '%s'",
							$_REQUEST['unmnome'],
							$_REQUEST['unmabrev'],
							$_REQUEST['unmstatus'],
							$_REQUEST['unmcod']);
		}
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		$db->sucesso( $_REQUEST['modulo'], $parametros );

		break;
		
	case 'excluir_unidade':
		
		$sql = sprintf("DELETE FROM 
							sisplan.unidademedida 
						WHERE 
							unmcod = '%s'", 
						$_REQUEST['codigo']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_unidade':
		$sql = "SELECT 
					unmcod, 
					unmnome, 
					unmabrev, 
					unmstatus 
				FROM 
					sisplan.unidademedida 
				WHERE
					unmcod = '{$_REQUEST['codigo']}'";
		
		$dados = $db->carregar($sql);

		$unmcod	   = $dados[0]['unmcod'];
		$unmnome   = $dados[0]['unmnome'];
		$unmabrev  = $dados[0]['unmabrev'];
		$unmstatus = $dados[0]['unmstatus'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Unidade de Medida' );
?>


<script language="javascript" type="text/javascript">
	
	function cadastrar_controle()
	{
		if ( validar_formulario_controle() ) 
		{
			document.controle.evento.value = 'cadastrar_unidade';
			document.controle.submit();
		}
	}
	
	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		if ( document.controle.unmcod.value == '' ) 
		{
			mensagem += '\nCódigo';
			validacao = false;
		}
		
		if ( document.controle.unmnome.value == '' ) 
		{
			mensagem += '\nNome';
			validacao = false;
		}

		if ( document.controle.unmstatus.value == '' ) 
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
		document.controle.evento.value = 'pesquisar_unidade';
		document.controle.submit();
	}
	
	
	function limpar_controle () 
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=A"?>';
	}
	
	
	function excluir_controle( unmcod )
	{
		if ( confirm( 'Deseja excluir a Unidade de Medida?' ) ) 
		{
			document.controle.codigo.value = unmcod;
			document.controle.evento.value = 'excluir_unidade';
			document.controle.submit();
		}
	}
	
	function editar_controle ( unmcod )
	{
		document.controle.codigo.value = unmcod;
		document.controle.evento.value = 'editar_unidade';
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
					<input type="hidden" name="codigo" value=""/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Código:</td>
							<td>
								<?= campo_texto( 'unmcod', 'S', $unmcod == '' ? '' : 'N', '', 8, 4, '####', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome:</td>
							<td>
								<?= campo_texto( 'unmnome', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Abreviatura:</td>
							<td>
								<?= campo_texto( 'unmabrev', 'S', 'S', '', 40, 10, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="unmstatus" value="A" <?= $unmstatus == 'A' || $unmstatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="unmstatus" value="I" <?= $unmstatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$cabecalho = array( 'Ações', 'Código', 'Nome', 'Abreviatura', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Unidade de Medida\" onclick=\"editar_controle(' || '\'' || unmcod || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Unidade de Medida\" onclick=\"excluir_controle(' || '\'' || unmcod || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_unidade' )
{
	$where = "";
	if ( !empty($_REQUEST['unmcod']) )
		$where .= "AND unmcod = '{$_REQUEST['unmcod']}'";

	if ( !empty($_REQUEST['unmnome']) )
		$where .= "AND unmnome ILIKE '%{$_REQUEST['unmnome']}%'";	
		
	if ( !empty($_REQUEST['unmabrev']) )
		$where .= "AND unmabrev ILIKE '%{$_REQUEST['unmabrev']}%'";
		
	if ( !empty($_REQUEST['unmstatus']) )
		$where .= "AND unmstatus = '{$_REQUEST['unmstatus']}'";
}
					 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 	
			unmcod, 
			unmnome, 
			unmabrev, 
			CASE  
				WHEN unmstatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as unmstatus
		FROM 
			sisplan.unidademedida 
		WHERE 
			1 = 1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>