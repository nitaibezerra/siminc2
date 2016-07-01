<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

if(empty($_GET['benid']) && !empty($_SESSION['benid'])){
	$_GET['benid'] = $_SESSION['benid'];
}

//caso esteja concluindo o processo de entrada
if($_POST['requisicao'] == 'concluir'){
	include_once APPSAP . 'modulos/principal/bens/concluir.inc';
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

		
		if($_POST['requisicao'] == 'pesquisaritem'){
			
			$qtdResultadoPesquisa = $oBensMaterial->contarRegistrosPesquisaItem($_POST['benid'],$_POST['pesquisadsc']);
			if($qtdResultadoPesquisa >= 1){
				$subTituloItens = 'Itens encontrados na pesquisa '.$_POST['pesquisadsc'];
			}
			else{
				$subTituloItens = 'Não foram encontrados itens na pesquisa '.$_POST['pesquisadsc'];
			}
			$pesquisadsc = $_POST['pesquisadsc'];
		}
		else{
			$qtdItensProcesso = $oBensMaterial->contarItensProcesso($_GET['benid']);
			if($qtdItensProcesso >= 1){
				$subTituloItens = 'Itens do Processo '.$bennumproc;
			}
			else{
				$subTituloItens = 'Não existem itens cadastrados no processo '.$bennumproc;
			}
		}
		
		
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

$arMnuid = array(ABA_EDITAR_BEM_REDIRECIONA,ABA_INCLUIR_BEM,ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_TOMBAMENTO,ABA_INCLUSAO_EDICAO_ITEM_PROCESSO,ABA_EDITAR_BEM_ETAPA2,ABA_EDITAR_BEM,ABA_ADICIONAR_BEM_RGP);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
$tituloTela = 'Entrada de Bens';
$subTituloTela = '<b>Dados do Processo '.$bennumproc.'</b>';
monta_titulo($tituloTela,$subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bens/views/formBemMaterial.inc';
