<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
	
	if($oEmpenho->verificaEmpenhoRepetido($_POST)){
		alerta('Já existe registro do Empenho informado.');
		extract($_POST);
	}
	else{
		
		$arrValidacao = $oEmpenho->validaEdicao($_POST);
		if($arrValidacao[0]){
			alerta('A data deve ser a mesma ou anterior à dos registros de entrada.');
			extract($_POST);
		}
		else if($arrValidacao[1]){
			alerta('O valor para Material Permanente deve ser igual ou superior à soma do Valor do Documento de todos os processos de Entrada relacionados.');
			extract($_POST);
		}
		else{
			$resultado = $oEmpenho->salvarEmpenho($_POST);
			if($resultado[0]){
				direcionar('?modulo=principal/empenho/pesquisarEmpenho&acao=A&popup=S','Operação realizada com sucesso!');
			}
			else if(!$resultado[0]){
				direcionar('?modulo=principal/empenho/pesquisarEmpenho&acao=A&popup=S','Operação falhou!');
			}
		}
		
		
	}
	
	
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET" && !empty($_GET['empid'])){
	$Empenho = $oEmpenho->carregaEmpenhoPorId($_GET['empid']);
	extract($Empenho);
	
	$empdata = formata_data($empdata);
	
	//verifica se o empenho está sendo usado por alguma
	//entrada de bens. caso esteja, bloqueia os campos de valor
	$emUso = $oEmpenho->verificaEmpenhoEmUso($_GET['empid']);
}

// monta o título da tela
monta_titulo('Nota de Empenho','');

// view
include_once APPSAP . 'modulos/principal/empenho/views/formDefault.inc';
