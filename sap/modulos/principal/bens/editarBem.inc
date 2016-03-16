<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST"){
	if ($_REQUEST['requisicao'] == 'concluir'){
		
		// verifica a unicidade do documento
		if($oBens->existe($_POST)){

			alert('Já existe uma entrada com este número de documento.');
			$erro = 'erro';
			extract($_POST);
		
		}else{
		
			
			$erro = '';
			if($_POST['tesslg'] != 'SUBS'){
				$erro = $oEmpenho->validarValorEmpenhoPermanente($_POST['empid'], $_POST['benvlrdoc'], $_POST['benid'],$_POST['benstatus']);
			}
			
			if($erro == 'less'){
				alert('O valor do documento ultrapassa o Valor para Material Permanente disponível no empenho selecionado.');
				extract($_POST);
			}
			
			
			
			// verifica se o valor do documento passa o limite do empenho
			if($erro == ''){
				
				$resultado = $oBens->salvarBem($_POST);
				
				include_once APPSAP . 'modulos/principal/bens/concluir.inc';
				
				/*if($resultado[0]){
					if($_POST['benstatus'] == 'I'){
						direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='.$resultado[1]);
					}else if($_POST['benstatus'] == 'A'){
						direcionar('?modulo=principal/bens/pesquisarBem&acao=A','A entrada de bens foi editada com sucesso.');
					}
				}
				else if(!$resultado[0]){
					direcionar('?modulo=principal/bens/editarBem&acao=A&benid='.$resultado[1],'Operação falhou!');
				}*/
		        
			}
		
		} // fim se existe		
		
	}
}
//caso seja via url
else if($_SERVER['REQUEST_METHOD']=="GET"){
	
	if(empty($_GET['benid']) && !empty($_SESSION['benid'])){
		$_GET['benid'] = $_SESSION['benid'];
	}
	if(!empty($_GET['benid']) && empty($_SESSION['benid'])){
		$_SESSION['benid'] = $_GET['benid'];
	}
	
	if(!empty($_GET['benid'])){
		$Bem = $oBens->carregaBemPorId($_GET['benid']);
		extract($Bem);
	}
	
}



// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_TOMBAMENTO,ABA_INCLUSAO_EDICAO_ITEM_PROCESSO,ABA_INCLUIR_BEM,ABA_EDITAR_BEM_ABERTO,ABA_ADICIONAR_BEM_MATERIAL,ABA_ADICIONAR_BEM_RGP);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
if(!empty($bennumproc)){
	$bennumproc = substr($bennumproc,0,5).'.'.substr($bennumproc,5,6).'.'.substr($bennumproc,11,4).'-'.substr($bennumproc,15,2);
}
$tituloTela = 'Edição de Entrada de Bens';
$subTituloTela = '<b>Dados do Processo '.$bennumproc.'</b>';
monta_titulo($tituloTela,$subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bens/views/formBem.inc';
