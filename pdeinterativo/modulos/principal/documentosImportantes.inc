<?php

function excluirArquivoImportante($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo.documentosimportantes SET dimstatus='I' WHERE dimid='".$dados['dimid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Arquivo excluido com sucesso');
			window.location='pdeinterativo.php?modulo=principal/documentosImportantes&acao=A';
		  </script>";
}

function gravarDocumentoImportante($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos = array();
	$file = new FilesSimec("documentosimportantes", $campos, "pdeinterativo");
	
	$arqdescricao = $dados['arqdescricao'];
	
	if( $file->setUpload($arqdescricao, "arquivo", false ) ) {
		$sql = "INSERT INTO pdeinterativo.documentosimportantes(arqid)
    			VALUES ('".$file->getIdArquivo()."');";
		$db->executar($sql);
		$db->commit();
		echo "<script>alert('Gravado com sucesso');window.location='pdeinterativo.php?modulo=principal/documentosImportantes&acao=A';</script>";
	} else {
		echo "<script>alert('Problemas para gravar o arquivo');window.location='pdeinterativo.php?modulo=principal/documentosImportantes&acao=A';</script>";
	}
	
}

if($_REQUEST['arqid']) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("documentoscomite", array(), "pdeinterativo");
	$file->getDownloadArquivo($_REQUEST['arqid']);
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}




include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	
function gravarDocumentoImportante() {
	jQuery('#formarquivo').submit();
}

function donwload(arqid) {
	window.location='pdeinterativo.php?modulo=principal/documentosImportantes&acao=A&arqid='+arqid;
}

function excluirArquivoImportante(dimid) {
	var conf = confirm('Deseja realmente excluir o arquivo?');
	if(conf) {
		window.location='pdeinterativo.php?modulo=principal/documentosImportantes&acao=A&requisicao=excluirArquivoImportante&dimid='+dimid;
	}
}
	
</script>
<? $perfil = pegaPerfilGeral(); ?>
<? if($db->testa_superuser() || in_array(PDEINT_PERFIL_EQUIPE_MEC, $perfil)) : ?>
<form name="formarquivo" id="formarquivo" action="" enctype="multipart/form-data" method="post">
<input type="hidden" name="requisicao" value="gravarDocumentoImportante">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">Arquivo:</td>
	<td><input type="file" name="arquivo" id="arquivo"></td>
</tr>
<tr>
	<td class="SubTituloDireita">Descrição:</td>
	<td><? echo campo_textarea( 'arqdescricao', 'S', 'S', '', '70', '4', '255'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="gravar" value="Gravar" onclick="gravarDocumentoImportante();"></td>
</tr>
</table>
</form>
<? $removerimg = true; ?>
<? endif; ?>
<?

$sql = "SELECT '<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=donwload('||arq.arqid||');>".(($removerimg)?" <img src=../imagens/exclui_p.gif border=0 style=cursor:pointer onclick=\"excluirArquivoImportante(\''||dim.dimid||'\')\">":"")."</center>' as acao, arq.arqnome||'.'||arq.arqextensao as nomearq, arq.arqdescricao, to_char(arq.arqdata, 'dd/mm/YYYY')||' '||arq.arqhora as data 
		FROM pdeinterativo.documentosimportantes dim 
		INNER JOIN public.arquivo arq ON arq.arqid = dim.arqid 
		WHERE dim.dimstatus='A'
		ORDER BY (arq.arqdata||' '||arq.arqhora)::timestamp";

$cabecalho = array("&nbsp;", "Arquivo", "Descrição", "Data");
$db->monta_lista($sql,$cabecalho,50,8,'N','center',$par2,"formulario");

?>