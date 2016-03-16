<?php
if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Remessa de Cadastro", "&nbsp;");
?>
<style>
	.link{cursor:pointer}
	.bold{font-weight:bold}
	.center{text-align:center}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<?php 
$rmcid = $_GET['rmcid'];
$tutor = new RemessaCabecalho();
$tutor->listarDetalheRemessa($rmcid);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td colspan="2" align="center" >
   		<input type="button" name="btn_voltar" value="Voltar" onclick="window.location='maismedicos.php?modulo=principal/listarRemessaCadastro&acao=A'" />
   	</td>
</tr>
</table>