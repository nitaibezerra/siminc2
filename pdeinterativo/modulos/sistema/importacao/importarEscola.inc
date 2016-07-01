<?php
if($_REQUEST['download'] == 'S'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'pdeinterativo.php?modulo=sistema/importacao/importarEscola&acao=A';</script>";
    exit;
}

if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
jQuery.noConflict();

jQuery(function() {
	<?php if($_SESSION['pdeinterativo']['msg']): ?>
		alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
		<?php unset($_SESSION['pdeinterativo']['msg']) ?>
	<?php endif; ?>
});
	
function importarEscola()
{
	if(document.getElementById('arquivo').value){
		document.getElementById('btn_importar').value = "Importando...";
		document.getElementById('btn_importar').disabled = "disabled";
		document.getElementById('form_importacao').submit();
	}else{
		alert('Favor selecionar o arquivo!');
	}
}
</script>
<form name="form_importacao" id="form_importacao"  method="post" action="" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" value="importarEscola" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Arquivo (.csv):</td>
			<td><input type="file" name="arquivo" id="arquivo" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" ></td>
			<td>
				<input type="button" id="btn_importar" name="btn_importar" value="Importar Escolas" onclick="importarEscola()" />
			</td>
		</tr>
	</table>
</form>
<?php listaArquivosImportacao() ?>