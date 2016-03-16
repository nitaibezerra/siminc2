<?
$sql_avaliacao = sqlAvaliacaoSupervisor(array("picid"=>$_SESSION['sisindigena2']['supervisories']['picid'],"iusd"=>$_SESSION['sisindigena2']['supervisories']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sisindigena2']['supervisories']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_SUPERVISORIES;
$sis 	  		= "supervisories";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
if($_REQUEST['fpbid']) $goto_sal = "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
else $goto_sal = false;
$goto_pro 		= false;


include_once APPRAIZ_SISINDIGENA."avaliarequipe.inc";

?>