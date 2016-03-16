<?php
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
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
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Lista de Campus", "");

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
?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?$academico->academico_listacampus( $entid, $orgid ); ?>
</div>

<script>
	function abredadoscampus( entid ){
		return window.location = '?modulo=principal/alterarCampus&acao=A&entid=' + entid;
	}
</script> 