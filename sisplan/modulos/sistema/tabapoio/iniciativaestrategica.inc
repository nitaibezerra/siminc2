<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_fonte':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.iniciativaestrategica 
								(itesigla, itedsc, itestatus, dfeid) 
							VALUES 
								('%s', '%s', '%s', %s)",
							$_REQUEST['itesigla'],
							$_REQUEST['itedsc'],
							$_REQUEST['itestatus'],
							$_REQUEST['dfeid']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.iniciativaestrategica SET
								itesigla = '%s', 
								itedsc = '%s',
								itestatus = '%s',
								dfeid = %s
							WHERE
								iteid = %s",
							$_REQUEST['itesigla'],
							$_REQUEST['itedsc'],
							$_REQUEST['itestatus'],
							$_REQUEST['dfeid'],
							$_REQUEST['iteid']);
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
							sisplan.iniciativaestrategica 
						WHERE 
							iteid = %s", 
						$_REQUEST['iteid']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_fonte':
		$sql = "SELECT 
					iteid, 
					itesigla,
					itedsc,
					itestatus,
					dfeid 
				FROM 
					sisplan.iniciativaestrategica 
				WHERE
					iteid = '{$_REQUEST['iteid']}'";
		
		$dados = $db->carregar($sql);
				
		$iteid     = $dados[0]['iteid'];
		$itesigla  = $dados[0]['itesigla'];
		$itedsc    = $dados[0]['itedsc'];
		$itestatus = $dados[0]['itestatus'];
		$dfeid     = $dados[0]['dfeid'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Iniciativa Estratégica', 'Tabelas de Apoio - PPA' );
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
	
		if ( document.controle.dfeid.value == '' ) 
		{
			mensagem += '\nDesafio Estratégico';
			validacao = false;
		}
		
		if ( document.controle.itedsc.value == '' ) 
		{
			mensagem += '\nIniciativa Estratégica';
			validacao = false;
		}
		
		if ( document.controle.itestatus.value == '' ) 
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
	
	
	function excluir_controle( iteid )
	{
		if ( confirm( 'Deseja excluir a Iniciativa Estratégica?' ) ) 
		{
			document.controle.iteid.value = iteid;
			document.controle.evento.value = 'excluir_fonte';
			document.controle.submit();
		}
	}
	
	function editar_controle ( iteid )
	{
		document.controle.iteid.value = iteid;
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
					<input type="hidden" name="iteid" value="<?=$iteid?>"/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Desafio Estratégico:</td>
							<td>
								<? 
								$sql = "SELECT dfeid AS codigo, dfedsc AS descricao FROM sisplan.desafioestrategico WHERE dfestatus = 'A' ORDER BY 2";
								$db->monta_combo("dfeid",$sql,'S','Selecione...','','','','','S','dfeid');
								?>				
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Sigla:</td>
							<td>
								<?= campo_texto( 'itesigla', 'N', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Iniciativa Estratégica:</td>
							<td>
								<?= campo_texto( 'itedsc', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="itestatus" value="A" <?= $itestatus == 'A' || $itestatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="itestatus" value="I" <?= $itestatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$cabecalho = array( 'Ações', 'Desafio Estratégico', 'Sigla', 'Iniciativa Estratégica', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Programa\" onclick=\"editar_controle(' || '\'' || ite.iteid || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Programa\" onclick=\"excluir_controle(' || '\'' || ite.iteid || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_fonte' )
{
	$where = "";

	if ( !empty($_REQUEST['itesigla']) )
		$where .= "AND ite.itesigla = '{$_REQUEST['itesigla']}'";
		
	if ( !empty($_REQUEST['itedsc']) )
		$where .= "AND ite.itedsc ILIKE '%{$_REQUEST['itedsc']}%'";
		
	if ( !empty($_REQUEST['dfeid']) )
		$where .= "AND ite.dfeid = '{$_REQUEST['dfeid']}'";
		
	if ( !empty($_REQUEST['itestatus']) )
		$where .= "AND ite.itestatus = '{$_REQUEST['itestatus']}'";
}
					 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 	
			dfe.dfedsc,
			ite.itesigla, 
			ite.itedsc,
			CASE  
				WHEN ite.itestatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as itestatus
		FROM 
			sisplan.iniciativaestrategica ite
		JOIN sisplan.desafioestrategico dfe
			ON dfe.dfeid = ite.dfeid
		WHERE 
			1=1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>