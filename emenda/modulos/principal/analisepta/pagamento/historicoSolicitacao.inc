<?php
# Ajax
$obPagamento = new PagamentoFNDE();
if($_POST['execFinan']){
	header('content-type: text/html; charset=ISO-8859-1');
	echo $obPagamento->carregaSituacaoHistorico($_POST['orbid']);
	die;
}
# Ajax -----

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];
$orbid = $_REQUEST['orbid'];

if ( !$ptrid ){
	echo '<script>
			alert("A sessão do Plano de Trabalho foi perdida!");
			window.location = "emenda.php?modulo=principal/analiseDadosPTA&acao=A";
		  </script>';
	die;
}

// Cria o título da tela
monta_titulo( 'Tela historico Pagamento', "");

echo cabecalhoPlanoTrabalho($ptrid);
$obPagamento->setPta( $ptrid );
$arDados = $obPagamento->carregaOrdemBancariaPagamento( $orbid );
include_once APPEMENDA.'modulos/principal/analisepta/pagamento/views/formHistorico.inc';
?>