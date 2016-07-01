<?
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

$sql_avaliacao  = sqlAvaliacaoOrientador(array("iusd"=>$_SESSION['sispacto2']['orientadorestudo']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto2']['orientadorestudo']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}

$pflcod_avaliador = PFL_ORIENTADORESTUDO;
$sis 	  		  = "orientadorestudo";

include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>