<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

//caso a requisição seja para gravar a movimentação
if($_POST['requisicao'] == 'salvar' && !empty($_POST['uendidatual'])){

	if(!empty($_POST['mvbid'])){
		//exclui os filhos da movimentação atualizando o estado na tabela rgp
		$oMovimentacao->cancelar($_POST['mvbid']);
		
		//exclui a movimentação
		$oMovimentacao->excluirMovimentacao($_POST['mvbid']);
	}
	
		
	//recupera os rgps do endereço selecionado
	$rgps = $oRgp->rgpsPorEnderecoUnidade($_POST['uendidatual'],4);
		
	//verifica se retornou rgp
	if(is_array($rgps) && count($rgps) > 0){
			
		//grava a movimentação
		$resultado = $oMovimentacao->salvarMovimentacao($_POST);
			
		//grava os filhos da movimentação
		$oBensMovimentacaoRgp->salvar($rgps,$resultado[1]);

		//coloca o novo id de movimentação no array de post
		$_POST['mvbid'] = $resultado[1];
		
		//pega a qtd de rgps da movimentação em questão
		$_POST['qtd_rgps'] = $oBensMovimentacaoRgp->verificaExistenciaBens($_POST['mvbid']);
	}
	else{
		alerta('Não existem bens no endereço selecionado.');
	}
       	
    extract($_POST);
	
}
//caso a requisição seja para concluir a movimentação
else if($_POST['requisicao'] == 'concluir'){
	
	//pega a qtd de rgps da movimentação em questão
	$qtd = $oBensMovimentacaoRgp->verificaExistenciaBens($_POST['mvbid']);
	
	if($qtd > 0){
		
		//pega os dados da movimentação
		$dadosMovimentacao = $oMovimentacao->pegarMovimentacaoLocalidade(array('mvbid'=>$_POST['mvbid']));
				
		
		//grava o termo
		$_POST['ternum'] = $oTermo->gerarTermo('MV',$dadosMovimentacao['ternum']);
		$_POST['ternumanterior'] = $dadosMovimentacao['ternum'];
		
	
		//atualiza o registro da movimentação
		$resultado = $oMovimentacao->concluirMovimentacao($_POST);
		
		if($resultado[0]){
			direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A&mvbid='.$resultado[1].'&mvbtipo=L','Operação realizada com sucesso!');
		}
		else if(!$resultado[0]){
			direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A','Operação falhou!');
		}
	}
	else{
		alerta('A movimentação não pôde ser concluída pois não existem bens relacionados ao processo');
		extract($_POST);
	}
}
else if($_POST['requisicao'] == 'cancelar'){
	
	if(!empty($_POST['mvbid'])){
		//exclui os filhos da movimentação atualizando o estado na tabela rgp
		$oMovimentacao->cancelar($_POST['mvbid']);
	
		//exclui a movimentação
		$oMovimentacao->excluirMovimentacao($_POST['mvbid']);
	}
	
	direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A','Operação realizada com sucesso!');
	
}
//caso a requisição seja via url
else if(!empty($_GET['mvbid'])){
	
	//recupera os dados da movimentação
	$resultado = $oMovimentacao->pegarMovimentacaoLocalidade($_GET);
	if(is_array($resultado)){
		extract($resultado);
	}
	
	//pega a qtd de rgps da movimentação em questão
	$qtd_rgps = $oBensMovimentacaoRgp->verificaExistenciaBens($_GET['mvbid']);
}

$pagina = 'adicionarMovimentacaoLocalidade';

$tituloTela = 'Movimentação de Bens por Localidade';
$subTituloTela = 'Dados do Processo de Movimentação';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_MOVIMENTACAO_GERAL,ABA_EDITAR_MOVIMENTACAO_LOCALIDADE,ABA_EDITAR_MOVIMENTACAO_SIAPE);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo($tituloTela, $subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bensmovimentacao/views/formMovimentacaoLocalidade.inc';