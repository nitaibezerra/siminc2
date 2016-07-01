<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

require_once('../../global/config.inc');
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();
$filtro = array();
if( !empty($_REQUEST['ptrid']) ) array_push( $filtro, 'ptrid = '.$_REQUEST['ptrid'] );
if( !empty($_REQUEST['logtipo']) ) array_push( $filtro, "logtipo = '".$_REQUEST['logtipo']."'" );

$sql = "SELECT * FROM emenda.logerrows ".( $filtro ? 'WHERE '.implode( ' and ', $filtro ) : '' );
$ar = $db->carregar($sql);
foreach($ar as $dados){
	echo "----- ----- ----- ----- ----- ----- ----- ----- ----- DEBUG INICIO ----- ----- ----- ----- ----- ----- ----- ----- ----- --<br />";
	echo "<pre>";
	print_r($dados);
	echo "</pre>";
	echo "----- ----- ----- ----- ----- ----- ----- ----- ----- DEBUG FIM ----- ----- ----- ----- ----- ----- ----- ----- ----- --<br />";
	
}

?>