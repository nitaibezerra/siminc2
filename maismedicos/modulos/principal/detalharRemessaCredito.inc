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
?>
<?php if($_REQUEST['gerarxls']): ?>
	<?php
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_REMESSA_CREDITO_".$_GET['rmcid'].".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_REMESSA_CREDITO_".$_GET['rmcid'].".xls");
	header ( "Content-Description: MID Gera excel" );
	?>
<?php else: ?>
	<?php 
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
<?php endif; ?>
<?php 
$rmcid = $_GET['rmcid'];
$tutor = new RemessaCabecalho();
$tutor->listarDetalheRemessaCredito($rmcid);
?>
<?php if(empty($_REQUEST['gerarxls'])): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
		    <td colspan="2" align="center" >
		   		<input type="button" name="btn_voltar" value="Voltar" onclick="window.location='maismedicos.php?modulo=principal/listarRemessaCredito&acao=A'" />
		   		<input type="button" name="btn_xls" value="Gerar XLS" onclick="window.location='maismedicos.php?modulo=principal/detalharRemessaCredito&acao=A&rmcid=<?php echo $rmcid; ?>&gerarxls=true'" />
		   	</td>
		</tr>
	</table>
<?php endif; ?>