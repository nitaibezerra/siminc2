<?php
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

if( empty($_SESSION["emenda"]["ptridAnalise"]) ){
	echo "<script>
			alert('Faltam dados na sessão sobre este PTA.');
			window.location.href = 'emenda.php?modulo=principal/listaPtaAnalise&acao=A';
		  </script>";
	die;
}

$boReformulacao = pegaPaiPTA( $_SESSION['emenda']['ptridAnalise'] );

$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION['emenda']['ptridAnalise'] );

if( $_GET['tipo'] == 'T' ){
	include "minutaTermoAditivo.inc";
} else {
	include "minutaConvenio.inc";
}

?>