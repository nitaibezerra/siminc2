<?php
ini_set("memory_limit","25000M");
set_time_limit(0);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "www/par/_constantes.php";

$db = new cls_banco();

//-- DIFERENÇA DE composicao maior que empenho
$sql = "select distinct e.empid, e.empvalorempenho, e.empnumero, e.empnumeroprocesso,  ( sum(o.eobvalorempenho) - e.empvalorempenho ) as diferenca -- , p.proid , e.empvalorempenho, sum(o.eobvalorempenho) as empenhocomposicao 
		from par.empenho  e
		inner join par.empenhoobra o on e.empid = o.empid
		inner join par.processoobra p on p.pronumeroprocesso = e.empnumeroprocesso and p.prostatus = 'A'
		where e.empstatus = 'A' and o.eobstatus = 'A' and e.empcodigoespecie not in ( '03', '04', '13')  and e.empid in (3654)  
		group by e.empid , e.empvalorempenho, e.empnumero, e.empnumeroprocesso -- , p.proid, e.empvalorempenho
		having  ( sum(o.eobvalorempenho) - e.empvalorempenho ) >= 0.01 ";

$recompor = $db->carregar($sql);
//ver($recompor,d);
if(is_array($recompor)){

	foreach($recompor as $chave=>$composicao){
		$valorSendoReduzido = 0;
		recompoem($composicao, $valorSendoReduzido, $db);
		$db->commit();
		echo "Script executado com sucesso para NE ".$composicao['empnumero']." do processo: ".$composicao['empnumeroprocesso']." <br>";
		
	} // foreach($recompor as $composicao)
}
	
