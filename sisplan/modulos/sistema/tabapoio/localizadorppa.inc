<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_localizador':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.localizador 
								(loccod, locnome, locstatus) 
							VALUES 
								('%s', '%s', '%s')",
							$_REQUEST['loccod'],
							$_REQUEST['locnome'],
							$_REQUEST['locstatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.localizador SET
								locnome = '%s', 
								locstatus = '%s'
							WHERE
								loccod = '%s'",
							$_REQUEST['locnome'],
							$_REQUEST['locstatus'],
							$_REQUEST['loccod']);
		}
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		$db->sucesso( $_REQUEST['modulo'], $parametros );

		break;
		
	case 'excluir_localizador':
		
		$sql = sprintf("DELETE FROM 
							sisplan.localizador 
						WHERE 
							loccod = '%s'", 
						$_REQUEST['codigo']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_localizador':
		$sql = "SELECT 
					loccod, 
					locnome, 
					locstatus 
				FROM 
					sisplan.localizador 
				WHERE
					loccod = '{$_REQUEST['codigo']}'";
		
		$dados = $db->carregar($sql);

		$loccod	   = $dados[0]['loccod'];
		$locnome   = $dados[0]['locnome'];
		$locstatus = $dados[0]['locstatus'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Localizador' );
?>


<script language="javascript" type="text/javascript">
	
	function cadastrar_controle()
	{
		if ( validar_formulario_controle() ) 
		{
			document.controle.evento.value = 'cadastrar_localizador';
			document.controle.submit();
		}
	}
	
	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		if ( document.controle.loccod.value == '' ) 
		{
			mensagem += '\nCódigo';
			validacao = false;
		}
		
		if ( document.controle.locnome.value == '' ) 
		{
			mensagem += '\nNome';
			validacao = false;
		}

		if ( document.controle.locstatus.value == '' ) 
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
		document.controle.evento.value = 'pesquisar_localizador';
		document.controle.submit();
	}
	
	
	function limpar_controle () 
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=A"?>';
	}
	
	
	function excluir_controle( loccod )
	{
		if ( confirm( 'Deseja excluir o Localizador?' ) ) 
		{
			document.controle.codigo.value = loccod;
			document.controle.evento.value = 'excluir_localizador';
			document.controle.submit();
		}
	}
	
	function editar_controle ( loccod )
	{
		document.controle.codigo.value = loccod;
		document.controle.evento.value = 'editar_localizador';
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
								<?= campo_texto( 'loccod', 'S', $loccod == '' ? '' : 'N', '', 8, 4, '', '', '','',0, '', 'this.value=this.value.toUpperCase()' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome:</td>
							<td>
								<?= campo_texto( 'locnome', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="locstatus" value="A" <?= $locstatus == 'A' || $locstatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="locstatus" value="I" <?= $locstatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$cabecalho = array( 'Ações', 'Código', 'Nome', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Localizador\" onclick=\"editar_controle(' || '\'' || loccod || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Localizador\" onclick=\"excluir_controle(' || '\'' || loccod || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_localizador' )
{
	$where = "";
	if ( !empty($_REQUEST['loccod']) )
		$where .= "AND loccod = '{$_REQUEST['loccod']}'";

	if ( !empty($_REQUEST['locnome']) )
		$where .= "AND locnome ILIKE '%{$_REQUEST['locnome']}%'";
		
	if ( !empty($_REQUEST['locstatus']) )
		$where .= "AND locstatus = '{$_REQUEST['locstatus']}'";
}
					 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 	
			loccod, 
			locnome, 
			CASE  
				WHEN locstatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as locstatus
		FROM 
			sisplan.localizador 
		WHERE 
			1 = 1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>