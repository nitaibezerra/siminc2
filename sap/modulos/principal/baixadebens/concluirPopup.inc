<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<?php

if($_POST['requisicao'] == 'concluir'){
	
		
	
	$bebid = $oBensBaixa->salvarPopup($_POST);
	
	$rgps = $oRgp->carregaRgpsDeUmaEntrada($_POST['benid']);
	
	$dados = array();
	$dados['bebid'] = $bebid;
	if(is_array($rgps) && count($rgps) >= 1){
		foreach($rgps as $key => $value){
			$dados['rgpid'] = $value['rgpid'];
			$oBensBaixaRgp->salvar($dados);
		}	
	}
	
	
	$termoBaixa = $oBensBaixa->ativarPopup($bebid);
	
	$termoEntrada = $oBens->ativarPopup($_POST['benid'],$termoBaixa);
	
	echo "<script>
	executarScriptPai(\"concluirBaixaPopup('".$_POST['benid']."','".$bebid."','".$termoBaixa."','".$termoEntrada."')\");
	self.close();
	</script>";
	
	//direcionar('?modulo=principal/bens/pesquisarBem&acao=A&benid='.$_POST['benid'].'&bebid='.$bebid, 'Operação Realizada com Sucesso.');
}


