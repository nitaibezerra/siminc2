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
$sql = "select distinct empid, empvalorempenho, empnumero, empnumeroprocesso, diferenca 
from (
select distinct e.empid, e.empvalorempenho, e.empnumero, e.empnumeroprocesso,  ( sum(o.eobvalorempenho) - e.empvalorempenho ) as diferenca -- , p.proid , e.empvalorempenho, sum(o.eobvalorempenho) as empenhocomposicao 
		from par.empenho  e
		inner join par.empenhoobra o on e.empid = o.empid
		inner join par.processoobra p on p.pronumeroprocesso = e.empnumeroprocesso and p.prostatus = 'A'
		where e.empstatus = 'A' and o.eobstatus = 'A' -- and e.empcodigoespecie not in ( '03', '04', '13') 
		-- and e.empid in (20972)  
		group by e.empid , e.empvalorempenho, e.empnumero, e.empnumeroprocesso -- , p.proid, e.empvalorempenho
		having  ( sum(o.eobvalorempenho) - e.empvalorempenho ) >= 0.01 order by e.empid
) foo
group by empid, empvalorempenho, empnumero, empnumeroprocesso, diferenca 
having diferenca < 1
		";

$recompor = $db->carregar($sql);
//dbg($recompor,1);
if(is_array($recompor)){

	foreach($recompor as $chave=>$composicao){
		$valorSendoReduzido = 0;
		recompoem($composicao, $valorSendoReduzido, $db);
		//$db->commit();
		echo "------------- NE {$composicao['empnumero']} ------------- <br>
				Script executado com sucesso para NE ".$composicao['empnumero'].". <br> 
				Processo: ".$composicao['empnumeroprocesso']." <br>
			------------------------------------------------------------------ 
			<br><br>";
		
	}

}

