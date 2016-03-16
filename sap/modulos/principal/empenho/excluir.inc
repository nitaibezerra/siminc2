<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

//caso seja via url
if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['empid'])){
	if(!empty($_GET['popup'])){
		$oEmpenho->excluirEmpenho($_GET['empid'],$_GET['popup']);	
	}
	else{
		$oEmpenho->excluirEmpenho($_GET['empid']);
	}
	
}

