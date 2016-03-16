<?php 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

if($_REQUEST['entcodent']){
	$_SESSION['proinfantil']['mds']['entcodent'] = $_REQUEST['entcodent']; 
	$_SESSION['proinfantil']['mds']['mdsid'] 	 = recuperarMdsidPorEntcodent();
}

echo "<br/>";

$abacod_tela = 57559;
$arMnuid = array();

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Questionário', $linha2);
cabecalhoMds();

$qrpid = pegaQrpidMds( $_SESSION['proinfantil']['mds']['entcodent'] );

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $habilitado, 'descricao' => 'N') );

?>