function recompoem($composicao, $valorSendoReduzido, $db){
	$valorSendoReduzido = $valorSendoReduzido;
	$diferenca 			= $composicao['diferenca'];
		
	if($valorSendoReduzido < $diferenca ){
		// QUANTIDADE DE OBRAS NO EMPENHO
		$sql = "SELECT count(preid)
				FROM par.empenhoobra 
				WHERE 	empid =".$composicao["empid"]." 
						and eobstatus = 'A'";
		$qtdObrasNoEmpenho =  $db->pegaUm($sql);
		
		// DADOS DO PAR.EMPENHOOBRA
		$sql = "SELECT eobid, preid, empid, SUM(eobvalorempenho) as eobvalorempenho   
				FROM par.empenhoobra 
				WHERE 	empid =".$composicao["empid"]." 
						and eobstatus = 'A'
				GROUP BY eobid, preid, empid";
		$dadosComposicaoPorEmpidDaObra = $db->carregar($sql);
		
		$valorEmpenho 					= $composicao['empvalorempenho'];
		$contObrasSaldoMaiorQueVlrObra 	= 0;
		$contObrasSaldoMenorQueVlrObra 	= 0;
		$arrObrasEmpenho 				= array();
		$arrObrasEmpenhoMenor 			= array();
		$arrObrasEmpenhoIgual 			= array();

		foreach($dadosComposicaoPorEmpidDaObra as $dadosObra){
			$preid 				= $dadosObra['preid'];
			$eobid				= $dadosObra['eobid'];
			$empid 				= $dadosObra['empid'];
			$eobvalorempenho 	= $dadosObra['eobvalorempenho'];
			// SALDO DE EMPENHO DA OBRA
			$sqlsaldoEmpenhoObra = "SELECT saldo::numeric(20,2) FROM par.vm_saldo_empenho_por_obra WHERE preid =".$preid;
			$saldoEmpenhoObra 	 = $db->pegaUm($sqlsaldoEmpenhoObra);
			
			// VALOR DA OBRA
			$sql 				= "SELECT prevalorobra::numeric(20,2) FROM obras.preobra WHERE preid =".$preid;
			$valorObra 			= $db->pegaUm($sql);
			
			$diferencaParaObra 	= round($saldoEmpenhoObra - $valorObra, 2);
			
			// VALOR PAGO DA OBRA
			$sql = "SELECT sum(po.pobvalorpagamento) 
					FROM par.pagamento p
					INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
					WHERE 	p.pagstatus = 'A' 
							AND  (pagsituacaopagamento <> 'CANCELADO' OR pagsituacaopagamento <> '9 - CANCELADO' ) 
							AND po.preid = ".$preid." AND empid = ".$empid;
			$valorPagamentoObra = $db->pegaUm($sql);
			
			// MONTA ARRAY COM OBRAS 
			if($saldoEmpenhoObra > $valorObra ){ // se o valor do empenho da obra for maior que o valor da obra
					$arrObrasEmpenho[] = array( "preid"=>$preid, 
												"saldoEmpenhoObra"=>$saldoEmpenhoObra, 
												"eobvalorempenho"=>$eobvalorempenho,
												"valorObra"=>$valorObra, 
												"ValorEmpenho"=>$valorEmpenho,
												"valorPagamentoObra"=>$valorPagamentoObra, 
												"diferencaParaObra"=>$diferencaParaObra, 
												"eobid"=>$eobid,
												"empid"=>$empid 
												);
			}else if ($saldoEmpenhoObra < $valorObra ) {
				$arrObrasEmpenhoMenor[] = array( "preid"=>$preid, 
												"saldoEmpenhoObra"=>$saldoEmpenhoObra, 
												"eobvalorempenho"=>$eobvalorempenho,
												"valorObra"=>$valorObra, 
												"ValorEmpenho"=>$valorEmpenho, 
												"valorPagamentoObra"=>$valorPagamentoObra,
												"diferencaParaObra"=>$diferencaParaObra, 
												"eobid"=>$eobid,
												"empid"=>$empid  
												);
			}else{
				$arrObrasEmpenhoIgual[] = array( "preid"=>$preid, 
												"saldoEmpenhoObra"=>$saldoEmpenhoObra, 
												"eobvalorempenho"=>$eobvalorempenho,
												"valorObra"=>$valorObra, 
												"ValorEmpenho"=>$valorEmpenho, 
												"valorPagamentoObra"=>$valorPagamentoObra,
												"diferencaParaObra"=>$diferencaParaObra, 
												"eobid"=>$eobid,
												"empid"=>$empid  
												);
			}
		}
		//ver($arrObrasEmpenhoMenor  );
		//----------------- EXECUTANDO ATUALIZAÇÕES DE VALORES DE EMPENHO -----------------//
		// MAIOR = EMPENHO DA OBRA MAIOR QUE O VALOR DA OBRA
		
		if(count($arrObrasEmpenho) > 0){
			if($qtdObrasNoEmpenho > 1){
				foreach($arrObrasEmpenho as $dados){
					$valorSendoReduzido =  $valorSendoReduzido + $dados['diferencaParaObra']; // contagem para função recursiva
					$valorAtualizar = $dados['eobvalorempenho'] - $dados['diferencaParaObra'];
					//ver($valorSendoReduzido, $diferenca);
					if( $valorSendoReduzido <= $diferenca && $dados['diferencaParaObra'] > 0  ){
						$sqlUpdate = "UPDATE par.empenhoobra SET eobvalorempenho = ".($valorAtualizar)."  WHERE preid = ".$dados['preid']." AND eobid =".$dados['eobid'];
						$db->executar($sqlUpdate);
					//	ver('1',$sqlUpdate);
					}else{
						break;
					}
				}
			}else{ // SE TIVER APENAS 1 OBRA NO EMPENHO	
				$valorSendoReduzido =  $diferenca;
				$sqlUpdate = "UPDATE par.empenhoobra SET eobvalorempenho = ".$arrObrasEmpenho[0]['ValorEmpenho']."  WHERE preid = ".$arrObrasEmpenho[0]['preid']." AND eobid =".$arrObrasEmpenho[0]['eobid']."; ";
			//	ver('2',$sqlUpdate);
				$db->executar($sqlUpdate);
			}
		}

		$db->commit();
		//recompoem($composicao, $valorSendoReduzido, $db);
	}else{ //if($valorSendoReduzido >= $diferenca )
		return false;	
	}
	
	if(count($arrObrasEmpenhoMenor)  == 0 && count($arrObrasEmpenho)  == 0){
		if(count($arrObrasEmpenhoIgual) > 0 && $diferenca == '0.01' ){
				$sqlUpdate = "UPDATE par.empenhoobra SET eobvalorempenho = ".($arrObrasEmpenhoIgual[0]['eobvalorempenho'] - 0.01)."  WHERE preid = ".$arrObrasEmpenhoIgual[0]['preid']." AND eobid =".$arrObrasEmpenhoIgual[0]['eobid'];
				//ver('3',$sqlUpdate);
				$db->executar($sqlUpdate);
				$db->commit();
		}
	}
	
	if(count($arrObrasEmpenhoIgual)  == 0 && count($arrObrasEmpenho)  == 0){
		if(count($arrObrasEmpenhoMenor) > 0 && $diferenca == '0.01' ){
				$sqlUpdate = "UPDATE par.empenhoobra SET eobvalorempenho = ".($arrObrasEmpenhoMenor[0]['eobvalorempenho'] - 0.01)."  WHERE preid = ".$arrObrasEmpenhoMenor[0]['preid']." AND eobid =".$arrObrasEmpenhoMenor[0]['eobid'];
				//ver('4',$sqlUpdate);
				$db->executar($sqlUpdate);
				$db->commit();
		}
	}
}

?>