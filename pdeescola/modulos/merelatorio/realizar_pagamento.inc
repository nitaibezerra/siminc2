<?

ini_set( "memory_limit", "256M" );
set_time_limit(0);

include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'www/pdeescola/pdeWs.php';

if($_REQUEST["submetido"])
{
	$listaEscolas = $db->carregarColuna("SELECT entid FROM pdeescola.meloteimpressao WHERE mlicodigolote in (".$_REQUEST["lista_escolas"].") ORDER BY mliid");
	$listaEscolas = implode(',',$listaEscolas);
	
	$sql = "SELECT
				trim(ent.entcodent) AS entcodent,
				trim(ent.entnome) AS entnome,
				to_char(max(hid.htddata), 'DD/MM/YYYY') AS htddata,
				mme.memid,
				mme.docid,
				coalesce(mme.memvlrsaldodisponivel, 0) AS memvlrprimeiraparcela,
				esd.esdid,
				trim(mme.memmodalidadeensino) AS memmodalidadeensino,
				ent.entid,
				mme.memsaldo
			FROM
				entidade.entidade ent
			INNER JOIN
				pdeescola.memaiseducacao mme ON mme.entid = ent.entid AND mme.memanoreferencia = " . $_SESSION["exercicio"] . " AND mme.memstatus = 'A'
			INNER JOIN
				workflow.documento doc ON doc.docid = mme.docid
			INNER JOIN
				workflow.historicodocumento hid ON hid.docid = doc.docid AND hid.aedid = 68
			INNER JOIN
				workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.esdid = 39
			WHERE
				ent.entid in (".$listaEscolas.")
			GROUP BY
				ent.entcodent,
				ent.entnome,
				mme.memid,
				mme.docid,
				mme.memvlrsaldodisponivel,
				esd.esdid,
				mme.memmodalidadeensino,
				ent.entid,
				mme.memsaldo";
	$entidades = $db->carregar($sql);
	
	// criando os arrays para armazenar a situação da tramitação para cada escola
	$arrErros['pagamento_realizado'] 	= array(); 
	$arrErros['erro_web_service'] 		= array();
	$arrErros['erro_valor_total'] 		= array();
	
	for($i=0; $i<count($entidades); $i++) {
		if($_REQUEST["pagamento"][(integer)$entidades[$i]["entcodent"]]) {
			$saldoAnoAnterior = (float)$entidades[$i]["memvlrprimeiraparcela"];
			
			// Inicializa as variáveis p/ os cálculos
			$kitCusteio 			= 0;
			$kitCapital 			= 0;
			$ressarcimentoMonitores = 0;
			$valorLimite 			= 0;
			$servicosMateriais 		= 0;
			$pagamentoProfessorPST	= 0;
			$escolaAbertaCapital	= 0;
			$escolaAbertaCusteio	= 0;
			
			/*** INICIO - Regras para cálculo do pagamento de escolas 2010/2009 - INICIO ***/
			if((integer)$_SESSION["exercicio"] == 2010) {
				
				// verifica se a escola participou em 2009
				$dadosEscola2009 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = 2009");
				
				// escola de 2010
				if(!$dadosEscola2009) {					
					// Quantidade de meses
					$meses = 10;
					// Chama a função para o cálculo
					calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					$valorLimite = 999999;
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses);
				}
				// participou em 2009
				else {
					
					// não foi paga em 2009
					if( $dadosEscola2009[0]["mempagofnde"] == 'f' )
					{
						// Quantidade de meses
						$meses = 10;
						
						/*** Atividades de 2010 ***/
						// Chama a função para o cálculo
						calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
						$valorLimite = 999999;
						// Chama a função para cálculo de Serviços e Materiais
						calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses);
					}
					// foi feito o pagamento em 2009
					else {
						$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola2009[0]["memid"]." AND meacomecounoano = 't'");
						
						// Escola iniciou as atividades
						if((integer)$iniciouAnoAnterior > 0) {
							// Quantidade de meses
							$meses = 10;
							// Chama a função para o cálculo
							calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
							$valorLimite = 999999;
							// Chama a função para cálculo de Serviços e Materiais
							calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses);
						}
						// Escola não iniciou as atividades
						else {
							// Quantidade de meses
							$meses = 4;
							/*** Atividades de 2009 ***/
							// Chama a função para o cálculo
							calculoKitsRessarcimento($dadosEscola2009[0]["memid"], 2009, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses, true, true);
							// Chama a função para cálculo do valor limite
							calculoValorLimite($dadosEscola2009[0]["memid"], 2009, 6, $valorLimite, $meses);
							// Chama a função para cálculo de Serviços e Materiais
							calculoServicosMateriais($dadosEscola2009[0]["memid"], 2009, $servicosMateriais, $meses);
							
							// Quantidade de meses
							$meses = 10;
							/*** Atividades de 2010 ***/
							// Chama a função para o cálculo
							calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
							// Chama a função para cálculo do valor limite
							calculoValorLimite($entidades[$i]["memid"], $_SESSION["exercicio"], 6, $valorLimite, $meses);
						}
					}
				}
			}
			// Escolas de 2008 e 2009
			else {
				
				$flag = true;
				$dadosEscolaAnoAnterior = $db->carregar("SELECT memid,mempagofnde FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
				
				if($dadosEscolaAnoAnterior)
				{
					$temAtividades = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']."");
						
					if( $temAtividades > 0 )
					{
						if( $temAtividades != $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano is null") )
						{
							$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano = 't'");
								
							if( (integer)$iniciouAnoAnterior == 0 && $dadosEscolaAnoAnterior[0]['mempagofnde'] == 't' )
							{
								// não paga nada
								$flag = false;
							}
						}
					}
				}
				
				if($flag)
				{
					// Quantidade de meses usada nos anos de 2008 e 2009
					$meses = 6;
										
					// Chama a função para o cálculo
					calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					
// 					if( in_array( (integer)$_SESSION["exercicio"], array(2012, 2013) ) ) {
					if((integer)$_SESSION["exercicio"] == 2012) {
						if($pagamentoProfessorPST > 0){
							$ressarcimentoMonitores += $pagamentoProfessorPST;
							$pagamentoProfessorPST = 0;
						}
					}
					
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
				}
			}
			/*** FIM - Regras para cálculo do pagamento de escolas 2010/2009 - FIM ***/
			
			/*--------------------------------nova regra-------------------------------------*/
			if($calculoServicosMateriaisCapital || $calculoServicosMateriaisCusteio){
			
				$ano = $_SESSION["exercicio"];
			
				$servicosMateriais = $calculoServicosMateriaisCapital+$calculoServicosMateriaisCusteio;
			
				$kitCusteioTemp = $kitCusteio;
				$kitCapitalTemp = $kitCapital;
			
				$kitCusteio = $ressarcimentoMonitores+$calculoServicosMateriaisCusteio+$kitCusteio;
				$kitCapital = $kitCapital+$calculoServicosMateriaisCapital+$escolaAbertaCapital;
			
				$sql = "select memjovem1517, mamescolaaberta, mempeif from pdeescola.memaiseducacao  where memid = {$entidades[$i]["memid"]}";
				$rsAtividades = $db->pegaLinha($sql);
			
				if($rsAtividades['memjovem1517'] == 't'){
			
					$calculoJovem15a17 = 0;
			
					calculoJovem15a17($entidades[$i]["memid"], $ano, $meses, $calculoJovem15a17);
			
					$jovem1517custeio = $calculoJovem15a17>0 ? 5000 : 0;
					$jovem1517capital = $calculoJovem15a17>0 ? 2000 : 0;
			
					$kitCusteio = $kitCusteio+$calculoJovem15a17+$jovem1517custeio;
					$kitCapital = $kitCapital+$jovem1517capital;
						
				}
				if($rsAtividades['mamescolaaberta'] == 't'){
			
					$escolaAbertaCapital = 0;
					$escolaAbertaCusteio = 0;
			
					calculoEscolaAberta($entidades[$i]["memid"], $ano, $escolaAbertaCapital, $escolaAbertaCusteio, $meses);
			
					$kitCusteio = $kitCusteio+$escolaAbertaCusteio;
					$kitCapital = $kitCapital+$escolaAbertaCapital;
						
				}
				if($rsAtividades['mempeif'] == 't'){
			
					$peifCusteio = 0;
					$peifCapital = 0;
			
					calculoPeif($entidades[$i]["memid"], $peifCusteio, $peifCapital, $ano);
				}
			}
			
			$totalGeral = ($kitCusteio + $kitCapital);
				
			if($totalGeral < 0) $totalGeral = 0;
			
			/*--------------------------------nova regra-------------------------------------*/
			
			/*-------novo-------*/
			$saldoTotalPagar = ($totalGeral - $entidades[$i]["memsaldo"]);
				
			if($saldoTotalPagar < 0) $saldoTotalPagar = 0;
				
			$repasseCusteio = $totalGeral>0 ? round( (($kitCusteio/$totalGeral)*$saldoTotalPagar)+$peifCusteio, 2 ) : $totalGeral;
			$repasseCapital = $totalGeral>0 ? round( (($kitCapital/$totalGeral)*$saldoTotalPagar)+$peifCapital, 2 ) : $totalGeral;
				
			$repasseTotal = ($repasseCusteio+$repasseCapital);
			/*-------novo-------*/
			
			$valorKits = ($kitCusteio + $kitCapital);
			
			/*** Regra da Clarissa (28/03/2011) ***/
			$porcentagemCusteio = ( $servicosMateriais * 0.8 );
			$porcentagemCapital = ( $servicosMateriais * 0.2 );
				
			// Calcula os Totais
			$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $porcentagemCusteio - $saldoAnoAnterior);
			
			//$totalCapital = $kitCapital;
			$totalCapital = ($kitCapital + $porcentagemCapital);
			
			// novo cálculo definido pelo Leandro
			if($totalCusteio < 0) {
				if(abs($totalCusteio) > $totalCapital)
					$totalCusteio = (($totalCapital) * (-1));
			}
			
			if( in_array( (integer)$_SESSION["exercicio"], array(2012, 2013) ) ) {
				
				$totalapagar = 0;
				$totalServicos = ($ressarcimentoMonitores + $servicosMateriais + $escolaAbertaCusteio + $escolaAbertaCapital); 
	
				//total a pagar soma 60% serviços
				$totalapagar += ($totalServicos * 0.6);
				//total a pagar soma 100% kits
				$totalapagar += ($kitCusteio + $kitCapital);
				
// 				$servicosMateriaisCusteio = ($servicosMateriais * 0.8);
// 				$servicosMateriaisCapital = ($servicosMateriais * 0.2);

				$servicosMateriaisCusteio = $calculoServicosMateriaisCusteio;
				$servicosMateriaisCapital = $calculoServicosMateriaisCapital;
			
				$textoTramitacao  = "Cód INEP: ".$entidades[$i]["entcodent"]." || Nome Escola: ".$entidades[$i]["entnome"]." || Data da Aprovação: ".$entidades[$i]["htddata"];
// 				$textoTramitacao .= " || Ressarcimento Monitores: R$".number_format($ressarcimentoMonitores, 2, ",", ".");
// 				$textoTramitacao .= " || Serviços/Mat. Cons.: R$".number_format($servicosMateriaisCusteio, 2, ",", ".")."(custeio) / R$".number_format($servicosMateriaisCapital, 2, ",", ".")."(capital) - Total: R$".number_format(($servicosMateriais), 2, ",", ".");
// 				$textoTramitacao .= " || Kits Materiais: R$".number_format($kitCusteio, 2, ",", ".")."(custeio) / R$".number_format($kitCapital, 2, ",", ".")."(capital) - Total: R$".number_format($valorKits, 2, ",", ".");
// 				$textoTramitacao .= " || Total Mais Educação: R$".number_format($totalGeral, 2, ",", ".");
// 				$textoTramitacao .= " || Escola Aberta: R$".number_format($escolaAbertaCusteio, 2, ",", ".")."(custeio) / R$".number_format($escolaAbertaCapital, 2, ",", ".")."(capital) - Total: R$".number_format(($escolaAbertaCusteio+$escolaAbertaCapital), 2, ",", ".");
// 				$textoTramitacao .= " || Total Geral: R$".number_format($totalGeral, 2, ",", ".");
// 				$textoTramitacao .= " || Total Kits: R$".number_format($valorKits, 2, ",", ".");
// 				$textoTramitacao .= " || Total Serviços: R$".number_format($totalServicos, 2, ",", ".");
// 				$textoTramitacao .= " || Total a Pagar (100% Kits + 60% Serviços): R$".number_format($totalapagar, 2, ",", ".");				
				$textoTramitacao .= " || Repasse (custeio): R$".number_format($repasseCusteio, 2, ",", ".")."";
				$textoTramitacao .= " || Repasse (capital): R$".number_format($repasseCapital, 2, ",", ".")."";
				$textoTramitacao .= " || Total do Repasse: R$".number_format($repasseTotal, 2, ",", ".")."";
				
			}
			else{
				$textoTramitacao  = "Cód INEP: ".$entidades[$i]["entcodent"]." || Nome Escola: ".$entidades[$i]["entnome"]." || Data da Aprovação: ".$entidades[$i]["htddata"];
				$textoTramitacao .= " || Ressarcimento Monitores: R$".number_format($ressarcimentoMonitores, 2, ",", ".")." || Serviços/Materiais: R$".number_format($servicosMateriais, 2, ",", ".");
				$textoTramitacao .= " || Total dos Kits Materiais: R$".number_format($totalCusteio, 2, ",", ".")."(custeio) / R$".number_format($totalCapital, 2, ",", ".")."(capital) - Total: R$".number_format(($totalCusteio+$totalCapital), 2, ",", ".");
				$textoTramitacao .= " || Saldo Ano Anterior: R$".number_format($saldoAnoAnterior, 2, ",", ".")." || Total Geral: R$".number_format($totalGeral, 2, ",", ".");

			}
			
			
			// Recupera o 'docid' da entidade
			$docid = $entidades[$i]["docid"];
			$docid = (integer)$docid;
			// O 'aedid' da ação: finalizado -> relatório emitido. (produção)
			$aedid = 215;
			$aedid = (integer)$aedid;
			// comentário
			$comentario = $textoTramitacao;
			// Sem dados no array já que não há chamada de função após a ação.
			$dados = array();
			
			//wf_alterarEstado( $docid, $aedid, $comentario, $dados );
			
			$anoExercicio 	= (string)$_SESSION["exercicio"];
			$entcodent 		= $entidades[$i]["entcodent"];
			$coProgramaFNDE = 'Z9';
			$coDestinacao 	= ($entidades[$i]["memmodalidadeensino"] == 'F') ? '01' : '02';			
			/*** Web Service de Produção ***/
			$tipoConexao	= 'P';
			/*** Web Service de Desenvolvimento ***/
// 			$tipoConexao	= 'D';
			
			if( in_array((integer)$_SESSION["exercicio"], array(2012, 2013)) ) {
				
// 				$wsCusteioMec = ($ressarcimentoMonitores + $servicosMateriaisCusteio + $escolaAbertaCusteio);
// 				$wsCusteio = ($wsCusteioMec) * 0.6;				
// 				$wsCusteio += $kitCusteio;
// 				$wsCusteioMec += $kitCusteio;
				
// 				$wsCapitalMec = ($servicosMateriaisCapital + $escolaAbertaCapital);
// 				$wsCapital = ($wsCapitalMec) * 0.6;
// 				$wsCapital += $kitCapital;
// 				$wsCapitalMec += $kitCapital;
				
// 				$wsValorTotal = ($wsCusteio + $wsCapital);		
// 				$wsValorTotalMec = ($wsCusteioMec + $wsCapitalMec);
								
				$wsCusteio 		= $repasseCusteio; 
				$wsCapital 		= $repasseCapital; 
				$wsValorTotal 	= $repasseTotal;
			}
			else{
				$wsCusteio = $totalCusteio + $escolaAbertaCusteio;
				$wsCapital = $totalCapital + $escolaAbertaCapital;
				
				$wsValorTotal = ($wsCusteio + $wsCapital);	
			}
			
			if($wsValorTotal < 0) $wsValorTotal = 0;
			
			// novo cálculo definido pelo Leandro
			if($wsCusteio < 0) {
				if(abs($wsCusteio) > $kitCapital)
					$wsCusteio = (($kitCapital) * (-1));
			}
			
			if($wsValorTotal >= 0) {

				$ws 	   = new pdeWs();
				$retornoWs = $ws->maisEducacaoWs('atualizaValoresEstimativa', $anoExercicio, $entcodent, $coProgramaFNDE, $tipoConexao, $coDestinacao, $wsCusteio, $wsCapital, $wsValorTotal);
// 				$retornoWs = $ws->maisEducacaoWs('atualiza-valor-estimativa', $anoExercicio, $entcodent, $coProgramaFNDE, $tipoConexao, $coDestinacao, $wsCusteio, $wsCapital, $wsValorTotal);
 		
				if((integer)$retornoWs === 1) {
					
					// Realiza a alteração do estado da entidade no ano corrente.
					wf_alterarEstado( $docid, $aedid, $comentario, $dados );
					
					// Atualiza o campo 'memvlrpago' com o valor enviado ao WS
					if(!$wsValorTotalMec) $wsValorTotalMec = 0;
					if(!$totalServicos) $totalServicos = 0;
					
					if((integer)$_SESSION["exercicio"] == 2012) {
						$db->executar("UPDATE pdeescola.memaiseducacao SET memvlrpago = ".$wsValorTotalMec.", memvlrpago_servico = ".$totalServicos.", memperc_pago = 60 WHERE memid = ".(integer)$entidades[$i]["memid"]);
					}
					else{
						$db->executar("UPDATE pdeescola.memaiseducacao SET memvlrpago = ".$wsValorTotal." WHERE memid = ".(integer)$entidades[$i]["memid"]);
					}
					
					array_push($arrErros['pagamento_realizado'], $entidades[$i]["entnome"]);
					
				} else {
					//array_push($arrErros['erro_web_service'], $retornoWs);
					array_push($arrErros['erro_web_service'], $entidades[$i]["entnome"]);
					
					$codErro = $retornoWs;
				}
				
			} else {
				
				array_push($arrErros['erro_valor_total'], $entidades[$i]["entnome"]);
				
			}
		}
	}

	
	// Monta a mensagem que será mostrada no alert para o usuário
	$textoPagamento = "";
	// escolas pagas
	if( !empty($arrErros['pagamento_realizado']) ) {
		// commita os updates realizados na tabela 'pdeescola.memaiseducacao'
		$db->commit();
		
		if( empty($arrErros['erro_valor_total']) && empty($arrErros['erro_web_service']) ) {
			
			$textoPagamento .= "O pagamento foi enviado com sucesso para todas as escolas selecionadas.";
			
		} else {
			
			$textoPagamento .= "O pagamento foi enviado com sucesso para as escolas:\\n";
			
			for($i=0; $i<count($arrErros['pagamento_realizado']); $i++) {
				$textoPagamento .= "\\t- ".$arrErros['pagamento_realizado'][$i]."\\n";
				
				// limite (10) para o alert não ficar grande demais...
				if($i==9) {
					$textoPagamento .= "\\t- (...)\\n";
					break;
				}
			}
			
			$textoPagamento .= "\\n";
			
		}
	}
	// erro com o valor total
	if( !empty($arrErros['erro_valor_total']) ) {
		$textoPagamento .= "As escolas abaixo não tiveram o pagamento realizado, devido ao Valor Total ser menor ou igual à 0(zero):\\n";
		
		for($i=0; $i<count($arrErros['erro_valor_total']); $i++) {
			$textoPagamento .= "\\t- ".$arrErros['erro_valor_total'][$i]."\\n";
		}
		
		$textoPagamento .= "\\n";
	}
	// erro com o Web Service
	if( !empty($arrErros['erro_web_service']) ) {
		$textoPagamento .= "As seguintes escolas não tiveram o pagamento realizado devido a algum erro ao utilizar o Web Service (descrição do erro: \"".$codErro."\"):\\n";
		
		for($i=0; $i<count($arrErros['erro_web_service']); $i++) {
			$textoPagamento .= "\\t- ".$arrErros['erro_web_service'][$i]."\\n";
		}
		
		$textoPagamento .= "\\n";
		$textoPagamento .= "Entre em contato com a equipe técnica do sistema.";
	}
	
	echo "<script type=\"text/javascript\">
			alert('".$textoPagamento."');
			
			//if(window.opener && !window.opener.closed) {
				//window.opener.location.reload();
			//}
			
			//self.close();
		  </script>";
}

