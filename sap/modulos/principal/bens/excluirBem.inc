<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

//caso seja via url
if($_SERVER['REQUEST_METHOD']=="GET"){
	if(!empty($_REQUEST['benid'])){
	    if($_GET['procab'])
		  $oBens->excluirBem($_REQUEST['benid'], '?modulo=principal/processosabertos/pesquisar&acao=A');
		else
		  $oBens->excluirBem($_REQUEST['benid']);
	}
}
