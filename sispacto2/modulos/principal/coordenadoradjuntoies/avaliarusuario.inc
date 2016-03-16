<?
$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);

$sql_avaliacao = sqlAvaliacaoCoordenadorAdjuntoIES(array("uncid"=>$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid'],"iusd"=>$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_COORDENADORADJUNTOIES;
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
if($_REQUEST['fpbid']) $goto_sal = "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
else $goto_sal = false;
$goto_pro 		= false;


include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>