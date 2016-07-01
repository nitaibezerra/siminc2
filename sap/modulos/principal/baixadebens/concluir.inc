<?php

if($_POST['requisicao'] == 'concluir'){
	
	$oBensBaixa->atualizarSemHistorico($_POST);
	
	extract($_GET);
	
	if(!empty($bbrid)){
	
		$baixaRgp = $oBensBaixaRgp->pegarRegistro($bbrid);
		$bebid = $baixaRgp['bebid'];
	
	}	
	
	// verifica se há itens na baixa
	if(!empty($bebid) && $oBensBaixaRgp->validaTotalBaixas($bebid)){
		
		$tesid = $_POST['tesid'];
		
		//verifica se a Situação de todos os RGPs relacionados ao processo é Em Processo de Baixa
		$erroSituacao = false;
		$rgps = $oBensBaixaRgp->pegarRpgsDaBaixa($bebid);
		
		foreach($rgps as $key => $value){
			$sbmid = $oRgp->pegarSituacao($value);
			if($sbmid != SBM_BAIXA && $sbmid != SBM_BAIXADO){
				$erroSituacao = true;
				break;
			}
		}
		//caso todos estejam em processo de baixa
		if(!$erroSituacao){
			
			//se o tipo de saída for diferente de doação confirma e salva
			if($tesid != TES_SAIDA_DOACAO){
				
				$_SESSION['just'] = $_POST['bhsjustificativa'];
				//apresenta confirmação para o usuário
				echo '<script>window.location = "?modulo=principal/baixadebens/confirm&acao=A&urlok=editar&urlcancel=pesquisar&bebid='.$bebid.'"</script>';

			//se o tipo de saída for doação
			}else if($tesid == TES_SAIDA_DOACAO){
				
				//verifica se a Situação de todos os RGPs relacionados ao processo é Inservível
				reset($rgps);
				$erroEstadoConservacao = false;
				foreach($rgps as $key => $value){
					$ecoid = $oRgp->pegaEstadoConservacao($value);
					if($ecoid != ECO_INSERVIVEL){
						$erroEstadoConservacao = true;
						break;
					}
				}
				//caso todos sejam inservíveis confirma e salva
				if(!$erroEstadoConservacao){

					$_SESSION['just'] = $_POST['bhsjustificativa'];
					//apresenta confirmação para o usuário					
					echo '<script>window.location = "?modulo=principal/baixadebens/confirm&acao=A&urlok=editar&urlcancel=pesquisar&bebid='.$bebid.'"</script>';

				}
				else{
					alert('O desfazimento não pôde ser concluído pois existem RGPs associados ao processo com o estado de conservação diferente de Inservível.');
				}
			}
		}
		else{
			alert('O desfazimento não pôde ser concluído pois existem RGPs associados ao processo com a situação diferente de Em Processo de Baixa.');
		}
	}else{
		
		alert('O desfazimento não pôde ser concluído pois não existem RGPs associados ao processo.');
		
	}
	
}
else if($_GET['confirmado'] == 'S'){
	$oBensBaixa->ativar($_GET['bebid']);
	direcionar('?modulo=principal/baixadebens/pesquisar&acao=A&bebid=' . $_GET['bebid']);
}

