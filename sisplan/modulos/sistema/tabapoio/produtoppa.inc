<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_produto':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.produto 
								(procod, pronome, prostatus) 
							VALUES 
								(%d, '%s', '%s')",
							$_REQUEST['procod'],
							$_REQUEST['pronome'],
							$_REQUEST['prostatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.produto SET
								pronome = '%s', 
								prostatus = '%s'
							WHERE
								procod = '%s'",
							$_REQUEST['pronome'],
							$_REQUEST['prostatus'],
							$_REQUEST['procod']);
		}
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'excluir_produto':
		
		$sql = sprintf("DELETE FROM 
							sisplan.produto 
						WHERE 
							procod = %d", 
						$_REQUEST['codigo']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_produto':
		$sql = "SELECT 
					procod, 
					pronome, 
					prostatus 
				FROM 
					sisplan.produto 
				WHERE
					procod = {$_REQUEST['codigo']}";
		
		$dados = $db->carregar($sql);

		$procod	   = $dados[0]['procod'];
		$pronome   = $dados[0]['pronome'];
		$prostatus = $dados[0]['prostatus'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Produto' );
?>


<script language="javascript" type="text/javascript">
	
	function cadastrar_controle()
	{
		if ( validar_formulario_controle() ) 
		{
			document.controle.evento.value = 'cadastrar_produto';
			document.controle.submit();
		}
	}
	
	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		if ( document.controle.procod.value == '' ) 
		{
			mensagem += '\nCódigo';
			validacao = false;
		}
		
		if ( document.controle.pronome.value == '' ) 
		{
			mensagem += '\nNome';
			validacao = false;
		}

		if ( document.controle.prostatus.value == '' ) 
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
		document.controle.evento.value = 'pesquisar_produto';
		document.controle.submit();
	}
	
	
	function limpar_controle () 
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=A"?>';
	}
	
	
	function excluir_controle( loccod )
	{
		if ( confirm( 'Deseja excluir o Produto?' ) ) 
		{
			document.controle.codigo.value = loccod;
			document.controle.evento.value = 'excluir_produto';
			document.controle.submit();
		}
	}
	
	function editar_controle ( loccod )
	{
		document.controle.codigo.value = loccod;
		document.controle.evento.value = 'editar_produto';
		document.controle.submit();
	}

	
	function numero(e)
	{
		alert(e.keyCode)
		var navegador = /msie/i.test(navigator.userAgent);
		if (navegador)
			var  tecla = event.keyCode;
		else
			var tecla = e.which;
		
		if(tecla > 47 && tecla < 58) // numeros de 0 a 9
			return true;
		else
		{
			if (tecla != 8) // backspace
				return false;
			else
				return true;
		}
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
								<?= campo_texto( 'procod', 'S', $procod == '' ? '' : 'N', '', 8, 4, '####', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome:</td>
							<td>
								<?= campo_texto( 'pronome', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="prostatus" value="A" <?= $prostatus == 'A' || $prostatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="prostatus" value="I" <?= $prostatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Produto\" onclick=\"editar_controle(' || '\'' || procod || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Produto\" onclick=\"excluir_controle(' || '\'' || procod || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_produto' )
{
	$where = "";
	if ( !empty($_REQUEST['procod']) )
		$where .= "AND procod = '{$_REQUEST['procod']}'";

	if ( !empty($_REQUEST['pronome']) )
		$where .= "AND pronome ILIKE '%{$_REQUEST['pronome']}%'";
		
	if ( !empty($_REQUEST['prostatus']) )
		$where .= "AND prostatus = '{$_REQUEST['prostatus']}'";
}
					 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 	
			procod, 
			pronome, 
			CASE  
				WHEN prostatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as prostatus
		FROM 
			sisplan.produto 
		WHERE 
			1 = 1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>