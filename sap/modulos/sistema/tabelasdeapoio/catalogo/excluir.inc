<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/partialsControl/topo.inc';

//caso seja via url
if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['catid'])){
	$oCatalogo->excluirCatalogo($_GET['catid']);
}

