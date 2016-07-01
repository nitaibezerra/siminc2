<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/partialsControl/topoPopup.inc';

// monta o título da tela
monta_titulo('Material','');

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
			
	// salva
	$resultado = $oMaterial->salvar($_POST);

}


// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/formDefaultPopup.inc';

//fazendo este teste aqui pq o javascript é incluído no form
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	if($resultado){
			
		//recupera o nome da conta contábil selecionada no cadastro do material
		$contacontabil = $oContaContabil->pegarRegistro($_POST['ccbid']);

		executarScriptPai("retornaDadosMaterial(\"".$_POST['matdsc']."\",".$resultado.",\"".$contacontabil['ccbdsc']."\",".$_POST['ccbid'].")");
		alerta('Operação realizada com sucesso!');
		fecharPopup();
		die;
	}
	else if(!$resultado){
		alerta('Operação Falhou!');
		die;
	}
}