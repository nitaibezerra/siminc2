<?php
// topo
include_once APPSAP . 'modulos/principal/fornecedor/partialsControl/topo.inc';

//caso seja via url
if($_SERVER['REQUEST_METHOD'] == "GET" && !empty($_GET['forcpfcnpj'])){
	if(!empty($_GET['popup'])){
		$oFornecedor->excluirFornecedor($_GET['forcpfcnpj'],$_GET['popup']);	
	}
	else{
		$oFornecedor->excluirFornecedor($_GET['forcpfcnpj']);
	}
	
}

