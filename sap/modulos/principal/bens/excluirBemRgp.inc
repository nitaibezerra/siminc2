<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

// se passado o id para exclusao
if(!empty($_GET['rgpid'])){

	// exclui
	if($oRgp->excluir($_GET['rgpid']))
		direcionar('?modulo=principal/bens/adicionarBemRgp&acao=A&benid=' . $_GET['benid'], 'Operação realizada com sucesso.');
	
}
