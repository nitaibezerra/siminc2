<?php
// topo
include_once APPSAP . 'modulos/principal/bens/partialsControl/topo.inc';

unset($_SESSION['benid'],$_SESSION['bmtid']);

//caso tenha postado o formulário
if ($_POST['requisicao'] == 'salvar'){
	
	// verifica a unicidade do documento
	if($oBens->existe($_POST)){

		alert('Já existe uma entrada com este número de documento.');
		$erro = 'erro';
		extract($_POST);
		
	}else{
					
		$erro = '';
		
		if($_POST['tesslg'] != 'SUBS'){
			$erro = $oEmpenho->validarValorEmpenhoPermanente($_POST['empid'], $_POST['benvlrdoc']);
		}
		
		if($erro == 'less'){
			alert('O valor do documento ultrapassa o Valor para Material Permanente disponível no empenho selecionado.');
			extract($_POST);
			
		}
		
		// verifica se o valor do documento passa o limite do empenho
		if($erro == ''){
			
			$resultado = $oBens->salvarBem($_POST);
			
			if($resultado[0]){
				
				//guarda o id na sessão
				$_SESSION['benid'] = $resultado[1];
				
       			direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='.$resultado[1]);
			}
			else if(!$resultado[0]){
				direcionar('?modulo=principal/bens/adicionarBem&acao=A&benid='.$resultado[1],'Operação falhou!');
			}
			
		}
	
	} // fim se existe
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_BEM_REDIRECIONA,ABA_EDICAO_TOMBAMENTO,ABA_TOMBAMENTO_ITEM,ABA_EDICAO_ITEM_PROCESSO,ABA_INCLUSAO_EDICAO_TOMBAMENTO,ABA_INCLUSAO_EDICAO_ITEM_PROCESSO,ABA_EDITAR_BEM_ETAPA2,ABA_EDITAR_BEM_ABERTO,ABA_EDITAR_BEM,ABA_ADICIONAR_BEM_MATERIAL,ABA_ADICIONAR_BEM_RGP);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
if(!empty($bennumproc)){
	$bennumproc = substr($bennumproc,0,5).'.'.substr($bennumproc,5,6).'.'.substr($bennumproc,11,4).'-'.substr($bennumproc,15,2);
}
$tituloTela = 'Entrada de Bens';
$subTituloTela = '<b>Dados do Processo '.$bennumproc.'</b>';
monta_titulo($tituloTela,$subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bens/views/formBem.inc';
