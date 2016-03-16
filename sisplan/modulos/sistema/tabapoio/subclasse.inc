<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_fonte':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.subclasse 
								(sbcdsc, sbcstatus, clsid) 
							VALUES 
								('%s', '%s', %s)",
							$_REQUEST['sbcdsc'],
							$_REQUEST['sbcstatus'],
							$_REQUEST['clsid']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.subclasse SET
								sbcdsc = '%s',
								sbcstatus = '%s',
								clsid = %s
							WHERE
								sbcid = %s",
							$_REQUEST['sbcdsc'],
							$_REQUEST['sbcstatus'],
							$_REQUEST['clsid'],
							$_REQUEST['sbcid']);
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
							sisplan.subclasse 
						WHERE 
							sbcid = %s", 
						$_REQUEST['sbcid']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_fonte':
		$sql = "SELECT 
					sbcid, 
					sbcdsc,
					sbcstatus,
					clsid
				FROM 
					sisplan.subclasse 
				WHERE
					sbcid = '{$_REQUEST['sbcid']}'";
		
		$dados = $db->carregar($sql);
				
		$sbcid     = $dados[0]['sbcid'];
		$sbcdsc    = $dados[0]['sbcdsc'];
		$sbcstatus = $dados[0]['sbcstatus'];
		$clsid	   = $dados[0]['clsid'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Sub-Classe', 'Tabelas de Apoio - PPA' );
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
		
		if ( document.controle.sbcdsc.value == '' ) 
		{
			mensagem += '\nDescrição';
			validacao = false;
		}
		
		if ( document.controle.sbcstatus.value == '' ) 
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
	
	
	function excluir_controle( sbcid )
	{
		if ( confirm( 'Deseja excluir o Desafio Estratégico?' ) ) 
		{
			document.controle.sbcid.value = sbcid;
			document.controle.evento.value = 'excluir_fonte';
			document.controle.submit();
		}
	}
	
	function editar_controle ( sbcid )
	{
		document.controle.sbcid.value = sbcid;
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
					<input type="hidden" name="sbcid" value="<?=$sbcid?>"/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Classe:</td>
							<td>
								<? 
								$sql = "SELECT clsid AS codigo, clsdsc AS descricao FROM sisplan.classe WHERE clsstatus = 'A' ORDER BY 2";
								$db->monta_combo("clsid",$sql,'S','Selecione...','','','','','S','clsid');
								?>				
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Sub-classe:</td>
							<td>
								<?= campo_texto( 'sbcdsc', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="sbcstatus" value="A" <?= $sbcstatus == 'A' || $sbcstatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="sbcstatus" value="I" <?= $sbcstatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
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

$cabecalho = array( 'Ações', 'Classe', 'Sub-Classe', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Programa\" onclick=\"editar_controle(' || '\'' || scl.sbcid || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Programa\" onclick=\"excluir_controle(' || '\'' || scl.sbcid || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_fonte' )
{
	$where = "";
		
	if ( !empty($_REQUEST['sbcdsc']) )
		$where .= "AND scl.sbcdsc ILIKE '%{$_REQUEST['sbcdsc']}%'";
		
	if ( !empty($_REQUEST['sbcstatus']) )
		$where .= "AND scl.sbcstatus = '{$_REQUEST['sbcstatus']}'";
}
					 
$sql = "SELECT 
			'{$img}&nbsp;{$img_1}' AS acao, 
			cls.clsdsc,
			scl.sbcdsc,
			CASE  
				WHEN scl.sbcstatus = 'I' THEN 'Inativo'
				ELSE 'Ativo'
			END as sbcstatus
		FROM 
			sisplan.subclasse scl
		JOIN sisplan.classe cls
			ON cls.clsid = scl.clsid
		WHERE 
			1=1
		{$where}";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>