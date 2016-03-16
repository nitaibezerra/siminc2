<?
$sql_avaliacao  = sqlAvaliacaoOrientador(array("iusd"=>$_SESSION['sispacto']['orientadorestudo']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sispacto']['orientadorestudo']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_ORIENTADORESTUDO;
$sis 	  		  = "orientadorestudo";
$goto_ant 		  = "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
if($_REQUEST['fpbid']) $goto_sal = "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
else $goto_sal    = false;

if(mostrarAbaAvaliacaoComplementar(array('abapai'=>'orientadorestudo'))) {
	$goto_pro = "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliacaocomplementar";
} else {
	$goto_pro = false;	
}


include_once APPRAIZ_SISPACTO."avaliarequipe.inc";

?>