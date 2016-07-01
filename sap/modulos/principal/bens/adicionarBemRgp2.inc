<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

//caso seja para editar o rgp(item já tombado)
if(!empty($_GET['rgpid'])){

	$dados = $oRgp->pegarRegistro($_GET['rgpid']);
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
		
		// verifica se o bem material foi selecionado
		if(!empty($_GET['bmtid'])){
			$bensMaterial = $oBensMaterial->pegarRegistro($_GET['bmtid']);
			extract($bensMaterial);
		}
		
		//formata o número do processo para ser exibido na tela
		$bennumproc = substr($bennumproc,0,5).'.'.substr($bennumproc,5,6).'.'.substr($bennumproc,11,4).'-'.substr($bennumproc,15,2);

		// se requisicao for para salvar
		if($_POST['requisicao'] == 'salvar' && empty($_POST['rgpid'])){

			// verifica a unicidade do material
			if($oRgp->existe($_POST)){
		
				alert('Já existe um material com este número de registro.');
				extract($_POST);
				
			}else{
				
					// salva o item
					$rgpid = $oRgp->salvar($_POST);
						
					// se salvar
					if($rgpid){
		
						// direciona para a tela de cadastro
						direcionar('?modulo=principal/bens/adicionarBemRgp&acao=A&benid='.$benid.'&bmtid='.$bmtid, 'Operação realizada com sucesso.');
		
					}else{
							
						alert('erro');
							
					}

			} // fim existe
			
		} // fim salvar
		else if($_POST['requisicao'] == 'salvar' && !empty($_POST['rgpid'])){
			
			// verifica a unicidade do material
			if($oRgp->existe($_POST)){
	
				alert('Já existe um material com este número de registro.');
				extract($_POST);
		
			}else{
	
				// atualiza
				if($oRgp->atualizar($_POST)){
			
					direcionar('?modulo=principal/bens/adicionarBemRgp&acao=A&benid='.$_POST['benid'].'&bmtid='.$_POST['bmtid'], 'Operação realizada com sucesso.');
			
				}else{
			
					alerta('Erro.');
			
				}
	
			} // fim existe
	
			extract($_POST);
			
		}
		
		//monta o subtítulo
		$subTituloMaterial = 'Inclusão/Edição de Tombamento do Item '.$matdsc;
		
		$bendtdoc = formata_data($bendtdoc);

	}else{

		// volta para a pagina de cadastro de bem
		direcionar('?modulo=principal/bens/adicionarBem&acao=A');

	} // fim carrega bem
	

}else{

	// volta para a pagina de cadastro de bem
	direcionar('?modulo=principal/bens/adicionarBem&acao=A');

}

//recupera a sigla do tipo de entrada/saída do registro
$tesslg = $oBens->pegaSiglaTipoEntradaSaida($_GET['benid']);

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_BEM_REDIRECIONA,ABA_INCLUIR_BEM,ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_ITEM_PROCESSO,ABA_EDITAR_BEM_ETAPA2,ABA_EDITAR_BEM);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o titulo da tela
$tituloTela = 'Entrada de Bens';
$subTituloTela = '<b>Dados do Processo '.$bennumproc.'</b>';
monta_titulo($tituloTela,$subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bens/views/formBemRgp2.inc';