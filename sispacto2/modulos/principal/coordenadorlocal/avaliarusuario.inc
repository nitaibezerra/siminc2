<?
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

// limpando var (Em algumas situações)
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

$sql_avaliacao = sqlAvaliacaoCoordenadorLocal(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"fpbid"=>$_REQUEST['fpbid'],"iusd"=>$_SESSION['sispacto2']['coordenadorlocal']['iusd']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}

$pflcod_avaliador = PFL_COORDENADORLOCAL;
$sis 	  		= "coordenadorlocal";

include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>