?>

<style>
table {
	font-size: 14px;
}
</style>
<script>var arrEntcodent = new Array();</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<form id="formPagamento" method="post" action="">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="lista_escolas" value="<?=$_REQUEST["lista_escolas"]?>" />
<table class="tabela" border="1" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" align="center" style="border: 1px solid black">
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="2">
			<b>Total para Pagamento</b>
		</td>
	</tr>
	<tr>
		<td align="center" bgcolor="#c0c0c0" width="70%">
			<b>Dados da Escola</b>
		</td>
		<td align="center" bgcolor="#c0c0c0" width="30%">
			<b>Realizar Pagamento</b>
		</td>
	</tr>
	<tr>
		<td align="center" bgcolor="#c0c0c0" width="70%">&nbsp;</td>
		<td align="center" bgcolor="#c0c0c0" width="30%">
			<b>Marcar Todos</b> <input type="checkbox" id="chMarcaTodos" onclick="marcaTodos();" />
		</td>
	</tr>
<?

if($_REQUEST["lista_escolas"]) {
	
	$listaEscolas = $db->carregarColuna("SELECT entid FROM pdeescola.meloteimpressao WHERE mlicodigolote in (".$_REQUEST["lista_escolas"].") ORDER BY mliid");
	$listaEscolas = implode(',',$listaEscolas);
	
	$sql = "SELECT
				trim(ent.entcodent) AS entcodent,
				trim(ent.entnome) AS entnome,
				to_char(max(hid.htddata), 'DD/MM/YYYY') AS htddata,
				mme.memid,
				mme.docid,
				coalesce(mme.memvlrsaldodisponivel, 0) AS memvlrprimeiraparcela,
				esd.esdid,
				ent.entid,
				mme.memsaldo
			FROM
				entidade.entidade ent
			INNER JOIN
				pdeescola.memaiseducacao mme ON mme.entid = ent.entid AND mme.memanoreferencia = " . $_SESSION["exercicio"] . " AND mme.memstatus = 'A'
			INNER JOIN
				pdeescola.meloteimpressao mli ON mli.memid = mme.memid
			INNER JOIN
				workflow.documento doc ON doc.docid = mme.docid
			INNER JOIN
				workflow.historicodocumento hid ON hid.docid = doc.docid AND hid.aedid = 68
			INNER JOIN
				workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.esdid = 39
			WHERE
				ent.entid in (".$listaEscolas.")
			AND
				ent.entcodent NOT IN ( SELECT entcodent FROM pdeescola.meescolasrestricao )
			GROUP BY
				ent.entcodent,
				ent.entnome,
				mme.memid,
				mme.docid,
				mme.memvlrsaldodisponivel,
				esd.esdid,
				mli.mlicodigolote,
				ent.entid,
				mme.memsaldo
			ORDER BY
				mli.mlicodigolote";
	$entidades = $db->carregar($sql);
// 	ver($sql);
if($entidades) {
	//$vAux = true;
	$contador = 0;
	for($i=0; $i<count($entidades); $i++) {
		
		$saldoAnoAnterior = (float)$entidades[$i]["memvlrprimeiraparcela"];
		
		// Inicializa as variáveis p/ os cálculos
		$kitCusteio 			= 0;
		$kitCapital 			= 0;
		$ressarcimentoMonitores = 0;
		$valorLimite 			= 0;
		$servicosMateriais 		= 0;
		$pagamentoProfessorPST	= 0;
		$escolaAbertaCapital	= 0;
		$escolaAbertaCusteio	= 0;
		
		/*** INICIO - Regras para cálculo do pagamento de escolas 2010/2009 - INICIO ***/
		if((integer)$_SESSION["exercicio"] == 2010) {
			
			// verifica se a escola participou em 2009
			$dadosEscola2009 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = 2009");
			
			// escola de 2010
			if(!$dadosEscola2009) {
				// Quantidade de meses
				$meses = 10;
				// Chama a função para o cálculo
				calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
				$valorLimite = 999999;
				// Chama a função para cálculo de Serviços e Materiais
				calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses);
			}
			// participou em 2009
			else {
				// não foi paga em 2009
				//if(!$dadosEscola2009[0]["memvlrpago"]) {
				if( $dadosEscola2009[0]["mempagofnde"] == 'f' )
				{
					// Quantidade de meses
					$meses = 10;
					
					/*** Atividades de 2010 ***/
					// Chama a função para o cálculo
					calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					$valorLimite = 999999;
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses);
				}
				// foi feito o pagamento em 2009
				else {
					$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola2009[0]["memid"]." AND meacomecounoano = 't'");
					
					// Escola iniciou as atividades
					if((integer)$iniciouAnoAnterior > 0) {
						// Quantidade de meses
						$meses = 10;
						// Chama a função para o cálculo
						calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
						$valorLimite = 999999;
						// Chama a função para cálculo de Serviços e Materiais
						calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses);
					}
					// Escola não iniciou as atividades
					else {
						// Quantidade de meses
						$meses = 4;
						/*** Atividades de 2009 ***/
						// Chama a função para o cálculo
						calculoKitsRessarcimento($dadosEscola2009[0]["memid"], 2009, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses, true, true);
						// Chama a função para cálculo do valor limite
						calculoValorLimite($dadosEscola2009[0]["memid"], 2009, 6, $valorLimite, $meses);
						// Chama a função para cálculo de Serviços e Materiais
						calculoServicosMateriais($dadosEscola2009[0]["memid"], 2009, $servicosMateriais, $meses);
						
						// Quantidade de meses
						$meses = 10;
						/*** Atividades de 2010 ***/
						// Chama a função para o cálculo
						calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
						// Chama a função para cálculo do valor limite
						calculoValorLimite($entidades[$i]["memid"], $_SESSION["exercicio"], 6, $valorLimite, $meses);
					}
				}
			}
		}
		// Escolas de 2008 e 2009
		else {
			$flag = true;
			$dadosEscolaAnoAnterior = $db->carregar("SELECT memid,mempagofnde FROM pdeescola.memaiseducacao WHERE entid = ".$entidades[$i]["entid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
			
			if($dadosEscolaAnoAnterior)
			{
				$temAtividades = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']."");
					
				if( $temAtividades > 0 )
				{
					if( $temAtividades != $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano is null") )
					{
						$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano = 't'");
							
						if( (integer)$iniciouAnoAnterior == 0 && $dadosEscolaAnoAnterior[0]['mempagofnde'] == 't' )
						{
							// não paga nada
							$flag = false;
						}
					}
				}
			}
			
			if($flag)
			{
				// Quantidade de meses usada nos anos de 2008 e 2009
				$meses = 6;
				
				// Chama a função para o cálculo
				calculoKitsRessarcimento($entidades[$i]["memid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
				
				if( in_array( (integer) $_SESSION["exercicio"], array(2012, 2013) ) ) {
					if($pagamentoProfessorPST > 0){
						$ressarcimentoMonitores += $pagamentoProfessorPST;
						$pagamentoProfessorPST = 0;
					}
				}
			
				// Chama a função para cálculo de Serviços e Materiais
				calculoServicosMateriais($entidades[$i]["memid"], $_SESSION["exercicio"], $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
			}
			
			/*--------------------------------nova regra-------------------------------------*/
			if($calculoServicosMateriaisCapital || $calculoServicosMateriaisCusteio){

				$ano = $_SESSION["exercicio"];
			
				$servicosMateriais = $calculoServicosMateriaisCapital+$calculoServicosMateriaisCusteio;
			
				$kitCusteioTemp = $kitCusteio;
				$kitCapitalTemp = $kitCapital;
			
				$kitCusteio = $ressarcimentoMonitores+$calculoServicosMateriaisCusteio+$kitCusteio;
				$kitCapital = $kitCapital+$calculoServicosMateriaisCapital+$escolaAbertaCapital;
			
				$sql = "select memjovem1517, mamescolaaberta, mempeif from pdeescola.memaiseducacao  where memid = {$entidades[$i]["memid"]}";
				$rsAtividades = $db->pegaLinha($sql);
			
				if($rsAtividades['memjovem1517'] == 't'){
						
					$calculoJovem15a17 = 0;
						
					calculoJovem15a17($entidades[$i]["memid"], $ano, $meses, $calculoJovem15a17);
						
					$jovem1517custeio = $calculoJovem15a17>0 ? 5000 : 0;
					$jovem1517capital = $calculoJovem15a17>0 ? 2000 : 0;
						
					$kitCusteio = $kitCusteio+$calculoJovem15a17+$jovem1517custeio;
					$kitCapital = $kitCapital+$jovem1517capital;
			
				}
				if($rsAtividades['mamescolaaberta'] == 't'){
						
					$escolaAbertaCapital = 0;
					$escolaAbertaCusteio = 0;
						
					calculoEscolaAberta($entidades[$i]["memid"], $ano, $escolaAbertaCapital, $escolaAbertaCusteio, $meses);
						
					$kitCusteio = $kitCusteio+$escolaAbertaCusteio;
					$kitCapital = $kitCapital+$escolaAbertaCapital;
			
				}
				if($rsAtividades['mempeif'] == 't'){
						
					$peifCusteio = 0;
					$peifCapital = 0;
						
					calculoPeif($entidades[$i]["memid"], $peifCusteio, $peifCapital, $ano);
				}
			}
			/*--------------------------------nova regra-------------------------------------*/
		}
		
		/*** FIM - Regras para cálculo do pagamento de escolas 2010/2009 - FIM ***/
		
		$valorKits = ($kitCusteio + $kitCapital);
			
		/*** Regra da Clarissa (28/03/2011) ***/
		$porcentagemCusteio = ( $servicosMateriais * 0.8 );
		$porcentagemCapital = ( $servicosMateriais * 0.2 );
		
		// Calcula os Totais
		$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $porcentagemCusteio - $saldoAnoAnterior);
		
		$totalCapital = ($kitCapital + $porcentagemCapital);
		
		$totalGeral = ($kitCusteio + $kitCapital);		

		if($totalGeral < 0) $totalGeral = 0;
		
		// novo cálculo definido pelo Leandro
		if($totalCusteio < 0) {
			if(abs($totalCusteio) > $totalCapital)
				$totalCusteio = (($totalCapital) * (-1));
		}
		
		if( in_array((integer)$_SESSION["exercicio"], array(2012, 2013)) ) {
			
			$totalapagar = 0;
			$totalServicos = ($ressarcimentoMonitores + $servicosMateriais + $escolaAbertaCusteio + $escolaAbertaCapital); 

			//total a pagar soma 60% serviços
			$totalapagar += ($totalServicos * 0.6);
			//total a pagar soma 100% kits
			$totalapagar += ($kitCusteio + $kitCapital);
			
			$servicosMateriaisCusteio = ($servicosMateriais * 0.8);
			$servicosMateriaisCapital = ($servicosMateriais * 0.2);
			
			/*-------novo-------*/
			$saldoTotalPagar = ($totalGeral - $entidades[$i]["memsaldo"]);
			
			if($saldoTotalPagar < 0) $saldoTotalPagar = 0;
			
			$repasseCusteio = $totalGeral>0 ? (($kitCusteio/$totalGeral)*$saldoTotalPagar)+$peifCusteio : $totalGeral;
			$repasseCapital = $totalGeral>0 ? (($kitCapital/$totalGeral)*$saldoTotalPagar)+$peifCapital : $totalGeral;
			
			$repasseTotal = ($repasseCusteio+$repasseCapital);
			/*-------novo-------*/
			
			/*
			 <b>Total Kits: R$".number_format($valorKits, 2, ",", ".")."</b><br />
			 <b>Total Serviços: R$".number_format($totalServicos, 2, ",", ".")."</b><br /><br />
			 <b>Total a Pagar (100% Kits + 60% Serviços): R$".number_format($totalapagar, 2, ",", ".")."</b><br>
			 
			<b>Ressarcimento Monitores:</b> R$".number_format($ressarcimentoMonitores, 2, ",", ".")."<br />
			<b>Serviços/Mat. Cons.:</b> R$".number_format($calculoServicosMateriaisCusteio, 2, ",", ".")."(custeio) / R$".number_format($calculoServicosMateriaisCapital, 2, ",", ".")."(capital) - Total: R$".number_format($servicosMateriais, 2, ",", ".")."<br />
			<b>Kits Materiais:</b> R$".number_format($kitCusteio, 2, ",", ".")."(custeio) / R$".number_format($kitCapital, 2, ",", ".")."(capital) - Total: R$".number_format($valorKits, 2, ",", ".")."<br /><br />
			<b>Total Mais Educação: R$".number_format($totalGeral, 2, ",", ".")."</b><br><br>
			<b>Escola Aberta:</b> R$".number_format($escolaAbertaCusteio, 2, ",", ".")."(custeio) / R$".number_format($escolaAbertaCapital, 2, ",", ".")."(capital) - Total: R$".number_format($escolaAbertaCusteio+$escolaAbertaCapital, 2, ",", ".")."<br /><br />
			<b>Total Geral: R$".number_format($totalGeral, 2, ",", ".")."</b><br /><br />
			 */
			echo "<tr>
					<td align=\"left\">
						<b>Cód INEP:</b> ".$entidades[$i]["entcodent"]."<br />
						<b>Nome Escola:</b> ".$entidades[$i]["entnome"]."<br />
						<b>Data da Aprovação:</b> ".$entidades[$i]["htddata"]."<br /><br />
						<b>Repasse (custeio): R$".number_format($repasseCusteio, 2, ",", ".")."</b><br />
						<b>Repasse (capital): R$".number_format($repasseCapital, 2, ",", ".")."</b><br />
						<b>Repasse Total: R$".number_format($repasseTotal, 2, ",", ".")."</b><br /><br />
					</td>";
		}
		else{
			echo "<tr>
					<td align=\"left\">
						<b>Cód INEP:</b> ".$entidades[$i]["entcodent"]."<br />
						<b>Nome Escola:</b> ".$entidades[$i]["entnome"]."<br />
						<b>Data da Aprovação:</b> ".$entidades[$i]["htddata"]."<br /><br />
						<b>Ressarcimento Monitores:</b> R$".number_format($ressarcimentoMonitores, 2, ",", ".")."<br />
						<b>Serviços/Materiais:</b> R$".number_format($servicosMateriais, 2, ",", ".")."<br />
						<b>Total dos Kits Materiais:</b> R$".number_format($totalCusteio, 2, ",", ".")."(custeio) / R$".number_format($totalCapital, 2, ",", ".")."(capital) - Total: R$".number_format($totalCusteio+$totalCapital, 2, ",", ".")."<br /><br />
						<b>Saldo Ano Anterior:</b> R$".number_format($saldoAnoAnterior, 2, ",", ".")."<br /><br />
						<b>Total Geral: R$".number_format($totalGeral, 2, ",", ".")."</b>
					</td>";
			
		}
		
		if((integer)$entidades[$i]["esdid"] == 40) {
			echo "<td align=\"center\">
					<input type=\"checkbox\" checked=\"checked\" disabled=\"disabled\" /><br /><b>Pagamento Realizado</b>
				  </td>
				</tr>";
			$contador++;
		} else {
			echo "<td align=\"center\">
					<input type=\"checkbox\" name=\"pagamento[".$entidades[$i]["entcodent"]."]\" /><br />Efetuar Pagamento
				  </td>
				</tr>
				<script> arrEntcodent.push(".(integer)$entidades[$i]["entcodent"]."); </script>";
		}
		
		//if((integer)$entidades[$i]["esdid"] != 39) $vAux = false;
	}
	
	//if($vAux) {
		echo "<tr>
				<td align=\"center\" bgcolor=\"c0c0c0\" colspan=\"2\">
					<input type=\"button\" id=\"btCancelar\" style=\"cursor:pointer;\" value=\"Cancelar\" onclick=\"self.close();\" />
					<input type=\"button\" id=\"btPagamento\" style=\"cursor:pointer;\" value=\"Enviar para Pagamento\" onclick=\"submetePagamento();\" ".((count($entidades) == $contador) ?  "disabled=\"disabled\"" : "")." />
				</td>
			</tr>";
	//}
} else {
	echo "<tr>
		  	<td align=\"center\" colspan=\"2\">
		  		<br /><br /><br /><br /><br /><br />
		  		<font color=\"red\">Já foi realizado o pagamento para todas as escolas deste lote.</font>
		  		<br /><br /><br /><br /><br /><br />
		  	</td>
		  </tr>
		  <tr>
			<td align=\"center\" bgcolor=\"c0c0c0\" colspan=\"2\">
				<input type=\"button\" id=\"btCancelar\" style=\"cursor:pointer;\" value=\"Cancelar\" onclick=\"self.close();\" />
				<input type=\"button\" id=\"btPagamento\" style=\"cursor:pointer;\" value=\"Enviar para Pagamento\" disabled=\"disabled\" />
			</td>
		</tr>";
}
}

?>
</table>
</form>
<script type="text/javascript">

function submetePagamento() {
	var form 		 = document.getElementById('formPagamento');
	var btPagamento	 = document.getElementById('btPagamento');
	var btCancelar	 = document.getElementById('btCancelar');
	var vAux 		 = 0;
	
	btPagamento.disabled = true;
	btCancelar.disabled  = true;
	
	for(var i=0; i<arrEntcodent.length; i++) {
		if(document.getElementsByName("pagamento[" + arrEntcodent[i] + "]")[0].checked == false)
			vAux++;
	}
	
	if(vAux == arrEntcodent.length) {
		alert("Pelo menos uma escola deve ser selecionada.");
		btPagamento.disabled = false;
		btCancelar.disabled  = false;
		return;
	}
	
	form.submit();
}

function marcaTodos() {
	var form		 	= document.getElementById('formPagamento');
	var chMarcaTodos	= document.getElementById('chMarcaTodos');
	
	for (var i=0; i<form.length; i++) {
  		if(form.elements[i].name.substr(0,9) == "pagamento") {
  			if(chMarcaTodos.checked == true)
  				form.elements[i].checked = true;
  			else
  				form.elements[i].checked = false;
  		}
  	}
}

</script>