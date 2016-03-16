<?php

if( $_REQUEST["obrid"] ){
	unset($_SESSION["obra"]);
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
	
}

// Realiza as rotinas da tela 
switch($_REQUEST['requisicao']) {
	case "inserirarquivo":
		$dir = 'documentosObras&acao=A';
		
		if( $_GET['arqid'] ){
			$sql = "UPDATE obras.arquivosobra SET
					  tpaid = ".$_POST['tpaid'].",
					  usucpf = '".$_SESSION['usucpf']."'
					WHERE arqid = ".$_POST['arqid'];
			$db->executar( $sql );
			
			$sql = "UPDATE public.arquivo SET
					  arqdescricao = '".$_POST['arqdescricao']."',
					  sisid = 15					 
					WHERE 
					  arqid = ".$_POST['arqid'];
			$db->executar( $sql );
			$db->commit();
			$db->sucesso("principal/documentosObras");
		} else {
			EnviarArquivoObras($_FILES, $_POST, $dir);
		}
		exit;
	break;
	case "download":
		DownloadArquivoObras( $_REQUEST );
	break;
	case "excluir":
		DeletarDocumentoObras( $_REQUEST );
	break;
}

$titulo_modulo = "Documentos da Obra";
monta_titulo( $titulo_modulo, '' );

if(!$_SESSION["obra"]["obrid"]) {
	die("<script>
			alert('Variavel de obra não encontrada');
			window.close();
		 </script>");
}

if( $_GET['arqid'] ){
	$sql = "SELECT ao.aqoid, ao.obrid, ao.tpaid, ao.arqid, a.arqdescricao, a.arqnome || '.'|| a.arqextensao as arquivo
				FROM obras.arquivosobra ao
					inner join public.arquivo a on a.arqid = ao.arqid
				WHERE
					ao.arqid = ".$_GET['arqid'];
	$arArquivo = $db->pegaLinha( $sql );
	extract($arArquivo);
}

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form method="post" id="anexo" name="anexo" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" value="inserirarquivo"/>
	<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<?if($_GET['arqid']){ ?>
					<input type="text" name="arqnome" disabled="disabled" id="arqnome" value="<?php echo $arquivo; ?>" />
				<?} else { ?>
					<input type="file" name="arquivo"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<?} ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			<td><?php
			
			$sql = "
				SELECT tpaid AS codigo, tpadesc AS descricao 
					FROM obras.tipoarquivo
			";
			
			$db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tpaid');
		?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="button" name="botao" value="Salvar" onclick="validaForm();";/>
				<input type="button" name="botao1" value="Fechar" onclick="javascript:window.close();";/>
			</td>
		</tr>
	</table>
</form>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
	<?	
		$caminho_atual = 'academico.php?modulo=principal/documentosObras&acao=A&acao=A';
		//if($habilitado){
			$permissaoBotaoExcluir = "'<center>
					<img src=\"../imagens/alterar.gif\" onClick=\"window.location.href=\'academico.php?modulo=principal/documentosObras&acao=A&arqid='|| arq.arqid ||' \'\" style=\"border:0; cursor:pointer;\" title=\"Alterar Descrição Anexo\">
					<a href=\"#\" onclick=\"javascript:ExcluirDocumento(\'" . $caminho_atual . "acao=A&requisicao=excluir" . "\',' || arq.arqid || ',' || aqb.aqoid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,";	
		/*}else{
			$permissaoBotaoExcluir = "'<center><img src=\"/imagens/excluir_01.gif\" title=\"Este Documento não pode ser excluído!\"></center>' as acao,";
		}	*/
		$sql = "SELECT
						{$permissaoBotaoExcluir}
						to_char(aqb.aqodtinclusao,'DD/MM/YYYY'),
						tarq.tpadesc,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
						arq.arqtamanho || ' kbs' as tamanho ,
						arq.arqdescricao,								
						usu.usunome
					FROM
						((public.arquivo arq INNER JOIN obras.arquivosobra aqb
						ON arq.arqid = aqb.arqid) INNER JOIN obras.tipoarquivo tarq
						ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
						ON usu.usucpf = aqb.usucpf
					WHERE
						aqb.aqostatus = 'A' AND	aqb.obrid = '" . $_SESSION["obra"]["obrid"] . "'
						AND (arqtipo <> 'image/jpeg' AND arqtipo <> 'image/png' AND arqtipo <> 'image/gif')";
		
		$cabecalho = array( "Ação", 
							"Data Inclusão",
							"Tipo Arquivo",
							"Nome Arquivo",
							"Tamanho (Mb)",
							"Descrição Arquivo",
							"Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<script type="text/javascript">
	/*var objForm = document.forms["anexo"];
	if(objForm.arqid.value != ''){
		objForm.arquivo.disabled = true;
	}*/
	function validaForm (){
		
		var tpaid = document.getElementById('tpaid');

		if ( tpaid.value == "" ){
			alert("Campo obrigatório.");
			tpaid.focus();
			return false;
		}

		document.getElementById('anexo').submit();
	}
	function ExcluirDocumento(url, arqid, aqoid){
		if(confirm("Deseja realmente excluir este documento ?")){
			window.location = url+'&aqoid='+aqoid+'&arqid='+arqid;
		}
	}
	DownloadArquivo = function(arqid){
		window.location = 'academico.php?modulo=principal/documentosObras&acao=A&requisicao=download&arqid='+arqid;
	}
</script>