<?php 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_novasturmas.php";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

echo "<br/>";

$abacod_tela = 57558;
$arMnuid = array();

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Questionário', $linha2);
cabecalhoTurma();

$qrpid = pegaQrpidNovasTurmas( $_SESSION['proinfantil']['turid'] );

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $habilitado, 'descricao' => 'N') );

?>