<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

if($_FILES['arquivo']) {
	
	include APPRAIZ ."includes/funcoes_public_arquivo.php";
	
	$resp = atualizarPublicArquivo();
	
	if($resp['TRUE']) $msg .= 'Foram processados '.count($resp['TRUE']).' arquivos.'.'\n';
	if($resp['FALSE']) {
		$msg .= 'Problemas encontrados:'.'\n';
		foreach($resp['FALSE'] as $k => $v) {
			$msg .= 'ARQID : '.$k.' | '.$v.'\n';
		}
	}
	
	die("<script>
			alert('".$msg."');
			window.location = window.location;
		 </script>");
}

include APPRAIZ ."includes/cabecalho.inc";
include "funcoes_brasilpro_arquivo.php";
echo '<br>';

$menu = carregarMenuBrasilPro();

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

?>
<style>
	.middle{vertical-align:middle;}
	.link{cursor:pointer;}
	.center{text-align:center;}
	.bold{font-weight:bold;}
</style>

<script type="text/javascript">

function limpaUpload(arqid)
{
	document.getElementById('arquivo_' + arqid).value = "";
}

function uploadArquivos()
{
	document.getElementById('btn_salvar').value="Carregando...";
	document.getElementById('btn_salvar').disabled = "true";
	document.getElementById('form_arquivo').submit();
}

</script>
<?php if(!$menu == array()): ?>
	<?php montaListaArquivosBrasilPro($menu[0]['tabela']); ?>
	<table cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" class="tabela">
		<tr>
			<td class="center" width="100%" ><input type="button" name="btn_salvar" id="btn_salvar" value="Salvar" onclick="uploadArquivos()"  /></td>
		</tr>
	</table>
<?php else: ?>
	<table cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" class="tabela">
		<tr>
			<td class="center bold" width="100%" >Não existe(m) arquivo(s) corrompido(s).</td>
		</tr>
	</table>
<?php endif; ?>