<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/partialsControl/topo.inc';

//caso seja via url
if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['tesid'])){
	$oTipoEntradaSaida->excluirTipoEntradaSaida($_GET['tesid']);
}

