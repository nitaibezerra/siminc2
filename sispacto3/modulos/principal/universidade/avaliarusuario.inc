<?
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

if($_REQUEST['fpbid'] && !is_numeric($_REQUEST['fpbid'])) {

	$al = array("alert"=>"Período de referência inválido","location"=>"sispacto3.php?modulo=".$_REQUEST['modulo']."&acao=A".(($_REQUEST['aba'])?"&aba=".$_REQUEST['aba']:""));
	alertlocation($al);
	
}



$sql_avaliacao = sqlAvaliacaoCoordenadorIES(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],"iusd"=>$_SESSION['sispacto3']['universidade']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto3']['universidade']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_COORDENADORIES;
$sis 	  		= "universidade";


include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>