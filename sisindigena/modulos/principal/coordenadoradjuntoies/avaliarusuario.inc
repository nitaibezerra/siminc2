<?

$sql_avaliacao = sqlAvaliacaoCoordenadorAdjuntoIES(array("picid"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'],"iusd"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_COORDENADORADJUNTOIES;
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&aba=principal";
$goto_ant 		= "sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&aba=gerenciarusuario";
if($_REQUEST['fpbid']) $goto_sal = "sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
else $goto_sal = false;
$goto_pro 		= false;


include_once APPRAIZ_SISINDIGENA."avaliarequipe.inc";

?>