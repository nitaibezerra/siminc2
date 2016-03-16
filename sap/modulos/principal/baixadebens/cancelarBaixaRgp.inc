<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topo.inc';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// monta o título da tela
monta_titulo('Baixa de Bens','Cancelar Baixa Rgp');

if(!empty($_GET['bbrid'])){

	$dados = $oBensBaixaRgp->pegarRegistro($_GET['bbrid']);
	extract($dados);
	
	if($oBensBaixaRgp->excluir($bbrid)){
	
		//altera a situação do bem 
		$oRgp->atualizarSituacao($rgpid, 2);
	
	}
	
	//redireciona para a tela de edicao
	if($_GET['origem'] == 'procab'){
		direcionar("?modulo=principal/baixadebens/editarBaixaAberto&acao=A&bebid=$bebid");
	}
	else{
		direcionar("?modulo=principal/baixadebens/editar&acao=A&bebid=$bebid");
	}
		
}else{

	direcionar("?modulo=principal/baixadebens/pesquisar&acao=A");

}