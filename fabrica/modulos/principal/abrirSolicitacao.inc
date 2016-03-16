<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once APPRAIZ ."includes/workflow.php";

if( $_REQUEST['scsid'] ) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid'];
}

if($_REQUEST['ansid']) {
	$_SESSION['fabrica_var']['ansid'] = $_REQUEST['ansid']; 
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$scsId      = $_SESSION['fabrica_var']['scsid'];
$estado_wf  = wf_pegarEstadoAtual($db->pegaUm("SELECT docid FROM fabrica.solicitacaoservico WHERE scsid='". $scsId ."'"));


if( $estado_wf['esdid'] ==  WF_ESTADO_ELABORACAO)
{
    include_once APPRAIZ . 'fabrica/modulos/principal/cadDemanda.inc';
} 
elseif($estado_wf['esdid'] == WF_ESTADO_PRE_ANALISE )
{
    include_once APPRAIZ . 'fabrica/modulos/principal/preAnaliseDemanda.inc';
}
elseif($estado_wf['esdid'] == WF_ESTADO_ANALISE )
{
    //include_once APPRAIZ . 'fabrica/modulos/principal/analiseDemanda.inc';
    include_once APPRAIZ . 'fabrica/modulos/principal/preAnaliseDemanda.inc';
}
elseif( $estado_wf['esdid'] == WF_ESTADO_DETALHAMENTO || $estado_wf['esdid'] == WF_ESTADO_SS_PAUSA )
{
    include_once APPRAIZ . 'fabrica/modulos/principal/cadDetalhamento.inc';
}
elseif( $estado_wf['esdid'] == WF_ESTADO_AVALIACAO || $estado_wf['esdid'] == WF_ESTADO_APROVACAO )
{
    include_once APPRAIZ . 'fabrica/modulos/principal/cadAvaliacaoAprovacao.inc';
}
elseif( $estado_wf['esdid'] == WF_ESTADO_EXECUCAO )
{
    include_once APPRAIZ . 'fabrica/modulos/principal/cadExecucao.inc';
}
elseif( $estado_wf['esdid'] == WF_ESTADO_FINALIZADA || $estado_wf['esdid'] == WF_ESTADO_AGUARDANDO_PAGAMENTO || $estado_wf['esdid'] == WF_ESTADO_CANCELADA_SEM_CUSTO || $estado_wf['esdid'] == WF_ESTADO_CANCELADA_COM_CUSTO )
{
    include_once APPRAIZ . 'fabrica/modulos/principal/cadFinalizada.inc';
}
else
{
    echo 'SS em estado indefinido. Entre em contato com o desenvolvedor do sistema';
}