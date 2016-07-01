<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select 
			dp.dopdatainiciovigencia, dp.dopid, to_char(dp.dopdatainclusao, 'MM/YYYY') as datainicio
		from par.documentopar dp
		where dp.dopidpai is null";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	$dopdatainiciovigencia 	= $v['dopdatainiciovigencia'];
	$datainicio			 	= $v['datainicio'];
	$dopid					= $v['dopid'];
	
	$sql = "update par.documentopar set dopdatainiciovigencia = '{$datainicio}' where dopid = {$dopid}";
	$db->executar($sql);
	
	$sql = "select 
				dp.dopdatainiciovigencia, dp.dopid, to_char(dp.dopdatainclusao, 'MM/YYYY') as datainicio
			from par.documentopar dp
			where dp.dopidpai = $dopid";
	$arrDocFilho = $db->carregar($sql);
	$arrDocFilho = $arrDocFilho ? $arrDocFilho : array();
	
	foreach ($arrDocFilho as $doc) {
		alteraData( $doc['dopid'], $datainicio );
	}
	$db->commit();
}

function alteraData( $dopid, $datainicio ){
	global $db;
	
	$sql = "select 
				dp.dopdatainiciovigencia, dp.dopid, to_char(dp.dopdatainclusao, 'MM/YYYY') as datainicio
			from par.documentopar dp
			where dp.dopidpai = $dopid";
	$arrDocFilho = $db->carregar($sql);
	$arrDocFilho = $arrDocFilho ? $arrDocFilho : array();
	
	$sql = "update par.documentopar set dopdatainiciovigencia = '{$datainicio}' where dopid = {$dopid}";
	$db->executar($sql);
		
	foreach ($arrDocFilho as $doc) {
		$sql = "update par.documentopar set dopdatainiciovigencia = '{$datainicio}' where dopid = {$doc['dopid']}";
		$db->executar($sql);
		
		alteraData( $doc['dopid'], $datainicio );
	}
	$db->commit();
}