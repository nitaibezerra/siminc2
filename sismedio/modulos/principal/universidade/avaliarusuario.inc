<?
$sql_avaliacao = sqlAvaliacaoCoordenadorIES(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid'],"iusd"=>$_SESSION['sismedio']['universidade']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

if($_REQUEST['fpbid']) $dados_mensario = criarMensario(array("iusd"=>$_SESSION['sismedio']['universidade']['iusd'],"fpbid"=>$_REQUEST['fpbid']));

$estado = wf_pegarEstadoAtual( $dados_mensario['docid'] );

if($estado['esdid'] != ESD_EM_ABERTO_MENSARIO) {
	$consulta = true;
}


$pflcod_avaliador = PFL_COORDENADORIES;
$sis 	  		= "universidade";
$goto_ant 		= "sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=gerenciarusuario";
if($_REQUEST['fpbid']) $goto_sal = "sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=avaliarusuario&fpbid=".$_REQUEST['fpbid'];
else $goto_sal = false;
$goto_pro 		= false;


include_once APPRAIZ_SISMEDIO."avaliarequipe.inc";

?>