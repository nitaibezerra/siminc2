<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_constantes.php";
 
session_start();

$_SESSION['usucpf'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select
	        mun.estuf,
	        mun.muncod,
	        mun.mundescricao,
	        doc.docid,
	        (cast(now() as date) - cast(max(h.htddata) as date)) as dias,
	        dcm.cpmid
	    from 
	        territorios.municipio mun
	        inner join proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = mun.muncod and dcm.cpmano = (2012 - 1)
	        inner join workflow.documento doc on doc.docid = dcm.docid
	        inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
	        inner join proinfantil.procenso pc on pc.muncod = dcm.muncod and pc.prcano = (2012 - 1)
	        inner join workflow.historicodocumento h on h.docid = doc.docid AND h.aedid = 1404
	    where 
	        doc.esdid = 541
	    group by mun.muncod, mun.estuf, mun.mundescricao, esd.esddsc, doc.docid, dcm.cpmid
	    order by 
	        mun.estuf, mun.mundescricao";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	if( $v['docid'] ){
		$arDados = Array('muncod' => $v['muncod']);
		
		$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$v['docid']}";								
		$esdid 			= $db->pegaUm( $sql );
		$esdiddestino 	= 1087;
		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
		$aedid = $db->pegaUm( $sql );
		
		wf_alterarEstado( $v['docid'], $aedid, 'Indeferido por decurso de prazo', $arDados );
		
	}
}


?>