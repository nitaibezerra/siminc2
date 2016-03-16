<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_vinculo':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.vinculofuncional 
								(vifnome, vifstatus) 
							VALUES 
								('%s', '%s')",
							$_REQUEST['vifnome'],
							$_REQUEST['vifstatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.vinculofuncional SET
								vifnome = '%s', 
								vifstatus = '%s'
							WHERE
								vifid = '%s'",
							$_REQUEST['vifnome'],
							$_REQUEST['vifstatus'],
							$_REQUEST['codigo']);
		}
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		$db->sucesso( $_REQUEST['modulo'], $parametros );

		break;
		
	case 'excluir_vinculo':
		
		$sql = sprintf("UPDATE sisplan.vinculofuncional SET
							vifstatus = 'I'
						WHERE
							vifid = '%s'",
						$_REQUEST['codigo']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_vinculo':
		$sql = "SELECT 
					vifid,
					vifnome, 
					vifstatus 
				FROM 
					sisplan.vinculofuncional 
				WHERE
					vifid = '{$_REQUEST['codigo']}'";
		
		$dados = $db->carregar($sql);

		$vifid     = $dados[0]['vifid'];
		$vifnome   = $dados[0]['vifnome'];
		$vifstatus = $dados[0]['vifstatus'];
		$alterar   = 'S';
		break;
		
	case 'pesquisar_vinculo':
		$where = "";
		if ( !empty($_REQUEST['vifnome']) )
		{
			$where .= "AND vifnome ILIKE '%{$_REQUEST['vifnome']}%'";
			$vifnome = $_REQUEST['vifnome'];
		}
		
		if ( !empty($_REQUEST['vifstatus']) )
		{
			$where .= "AND vifstatus = '{$_REQUEST['vifstatus']}'";
			$vifstatus = $_REQUEST['vifstatus'];
		}
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Vínculo Funcional' );
?>

<script language="javascript" type="text/javascript">
	
	function cadastrar_controle()
	{
		if ( validar_formulario_controle() ) 
		{
			document.controle.evento.value = 'cadastrar_vinculo';
			document.controle.submit();
		}
	}
	
	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		if ( document.controle.vifnome.value == '' ) 
		{
			mensagem += '\nNome';
			validacao = false;
		}

		if ( document.controle.vifstatus.value == '' ) 
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
		document.controle.evento.value = 'pesquisar_vinculo';
		document.controle.submit();
	}
	
	
	function limpar_controle () 
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=A"?>';
	}
	
	
	function excluir_controle( vifid )
	{
		if ( confirm( 'Deseja excluir o Vínculo Funcional?' ) ) 
		{
			document.controle.codigo.value = vifid;
			document.controle.evento.value = 'excluir_vinculo';
			document.controle.submit();
		}
	}
	
	function editar_controle ( vifid )
	{
		document.controle.codigo.value = vifid;
		document.controle.evento.value = 'editar_vinculo';
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
					<input type="hidden" name="codigo" value="<?=$vifid?>"/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome:</td>
							<td>
								<?= campo_texto( 'vifnome', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="vifstatus" value="A" <?= $vifstatus == 'A' || $vifstatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="vifstatus" value="I" <?= $vifstatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$cabecalho = array( 'Ações', 'Nome', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Vínculo Funcional\" onclick=\"editar_controle(' || '\'' || vifid || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Vínculo Funcional\" onclick=\"excluir_controle(' || '\'' || vifid || '\'' || ')\" >";

				 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 	
			vifnome, 
			CASE  
				WHEN vifstatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as locstatus
		FROM 
			sisplan.vinculofuncional 
		WHERE 
			1 = 1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>