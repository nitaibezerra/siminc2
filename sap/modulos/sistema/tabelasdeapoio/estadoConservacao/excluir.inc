<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/partialsControl/topo.inc';

//caso seja via url
if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['ecoid'])){
	$oEstadoConservacao->excluirEstadoConservacao($_GET['ecoid']);
}

