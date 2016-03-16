<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoDocumento/partialsControl/topo.inc';

// se passado o id para exclusão
if(!empty($_GET['tdcid'])){

	// exclui
	if($oTipoDocumento->excluir($_GET['tdcid']))
		direcionar('?modulo=sistema/tabelasdeapoio/tipoDocumento/pesquisar&acao=A', 'Operação realizada com sucesso.');
	
}

