<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

// se passado o id para exclusao
if(!empty($_GET['bmtid'])){

	// exclui
	if($oBensMaterial->excluir($_GET['bmtid']))
		direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid=' . $_GET['benid'], 'O item foi excluído do processo');
	
}
