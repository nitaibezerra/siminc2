<?php
// verifica se a soma total dos itens é igual ao valor do documento
if($oBensMaterial->validaValorDocumento($_GET['benid'])){

	// verifica a quantidade de materiais cadastrados é igual ao informado na primeira tela
	//if($oBensMaterial->validaTotalUnidade($_GET['benid'])){

		$nomeMaterial = $oRgp->validaTotalTombamentos($_GET['benid']);
		
		if(!$nomeMaterial){
			
			$dados = $oBens->carregaBemPorId($_GET['benid']);
			if($dados['tesslg'] == 'SUBS' && $dados['benstatus'] != 'A'){
				echo "<script language='javascript' type='text/javascript' src='./js/default.js'></script>";
				echo "<script>
						alert('Para a conclusão do processo de substituição de bens, é necessário registrar a baixa do(s) bem(ns) perdido(s)/roubado(s).');
	  				  </script>";
				echo "<script>AbrirPopUp('?modulo=principal/baixadebens/adicionarPopup&acao=A&benid=".$_GET["benid"]."&tesid=".$dados['tesid']."','baixa','scrollbars=yes, width=800, height=570')</script>";
			}
			else{
			
				// ativa o bem e redireciona para a tela de listagem
				$oBens->ativar($_GET['benid']);
			
				direcionar('?modulo=principal/bens/pesquisarBem&acao=A&benid='.$_GET['benid'], 'Operação realizada com sucesso.');
			}
			
		}else{

			direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='.$_GET['benid'], 'A quantidade de itens do material ' . $nomeMaterial . ' tombados na tela Tombamento está diferente da quantidade informada no campo Qtd. de Unidades deste Material da tela Itens do Processo. Corrija esta inconsistência para concluir o cadastramento.');
			
		}
	
	//}else{

		//direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='.$_GET['benid'], 'A quantidade de itens cadastrados na tela Itens do Processo está diferente da quantidade informada no campo Qtd. de Itens do Documento da tela Dados do Processo. Corrija esta inconsistência para concluir o cadastramento.');

	//} // fim validaTotalUnidade

}else{

	direcionar('?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='.$_GET['benid'], 'O valor total dos itens tem que ser igual ao valor do documento.');

} // fim validaValorDocumento


