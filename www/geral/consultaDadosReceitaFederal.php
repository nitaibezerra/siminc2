<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/classes/consultaReceitaFederal.class.inc';

if($_REQUEST['requisicao']){
	ob_clean();
	$RF = new ConsultaReceitaFederal($_REQUEST['tipo']);
	$RF->$_REQUEST['requisicao']();
	exit;
}