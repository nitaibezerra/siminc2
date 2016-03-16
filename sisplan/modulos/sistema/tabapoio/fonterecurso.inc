<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_fonte':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.fonterecurso 
								(fondsc, fonstatus) 
							VALUES 
								('%s', '%s')",
							$_REQUEST['fondsc'],
							$_REQUEST['fonstatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.fonterecurso SET
								fondsc = '%s', 
								fonstatus = '%s'
							WHERE
								fonid = %s",
							$_REQUEST['fondsc'],
							$_REQUEST['fonstatus'],
							$_REQUEST['fonid']);
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
							sisplan.fonterecurso 
						WHERE 
							fonid = '%s'", 
						$_REQUEST['fonid']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_fonte':
		$sql = "SELECT 
					fonid, 
					fondsc, 
					fonstatus 
				FROM 
					sisplan.fonterecurso 
				WHERE
					fonid = '{$_REQUEST['fonid']}'";
		
		$dados = $db->carregar($sql);
				
		$fondsc    = $dados[0]['fondsc'];
		$fonstatus = $dados[0]['fonstatus'];
		$fonid     = $dados[0]['fonid'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Fonte de Recurso' );
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
		
		if ( document.controle.fondsc.value == '' ) 
		{
			mensagem += '\nDescrição';
			validacao = false;
		}
		
		if ( document.controle.fonstatus.value == '' ) 
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
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=A"?>';
	}
	
	
	function excluir_controle( fonid )
	{
		if ( confirm( 'Deseja excluir o Programa?' ) ) 
		{
			document.controle.fonid.value = fonid;
			document.controle.evento.value = 'excluir_fonte';
			document.controle.submit();
		}
	}
	
	function editar_controle ( fonid )
	{
		document.controle.fonid.value = fonid;
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
					<input type="hidden" name="fonid" value="<?=$fonid?>"/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome:</td>
							<td>
								<?= campo_texto( 'fondsc', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="fonstatus" value="A" <?= $fonstatus == 'A' || $fonstatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="fonstatus" value="I" <?= $fonstatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$cabecalho = array( 'Ações', 'Descrição', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Programa\" onclick=\"editar_controle(' || '\'' || fonid || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Programa\" onclick=\"excluir_controle(' || '\'' || fonid || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_fonte' )
{
	$where = "";

	if ( !empty($_REQUEST['fondsc']) )
		$where .= "AND fondsc ILIKE '%{$_REQUEST['fondsc']}%'";
		
	if ( !empty($_REQUEST['fonstatus']) )
		$where .= "AND fonstatus = '{$_REQUEST['fonstatus']}'";
}
					 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 	
			fondsc, 
			CASE  
				WHEN fonstatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as fonstatus
		FROM 
			sisplan.fonterecurso 
		WHERE 
			1=1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>