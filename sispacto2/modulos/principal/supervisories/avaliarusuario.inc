<?
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

$sql_avaliacao = sqlAvaliacaoSupervisor(array("uncid"=>$_SESSION['sispacto2']['supervisories']['uncid'],"iusd"=>$_SESSION['sispacto2']['supervisories']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto2']['supervisories']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_SUPERVISORIES;
$sis 	  		= "supervisories";

include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>