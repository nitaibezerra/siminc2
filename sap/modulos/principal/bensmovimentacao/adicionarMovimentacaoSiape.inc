<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

// buscar os rgps 
if($_POST['requisicao'] == 'buscarRgps'){
	
	//grava os rgps da movimentação
	$rgps = $oRgp->rgpsPorResponsavel($_POST['nu_matricula_siape_atual'], SBM_USO);

	// se existem rgps
	if($rgps){
       	
		// se for edicao de movimetnacao
		if(!empty($mvbid)){
		
			// retira os rgps existentes
			$oMovimentacao->cancelar($mvbid);
			
		}else{

			//salvando movimentacao
			$resultado = $oMovimentacao->salvarMovimentacao($_POST);
			$mvbid = $resultado[1];
			
		}
		
		// adiciona os rgps na movimetnacao
		$oBensMovimentacaoRgp->salvar($rgps, $mvbid);
		$_POST['mvbid'] = $mvbid;
       		
	}else{
			
    	alerta('O servidor ' . $_POST['no_servidor_atual'] . ' não possui bens sob sua responsabilidade!');
       	$_POST['no_servidor_atual'] = $_POST['nu_matricula_siape_atual'] = '';
       		
	}
		
	extract($_POST);

// conclusão
}else if($_POST['requisicao'] == 'concluir'){

	if(!empty($_POST['mvbid']) && $oBensMovimentacaoRgp->verificaExistenciaBens($_POST['mvbid']) > 0){
		
		//pega os dados da movimentação
		$dadosMovimentacao = $oMovimentacao->pegarMovimentacaoSiape($_POST['mvbid']);
		
		$_POST['ternum'] = $oTermo->gerarTermo('MV',$dadosMovimentacao['ternum']);
		$_POST['ternumanterior'] = $dadosMovimentacao['ternum'];

		//atualiza o registro da movimentação
		$resultado = $oMovimentacao->concluirMovimentacao($_POST);
		
		if($resultado[0]){
			direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A&mvbtipo=S&mvbid='.$resultado[1],'Operação realizada com sucesso!');
		}
		else if(!$resultado[0]){
			direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A','Operação falhou!');
		}
		
		
	}else{
		
		alerta('A movimentação não pôde ser concluída pois não existem bens relacionados ao processo');
		extract($_POST);
	
	}

	
// cancelar
}else if($_POST['requisicao'] == 'cancelar'){
	
	if(!empty($_POST['mvbid'])){
	
		//exclui os filhos da movimentação atualizando o estado na tabela rgp
		$oMovimentacao->cancelar($_POST['mvbid']);
       
		//exclui a movimentação
		$oMovimentacao->excluirMovimentacao($_POST['mvbid']);
	
	}	
       
	direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A','Operação realizada com sucesso!');
	
// edição
}else if(!empty($_GET['mvbid'])){

	$dados = $oMovimentacao->pegarMovimentacaoSiape($_GET['mvbid']);
	extract($dados);
	
	// se forem iguais limpa o novo, porque não foi preenchido
	if($nu_matricula_siape_atual == $nu_matricula_siape_novo){
		$nu_matricula_siape_novo = $no_servidor_novo = ''; 
	}
	
}

$pagina = 'adicionarMovimentacaoSiape';

$tituloTela = 'Movimentação de Bens por Siape';
$subTituloTela = 'Dados do Processo de Movimentação';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_MOVIMENTACAO_GERAL,ABA_EDITAR_MOVIMENTACAO_LOCALIDADE,ABA_EDITAR_MOVIMENTACAO_SIAPE);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo($tituloTela, $subtituloTela);

// view
include_once APPSAP . 'modulos/principal/bensmovimentacao/views/formMovimentacaoSiape.inc';