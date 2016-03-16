<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/partialsControl/topo.inc';

// se passado o id para exclusao
if(!empty($_GET['mecid'])){

	// exclui
	if($oMotivoEstadoConservacao->excluir($_GET['mecid']))
		direcionar('?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/pesquisar&acao=A', 'Operação realizada com sucesso.');
	
}

