<?php
	
	global $db;
	
	$sql = "SELECT DISTINCT
				*
			FROM 
				par.historicowsprocessoobra 
			WHERE 
			--	hwpid = 6899415 AND
				hwpwebservice = 'solicitarPagamento - Sucesso'
				AND  hwpdataenvio BETWEEN '2013-12-23' AND '2013-12-27'
			ORDER BY 
				hwpdataenvio desc";
	
	$dados = $db->carregar($sql);
	
	$conta = 0;
	$contaIgual = 0;
	$contaDiferente = 0;
	
	if(is_array($dados)){
		foreach($dados as $historicoProcesso){
			$conta++;
			$hwpid = $historicoProcesso['hwpid'];
			$proid = $historicoProcesso['proid'];
			$xmlEnvio = simplexml_load_string($historicoProcesso['hwpxmlenvio'])->children();
        	$xmlRetorno = simplexml_load_string($historicoProcesso['hwpxmlretorno'])->children();
        	
        	if( $xmlEnvio->body->params->detalhamento_pagamento->item ){
        		$ano = $xmlEnvio->body->params->detalhamento_pagamento->item->an_exercicio;
        		$seq = $xmlEnvio->body->params->detalhamento_pagamento->item->nu_documento_siafi_ne;
        		$processo = $xmlEnvio->body->params->nu_processo;
        		$ob = $xmlRetorno->body->nu_registro_ob;
	        	
	        	$neoriginal = $db->pegaLinha("SELECT * FROM par.empenho WHERE empnumeroprocesso = '".$processo."' AND substr(empnumero,7,6) = '".$seq."' and empstatus = 'A'");

	        	if($neoriginal['empnumero'] != $ano . "NE" . $seq){
	        		$contaDiferente++;
	        		echo "DIFERENTE: proid: ".$proid." - empid: ".$neoriginal['empid'] ." - Original: ". $neoriginal['empnumero'] . " != Pagamento: ".  $ano . "NE" . $seq. " - OB: ".$ob."<br>";		
	        	} else {
	        		$contaIgual++;
	        		//ver("IGUAL: ".$neoriginal['empid'] ." - Original: ". $neoriginal['empnumero'] . " != Pagamento: ".  $ano . "NE" . $seq);		
	        	}
        	}
		}
	}
	
	echo "Foram executados ".$conta." registros. ".$contaIgual." são iguais e ".$contaDiferente." são diferentes.";

?>