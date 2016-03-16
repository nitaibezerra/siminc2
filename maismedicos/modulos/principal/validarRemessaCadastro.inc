<?php
/*ini_set("upload_max_filesize", "100M");
ini_set("post_max_size", "100M");
ini_set("memory_limit", "100");
*/
if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

if($_GET['arquivoremessa'] && $_GET['rmcid'])
{
	$n = new RemessaCabecalho($_GET['rmcid']);
	$n->gerarArquivoRemessaCadastro($_GET['rmcid']);
	exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Validar Remessa de Cadastro", "Anexe o arquivo de retorno.");

$arrPerfil = pegaPerfilGeral();
if(in_array(PERFIL_CONSULTA,$arrPerfil)){
	$habilitado = "N";
}else{
	$habilitado = "S";
}
?>
<style>
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.center{text-align:center}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data" >
<input type="hidden" id="requisicao" name="requisicao" value="" />
<input type="hidden" id="classe" name="classe" value="RemessaCabecalho" />
<?php if($habilitado == "S"): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%"  >
		<tr>
			<td width="25%" class="SubTituloDireita">Arquivo de Retorno da Remessa de Cadastro</td>
		    <td><input type="file" name="arquivoremesssa" id="arquivoremesssa" /></td>
		</tr>
		<tr>
			<td colspan="2" align="center" >
				<input type="button" name="btn_validar" value="Validar" onclick="validarRemessaCadastro()" />
			</td>
		</tr>
	</table>
<?php endif; ?>
</form>
<script>
function validarRemessaCadastro(rmcid)
{
	var erro = 0;
	if(!$("#arquivoremesssa").val()){
		erro = 1;
		alert('Favor anexar o retorno da Remessa de Cadastro.');
		return false;
	}
	if(erro == 0){
		$("#classe").val("RemessaCabecalho");
		$("#requisicao").val("validarRemessaCadastro");
		$("#formulario").submit();
	}
}

$(function() {
	<?php if($_SESSION['maismedicos']['remessa']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['remessa']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['remessa']['alert']) ?>
	<?php endif; ?>
});
</script>