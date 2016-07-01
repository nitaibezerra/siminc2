<?
// limpando var (Em algumas situações)
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

$sql_avaliacao = sqlAvaliacaoCoordenadorLocal(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'],"fpbid"=>$_REQUEST['fpbid'],"iusd"=>$_SESSION['sispacto']['coordenadorlocal']['iusd']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_COORDENADORLOCAL;
$sis 	  		= "coordenadorlocal";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=gerenciarusuario";
if($_REQUEST['fpbid']) $goto_sal = "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
else $goto_sal = false;
$goto_pro 		= false;


include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>