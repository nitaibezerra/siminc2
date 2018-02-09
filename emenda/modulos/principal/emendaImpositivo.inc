<?php

$etoid = $db->pegaUm("select e.etoid 
					from 
						emenda.emenda e
					    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
					where
						ed.emdid = {$_REQUEST['emdid']}");

$etoid = ( !empty($etoid) ? $etoid : '3');

if( in_array($etoid, array(1, 2)) && $_REQUEST['tipo'] != 'emenda' ){
	$habilitabtn = false;
} else {
	$habilitabtn = true;
}

include_once APPRAIZ.'www/emenda/emendaImpositivo.php';
?>