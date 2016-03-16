<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

if(empty($_GET['benid']) && !empty($_SESSION['benid'])){
	$_GET['benid'] = $_SESSION['benid'];
}

if(empty($_GET['bmtid']) && !empty($_SESSION['bmtid'])){
	$_GET['bmtid'] = $_SESSION['bmtid'];
}

//recuperando dados do bem para completar o cadastro
if(!empty($_GET['benid'])){

	//guarda o id na sessão
	$_SESSION['bmtid'] = $_GET['bmtid'];
	
	
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

		if($_POST['requisicao'] == 'pesquisaritem'){
			$qtdResultadoPesquisa = $oRgp->contarRegistrosPesquisaTombamento($_POST['benid'],$_POST['pesquisadsc']);
			if($qtdResultadoPesquisa >= 1){
				$subTituloItens = 'Registros do tombamento encontrados na pesquisa '.$_POST['pesquisadsc'];
			}
			else{
				$subTituloItens = 'Não foram encontrados registros na pesquisa '.$_POST['pesquisadsc'];
			}
			$pesquisadsc = $_POST['pesquisadsc'];
		}
		else{
			$subTituloItens = 'Tombamento do Item '.$matdsc;
			$qtdResultadoPesquisa = 1;
		}
		
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

$arMnuid = array(ABA_EDITAR_BEM_REDIRECIONA,ABA_INCLUIR_BEM,ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_TOMBAMENTO,ABA_INCLUSAO_EDICAO_ITEM_PROCESSO,ABA_EDITAR_BEM_ETAPA2,ABA_EDITAR_BEM);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
$tituloTela = 'Entrada de Bens';
$subTituloTela = '<b>Dados do Processo '.$bennumproc.'</b>';
monta_titulo($tituloTela,$subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bens/views/formBemRgp.inc';