function recompoem($composicao, $valorSendoReduzido, $db){
		//global $db;
		
		$valorSendoReduzido = $valorSendoReduzido;
		$diferenca 			= $composicao['diferenca'];
		
		if($valorSendoReduzido < $diferenca ){
		
		$valorEmpenho = $composicao['empvalorempenho'];
		// QUANTIDADE DE OBRAS NO EMPENHO
		$sql = "SELECT count(preid)
			FROM par.empenhoobra 
			WHERE 	empid =".$composicao["empid"]." 
					and eobstatus = 'A'";
		
		$qtdObrasNoEmpenho =  $db->pegaUm($sql);
		
		$contObrasSaldoMaiorQueVlrObra = 0;
		// DADOS DO EMPENHOOBRA
		$sql = "SELECT eobid, preid, empid, SUM(eobvalorempenho) as eobvalorempenho   
				FROM par.empenhoobra 
				WHERE 	empid =".$composicao["empid"]." 
						and eobstatus = 'A'
				GROUP BY eobid, preid, empid
				";
		$dadosComposicaoPorEmpidDaObra = $db->carregar($sql);
		
		$contObrasSaldoMenorQueVlrObra = 0;

		// Percorre obras do empenho
		$arrObrasEmpenho = array();
		$arrObrasEmpenhoMenor = array();
		$arrObrasEmpenhoIgual = array();
		//$valorSendoReduzido = 0;
		foreach($dadosComposicaoPorEmpidDaObra as $dadosObra){
			$preid 				= $dadosObra['preid'];
			$eobid				= $dadosObra['eobid'];
			$empid 				= $dadosObra['empid'];
			$eobvalorempenho 	= $dadosObra['eobvalorempenho'];
			// SALDO DE EMPENHO DA OBRA
			$sqlsaldoEmpenhoObra = "SELECT saldo::numeric(20,2) FROM par.vm_saldo_empenho_por_obra WHERE preid =".$preid;
			$saldoEmpenhoObra 	 = $db->pegaUm($sqlsaldoEmpenhoObra);
			
			// VALOR DA OBRA
			$sql 		= "SELECT prevalorobra::numeric(20,2) FROM obras.preobra WHERE preid =".$preid;
			$valorObra 	= $db->pegaUm($sql);
			$diferencaParaObra = round($saldoEmpenhoObra - $valorObra, 2);
			
			// VALOR PAGO DA OBRA
			$sql = "SELECT sum(po.pobvalorpagamento) 
					FROM par.pagamento p
					INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
					WHERE 	p.pagstatus = 'A' 
						AND  (pagsituacaopagamento <> 'CANCELADO' OR pagsituacaopagamento <> '9 - CANCELADO' ) 
						AND po.preid = ".$preid." AND empid = ".$empid;
			$valorPagamentoObra = $db->pegaUm($sql);
			
			//ver($saldoEmpenhoObra,$valorObra );
			if($saldoEmpenhoObra > $valorObra ){ // se o valor do empenho da obra for maior que o valor da obra
					$arrObrasEmpenho[] = array( "preid"=>$preid, 
												"saldoEmpenhoObra"=>$saldoEmpenhoObra, 
												"eobvalorempenho"=>$eobvalorempenho,
												"valorObra"=>$valorObra, 
												"ValorEmpenho"=>$valorEmpenho,
												"valorPagamentoObra"=>$valorPagamentoObra, 
												"diferencaParaObra"=>$diferencaParaObra, 
												"eobid"=>$eobid 
												);
			}else if ($saldoEmpenhoObra < $valorObra ) {
				$arrObrasEmpenhoMenor[] = array( "preid"=>$preid, 
												"saldoEmpenhoObra"=>$saldoEmpenhoObra, 
												"eobvalorempenho"=>$eobvalorempenho,
												"valorObra"=>$valorObra, 
												"ValorEmpenho"=>$valorEmpenho, 
												"valorPagamentoObra"=>$valorPagamentoObra,
												"diferencaParaObra"=>$diferencaParaObra, 
												"eobid"=>$eobid 
												);
			}else{
				$arrObrasEmpenhoIgual[] = array( "preid"=>$preid, 
												"saldoEmpenhoObra"=>$saldoEmpenhoObra, 
												"eobvalorempenho"=>$eobvalorempenho,
												"valorObra"=>$valorObra, 
												"ValorEmpenho"=>$valorEmpenho, 
												"valorPagamentoObra"=>$valorPagamentoObra,
												"diferencaParaObra"=>$diferencaParaObra, 
												"eobid"=>$eobid 
												);
			}
		}
		//ver($arrObrasEmpenho,$arrObrasEmpenhoMenor, $arrObrasEmpenhoIgual, d );
		// EXECUTANDO ATUALIZAÇÕES DE VALORES DE EMPENHO
		// MAIOR
		if(count($arrObrasEmpenho) > 0){
			if($qtdObrasNoEmpenho > 1){
				foreach($arrObrasEmpenho as $dados){
					//ver($dados['diferencaParaObra']);
					$valorSendoReduzido =  $valorSendoReduzido + $dados['diferencaParaObra'];
					if( $valorSendoReduzido <= $diferenca  ){
						$valorAtualizar = $dados['eobvalorempenho'] - $dados['diferencaParaObra'];
						// SE TENHO PAGAMENTO E O VALOR A REDUZIR FOR MENOR QUE O VALOR PAGO POSSO REDUZIR
						if($dados['valorPagamentoObra'] && $valorAtualizar >= $dados['valorPagamentoObra'] ){
							$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid'];
							ver('1',$sqlUpdata);
							$db->executar($sqlUpdata);
						}else if($dados['valorPagamentoObra'] && $valorAtualizar < $dados['valorPagamentoObra']){
								$valorSendoReduzido =  $valorSendoReduzido - $dados['diferencaParaObra'];
								continue;
						}else if($dados['valorPagamentoObra'] == null){
							$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid'];
							ver('2',$sqlUpdata);
							$db->executar($sqlUpdata);
						}	
					}
				}
			}else{ // SE TIVER APENAS 1 OBRA NO EMPENHO	
				$valorSendoReduzido =  $diferenca;
				$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".$arrObrasEmpenhoIgual[0]['ValorEmpenho']."  WHERE preid = ".$arrObrasEmpenhoIgual[0]['preid']." AND eobid =".$arrObrasEmpenhoIgual[0]['eobid']."; ";
				ver('3',$sqlUpdata);
				$db->executar($sqlUpdata);
			}
		}

		// MENOR
		if(count($arrObrasEmpenhoMenor) > 0){
			if($qtdObrasNoEmpenho > 1){

				if( (count($arrObrasEmpenhoMenor) % 2) == 0 && $diferenca != "0.01"  ){
					$retirarDaObra = ($diferenca - $valorSendoReduzido) / count($arrObrasEmpenhoMenor) ;
				}else{
					$retirarDaObra = ($diferenca - $valorSendoReduzido);
				}
				//ver($retirarDaObra );
				foreach($arrObrasEmpenhoMenor as $dados){
					$valorSendoReduzido =  $valorSendoReduzido + $retirarDaObra;
					if( $valorSendoReduzido <= $diferenca  ){
						//ver($valorSendoReduzido, $diferenca);
						$valorAtualizar = $dados['eobvalorempenho'] - $retirarDaObra;
						// SE TENHO PAGAMENTO E O VALOR A REDUZIR FOR MENOR QUE O VALOR PAGO POSSO REDUZIR
						if($dados['valorPagamentoObra'] && $valorAtualizar >= $dados['valorPagamentoObra'] ){
							$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid']."; ";
							ver('4',$sqlUpdata);
							$db->executar($sqlUpdata);
						}else if($dados['valorPagamentoObra'] && $valorAtualizar < $dados['valorPagamentoObra']){
								$valorSendoReduzido =  $valorSendoReduzido - $retirarDaObra;
								continue;
						}else if($dados['valorPagamentoObra'] == null){
							$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid']."; ";
							ver('5',$sqlUpdata);
							$db->executar($sqlUpdata);
						}
					}
					
				}
			}else{ // SE TIVER APENAS 1 OBRA NO EMPENHO	
				$valorSendoReduzido =  $diferenca;
				$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".$arrObrasEmpenhoMenor[0]['ValorEmpenho']."  WHERE preid = ".$arrObrasEmpenhoMenor[0]['preid']." AND eobid =".$arrObrasEmpenhoMenor[0]['eobid']."; ";
				ver('6',$sqlUpdata);
				$db->executar($sqlUpdata);
			}
		}
		
		// IGUAL
		if(count($arrObrasEmpenhoIgual) > 0){
			if($qtdObrasNoEmpenho > 1){
				if(count($arrObrasEmpenhoMenor) == 0 && count($arrObrasEmpenho) == 0   ){
					if( (count($arrObrasEmpenhoIgual) % 2) == 0  && $diferenca != "0.01" ){
						$retirarDaObra = ($diferenca - $valorSendoReduzido) / count($arrObrasEmpenhoIgual) ;
					}else{
						$retirarDaObra = ($diferenca - $valorSendoReduzido);
					}
					
					foreach($arrObrasEmpenhoIgual as $dados){

						$valorSendoReduzido =  $valorSendoReduzido + $retirarDaObra;
						//ver($valorSendoReduzido,1);
						//ver($valorSendoReduzido <= $diferenca);
						if( $valorSendoReduzido <= $diferenca  ){
							$valorAtualizar = $dados['eobvalorempenho'] - $retirarDaObra;
							// SE TENHO PAGAMENTO E O VALOR A REDUZIR FOR MENOR QUE O VALOR PAGO POSSO REDUZIR
							//ver( $dados['valorPagamentoObra'] , $valorAtualizar , $dados['valorPagamentoObra']);
							if($dados['valorPagamentoObra'] && $valorAtualizar >= $dados['valorPagamentoObra'] ){
								$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid']."; ";
								$db->executar($sqlUpdata);
								ver('7',$sqlUpdata);
							}else if($dados['valorPagamentoObra'] && $valorAtualizar < $dados['valorPagamentoObra']){
								$valorSendoReduzido =  $valorSendoReduzido - $retirarDaObra;
								//continue;
							}else if($dados['valorPagamentoObra'] == null){
								$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid']."; ";
								$db->executar($sqlUpdata);
								ver('8',$sqlUpdata);
							}
						}
					}					
				}
			}else{ // SE TIVER APENAS 1 OBRA NO EMPENHO	
					$valorSendoReduzido =  $diferenca;
					$sqlUpdata = "UPDATE par.empenhoobra SET eobvalorempenho = ".$arrObrasEmpenhoIgual[0]['ValorEmpenho']."  WHERE preid = ".$arrObrasEmpenhoIgual[0]['preid']." AND eobid =".$arrObrasEmpenhoIgual[0]['eobid']."; ";
					ver('9',$sqlUpdata);
					$db->executar($sqlUpdata);
			}
		}
		//ver($valorSendoReduzido, $diferenca);
		//die();
		recompoem($composicao, $valorSendoReduzido, $db);
		//return  $valorSendoReduzido;	
	}else{ //if($valorSendoReduzido >= $diferenca )
		return false;	
	}
}
?>