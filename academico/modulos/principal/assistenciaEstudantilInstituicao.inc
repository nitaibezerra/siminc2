<?php

// instancia o objeto do módulo
$academico = new academico();

// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

// Monta as abas
if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : '57135'; //Dados Desenv
}else{
	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : '57135';
}
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Assistência Estudantil", "");

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);

$entid = $_SESSION['academico']['entid'];
$orgid = $_SESSION['academico']['orgid'];

/*
 * VALIDANDO SE EXISTE O ENTID
 * FEITO POR ALEXANDRE DOURADO 16/11/2009
 */
if(!$entid) {
	echo "<script>
			alert('Problemas nas variáveis globais. Acesse novamente');
			window.location='academico.php?modulo=inicio&acao=C';
		  </script>";
	exit;
}

$academico->listaCampusAssistenciaEstudantil( $entid, $orgid ); 

?>

<script>
	function abreAssistenciaEstudantil( entid ){
		return window.location = '?modulo=principal/assistenciaEstudantil&acao=A&entid=' + entid;
	}
</script> 