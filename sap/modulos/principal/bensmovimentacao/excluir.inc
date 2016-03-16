<?php

include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

$rgps = $oRgp->carregaRgpsDeUmaMovimentacao($_GET['mvbid']);
if(is_array($rgps) && count($rgps) >= 1){
	foreach($rgps as $key => $value){
		$oBensMovimentacaoRgp->excluir($value['rgpid'],$_GET['mvbid']);
	}
}

$oMovimentacao->excluirMovimentacao($_GET['mvbid']);

if($_GET['origem'] == 'mov'){
	direcionar("?modulo=principal/bensmovimentacao/processosAbertos&acao=A");
}else{
    direcionar("?modulo=principal/processosabertos/pesquisar&acao=A");
}

?>