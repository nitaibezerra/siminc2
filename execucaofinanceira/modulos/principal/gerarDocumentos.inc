<?php
// ------------- DECLARAÇÃO DE VARIAVEIS ------------- //
$empid	 	= $_REQUEST['empid'] 	? $_REQUEST['empid'] 	: 0;
if( $_REQUEST['proid'] ){
	$processo 	= $_REQUEST['proid'];
	$_SESSION['execucaofinanceira']['proid'] = $_REQUEST['proid']; 
} else {
	$processo = $_SESSION['execucaofinanceira']['proid'];
}
if( empty($processo) ) die("<script> alert('Erro ao tentar identificar o processo!'); window.location.href = 'execucaofinanceira.php?modulo=principal/listaDeProcessos&acao=A';</script>");

$obProcesso = new Processo();
$obProcesso->carregarPorId($processo);

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);

?>
<?php monta_titulo( $titulo_modulo, '' ); ?>
<? $obProcesso->montaCabecalhoEmpenho(); ?>