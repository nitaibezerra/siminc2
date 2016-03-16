<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/partialsControl/topo.inc';

// se passado o id para exclusao
if(!empty($_GET['sbmid'])){

	// exclui
	if($oSituacaoBem->excluir($_GET['sbmid']))
		direcionar('?modulo=sistema/tabelasdeapoio/situacaoBem/pesquisar&acao=A', 'Operação realizada com sucesso.');
	
}

