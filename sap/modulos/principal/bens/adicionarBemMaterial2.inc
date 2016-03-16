<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

if(!empty($_GET['bmtid'])){

	$dados = $oBensMaterial->pegarRegistro($_GET['bmtid']);
	extract($dados);

}

//recuperando dados do bem para completar o cadastro
if(!empty($_GET['benid'])){

	// recupera dados do bem
	$bem = $oBens->pegarRegistroSimples($_GET['benid']);

	// se encontrar
	if($bem){

		// mostra os dados no formulario
		extract($bem);
		
		//formata o número do processo para ser exibido na tela
		$bennumproc = substr($bennumproc,0,5).'.'.substr($bennumproc,5,6).'.'.substr($bennumproc,11,4).'-'.substr($bennumproc,15,2);

		// se requisicao for para salvar
		if($_POST['requisicao'] == 'salvar' && empty($_POST['bmtid'])){

				$erro = false;

				//validações para que os itens adicionados ao documento
				//não ultrapassem o valor definido para o documento na primeira etapa
				if(strpos($_POST['bmtvlrunitario'],'.') !== false){
					$_POST['bmtvlrunitario'] = str_replace('.','',$_POST['bmtvlrunitario']);
				}
				$multiplica = new Math($_POST['bmtvlrunitario'],$_POST['bmtitmat']);
				$valor = $multiplica->mul()->getResult();

				if(strpos($_POST['benvlrdoc'],'.') !== false){
					$_POST['benvlrdoc'] = str_replace('.','',$_POST['benvlrdoc']);
				}
				$maior = new Math($valor,$_POST['benvlrdoc']);
				$eMaior = $maior->isLarger();
				if($eMaior){
					$erro = true;
					extract($_POST);
					alert('O valor total deste material, somado aos materiais já cadastrados no processo de baixa, ultrapassa o valor do documento');
				}
				else{
					$parcial = $oBensMaterial->somaBensMaterial($benid);
					$soma = new Math($valor,$parcial);
					$novoValor = $soma->sum()->getResult();

					$maior = new Math($novoValor,$_POST['benvlrdoc']);
					$eMaior = $maior->isLarger();

					if($eMaior){
						$erro = true;
						extract($_POST);
						alert('O valor total deste material, somado aos materiais já cadastrados no processo de baixa, ultrapassa o valor do documento');
					}

				}

				//validação para que a qtd de itens do documento
				//não ultrapasse o informado na primeira etapa
				/*if(!$erro){
					$tot = $oBensMaterial->quantidadeMaterial($benid);
					$totAbsoluto = $tot;
					if($tot >= 1){

						$tot = $tot + 1;

						if($tot > $_POST['benitdoc']){
							$erro = true;
							extract($_POST);
							if($totAbsoluto > 1){
								alert("O material não pode ser associado ao processo de entrada pois já existem $totAbsoluto materiais associados à ele. Para cadastrar outros materiais neste processo, exclua algum material existente ou altere a informação do campo Qtd. de Itens do Documento da tela Dados de Entrada.");
							}
							else{
								alert("O material não pode ser associado ao processo de entrada pois já existe $totAbsoluto material associado à ele. Para cadastrar outros materiais neste processo, exclua algum material existente ou altere a informação do campo Qtd. de Itens do Documento da tela Dados de Entrada.");
							}
							
						}


					}
				}*/









				// verifica a unicidade do material
				if($oBensMaterial->existe($_POST)){

					alert('Já existe um item de processo para este material.');
					extract($_POST);

				}else if(!$erro){

					// salva o item
					$resultado = $oBensMaterial->salvar($_POST);

					// se salvar
					if($resultado[0]){

						// direciona para a tela de cadastro
						direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid=' . $benid, 'Operação realizada com sucesso.');

					}else if(!$resultado[0]){
						direcionar('?modulo=principal/bens/adicionarBemMaterial2&acao=A&benid=' . $benid, 'Operação falhou!');
					}

				} // fim existe

		} // fim requisicao salvar
		//caso esteja editando um item
		else if($_POST['requisicao'] == 'salvar' && !empty($_POST['bmtid'])){
			
			$erro = false;
			
			$total = $oBensMaterial->somaBensMaterial($_POST['benid']);
			$valorDoItem = $oBensMaterial->pegaValorDoItem($_POST['bmtid'],$_POST['benid']);
			
			$diferenca = new Math($total,$valorDoItem);
			$parcial = $diferenca->sub()->getResult();
			
			if(strpos($_POST['bmtvlrunitario'],'.') !== false){
				$_POST['bmtvlrunitario'] = str_replace('.','',$_POST['bmtvlrunitario']);
			}
			$multiplica = new Math($_POST['bmtvlrunitario'],$_POST['bmtitmat']);
			$teste = $multiplica->mul()->getResult();
			
			$soma = new Math($parcial,$teste);
			$valorParaSerValidado = $soma->sum()->getResult();
			
			if(strpos($_POST['benvlrdoc'],'.') !== false){
				$_POST['benvlrdoc'] = str_replace('.','',$_POST['benvlrdoc']);
			}
			$maior = new Math($valorParaSerValidado,$_POST['benvlrdoc']);
			$eMaior = $maior->isLarger();
			
			if($eMaior){
				$erro = true;
				extract($_POST);
				alert('O valor total deste material, somado aos materiais já cadastrados no processo de baixa, ultrapassa o valor do documento');
			}
			
			
			/*if(!$erro){
				$tot = $oBensMaterial->quantidadeMaterial($benid);
				if($tot >= 1){
					if($tot > $_POST['benitdoc']){
						$erro = true;
						extract($_POST);
						alert('A quantidade de materiais cadastrados na tela Itens do Processo está diferente da quantidade informada no campo Qtd. de Itens do Documento da tela Dados de Entrada. Corrija esta inconsistência para continuar com o cadastramento.');
					}
				}
			}*/
			
			// verifica a unicidade do material
			if($oBensMaterial->existe($_POST)){
		
				alert('Já existe um item de processo para este material.');
				extract($_POST);
				
			}else if(!$erro){
			
			
				// valida se a quantidade de itens é menor que a quantidade já tomabada
				if($oBensMaterial->validaQuantidadeItens($_POST['bmtid'], $_POST['bmtitmat'])){
			
					// atualiza
					if($oBensMaterial->atualizar($_POST)){
						
						direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='. $_POST['benid'], 'Operação realizada com sucesso.');
						
					}else{
						
						alerta('Erro.');
						
					}
			
				}else{
			
					alert('A quantidade de itens não pode ser menor que a quantidade de RGPs tombados para este Material.');
			
				} // fim validaQuantidadeItens
				
			} // fim se existe
			
			extract($_POST);
			
		}
		
		
		
		//monta o subtítulo
		$subTituloMaterial = 'Inclusão/Edição de Item no Processo '.$bennumproc;
				
		$bendtdoc = formata_data($bendtdoc);

	}else{

		// volta para a pagina de cadastro de bem
		direcionar('?modulo=principal/bens/adicionarBem&acao=A');

	}

}else{

	// volta para a pagina de cadastro de bem
	direcionar('?modulo=principal/bens/adicionarBem&acao=A');

}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_BEM_REDIRECIONA,ABA_INCLUIR_BEM,ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_TOMBAMENTO,ABA_EDITAR_BEM_ETAPA2,ABA_EDITAR_BEM,ABA_ADICIONAR_BEM_RGP);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
$tituloTela = 'Entrada de Bens';
$subTituloTela = '<b>Dados do Processo '.$bennumproc.'</b>';
monta_titulo($tituloTela,$subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bens/views/formBemMaterial2.inc';