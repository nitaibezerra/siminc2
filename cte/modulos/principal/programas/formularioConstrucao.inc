<?php 

if(!isset($_SESSION['inuid']) || $_SESSION['inuid'] == ''){
	echo "<script>document.location.href = 'cte.php?modulo=principal/estrutura_avaliacao&acao=A'</script>";
	die();
}

//INCLUDES
include_once( APPRAIZ. "cte/classes/AcaoIndicador.class.inc" );
include_once( APPRAIZ. "cte/classes/SubacaoIndicador.class.inc" );
include_once( APPRAIZ. "cte/classes/ComposicaoSubacao.class.inc" );
include_once( APPRAIZ. "cte/classes/ProposicaoAcao.class.inc" );
include_once( APPRAIZ. "cte/classes/ProposicaoSubacao.class.inc" );
include_once( APPRAIZ. "cte/classes/SubacaoBeneficiario.class.inc" );
include_once( APPRAIZ. "cte/classes/SubacaoParecerTecnico.class.inc" );
include_once( APPRAIZ. "includes/classes/dateTime.inc" );

// CRIAÇÃO DE OBJETOS 
$obAcao 				= new AcaoIndicador();
$obProposicaoAcao 		= new ProposicaoAcao();
$obSubacao 				= new SubacaoIndicador();
$obComposicaoSub 		= new ComposicaoSubacao();
$obProposicaoSubacao  	= new ProposicaoSubacao();
$obBeneficiarioSub		= new SubacaoBeneficiario();
$obData					= new Data();
$obSubParecer			= new SubacaoParecerTecnico();

/********* RECUPERA SUBAÇÃO SE EXISTIR *********/
// Programa Proinfância 			- '1115,1085,1,1105,4'
// Urbana Ensino Fundamental 		- '1190,1191, 5, 1192, 8'
// Rural Ensino Fundamental 		- '146,1200,1198,1199'
// Indígena Ensino Fundamental 		- '1207,1208,152,1209,153'
// Quilombola Ensino Fundamental 	- '1218,1217,158,1216,159'
$ppsid = '1115,1085,1,1105,4,1190,1191, 5, 8, 1192,147, 146,1200,1198,1199, 1207,1208,152,1209,153, 1218,1217,158,1216,159';
$subacoes = carregaSubacao($ppsid);
if(is_array($subacoes)){
	// VER
	foreach($subacoes as $subacao){
		if($subacao['sbaformulario'] == "t" && $subacao['sbaid'] && $subacao['sbasituacaoarvore'] == '1'){
			if($_REQUEST['pr'] != 'A'){
				die( '<script type="text/javascript"> 
						alert("Os dados já foram enviados para análise do MEC. \r \n Não é possível alterar a demanda de melhoria."); 
						window.location.href="cte.php?modulo=principal/estrutura_avaliacao&acao=A";</script>');
			}
		}
		/********* DELETE DADOS, SE OS DADOS CADASTRADOS NÃO FOR DO FORMULARIO. *********/
		if(($subacao['sbaformulario'] == "f") && ($subacao['sbaid'])){
			limpaSubacao($subacao['sbaid'], $subacao['sbaporescola'] );
		}
	}
}

// DECLARAÇÃO DE VARIAVEIS
$totalLinhasItens 			= $_REQUEST['totallinhasitens'];
$totalLinhasBeneficiario 	= $_REQUEST['totallinhasben'];
$sucessoSub					= false;
$sucessoItens				= false;
$arDeSubacao				= array();

//EXCLUI ITENS DE COMPOSIÇÃO
if( $_REQUEST['excluir'] ){
	$excluidoComSucesso10 = false;
	$excluidoComSucesso11 = false;
	$cosid2010 = $_REQUEST['cos2010'];
	$cosid2011 = $_REQUEST['cos2011'];
	if($cosid2010){
		$obComposicaoSub = new ComposicaoSubacao();
		$obComposicaoSub->carregarPorId($cosid2010);
		$sbaidItem = $obComposicaoSub->excluir($obComposicaoSub->cosid, 'sbaid');
		
		if($sbaidItem){
			$obComposicaoSub->commit();
			$excluidoComSucesso10 = true;
		}else{
			$excluidoComSucesso10 = false;
		}
	}
	if($cosid2011){
		$obComposicaoSub = new ComposicaoSubacao();
		$obComposicaoSub->carregarPorId($cosid2011);
		$sbaidItem = $obComposicaoSub->excluir($obComposicaoSub->cosid, 'sbaid');
		if($sbaidItem){
			$obComposicaoSub->commit();
			$excluidoComSucesso11 = true;
		}else{
			$excluidoComSucesso11 = false;
		}
	}
	if($excluidoComSucesso10 || $excluidoComSucesso11){
		die($sbaidItem);
	}else{
		die('null');
	}
	
}

//EXCLUI BENEFICIARIOS
	//---------------- 2010 ------------------//
	if($_REQUEST['excluirben2010']){
		$sbaidBen = $_REQUEST['sbaid'];
		$excluidoComSucesso = false;
		$benid2010 = $_REQUEST['ben2010'];
		$sabano2010 = $_REQUEST['sabano2010'];
		//Exclui Beneficiario 
		if($_REQUEST['excluirben2010'] == '1'){
			if($benid2010){
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "DELETE FROM cte.subacaobeneficiario 
						WHERE benid = " . (integer) $benid2010 ."
						AND sabano = ". (integer) $sabano2010 ."
						AND sbaid = ". (integer) $sbaidBen;

				if($obBeneficiarioSub->executar($sql)){
					$obBeneficiarioSub->commit();
					$excluidoComSucesso = true;
				}else{
					$excluidoComSucesso = false;
				}
			}
		}
		// Não pode excluir apenas alterar o valor do Beneficiario
		else if ($_REQUEST['excluirben2010'] == '2'){
			$zona = $_REQUEST['zona'];
			$vlr2010 = $_REQUEST['vlr2010'];
			// Se for zona urbana eu limpo o valor de Urbana se não limpo rural.
			if($zona == 'Urbana'){
				$ur = $vlr2010;
				$ru = 0;
			}else{
				$ru = $vlr2010;
				$ur = 0;
			}
			
			if($benid2010){
				$valor2010 = $_REQUEST['ben2010'];
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "UPDATE cte.subacaobeneficiario 
						SET vlrrural = ".$ru."
						, vlrurbano = ".$ur."
						WHERE benid = " . (integer) $benid2010 ."
						AND sabano = ". (integer) $sabano2010 ."
						AND sbaid = ". (integer) $sbaidBen;
				if($obBeneficiarioSub->executar($sql)){
					$obBeneficiarioSub->commit();
					$excluidoComSucesso = true;
				}else{
					$excluidoComSucesso = false;
				}
			}
		}
		if($excluidoComSucesso){
			die('null');
		}else{
			die();
		}
	}
	
	//---------------- 2011 ------------------//
	if($_REQUEST['excluirben2011'] ){
		$sbaidBen = $_REQUEST['sbaid'];
		$excluidoComSucesso = false;
		$benid2011 = $_REQUEST['ben2011'];
		$sabano2011 = $_REQUEST['sabano2011'];
		
		//Exclui Beneficiario
		if($_REQUEST['excluirben2011'] == '1'){
			if($benid2011){
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "DELETE FROM cte.subacaobeneficiario 
						WHERE benid = " . (integer) $benid2011 ."
						AND sabano = ". (integer) $sabano2011 ."
						AND sbaid = ". (integer) $sbaidBen;
	
				if($obBeneficiarioSub->executar($sql)){
					$obBeneficiarioSub->commit();
					$excluidoComSucesso = true;
				}else{
					$excluidoComSucesso = false;
				}
			}
		}
		// Não pode excluir apenas alterar o valor do Beneficiario
		else if ($_REQUEST['excluirben2011'] == '2'){
			$vlr2011 = $_REQUEST['vlr2011'];
			$zona = $_REQUEST['zona'];
			if($zona == 'Urbana'){
				$ur = $vlr2011;
				$ru = 0;
			}else{
				$ru = $vlr2011;
				$ur = 0;
			}
			if($benid2011){
				$valor2011 = $_REQUEST['ben2011'];
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "UPDATE cte.subacaobeneficiario 
						SET vlrrural = ".$ru."
						, vlrurbano = ".$ur."
						WHERE benid = " . (integer) $benid2011 ."
						AND sabano = ". (integer) $sabano2011 ."
						AND sbaid = ". (integer) $sbaidBen;

				if($obBeneficiarioSub->executar($sql)){
					$obBeneficiarioSub->commit();
					$excluidoComSucesso = true;
				}else{
					$excluidoComSucesso = false;
				}
			}
		}


		if($excluidoComSucesso){
			die('null');
		}else{
			die();
		}
	}


if( $_REQUEST['salvar'] || $_REQUEST['enviarparamec']  ){
	// Se existe as subaçãoes eu limpo antes de salvar os dados.
	if(count($totalLinhasItens) == 0){
		die('<script type="text/javascript"> 
				alert("Não existe itens de composição. Insira um plano de trabalho."); 
				window.location.href="cte.php?modulo=principal/estrutura_avaliacao&acao=A";</script>');
	}
	if(is_array($subacoes)){
		foreach($subacoes as $sub){
			limpaSubacao($sub['sbaid'], $sub['sbaporescola'] );
		}
	}
	
	// VERIFICA OS ITENS DE COMPOSIÇÃO
	for($i = 0; $i < $totalLinhasItens;  $i++){
		// Recupera o Tipo de item
		if($_REQUEST['cosdsc_'.$i] == 1 || $_REQUEST['cosdsc_'.$i] == 2 ){ 
			$ppsid = '1115,1085,1,1105,4';
			$tipoBen = 1;
		}else if($_REQUEST['cosdsc_'.$i] == 3 || $_REQUEST['cosdsc_'.$i] == 4){
			$ppsid = '1190,1191, 5, 8, 1192';
			$tipoBen = 2;
		}else if($_REQUEST['cosdsc_'.$i] == 5 || $_REQUEST['cosdsc_'.$i] == 6 || $_REQUEST['cosdsc_'.$i] == 7 || $_REQUEST['cosdsc_'.$i] == 8){
			$ppsid = '147,146,1200,1198,1199';
			$tipoBen = 3;
		}else if($_REQUEST['cosdsc_'.$i] == 9 || $_REQUEST['cosdsc_'.$i] == 10 || $_REQUEST['cosdsc_'.$i] == 11 || $_REQUEST['cosdsc_'.$i] == 12){
			$ppsid = '1207,1208,152,1209,153';
			$tipoBen = 4;
		}else if($_REQUEST['cosdsc_'.$i] == 13 || $_REQUEST['cosdsc_'.$i] == 14 || $_REQUEST['cosdsc_'.$i] == 15 || $_REQUEST['cosdsc_'.$i] == 16){
			$ppsid = '1218,1217,158,1216,159';
			$tipoBen = 5;
		}
		
		$subacao = carregaSubacao($ppsid);
		if($subacao[0]['sbaid']){// Se exite a subacao
			$obSubacao->carregarPorId($subacao[0]['sbaid']); // carrega subacao
			$obSubacao->sbaformulario = 'true'; // feito pelo formulário
			// Carrega parecer padrão
			$obSubParecer->carregarPorId($subacao[0]['sbaid']);
			if($obSubParecer->sptparecer == NULL && $obSubacao->ppsid){
				$obProposicaoSubacao->carregarPorId($obSubacao->ppsid);
			}
			
			// Monta array de subaçãoes
			$arDeSubacao[$subacao[0]['sbaid']][$i] = $_REQUEST['cosdsc_'.$i];
			$arDeSubacao[$subacao[0]['sbaid']]['parecer'] = $obProposicaoSubacao->ppsparecerpadrao;
			$arDeSubacao[$subacao[0]['sbaid']]['tipobeneficiario'] = $tipoBen;
		
			// Se for finalizar envia, mostra na árvore a subação
			if($_REQUEST['enviarparamec'] == 'true'){
				$obSubacao->sbasituacaoarvore = '1';
			}else{
				$obSubacao->sbasituacaoarvore = '0';
			}
			$obSubacao->salvar();
			$obSubacao->commit();
		}else{// Se não existe subação cria
			$propostaSubacao = carregaSubacaoGuia($ppsid);
			if(is_array($propostaSubacao)){
				$obProposicaoSubacao->carregarPorId($propostaSubacao[0]['ppsid']);
				//SE NÃO EXISTE AÇÃO CRIA
				if(!$propostaSubacao[0]['aciid']){
					$obProposicaoAcao->carregarPorId($propostaSubacao[0]['ppaid']);
					$obAcao->ptoid 			= $propostaSubacao[0]['ptoid'];
					$obAcao->acidsc 		= $obProposicaoAcao->ppadsc;
					$obAcao->ppaid 			= $obProposicaoAcao->ppaid;
					$obAcao->acilocalizador = 'M';
					$idAcao 				= $obAcao->salvar();
					$obAcao->commit();
					$propostaSubacao[0]['aciid'] = $idAcao;
				}
				//SUBACAO RECEBE PROPOSTA DO GUIA
				$obSubacao 				= new SubacaoIndicador();
				$obSubacao->aciid 		= $propostaSubacao[0]['aciid'];
				$obSubacao->undid 		= $obProposicaoSubacao->undid;
				$obSubacao->frmid 		= $obProposicaoSubacao->frmid;
				$obSubacao->sbadsc 		= $obProposicaoSubacao->ppsdsc;
				$obSubacao->sbastgmpl 	= $obProposicaoSubacao->ppsmetodologia;
				$obSubacao->sbadata 	= $obData->dataAtual('Y-m-d:H:m:s');
				$obSubacao->usucpf 		= $_SESSION['usucpf']; 
				$obSubacao->prgid 		= $obProposicaoSubacao->prgid;
				$obSubacao->sbaporescola = $obProposicaoSubacao->indqtdporescola;
				$obSubacao->ppsid 		= $obProposicaoSubacao->ppsid;
				$obSubacao->sbaordem 	= $obProposicaoSubacao->ppsordem;
			
				// Se for finalizar envia, mostra na árvore a subação
				if($_REQUEST['enviarparamec'] == 'true'){
					$obSubacao->sbasituacaoarvore = '1';
				}else{
					$obSubacao->sbasituacaoarvore = '0';
				}
				$obSubacao->sbaformulario = 'true';
				$sbaid = $obSubacao->salvar();
				if($sbaid){
					$obSubacao->commit();
					$arDeSubacao[$sbaid][$i] = $_REQUEST['cosdsc_'.$i];
					$arDeSubacao[$sbaid]['parecer'] = $obProposicaoSubacao->ppsparecerpadrao;
					$arDeSubacao[$sbaid]['tipobeneficiario'] = $tipoBen;
					$sucessoSub = true;
				}else{
					$obSubacao->rollback();
					echo '<script>alert("Ocorreu um erro ao tentar cadastrar a subação.")
							window.location.href="cte.php?modulo=principal/estrutura_avaliacao&acao=A";
						</script>';
					die();
				}
			}else{
				echo '	<script>alert("Não foi encontrada proposta de subação para este município. \r \n Entre em contado com o Administrador do Sitema.")
							window.location.href="cte.php?modulo=principal/estrutura_avaliacao&acao=A";
						</script>';
				die();
			}
		}
		
	}
	
	//SALVA Dados
	foreach($arDeSubacao as $chaves => $dadosItensSub){
		$totalGlobal2010 			= 0;
		$totalGlobal2011 			= 0;
		$totalBeneficiario1_2010	= 0;
		$totalBeneficiario2_2010	= 0;
		$totalBeneficiario3_2010	= 0;
		$totalBeneficiario4_2010	= 0;
		$totalBeneficiario5_2010	= 0;
		$totalBeneficiario1_2011	= 0;
		$totalBeneficiario2_2011	= 0;
		$totalBeneficiario3_2011	= 0;
		$totalBeneficiario4_2011	= 0;
		$totalBeneficiario5_2011	= 0;
		$totalGlobal1_2010 			= 0;
		$totalGlobal2_2010 			= 0;
		$totalGlobal3_2010 			= 0;
		$totalGlobal4_2010 			= 0;
		$totalGlobal5_2010 			= 0;
		$totalGlobal1_2011 			= 0;
		$totalGlobal2_2011 			= 0;
		$totalGlobal3_2011 			= 0;
		$totalGlobal4_2011 			= 0;
		$totalGlobal5_2011 			= 0;
		// Salva itens
		for($i = 0; $i < $totalLinhasItens;  $i++){
			$totalBen = 0;
			//Formata Dados

			$valor = str_replace(".", "", $_REQUEST['valor_unitario_obra_'.$i]);
			$valor = str_replace(",", ".", $valor);

			if($_REQUEST['cosdsc_'.$i] == $dadosItensSub[$i]){
				// Se quantidade de 2010 preenchida insere item em 2011
				if($_REQUEST['cosqtd_2010_'.$i]){
					
					$valor = $valor ? $valor : 0;
					
					$obComposicaoSub 			= new ComposicaoSubacao();
					$obComposicaoSub->cosdsc 	= $_REQUEST['descritem_'.$i];
					$obComposicaoSub->cosvlruni = $valor; 
					$obComposicaoSub->unddid 	= $_REQUEST['unddid_'.$i];
					$obComposicaoSub->sbaid 	= $chaves;
					$obComposicaoSub->cosano 	= '2010';
					$obComposicaoSub->cosqtd 	= $_REQUEST['cosqtd_2010_'.$i];

					// recupera e calcula os valores para o beneficiario em 2010 e valor global
					$totalBen = $obComposicaoSub->cosqtd * $_REQUEST['unidade_'.$i];
					if($dadosItensSub["tipobeneficiario"] == 1){
						$totalBeneficiario1_2010 = $totalBeneficiario1_2010 + $totalBen;
						$totalGlobal1_2010 = $totalGlobal1_2010 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 2){
						$totalBeneficiario2_2010 = $totalBeneficiario2_2010 + $totalBen;
						$totalGlobal2_2010 = $totalGlobal2_2010 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 3){
						$totalBeneficiario3_2010 = $totalBeneficiario3_2010 + $totalBen;
						$totalGlobal3_2010 = $totalGlobal3_2010 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 4){
						$totalBeneficiario4_2010 = $totalBeneficiario4_2010 + $totalBen;
						$totalGlobal4_2010 = $totalGlobal4_2010 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 5){
						$totalBeneficiario5_2010 = $totalBeneficiario5_2010 + $totalBen;
						$totalGlobal5_2010 = $totalGlobal5_2010 + $obComposicaoSub->cosqtd;
					}
					$obComposicaoSub->salvar();
					$obComposicaoSub->commit();
					$sucessoItens = true;
				}
				// Se quantidade de 2011 preenchida insere item em 2011
				if($_REQUEST['cosqtd_2011_'.$i]){
					
					$valor = $valor ? $valor : 0;
					
					$obComposicaoSub 			= new ComposicaoSubacao();
					$obComposicaoSub->cosdsc 	= $_REQUEST['descritem_'.$i];
					$obComposicaoSub->cosvlruni = $valor; 
					$obComposicaoSub->unddid 	= $_REQUEST['unddid_'.$i];
					$obComposicaoSub->sbaid 	= $chaves;
					$obComposicaoSub->cosano 	= '2011';
					$obComposicaoSub->cosqtd 	= $_REQUEST['cosqtd_2011_'.$i];
					
					// recupera e calcula os valores para o beneficiario em 2011 e valor global
					$totalBen = $obComposicaoSub->cosqtd * $_REQUEST['unidade_'.$i];
					if($dadosItensSub["tipobeneficiario"] == 1){
						$totalBeneficiario1_2011 = $totalBeneficiario1_2011 + $totalBen;
						$totalGlobal1_2011 = $totalGlobal1_2011 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 2){
						$totalBeneficiario2_2011 = $totalBeneficiario2_2011 + $totalBen;
						$totalGlobal2_2011 = $totalGlobal2_2011 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 3){
						$totalBeneficiario3_2011 = $totalBeneficiario3_2011 + $totalBen;
						$totalGlobal3_2011 = $totalGlobal3_2011 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 4){
						$totalBeneficiario4_2011 = $totalBeneficiario4_2011 + $totalBen;
						$totalGlobal4_2011 = $totalGlobal4_2011 + $obComposicaoSub->cosqtd;
					}elseif($dadosItensSub["tipobeneficiario"] == 5){
						$totalBeneficiario5_2011 = $totalBeneficiario5_2011 + $totalBen;
						$totalGlobal5_2011 = $totalGlobal5_2011 + $obComposicaoSub->cosqtd;
					}
					$obComposicaoSub->salvar();
					$obComposicaoSub->commit();
					$sucessoItens = true;
				}
			}
		}
		
		// SALVA DADOS DO PARECER E CRONOGRAMA
		if($totalGlobal1_2010 != 0 || $totalGlobal2_2010 != 0 || $totalGlobal3_2010 != 0 || $totalGlobal4_2010 != 0 || $totalGlobal5_2010 != 0 ){
			if($dadosItensSub['tipobeneficiario'] == '1'){
				$totalGlobal2010 = $totalGlobal1_2010;
			}else if($dadosItensSub['tipobeneficiario'] == '2'){
				$totalGlobal2010 = $totalGlobal2_2010;
			}else if($dadosItensSub['tipobeneficiario'] == '3'){
				$totalGlobal2010 = $totalGlobal3_2010;
			}else if($dadosItensSub['tipobeneficiario'] == '4'){
				$totalGlobal2010 = $totalGlobal4_2010;
			}else if($dadosItensSub['tipobeneficiario'] == '5'){
				$totalGlobal2010 = $totalGlobal5_2010;
			}else{
				$totalGlobal2010 = 0;
			}
			$parecerPadrao = $obSubParecer->sptparecer == NULL ? $obProposicaoSubacao->ppsparecerpadrao  : $obSubParecer->sptparecer;
			$obSubParecerTecnico				= new SubacaoParecerTecnico();
			$obSubParecerTecnico->sbaid 		= $chaves;
			$obSubParecerTecnico->sptparecer 	= $dadosItensSub['parecer'];
			$obSubParecerTecnico->sptunt 		= $totalGlobal2010;
			$obSubParecerTecnico->sptano 		= 2010;
			$obSubParecerTecnico->sptinicio 	= 4;
			$obSubParecerTecnico->sptfim 		= 12;
			$obSubParecerTecnico->tppid 		= 4;
			$obSubParecerTecnico->ssuid 		= 1;
			
			$obSubParecerTecnico->salvar();
			$obSubParecerTecnico->commit();
		}
		
		// SALVA DADOS DO PARECER E CRONOGRAMA
		if($totalGlobal1_2011 != 0 || $totalGlobal2_2011 != 0 || $totalGlobal3_2011 != 0 || $totalGlobal4_2011 != 0 || $totalGlobal5_2011 != 0 ){
			if($dadosItensSub['tipobeneficiario'] == '1'){
				$totalGlobal2011 = $totalGlobal1_2011;
			}else if($dadosItensSub['tipobeneficiario'] == '2'){
				$totalGlobal2011 = $totalGlobal2_2011;
			}else if($dadosItensSub['tipobeneficiario'] == '3'){
				$totalGlobal2011 = $totalGlobal3_2011;
			}else if($dadosItensSub['tipobeneficiario'] == '4'){
				$totalGlobal2011 = $totalGlobal4_2011;
			}else if($dadosItensSub['tipobeneficiario'] == '5'){
				$totalGlobal2011 = $totalGlobal5_2011;
			}else{
				$totalGlobal2011 = 0;
			}
			$parecerPadrao = $obSubParecer->sptparecer == NULL ? $obProposicaoSubacao->ppsparecerpadrao  : $obSubParecer->sptparecer;
			$obSubParecerTecnico				= new SubacaoParecerTecnico();
			$obSubParecerTecnico->sbaid 		= $chaves;
			$obSubParecerTecnico->sptparecer 	= $dadosItensSub['parecer'];
			$obSubParecerTecnico->sptunt 		= $totalGlobal2011;
			$obSubParecerTecnico->sptano 		= 2011;
			$obSubParecerTecnico->sptinicio 	= 1;
			$obSubParecerTecnico->sptfim 		= 12;
			$obSubParecerTecnico->tppid 		= 4;
			$obSubParecerTecnico->ssuid 		= 1;
			
			$obSubParecerTecnico->salvar();
			$obSubParecerTecnico->commit();
		}
		//SALVA BENEFICIARIOS
		for($i = 0; $i < $totalLinhasBeneficiario;  $i++){	
			if($_REQUEST['tiposubben_'.$i.'2010'] == $dadosItensSub['tipobeneficiario'] ){
				if($_REQUEST['ano_'.$i.'2010']){
					if($dadosItensSub['tipobeneficiario'] == '1'){
						$vlrBen = $totalBeneficiario1_2010;
					}else if($dadosItensSub['tipobeneficiario'] == '2'){
						$vlrBen = $totalBeneficiario2_2010;
					}else if($dadosItensSub['tipobeneficiario'] == '3'){
						$vlrBen = $totalBeneficiario3_2010;
					}else if($dadosItensSub['tipobeneficiario'] == '4'){
						$vlrBen = $totalBeneficiario4_2010;
					}else if($dadosItensSub['tipobeneficiario'] == '5'){
						$vlrBen = $totalBeneficiario5_2010;
					}else{
						$vlrBen = 0;
					}
					$obBeneficiarioSub 			= new SubacaoBeneficiario();
					$obBeneficiarioSub->sbaid 		= $chaves;
					// De acordo com o tipo de ben o beneficiario e urbano ou rural
					if($dadosItensSub['tipobeneficiario'] == '1' || $dadosItensSub['tipobeneficiario'] == '2' ){
						$obBeneficiarioSub->vlrurbano 	=  $vlrBen;
						$obBeneficiarioSub->vlrrural 	= 0;
					}else{
						$obBeneficiarioSub->vlrurbano 	= 0;
						$obBeneficiarioSub->vlrrural 	= $vlrBen;
					}
					$obBeneficiarioSub->sabano 		= $_REQUEST['ano_'.$i.'2010'];
					$obBeneficiarioSub->benid		= $_REQUEST['benid_'.$i.'2010'];
					$sql="INSERT INTO cte.subacaobeneficiario (sbaid,benid,vlrurbano,vlrrural,sabano) 
					VALUES (".$obBeneficiarioSub->sbaid.",".$obBeneficiarioSub->benid.",".$obBeneficiarioSub->vlrurbano.",".$obBeneficiarioSub->vlrrural.",".$obBeneficiarioSub->sabano.")";
					$obBeneficiarioSub->executar($sql);
					$obBeneficiarioSub->commit();
				}
			}
			if($_REQUEST['tiposubben_'.$i.'2011'] == $dadosItensSub['tipobeneficiario'] ){
				if($dadosItensSub['tipobeneficiario'] == '1'){
					$vlrBen = $totalBeneficiario1_2011;
				}else if($dadosItensSub['tipobeneficiario'] == '2'){
					$vlrBen = $totalBeneficiario2_2011;
				}else if($dadosItensSub['tipobeneficiario'] == '3'){
					$vlrBen = $totalBeneficiario3_2011;
				}else if($dadosItensSub['tipobeneficiario'] == '4'){
					$vlrBen = $totalBeneficiario4_2011;
				}else if($dadosItensSub['tipobeneficiario'] == '5'){
					$vlrBen = $totalBeneficiario5_2011;
				}else{
					$vlrBen = 0;
				}
				if($_REQUEST['ano_'.$i.'2011']){
					$obBeneficiarioSub 			= new SubacaoBeneficiario();
					$obBeneficiarioSub->sbaid 		= $chaves;
					// De acordo com o tipo de ben o beneficiario e urbano ou rural
					if($dadosItensSub['tipobeneficiario'] == '1' || $dadosItensSub['tipobeneficiario'] == '2' ){
						$obBeneficiarioSub->vlrurbano 	= $vlrBen;
						$obBeneficiarioSub->vlrrural 	= 0;
					}else{
						$obBeneficiarioSub->vlrurbano 	= 0;
						$obBeneficiarioSub->vlrrural 	= $vlrBen;
					}
					$obBeneficiarioSub->sabano 		= $_REQUEST['ano_'.$i.'2011'];
					$obBeneficiarioSub->benid		= $_REQUEST['benid_'.$i.'2011'];
					$sql="INSERT INTO cte.subacaobeneficiario (sbaid,benid,vlrurbano,vlrrural,sabano) 
					VALUES (".$obBeneficiarioSub->sbaid.",".$obBeneficiarioSub->benid.",".$obBeneficiarioSub->vlrurbano.",".$obBeneficiarioSub->vlrrural.",".$obBeneficiarioSub->sabano.")";
					$obBeneficiarioSub->executar($sql);
					$obBeneficiarioSub->commit();
				}
			}
		}

	}
	if($sucessoSub || $sucessoItens){
		if($_REQUEST['enviarparamec'] == 'true'){
			die('<script type="text/javascript"> 
				alert("Dados enviados para análise do MEC."); 
				window.location.href="cte.php?modulo=principal/estrutura_avaliacao&acao=A";</script>');
		}else{
			die( '<script type="text/javascript"> 
				alert("Dados salvos com sucesso."); 
				window.location.href="cte.php?modulo=principal/programas/formularioConstrucao&acao=A";</script>');
		}
	}
}

//CARREGA DADOS PARA TELA
$dadosSubacaoItens = array();
$valoresRecuperados = array();
$cont = 0;
if(is_array($subacoes)){
	foreach($subacoes as $subacao){
		if($subacao['sbaid']){
			 $valoresRecuperados = carregaDadosItensAgrupado($subacao['sbaid']);
			 if($valoresRecuperados != false){
			 	$valoresRecuperados = array_merge($dadosSubacaoItens, $valoresRecuperados);
			 	$dadosSubacaoItens = $valoresRecuperados;
			 }
		}
	}
	
	if(is_array($dadosSubacaoItens)){
		foreach($dadosSubacaoItens as $chave=>$dados){
			$dadosSubacaoItens[$chave]['cosdsc'] = iconv("ISO-8859-1", "UTF-8", $dados["cosdsc"]);
		}
	}
}

echo '<div id="composicao" style="display:none;"  >'.simec_json_encode($dadosSubacaoItens).'</div>';
//dbg($dadosSubacaoItens,1);
// CRIA CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
cte_montaTitulo($titulo_modulo, 'Programa Construção');
?>
<script type="text/javascript" src="../../includes/prototype.js"></script>
<style>
#loader-container,
#LOADER-CONTAINER
{
    background: none;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 1200px;
}

#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
<div id="loader-container">
	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form action="" id="formulario" name="formulario" method="post">
	<input type=hidden name="salvar" value="false" />
	<input type=hidden name="enviarparamec" value="false" />
	<input type=hidden name="totallinhasitens" value="0" />
	<input type=hidden name="totallinhasben" value="0" />
	
	<!-- Tabela de Itens de Composição -->
	<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr>
			<td class="SubTituloCentro">Cadastro de Itens de Composição</td>
		</tr>
		<tr style="background-color: #cccccc;">
			<td>
				<div style="margin: 0; padding: 0; height: 200px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
					<table id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
						<tr style="text-align: center">
					        <td style="padding: 2px; font-weight: bold;">Ação</td>
					        <td style="padding: 2px; font-weight: bold;">Identificação do Item</td>
					        <td style="padding: 2px; font-weight: bold;">Dimensão Mínima <br> do Terreno</td>
					        <td style="padding: 2px; font-weight: bold;">Área  <br> Construída (m2)</td>
					        <td style="padding: 2px; font-weight: bold;">Qtd. <br> 2010</td>
					        <td style="padding: 2px; font-weight: bold;">Qtd. <br> 2011</td>
					        <td style="padding: 2px; font-weight: bold;">Valor por <br> m2 (R$)</td>
					        <td style="padding: 2px; font-weight: bold;">Valor Unitário <br> da Obra (R$)</td>
					        <td style="padding: 2px; font-weight: bold;">Valor Total <br> 2010(R$)</td>
					        <td style="padding: 2px; font-weight: bold;">Valor da Contrapartida <br> do Município 2010 (1% em R$)</td>
					        <td style="padding: 2px; font-weight: bold;">Valor Total <br> 2011(R$)</td>
					        <td style="padding: 2px; font-weight: bold;">Valor da Contrapartida <br> do Município 2011 (1% em R$)</td>
					        <td style="padding: 2px; font-weight: bold;">Beneficiários <br> por Unidade</td>
				        </tr>		
					</table>
				</div>
				<div style="padding: 5px 0px 0px 5px">
					<span style="cursor:pointer;"  id="linkInserirItem" onclick="return adicionarItemComposicao('null');"><img src="../imagens/gif_inclui.gif" align="left" style="border: none" />Inserir Itens de Composição</span>
				</div>
			</td>
		</tr>
	</table>
	
	<!-- Tabela de Beneficiarios -->
	<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr>
			<td class="SubTituloCentro">Beneficiário por Item Solicitado </td>
		</tr>
		<tr style="background-color: #cccccc;">
			<td>
				<div style="margin: 0; padding: 0; height: 160px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
					<table id="beneficiariosSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
						<tr style="text-align: center">
							<td style="padding: 2px; font-weight: bold;">Ano</td>
							<td style="padding: 2px; font-weight: bold;">Descrição do <br> Beneficiário</td>
					        <td style="padding: 2px; font-weight: bold;">Total</td>
					        <td style="padding: 2px; font-weight: bold;">Zona</td>
					        <td style="padding: 2px; font-weight: bold;">Referente ao Item</td>
				        </tr>		
					</table>
				</div>
			</td>
		</tr>
	</table>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
		<tr>
			<td class="SubTituloDireita">
				<input type="button" name="salvar" value="Salvar"  onclick="salvarDados('salva');" /> 
				<input type="button" name="enviar" value="Enviar para Análise do MEC"  onclick="salvarDados('envia');" />
				<input type="button" name="voltar" id="voltar" value="Voltar"  onclick=" voltarPag();" />	
			</td>
		</tr>
	</table>
</form>

<script type="text/javascript">
// CRIA OBJETO COM OS ITENS DE COMPOSIÇÃO
var objsDescricao 	= new Object;
objsDescricao.dados = {
		texto : new Array(	'Selecione',
							'Construção de unidade escolar, modelo padrão Proinfância B',
							'Construção de unidade escolar, modelo padrão Proinfância C',
							'Construção de unidade escolar, modelo padrão FNDE Urbana - 4 salas de aula',
							'Construção de unidade escolar, modelo padrão FNDE Urbana - 6 salas de aula',
							'Construção de unidade escolar, modelo padrão FNDE Rural - 1 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Rural - 2 salas de aula',
							'Construção de unidade escolar, modelo padrão FNDE Rural - 4 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Rural - 6 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Indígena - 1 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Indígena - 2 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Indígena - 4 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Indígena - 6 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Quilombola - 1 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Quilombola - 2 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Quilombola - 4 salas de aula', 
							'Construção de unidade escolar, modelo padrão FNDE Quilombola - 6 salas de aula' 
						), 
		valor : new Array('',1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) 
};

//FUNÇÃO PARA DUPLICAR OBJETOS JAVASCRIPT
function clone(obj){
    if(obj == null || typeof(obj) != 'object')
        return obj;
    
    var temp = obj.constructor(); // breaks
    for(var key in obj)
        temp[key] = clone(obj[key]);
    return temp;
}

// DUPLICA OBJETO
var objsDescricaoBac 	= new Object;
objsDescricaoBac 		= clone(objsDescricao);

<?php  if(is_array($dadosSubacaoItens)){ ?>
	var arDados 	= new Object;
	var arDados = eval('(' + document.getElementById('composicao').innerHTML + ')');
	for (var x = 0; x < arDados.length; x++) {
		adicionarItemComposicao( arDados[x]);
	}
<?php }  ?>

function carregaTipoItem(descricao){
	var index;
	index = objsDescricao.dados.texto.indexOf( descricao );
	return index;

}

// CRIA LINHA DE ITENS DE COMPOSIÇÃO
function adicionarItemComposicao(objetoDeDados){	
	//VALIDANDO E CARREGANDO DADOS
	var carregaCombo = objsDescricao.dados.texto;
    if(carregaCombo.length == 1){
		alert('Não é possivel inserir mais itens de composição.');
		return false;
    }
	var carregaBanco = objetoDeDados == 'null' ? 0 : 1;
	var cosid2010 	= objetoDeDados.cosid2010 		!= 0 ? objetoDeDados.cosid2010 : '';
	var cosid2011 	= objetoDeDados.cosid2011 		!= 0 ? objetoDeDados.cosid2011 : '';
	var cosqtd2010 	= objetoDeDados.cosqtd2010 		!= 0 ? objetoDeDados.cosqtd2010 : '';
	var cosqtd2011 	= objetoDeDados.cosqtd2011 		!= 0 ? objetoDeDados.cosqtd2011 : '';
	var cosvlruni 	= objetoDeDados.cosvlruni2010 	!= 0 ? objetoDeDados.cosvlruni2010 : objetoDeDados.cosvlruni2011;
	var cosdsc 		= objetoDeDados.cosdsc 			!= null ? objetoDeDados.cosdsc : '';
	var unddid 		= objetoDeDados.unddid    		!= null ? objetoDeDados.unddid : '185';
	var valorDescricao;
	
	
	// Cria Item via BTN
	if(carregaBanco == 0){
		cosid2010 	= '';
		cosid2011 	= '';
		cosqtd2010 	= '';
		cosqtd2011 	= '';
		cosvlruni 	= '';
	}
	
	if(carregaBanco == 1 && cosdsc != null){
		valorDescricao = carregaTipoItem(cosdsc);
		cosvlruni = mascaraglobal( '###.###.###,##', Number(cosvlruni).toFixed(2) );
	}
	// FIM VALIDANDO DADOS
	
    var tabelaItensComposicaoSubAcao = $('itensComposicaoSubAcao');
    var qtdLinha = tabelaItensComposicaoSubAcao.rows.length;
    var qtd = qtdLinha - 1;
    
	if(qtd != 0){
		var valorVerificar = qtd - 1;
		if($('descritem_'+ valorVerificar).value == ''){
			alert('Selecione a descrição do item no combo anterior.');
			return false;
		}
	}
    
    var btnExcluirItemComposicao     = document.createElement('img');
    btnExcluirItemComposicao.src     = '/imagens/excluir.gif';
    btnExcluirItemComposicao.onclick = function()
    {
            removerItemComposicao(this.parentNode.parentNode.getAttribute("id"), cosid2010, cosid2011, qtd, carregaBanco);
    }

	// CAMPO DESCRIÇÃO COMBO
    input_cosdsc    = document.createElement('select');
    input_cosdsc.setAttribute('class', 'CampoEstilo');
    input_cosdsc.setAttribute('name', 'cosdsc_'+qtd);
    input_cosdsc.setAttribute('id', 'cosdsc_'+qtd);
    input_cosdsc.setAttribute('value', '');
		// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_cosdsc.setAttribute( "onchange", 'carregaDadosPorLinha(this.value, '+qtd+', this.selectedIndex, 0 )' );
		}else{ // IE
			input_cosdsc.attachEvent( "onchange", function() { eventosIE( document.getElementById( 'cosdsc_' + qtd ), qtd,1, 0 ); } );
		}
		
    	// CARREGA LISTA DE ITENS QUE PODEM SER INSERIDOS NO PROGRAMA
    	var carregaCombovlr = objsDescricao.dados.valor;
		for (cont=0; cont < carregaCombo.length; cont++){
			input_cosdsc.options[cont] = new Option(carregaCombo[cont],carregaCombovlr[cont]);
		}

	 // CAMPO DESCRICAO
	    input_descritem     = document.createElement('input');
	    input_descritem.setAttribute('type', 'hidden');
	    input_descritem.setAttribute('name', 'descritem_'+ qtd);
	    input_descritem.setAttribute('id', 'descritem_'+ qtd);
	    input_descritem.setAttribute('value', cosdsc);

	    
	 // CAMPO METRO QUADRADO
		input_mquadrado    = document.createElement('input');
	    input_mquadrado.setAttribute('type', 'text');
	    input_mquadrado.setAttribute('class', 'disabled');
	    input_mquadrado.setAttribute('readOnly', 'readonly');
	    input_mquadrado.setAttribute('name', 'mquadrado_'+ qtd);
	    input_mquadrado.setAttribute('id', 'mquadrado_'+ qtd);
	    input_mquadrado.setAttribute('maxlength', '100');
	    input_mquadrado.setAttribute('size', '12');
	    input_mquadrado.setAttribute('value', '');

	 // CAMPO ÁREA CONSTRUIDA
		input_areaconstruida    = document.createElement('input');
	    input_areaconstruida.setAttribute('type', 'text');
	    input_areaconstruida.setAttribute('class', 'disabled');
	    input_areaconstruida.setAttribute('readOnly', 'readonly');
	    input_areaconstruida.setAttribute('name', 'areaconstruida_'+ qtd);
	    input_areaconstruida.setAttribute('id', 'areaconstruida_'+ qtd);
	    input_areaconstruida.setAttribute('maxlength', '100');
	    input_areaconstruida.setAttribute('size', '10');
	    input_areaconstruida.setAttribute('value', '');

	 // CAMPO QUANTIDADE 2010
		input_cosqtd2010    = document.createElement('input');
	    input_cosqtd2010.setAttribute('type', 'text');
	    input_cosqtd2010.setAttribute('class', 'normal');
	    input_cosqtd2010.setAttribute('name', 'cosqtd_2010_'+ qtd);
	    input_cosqtd2010.setAttribute('id', 'cosqtd_2010_'+ qtd);
	    input_cosqtd2010.setAttribute('size', '8');
	    input_cosqtd2010.setAttribute('value', cosqtd2010);
	    input_cosqtd2010.setAttribute('onkeypress', "return somenteNumeros(event);");
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_cosqtd2010.setAttribute( "onchange", "carregaValores(this.value, "+qtd+", '2010','' )" );
		}else{ // IE
			input_cosqtd2010.attachEvent( "onchange", function() { eventosIE( document.getElementById( 'cosqtd_2010_' + qtd ).value, qtd, 2, '2010','' ); } );
		}

	// CAMPO QUANTIDADE 2011
		input_cosqtd2011    = document.createElement('input');
	    input_cosqtd2011.setAttribute('type', 'text');
	    input_cosqtd2011.setAttribute('class', 'normal');
	    input_cosqtd2011.setAttribute('name', 'cosqtd_2011_'+ qtd);
	    input_cosqtd2011.setAttribute('id', 'cosqtd_2011_'+ qtd);
	    input_cosqtd2011.setAttribute('size', '8');
	    input_cosqtd2011.setAttribute('value', cosqtd2011);
	    input_cosqtd2011.setAttribute('onkeypress', "return somenteNumeros(event);");
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_cosqtd2011.setAttribute( "onchange", "carregaValores(this.value, "+qtd+", '2011', '' )" );
		}else{ // IE
			input_cosqtd2011.attachEvent( "onchange", function() { eventosIE( document.getElementById( 'cosqtd_2011_' + qtd ).value, qtd, 2, '2011', '' ); } );
		}

	// CAMPO DE ID DO ITEM DE COMPOSICAO POR ANO
	    input_cosid2010 = document.createElement('input');
	    input_cosid2010.setAttribute('type', 'hidden');
	    input_cosid2010.setAttribute('name', 'cosid2010_'+ qtd);
	    input_cosid2010.setAttribute('id', 'cosid2010_'+ qtd);
	    input_cosid2010.setAttribute('value', cosid2010);

	    input_cosid2011 = document.createElement('input');
	    input_cosid2011.setAttribute('type', 'hidden');
	    input_cosid2011.setAttribute('name', 'cosid2011_'+ qtd);
	    input_cosid2011.setAttribute('id', 'cosid2011_'+ qtd);
	    input_cosid2011.setAttribute('value', cosid2011);

	 // CAMPO VALOR POR METRO QUADRADO ( COSVLRUNI )
		input_cosvlruni    = document.createElement('input');
	    input_cosvlruni.setAttribute('type', 'text');
	    input_cosvlruni.setAttribute('class', 'normal');
	    input_cosvlruni.setAttribute('name', 'cosvlruni_'+ qtd);
	    input_cosvlruni.setAttribute('id', 'cosvlruni_'+ qtd);
	    input_cosvlruni.setAttribute('maxlength', '100');
	    input_cosvlruni.setAttribute('size', '20');
	    input_cosvlruni.setAttribute('value', cosvlruni);
	    input_cosvlruni.setAttribute('onkeypress', "return somenteNumeros(event);");
	    input_cosvlruni.setAttribute('onkeyup', "this.value = mascaraglobal( '###.###.###,##', this.value );");
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_cosvlruni.setAttribute( "onchange", "carregaValores(0, "+qtd+", '', this.value )" );
		}else{ // IE
			input_cosvlruni.attachEvent( "onchange", function() { eventosIE( 0, qtd, 2, '', document.getElementById( 'cosvlruni_' + qtd ).value ); } );
		}

	// CAMPO VALOR UNITARIO DA OBRA
		input_valor_unitario_obra    = document.createElement('input');
	    input_valor_unitario_obra.setAttribute('type', 'text');
	    input_valor_unitario_obra.setAttribute('class', 'disabled');
	    input_valor_unitario_obra.setAttribute('readOnly', 'readonly');
	    input_valor_unitario_obra.setAttribute('name', 'valor_unitario_obra_'+ qtd);
	    input_valor_unitario_obra.setAttribute('id', 'valor_unitario_obra_'+ qtd);
	    input_valor_unitario_obra.setAttribute('size', '20');
	    input_valor_unitario_obra.setAttribute('value', '');

	 // CAMPO VALOR FINAL 2010
		input_valor_final2010    = document.createElement('input');
	    input_valor_final2010.setAttribute('type', 'text');
	    input_valor_final2010.setAttribute('class', 'disabled');
	    input_valor_final2010.setAttribute('readOnly', 'readonly');
	    input_valor_final2010.setAttribute('name', 'valor_final_2010_'+ qtd);
	    input_valor_final2010.setAttribute('id', 'valor_final_2010_'+ qtd);
	    input_valor_final2010.setAttribute('size', '20');
	    input_valor_final2010.setAttribute('value', '');

	 // CAMPO CONTRAPARTIDA 2010
		input_contrapartida_2010    = document.createElement('input');
		input_contrapartida_2010.setAttribute('type', 'text');
		input_contrapartida_2010.setAttribute('class', 'disabled');
		input_contrapartida_2010.setAttribute('readOnly', 'readonly');
		input_contrapartida_2010.setAttribute('name', 'contrapartida_2010_'+ qtd);
		input_contrapartida_2010.setAttribute('id', 'contrapartida_2010_'+ qtd);
		input_contrapartida_2010.setAttribute('size', '20');
		input_contrapartida_2010.setAttribute('value', ''); 

	// CAMPO VALOR FINAL 2011
		input_valor_final2011    = document.createElement('input');
	    input_valor_final2011.setAttribute('type', 'text');
	    input_valor_final2011.setAttribute('class', 'disabled');
	    input_valor_final2011.setAttribute('readOnly', 'readonly');
	    input_valor_final2011.setAttribute('name', 'valor_final_2011_'+ qtd);
	    input_valor_final2011.setAttribute('id', 'valor_final_2011_'+ qtd);
	    input_valor_final2011.setAttribute('size', '20');
	    input_valor_final2011.setAttribute('value', '');

	 // CAMPO CONTRAPARTIDA 2011
		input_contrapartida_2011    = document.createElement('input');
		input_contrapartida_2011.setAttribute('type', 'text');
		input_contrapartida_2011.setAttribute('class', 'disabled');
		input_contrapartida_2011.setAttribute('readOnly', 'readonly');
		input_contrapartida_2011.setAttribute('name', 'contrapartida_2011_'+ qtd);
		input_contrapartida_2011.setAttribute('id', 'contrapartida_2011_'+ qtd);
		input_contrapartida_2011.setAttribute('size', '20');
		input_contrapartida_2011.setAttribute('value', '');

	// CAMPO POR UNIDADE de beneficiario
		input_unidade    = document.createElement('input');
	    input_unidade.setAttribute('type', 'text');
	    input_unidade.setAttribute('class', 'disabled');
	    input_unidade.setAttribute('readOnly', 'readonly');
	    input_unidade.setAttribute('name', 'unidade_'+ qtd);
	    input_unidade.setAttribute('id', 'unidade_'+ qtd);
	    input_unidade.setAttribute('size', '10');
	    input_unidade.setAttribute('value', '');

	 // CAMPO ZONA DO BENEFICIARIO
	    input_zonabeneficiario     = document.createElement('input');
	    input_zonabeneficiario.setAttribute('type', 'hidden');
	    input_zonabeneficiario.setAttribute('name', 'zonabeneficiario_'+ qtd);
	    input_zonabeneficiario.setAttribute('id', 'zonabeneficiario_'+ qtd);
	    input_zonabeneficiario.setAttribute('value', '');

	 // CAMPO TIPO DE SUB A GERAR PRO BENEFICIARIO
	    input_tiposub     = document.createElement('input');
	    input_tiposub.setAttribute('type', 'hidden');
	    input_tiposub.setAttribute('name', 'tiposub_'+ qtd);
	    input_tiposub.setAttribute('id', 'tiposub_'+ qtd);
	    input_tiposub.setAttribute('value', '');

	 // CAMPO UNIDADE DE MEDIDA
	    input_unddid     = document.createElement('input');
	    input_unddid.setAttribute('type', 'hidden');
	    input_unddid.setAttribute('name', 'unddid_'+ qtd);
	    input_unddid.setAttribute('id', 'unddid_'+ qtd);
	    input_unddid.setAttribute('value', '302');


    var novaLinha                    = tabelaItensComposicaoSubAcao.insertRow(tabelaItensComposicaoSubAcao.rows.length);
    novaLinha.id                     = 'linhaItensComposicaoSubAcao' + tabelaItensComposicaoSubAcao.rows.length + 1;
    novaLinha.style.textAlign        = 'center'
    novaLinha.style.backgroundColor  = '#f0f0f0';
    
    //INSERINDO NAS CELULAS DA TABELA
    //Botao excluir
    var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula0.appendChild(btnExcluirItemComposicao);

    //Descricao
    var novaCelula1 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula1.appendChild(input_cosdsc);
    novaCelula1.appendChild(input_descritem);

  	//Unidade de medida
    novaCelula1.appendChild(input_unddid);

    //Metro Quadrado 
    var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula2.appendChild(input_mquadrado);

  	//Área Construida
    var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula3.appendChild(input_areaconstruida);

  	//Quantidade 2010
    var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula4.appendChild(input_cosqtd2010);
    novaCelula4.appendChild(input_cosid2010);

    //Quantidade 2011
    var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula5.appendChild(input_cosqtd2011);
    novaCelula5.appendChild(input_cosid2011);

  	//Valor por metro quadrado cosvlruni
    var novaCelula6 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula6.appendChild(input_cosvlruni);

  	//Valor unitario da obra
    var novaCelula7 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula7.appendChild(input_valor_unitario_obra);

  	//Valor Final 2010
    var novaCelula8 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula8.appendChild(input_valor_final2010);

  	//Valor Contrapartida 2010
    var novaCelula9 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula9.appendChild(input_contrapartida_2010);

  	//Valor Final 2011
    var novaCelula10 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula10.appendChild(input_valor_final2011);

  	//Valor Contrapartida 2011
    var novaCelula11 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula11.appendChild(input_contrapartida_2011);

  	//Por unidade beneficiário
    var novaCelula12 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula12.appendChild(input_unidade);
    novaCelula12.appendChild(input_zonabeneficiario);
    novaCelula12.appendChild(input_tiposub);

    // BUG DO IE PARA CARREGAR O COMBO SELECIONADO
    for (var cont=0; cont < $('cosdsc_'+qtd).length; cont++){
        if($('cosdsc_'+qtd).options[cont].text == cosdsc){
			$('cosdsc_'+qtd).options[cont].selected=true;
        }
    }
    
  	//CARREGA DADOS SE JÁ EXISTIR NO BANCO DE DADOS
    if(carregaBanco == 1 ){
		carregaDadosPorLinha($('cosdsc_'+qtd).value, qtd, $('cosdsc_'+qtd).selectedIndex, cosvlruni);
	}
  	
	if(carregaBanco == 1 && cosqtd2010 != '' && cosvlruni != '' ){
		carregaValores(cosqtd2010, qtd, '2010' );
	}

	if(carregaBanco == 1 && cosqtd2011 != '' && cosvlruni != '' ){
		carregaValores(cosqtd2011, qtd, '2011' );
	}
    return true;
}

function alterarBeneficiario(ano,tipo){
	
	var tabelaItensComposicaoSubAcao = $('itensComposicaoSubAcao');
    var qtdLinha = tabelaItensComposicaoSubAcao.rows.length;
    var tabelaBeneficiario = $('beneficiariosSubAcao');
    var qtdLinhaBen = tabelaBeneficiario.rows.length;
    var valorTotal = 0;
    for (var xp=0; xp < qtdLinhaBen-1; xp++){
		for (var cont=0; cont < qtdLinha-1; cont++){
			if($('tiposubben_'+xp+ano)){
				if( (tipo == $('tiposubben_'+xp+ano).value)){
					if($('tiposub_'+cont)){
						if( $('tiposub_'+cont).value == $('tiposubben_'+xp+ano).value ){
							valorTotal =  valorTotal + (Number($('cosqtd_'+ano+'_'+cont).value) * Number($('unidade_'+cont).value));
						}
					}
				}	
			}
		}
    }

	if(document.getElementById('trbeneficiario_'+ano+tipo)){
		var valorAtual = document.getElementById('trbeneficiario_'+ano+tipo).cells[2].childNodes[0].value;
		document.getElementById('trbeneficiario_'+ano+tipo).cells[2].childNodes[0].value = valorTotal; 
	}
}

function adicionarBeneficiario(qtdDoItem, linha, ano, tiposub){	
    var input_bendsc;
    var input_benid;
    //var qtd = linha;
    var nomeItem =  $('descritem_'+ linha).value;
    var tabelabeneficiariosSubAcao   = $('beneficiariosSubAcao');
    var qtdLinha = tabelabeneficiariosSubAcao.rows.length;
    var qtd = qtdLinha - 1;
    
    var benid = 0;
    var zonaBen = $('zonabeneficiario_'+linha).value == '' ? '' : $('zonabeneficiario_'+linha).value; 
    var unidadeItem = $('unidade_'+linha).value == null ? 0 : $('unidade_'+linha).value; 
    var totalBen = qtdDoItem * unidadeItem;
    
 	// CAMPO DESCRIÇÃO
	input_bendsc    = document.createElement('input');
    input_bendsc.setAttribute('type', 'text');
    input_bendsc.setAttribute('class', 'disabled');
    input_bendsc.setAttribute('readOnly', 'readonly');
    input_bendsc.setAttribute('name', 'bendsc_'+ qtd + ano);
    input_bendsc.setAttribute('id', 'bendsc_'+ qtd + ano);
    input_bendsc.setAttribute('size', '30');
    if( (tiposub == 1) || (tiposub == 1) ){
    	input_bendsc.setAttribute('value', 'CRIANÇAS'); // crianca
    }else{
    	input_bendsc.setAttribute('value', 'ALUNOS'); // aluno 
    }
    
 	// CAMPO Nome do Item
	input_noItem    = document.createElement('input');
	input_noItem.setAttribute('type', 'text');
	input_noItem.setAttribute('class', 'disabled');
	input_noItem.setAttribute('readOnly', 'readonly');
	input_noItem.setAttribute('name', 'input_noItem_'+ qtd + ano);
	input_noItem.setAttribute('id', 'input_noItem_'+ qtd + ano);
	input_noItem.setAttribute('size', '30');
	input_noItem.setAttribute('value', nomeItem);
	input_noItem.setAttribute('title', nomeItem);
    
 	// CAMPO ANO
	input_ano    = document.createElement('input');
    input_ano.setAttribute('type', 'text');
    input_ano.setAttribute('class', 'disabled');
    input_ano.setAttribute('readOnly', 'readonly');
    input_ano.setAttribute('name', 'ano_'+ qtd + ano);
    input_ano.setAttribute('id', 'ano_'+ qtd + ano);
    input_ano.setAttribute('size', '30');
    input_ano.setAttribute('value', ano);

    // CAMPO DE ID BENEFICIARIO
    input_benid     = document.createElement('input');
    input_benid.setAttribute('type', 'hidden');
    input_benid.setAttribute('name', 'benid_'+qtd + ano);
    input_benid.setAttribute('id', 'benid_'+qtd + ano);
    if( (tiposub == 1) || (tiposub == 1) ){
    	input_benid.setAttribute('value', '115'); // crianca
    }else{
    	input_benid.setAttribute('value', '76'); // aluno 
    }

 	// TIPO DE SUBACAO
    input_tiposubben     = document.createElement('input');
    input_tiposubben.setAttribute('type', 'hidden');
    input_tiposubben.setAttribute('name', 'tiposubben_'+qtd + ano);
    input_tiposubben.setAttribute('id', 'tiposubben_'+qtd + ano);
    input_tiposubben.setAttribute('value', tiposub);
    
 	// CAMPO TOTAL DE ITEM
	input_totalBen    = document.createElement('input');
    input_totalBen.setAttribute('type', 'text');
    input_totalBen.setAttribute('class', 'disabled');
    input_totalBen.setAttribute('readOnly', 'readonly');
    input_totalBen.setAttribute('name', 'totalBen_'+ qtd + ano);
    input_totalBen.setAttribute('id', 'totalBen_'+ qtd + ano);
    input_totalBen.setAttribute('size', '30');
    input_totalBen.setAttribute('value', totalBen);

 	// CAMPO ZONA
	input_zona    = document.createElement('input');
    input_zona.setAttribute('type', 'text');
    input_zona.setAttribute('class', 'disabled');
    input_zona.setAttribute('readOnly', 'readonly');
    input_zona.setAttribute('name', 'zona_'+ qtd + ano);
    input_zona.setAttribute('id', 'zona_'+ qtd + ano);
    input_zona.setAttribute('size', '30');
    input_zona.setAttribute('value', zonaBen);

    var novaLinha                    = tabelabeneficiariosSubAcao.insertRow(tabelabeneficiariosSubAcao.rows.length);
    novaLinha.id                     = 'trbeneficiario_'+ano+tiposub;
    novaLinha.style.textAlign        = 'center'
    novaLinha.style.backgroundColor  = '#f0f0f0';

  	//Ano
    var novaCelula1 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula1.appendChild(input_ano);

    //Descricao
    var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula2.appendChild(input_benid);
    novaCelula2.appendChild(input_bendsc);
    novaCelula2.appendChild(input_tiposubben);
    
    //TOTAL DE BENEFICIARIO
    var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula3.appendChild(input_totalBen);

  	//Descricao
    var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula4.appendChild(input_zona);

  	//Descricao
    var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula5.appendChild(input_noItem);
    
}


function carregaDadosPorLinha(tipoDeItem, local, localOption, CaregaVlrDobanco){
	var m2 				= '0';
	var areaConstruida 	= 0;
	var unidadeBen 		= 0;
	var sub 			= 0;
	var zona;
	var i;
	
	switch(Number(tipoDeItem)){
		case 1: 
			m2 = '40 x 70m'; 
			areaConstruida = 1211;
			unidadeBen = 224;
			zona = 'Urbana';
			sub = 1;
		    break;
	    case 2: 
	    	m2 = '60 x 45m'; 
	    	areaConstruida = 565;
	    	unidadeBen = 112;
	    	zona = 'Urbana';
	    	sub = 1;
		    break;
	    case 3: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 734;
	    	unidadeBen = 144;
	    	zona = 'Urbana';
	    	sub = 2;
		    break;
	    case 4:
	    	m2 = '80 x 60'; 
	    	areaConstruida = 854;
	    	unidadeBen = 216;
	    	zona = 'Urbana';
	    	sub = 2;
		    break;
	    case 5: 
	    	m2 = '50 x 60'; 
	    	areaConstruida = 90;
	    	unidadeBen = 36;
	    	zona = 'Rural';
	    	sub = 3;
		    break;
	    case 6: 
	    	m2 = '50 x 60'; 
	    	areaConstruida = 204;
	    	unidadeBen = 72;
	    	zona = 'Rural';
	    	sub = 3;
		    break;
	    case 7: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 734;
	    	unidadeBen = 144;
	    	zona = 'Rural';
	    	sub = 3;
		    break;
	    case 8: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 854;
	    	unidadeBen = 216;
	    	zona = 'Rural';
	    	sub = 3;
		    break;
	    case 9: 
	    	m2 = '50 x 60'; 
	    	areaConstruida = 90;
	    	unidadeBen = 36;
	    	zona = 'Rural';
	    	sub = 4;
		    break;
	    case 10: 
	    	m2 = '50 x 60'; 
	    	areaConstruida = 204;
	    	unidadeBen = 72;
	    	zona = 'Rural';
	    	sub = 4;
		    break;
	    case 11: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 734;
	    	unidadeBen = 144;
	    	zona = 'Rural';
	    	sub = 4;
		    break;
	    case 12: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 854;
	    	unidadeBen = 216;
	    	zona = 'Rural';
	    	sub = 4;
		    break;
	    case 13: 
	    	m2 = '50 x 60'; 
	    	areaConstruida = 90;
	    	unidadeBen = 36;
	    	zona = 'Rural';
	    	sub = 5;
		    break;
	    case 14: 
	    	m2 = '50 x 60'; 
	    	areaConstruida = 204;
	    	unidadeBen = 72;
	    	zona = 'Rural';
	    	sub = 5;
		    break;
	    case 15: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 734;
	    	unidadeBen = 144;
	    	zona = 'Rural';
	    	sub = 5;
		    break;
	    case 16: 
	    	m2 = '80 x 60'; 
	    	areaConstruida = 854;
	    	unidadeBen = 216;
	    	zona = 'Rural';
	    	sub = 5;
		    break;
	    default: 
	    	m2 = '0';  
	    	areaConstruida = 0;
	    	unidadeBen = 0;
	    	zona = 'Rural';
	    	sub = 0;
	    	break;
		}
	//CARREGA VLR UNITÁRIO SE FOR CARREGADO DO BANCO
	if(CaregaVlrDobanco != 0){
		//CaregaVlrDobanco
		CaregaVlrDobanco = parseFloat(replaceAll(replaceAll(CaregaVlrDobanco, ".", ""), ",", "."));
		var valorRealUnitario = Number(CaregaVlrDobanco) / Number(areaConstruida);
		valorRealUnitario 	= mascaraglobal( '###.###.###,##', valorRealUnitario.toFixed(2) );
		$('cosvlruni_'+ local).value = valorRealUnitario;
	}

	// CARREGA METRO QUADRADO
	var campo 	= $('mquadrado_'+ local);
	campo.value = m2;

	//CARREGA ÁREA CONSTRUIDA
	var campoArea 	= $('areaconstruida_'+ local);
	campoArea.value = areaConstruida;

	//CARREGA UNIDADE DE MEDIDA DO BENEFICIARIO
	var campoUnidade 	= $('unidade_'+ local);
	campoUnidade.value = unidadeBen;

	//CARREGA A ZONA DO BENEFICIARIO
	var campoZonaBen 	= $('zonabeneficiario_'+ local);
	campoZonaBen.value = zona;

	//CARREGA TIPO SE SUB 
	var campoTipoSub 	= $('tiposub_'+ local);
	campoTipoSub.value = sub;
	
	// CARREGA PARA DESCRICAO DO ITEM PARA CAMPO HIDDEN
	var descricaoItem 	= $('descritem_'+ local);
	descricaoItem.value = $('cosdsc_'+local).options[$('cosdsc_'+local).selectedIndex].text;
	
	// retira do objeto (combo) o item selecionado para os proximo combo a ser inserido.
	objsDescricao.dados.texto.splice(localOption, 1); 
	objsDescricao.dados.valor.splice(localOption, 1);

	for (i = $('cosdsc_'+local).options.length - 1; i>=0; i--) {
		if(Number(tipoDeItem) != $('cosdsc_'+local).options[i].value ){
			$('cosdsc_'+local).remove(i);
		}
	}

	return true;
}

function carregaValores(qtd, local, ano, vlrItem){
	var qtd2010 				= 0;
	var qtd2011 				= 0;
	var valorUnitario 			= 0;
	var total2010				= 0;
	var total2011				= 0;
	var areaXvlrObra			= 0;
	var campoVlr2010 			= $('valor_final_2010_'+ local);
	var campoVlr2011 			= $('valor_final_2011_'+ local);
	var campoContraPartida2010 	= $('contrapartida_2010_'+ local);
	var campoContraPartida2011 	= $('contrapartida_2011_'+ local);
	var valorUnitarioDaObra 	= $('valor_unitario_obra_'+local);
	var contrapartida2010;
	var contrapartida2011;
	var totalFormatado2010;
	var totalFormatado2011;
	var valorTotalObra2010 = 0;
	var valorTotalObra2011 = 0;
	var tiposub;

	if(ano == ''){ // se o calculo for pelo campo valor
		vlrItem = parseFloat(replaceAll(replaceAll(vlrItem, ".", ""), ",", "."));
		if(vlrItem > 1100.00){
			alert('O valor do metro quadrado não pode ultrapassar R$1.100,00.');
			$('cosvlruni_'+local).value = null;
			return false;
		}

		areaXvlrObra =  Number($('areaconstruida_'+local).value) * vlrItem;
		
		if($('cosqtd_2010_'+local).value){
			qtd2010 = $('cosqtd_'+'2010'+'_'+local).value;
			valorTotalObra2010 = qtd2010 * areaXvlrObra;
		}
		if($('cosqtd_2011_'+local).value){
			qtd2011 = $('cosqtd_'+'2011'+'_'+local).value;
			valorTotalObra2011 = qtd2011 * areaXvlrObra;
		}

		contrapartida2010 	= valorTotalObra2010 * 0.01;
		contrapartida2011 	= valorTotalObra2011 * 0.01;

		totalFormatado2010 	= mascaraglobal( '###.###.###,##', valorTotalObra2010.toFixed(2) );
		totalFormatado2011 	= mascaraglobal( '###.###.###,##', valorTotalObra2011.toFixed(2) );
		contrapartida2010 	= mascaraglobal( '###.###.###,##', contrapartida2010.toFixed(2) );
		contrapartida2011 	= mascaraglobal( '###.###.###,##', contrapartida2011.toFixed(2) );

		valorUnitarioDaObra.value =  mascaraglobal( '###.###.###,##',areaXvlrObra.toFixed(2) );
		
		campoVlr2010.value 				= totalFormatado2010;
		campoVlr2011.value 				= totalFormatado2011;
		campoContraPartida2010.value 	= contrapartida2010;
		campoContraPartida2011.value 	= contrapartida2011;
		
	}else{ // se o calculo for pelos campos de qtd.
		valorUnitario = parseFloat(replaceAll(replaceAll($('cosvlruni_'+ local).value, ".", ""), ",", "."));
		areaXvlrObra =  Number($('areaconstruida_'+local).value) * valorUnitario;
		var total = qtd * areaXvlrObra;

		//valorUnitario = parseFloat(replaceAll(replaceAll($('cosvlruni_'+ local).value, ".", ""), ",", "."));
		//var total = qtd * valorUnitario;
		// valor de contrapartida
		var contrapartida = total * 0.01;

		if(ano == '2010'){
			var campo = $('valor_final_2010_'+ local);
			var campoContraPartida = $('contrapartida_2010_'+ local);
		}else if (ano == '2011'){
			var campo = $('valor_final_2011_'+ local);
			var campoContraPartida = $('contrapartida_2011_'+ local);
		}
		areaXvlrObra =  Number($('areaconstruida_'+local).value) * valorUnitario;
		valorUnitarioDaObra.value =  mascaraglobal( '###.###.###,##',areaXvlrObra.toFixed(2) );
		
		valorTotalObra = qtd * areaXvlrObra;
		
		totalFormatado = mascaraglobal( '###.###.###,##', valorTotalObra.toFixed(2) );
		contrapartida =  mascaraglobal( '###.###.###,##', contrapartida.toFixed(2) );
		campo.value = totalFormatado;
		campoContraPartida.value = contrapartida;

		
	}

	tiposub = $('tiposub_'+ local).value;

	//add beneficiario
	if(qtd){
		if($('trbeneficiario_'+ano+tiposub) == null){
			adicionarBeneficiario(qtd, local ,ano, tiposub );
		}else{
			var exiteBeneficiario = $('trbeneficiario_'+ano+tiposub).id;
			if(exiteBeneficiario){
				alterarBeneficiario(ano, tiposub);
			}
		}
	}
	return true;
}

function removerItemComposicao(linha, cosid2010, cosid2011, qtd, existeDadosNoBanco)
{
 	var select;
 	var numItem;
    var linha = $(linha);
    linha.style.backgroundColor = 'rgb(255,255,210)';
    linha.style.backgroundColor = '#DAE0D2';
    var tiposub = $('tiposub_'+ qtd).value;

    if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
        linha.style.backgroundColor = '#f0f0f0';
    } else {
        $('loader-container').show();
            if (cosid2010 != '' || cosid2011 != ''  ) {
                return new Ajax.Request(window.location.href,
                                           {
                                               method: 'post',
                                               parameters: 'excluir=1&cos2010='+cosid2010+'&cos2011='+cosid2011,
                                               onComplete: function(r)
                                               {  	
                    								if(r.responseText != 'null'){
                   										linha.parentNode.removeChild(linha);
                                                       	linha.style.backgroundColor = '#f0f0f0';
                                                       	removerBeneficiario(qtd, tiposub, existeDadosNoBanco, r.responseText);
                                                        // retira do objeto (combo) o item selecionado para os proximo.
                                	                    objsDescricao = clone(objsDescricaoBac);
                                	        	     	for (z=0;z<document.formulario.elements.length;z++){ 
                                	        	     		if(document.formulario.elements[z].type == 'select-one'){
                                	        	     			selectValor = $(document.formulario.elements[z]).value;
                                	        	     			selectTipo  = $(document.formulario.elements[z]).options[$(document.formulario.elements[z]).selectedIndex].text;
                                	        	     			for (y=0; y < objsDescricao.dados.texto.length; y++){ 
                                	        		     			if(selectTipo == objsDescricao.dados.texto[y] ){
                                	        		     				var localTextoObj = objsDescricao.dados.texto.indexOf(selectTipo);
                                	        		     				objsDescricao.dados.texto.splice(localTextoObj, 1); 
                                	        		     			}
                                	        		     			if(selectValor == objsDescricao.dados.valor[y] ){
                                	        							var localValorObj = objsDescricao.dados.valor.indexOf(selectValor);
                                	        							objsDescricao.dados.valor.splice(localValorObj, 1);
                                	        		     				
                                	        		     			}
                                	        	     			}
                                	        	     		}
                                	        	     	}
                                	        	     	// Fim retira do objeto (combo) o item selecionado para os proximo.
                                	        	     	alterarBeneficiario(2010, tiposub);
           	 											alterarBeneficiario(2011, tiposub);
                                	        	     	$('loader-container').hide();
                    								}else{
														alert("Ocorreu um erro ao tentar excluir o item de composição.");
														 $('loader-container').hide();
														return false;
                    								}  
                                               }
                                           });
            } else {
                var tabelaItensComposicao = $('itensComposicaoSubAcao');
                var qtdLinha = tabelaItensComposicao.rows.length - 1;
				var existeMaisDeUmItemMsmTipo = 0;
                for(var v = 0; v < qtdLinha; v++ ){
					if($('tiposub_'+v)){
	            		if($('tiposub_'+v).value == tiposub ){
	            			existeMaisDeUmItemMsmTipo++;
	            		}
					}
                }
                linha.parentNode.removeChild(linha);
               if(existeMaisDeUmItemMsmTipo == 1 || existeMaisDeUmItemMsmTipo == 0 ){
                	removerBeneficiario(qtd, tiposub, existeDadosNoBanco, 0 );
                }else{
                	alterarBeneficiario(2010, tiposub);
           	 		alterarBeneficiario(2011, tiposub);
                }
               	
               
           	 	
             // retira do objeto (combo) o item selecionado para os proximo.
                objsDescricao = clone(objsDescricaoBac);
    		 	
    	     	for (z=0;z<document.formulario.elements.length;z++){ 
    	     		if(document.formulario.elements[z].type == 'select-one'){
    	     			selectValor = $(document.formulario.elements[z]).value;
    	     			selectTipo  = $(document.formulario.elements[z]).options[$(document.formulario.elements[z]).selectedIndex].text;
    	     			for (y=0; y < objsDescricao.dados.texto.length; y++){ 
    		     			if(selectTipo == objsDescricao.dados.texto[y] ){
    		     				var localTextoObj = objsDescricao.dados.texto.indexOf(selectTipo);
    		     				objsDescricao.dados.texto.splice(localTextoObj, 1); 
    		     			}
    		     			if(selectValor == objsDescricao.dados.valor[y] ){
    							var localValorObj = objsDescricao.dados.valor.indexOf(selectValor);
    							objsDescricao.dados.valor.splice(localValorObj, 1);
    		     			}
    	     			}
    	     		}
    	     	}
    	     	// Fim // retira do objeto (combo) o item selecionado para os proximo.
    	     	$('loader-container').hide(); 
            }  
    }
   
    return true;
}

function removerBeneficiario(linha, tipo, existeDadosNoBanco, sbaid)
{	
	var excluir = true;
	var sucesso = false;
	var totalLinhasItens = $('itensComposicaoSubAcao').rows.length -1;
	var totalLinhasBeneficiarios = $('beneficiariosSubAcao').rows.length - 1;
	var existeQtd2010 = false;
	var existeQtd2011 = false;
	var excluirben2010 = '1';// Pode Excluir
	var excluirben2011 = '1';// Pode Excluir
	var existeMaisDeUmItemMsmTipo = 0;

	for(var x = 0; x < totalLinhasItens; x++ ){
		if($('tiposub_'+x)){
			if($('tiposub_'+x).value == tipo ){
				if($('cosqtd_2010_'+x).value){
					existeMaisDeUmItemMsmTipo++;
				}
			}
		}
	}

	if(existeMaisDeUmItemMsmTipo == 0 ){
		excluirben2010 = '1'; //Pode excluir;
	}else{
		excluirben2010 = '2'; //Não pode excluir;
	}
	
	var existeMaisDeUmItemMsmTipo = 0;
	for(var x = 0; x < totalLinhasItens; x++ ){
		if($('tiposub_'+x)){
			if($('tiposub_'+x).value == tipo ){
				if($('cosqtd_2011_'+x).value){
					existeMaisDeUmItemMsmTipo++;
				}
			}
		}
	}
	if(existeMaisDeUmItemMsmTipo == 0 ){
		excluirben2011 = '1'; //Pode excluir;
	}else{
		excluirben2011 = '2'; //Não pode excluir;
	}

	for(var v = 0; v < totalLinhasBeneficiarios; v++ ){
		//2010
		if($('tiposubben_'+v+'2010')){
			if($('tiposubben_'+v+'2010').value == tipo){
				var benid2010 =  $('benid_'+v+'2010').value;
				alterarBeneficiario(2010, tipo);
				var retira2010 =  $('totalBen_'+v+'2010').value;
				var zona = $('zona_'+v+'2010').value;
		            if (existeDadosNoBanco == 1) { // Se tiver dados no banco de dados deleta do banco
		                var exc = new Ajax.Request(window.location.href,
		                                           {
		                                               method: 'post',
		                                               parameters: '&excluirben2010='+excluirben2010+'&ben2010=' + benid2010 + '&sabano2010=2010&vlr2010='+retira2010+'&sbaid='+sbaid+'&zona='+zona,
		                                               onComplete: function(r)
		                                               { 
		                                                   if (r.responseText != 'null') {
		                                                       excluir = false;
		                                                       alert('Não foi possível excluir o Beneficiario de 2010.');
		                                                   }
		                                               }
		                                           });
		            }
					if(excluirben2010 == '1'){
	            		var linha1 = $('trbeneficiario_2010'+tipo).rowIndex;
	    				document.getElementById('beneficiariosSubAcao').deleteRow(linha1);
	    				sucesso = true;
					}
			}
		}
		// 2011
		if($('tiposubben_'+v+'2011')){
			if($('tiposubben_'+v+'2011').value == tipo){
				var benid2011 =  $('benid_'+v+'2011').value;
				alterarBeneficiario(2011, tipo);
				var retira2011 =  $('totalBen_'+v+'2011').value;
				var zona = $('zona_'+v+'2011').value;
		            if (existeDadosNoBanco == 1) { // Se tiver dados no banco de dados deleta do banco
		                var exc = new Ajax.Request(window.location.href,
		                                           {
		                                               method: 'post',
		                                               parameters: '&excluirben2011='+excluirben2011+'&ben2011=' + benid2011 + '&sabano2011=2011&vlr2011='+retira2011+'&sbaid='+sbaid+'&zona='+zona,
		                                               onComplete: function(r)
		                                               {  
		                                                   if (r.responseText != 'null') {
		                                                       excluir = false;
		                                                       alert('Não foi possível excluir o Beneficiario de 2011.');
		                                                   }
		                                               }
		                                           });
		            }
					
		            if(excluirben2011 == '1'){
		            	var linha2 = $('trbeneficiario_2011'+tipo).rowIndex;
		    			document.getElementById('beneficiariosSubAcao').deleteRow(linha2);
		    			sucesso = true;
		            }
			}
		}
	}
	if(sucesso){
		return true;
	}else{
		return false;
	}
	
}

function eventosIE( obj, qtd, tipo, ano, vlr ){
        var valor = obj.value; 
        var localOption = obj.selectedIndex;
    switch( tipo ){
		case(1): carregaDadosPorLinha(valor, qtd, localOption, 0 ); break;
		case(2): carregaValores(obj, qtd, ano, vlr ); break;
	}
}

function voltarPag(){
	var url 	= 'cte.php?modulo=principal/estrutura_avaliacao&acao=A';
	window.location = url;
}

function salvarDados(tipo){
    var totalLinhasItens         = $('itensComposicaoSubAcao').rows.length -1;
    var totalLinhasBeneficiarios = $('beneficiariosSubAcao').rows.length - 1;
    var alertaQtd;
    var alertaCombo;
    var alertaVlr;

	if(totalLinhasItens == 0){
		alert('Não existe itens de composição. Insira um plano de trabalho.');
		return false;
	}
	if(totalLinhasItens > 1){
		alertaCombo = 'Selecione a descrição dos itens de composição.';
		alertaQtd = 'Obrigatório informar a quantidade em pelo menos um dos anos \r \n para todos os itens inseridos.';
		alertaVlr = 'Existem valores monetários que não foram inseridos.';
	}else{
		alertaCombo = 'Selecione a descrição do item de composição.';
		alertaQtd = 'Obrigatório informar a quantidade em pelo menos um dos anos no item.';
		alertaVlr = 'Existem valores monetários que não foram inseridos.';
	}
	
	for(var x = 0; x < totalLinhasItens; x++ ){
		if($('cosdsc_'+x).value == ''){
			alert(alertaCombo);
			return false;
			break;
		}
		if( ($('cosqtd_2010_'+x).value == '') && ($('cosqtd_2011_'+x).value == '') ){
			alert(alertaQtd);
			return false;
			break;
		}
		if($('cosvlruni_'+x).value == ''){
			alert(alertaVlr);
			return false;
			break;
		}
	}
	
    document.formulario.totallinhasitens.value = totalLinhasItens;
    document.formulario.totallinhasben.value = totalLinhasBeneficiarios;

	if(tipo == 'salva'){
		document.formulario.salvar.value = true;
		document.formulario.enviarparamec.value = false;
	}else if(tipo == 'envia'){
		if(confirm('Após enviar para análise do MEC não é possível altera seu plano de trabalho. \r \n Deseja continuar?')){
			document.formulario.salvar.value = false;
			document.formulario.enviarparamec.value = true;
			document.formulario.submit();
		}else{
			return false;
		}
	}
	document.formulario.submit();
}

$('loader-container').hide();
</script>
<?php 
// CARREGA SUBAÇÃO SE EXISTIR
	function carregaSubacao($ppsid){
		$obSubacao 	= new SubacaoIndicador();
		$sql = "SELECT s.sbaid, sbaformulario, sbaporescola, sbasituacaoarvore, ps.ppsid  
		FROM cte.subacaoindicador  			s
		INNER JOIN cte.proposicaosubacao 	ps ON ps.ppsid = s.ppsid
		INNER JOIN cte.acaoindicador 		a  ON a.aciid = s.aciid 
		INNER JOIN cte.pontuacao 			p  ON p.ptoid = a.ptoid
		INNER JOIN cte.criterio 			c  ON c.crtid = p.crtid
		INNER JOIN cte.indicador 			i  ON i.indid = c.indid
		INNER JOIN cte.areadimensao 		ad ON ad.ardid = i.ardid
		INNER JOIN cte.dimensao 			d  ON d.dimid = ad.dimid 
		WHERE p.inuid = ".$_SESSION['inuid']."
		AND ptostatus = 'A'
		AND indcod = 5
		AND ardcod = 1 
		AND dimcod = 4 
		AND ps.ppsid in (".$ppsid.")";
		//dbg($sql,1);
		$subacao = $obSubacao->carregar($sql);
		return $subacao;
	}

//CARREGA SUBACAO GUIA
	function carregaSubacaoGuia($ppsid){
		$obProposicaoSubacao  	= new ProposicaoSubacao();
			$sql = " select ps.ppsid, a.aciid, p.ppaid, p.crtid, p.ppadsc, pa.ptoid
				from cte.proposicaosubacao 		ps  
				 inner join cte.proposicaoacao 	p ON p.ppaid = ps.ppaid
				 inner join cte.criterio 		c on c.crtid = p.crtid
				 INNER JOIN cte.pontuacao 		pa ON pa.crtid = c.crtid
				 INNER JOIN cte.indicador 		i ON i.indid = c.indid
				 INNER JOIN cte.areadimensao 	ad ON ad.ardid = i.ardid
				 INNER JOIN cte.dimensao 		d ON d.dimid = ad.dimid 
				 left join cte.acaoindicador 	a ON a.ppaid = p.ppaid and pa.ptoid = a.ptoid
				 where inuid = ".$_SESSION['inuid']."
				 AND ptostatus = 'A'  
				 AND indcod = 5 
				 AND ardcod = 1 
				 AND dimcod = 4 
				 AND ps.ppsid in ( ".$ppsid." )";

			$propostaSubacao = $obProposicaoSubacao->carregar($sql);
			return $propostaSubacao;
	}
	
//	LIMPA DADOS DA SUBAÇÃO
function limpaSubacao($sbaid, $tipo){
	global $db;
// Deleta beneficiario
	$sql = "SELECT sbaid, benid, sabano FROM cte.subacaobeneficiario where sbaid = ".$sbaid." and sabano in ('2010', '2011')";
	$dadosBen = $db->carregar($sql);
	if(is_array($dadosBen)){
		foreach($dadosBen as $dados){
			$sql = "DELETE FROM cte.subacaobeneficiario 
						WHERE benid = " . (integer) $dados['benid'] ."
						AND sabano = ". (integer) $dados['sabano'] ."
						AND sbaid = ". (integer) $dados['sbaid'];
			$db->executar($sql);
			$db->commit();
		}
	}
	
// Deleta Composicao Pessoa
	$sql = "SELECT cspid FROM cte.composicaopessoa where sbaid = ".$sbaid." and cspano in ('2010', '2011')";
	$dadosComPessoa = $db->carregar($sql);
	if(is_array($dadosComPessoa)){
		foreach($dadosComPessoa as $dados){
			$sql = "DELETE FROM cte.composicaopessoa 
						WHERE cspid = " . (integer) $dados['cspid'];
			$db->executar($sql);
			$db->commit();
		}
	}
			
// Deleta Parecer
	$sql = "SELECT sptid FROM cte.subacaoparecertecnico where sbaid = ".$sbaid." and sptano in ('2010', '2011')";
	$dadosPar = $db->carregar($sql);
	if(is_array($dadosPar)){
		foreach($dadosPar as $dados){
			$sql = "DELETE FROM cte.subacaoparecertecnico 
						WHERE sptid = " . (integer) $dados['sptid'];
			$db->executar($sql);
			$db->commit();
		}
	}

//Deleta escolas e itens por tipo - se é por escola ou global a subação.
	if($tipo == true){	
		// Delete escolas	
		$sql = "select qfaid from cte.qtdfisicoano where sbaid = ".$sbaid." and qfaano in (2010,2011)";
		$dadosQtdF = $db->carregar($sql);
		if(is_array($dadosQtdF)){
			foreach($dadosQtdF as $dados){
				$sql = "DELETE FROM cte.qtdfisicoano 
						WHERE qfaid = " . (integer) $dados['qfaid'];
				$db->executar($sql);
				$db->commit();
			}
		}
		// Deleta Itens
		$sql = "SELECT cosid FROM cte.composicaosubacao where sbaid = ".$sbaid." and cosano in ('2010', '2011')";
		$dadosIte = $db->carregar($sql);
		if(is_array($dadosIte)){
			foreach($dadosIte as $dados){
				//Deleta quantidade por escolas
				$sql = "DELETE FROM cte.escolacomposicaosubacao where cosid = " . (integer) $dados['cosid'];
				$db->executar($sql);
				$db->commit();
				//delete item
				$sql = "DELETE FROM cte.composicaosubacao 
							WHERE cosid = " . (integer) $dados['cosid'];
				$db->executar($sql);
				$db->commit();
			}
		}		
	}else{
		// Deleta Itens
		$sql = "SELECT cosid FROM cte.composicaosubacao where sbaid = ".$sbaid." and cosano in ('2010', '2011')";
		$dadosIte = $db->carregar($sql);
		if(is_array($dadosIte)){
			foreach($dadosIte as $dados){
				$sql = "DELETE FROM cte.composicaosubacao 
							WHERE cosid = " . (integer) $dados['cosid'];
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function carregaDadosItensAgrupado($sbaid){
	$obItens  	= new ProposicaoSubacao();
	$sql = "	
		SELECT 	cosdsc, 
			sbaid,
			unddid,
			sum(cosid1) as cosid2010, 
			sum(cosano1) as cosano2010, 
			sum(cosqtd1) as cosqtd2010,
			sum(cosid2) as cosid2011, 
			sum(cosano2) as cosano2011, 
			sum(cosqtd2) as cosqtd2011,
			sum(cosvlruni2010) as cosvlruni2010,
			sum(cosvlruni2011) as cosvlruni2011
		FROM (
			SELECT  c1.cosdsc, c1.cosid as cosid1, c1.cosano as cosano1, 0 as cosid2, 0 as cosano2, sbaid, cosqtd as cosqtd1, 0 as cosqtd2,	
			c1.cosvlruni as cosvlruni2010, 0 as cosvlruni2011, unddid	
			FROM cte.composicaosubacao c1 
			WHERE sbaid = ".$sbaid."
			and cosano = '2010'
		UNION ALL
			SELECT  c2.cosdsc, 0 as cosid1, 0 as cosano1,  c2.cosid as cosid2,  c2.cosano as cosano2, sbaid	, 0 as cosqtd1, cosqtd as cosqtd2,
			0 as cosvlruni2010, c2.cosvlruni as cosvlruni2011, unddid	
			FROM cte.composicaosubacao c2 
			WHERE sbaid = ".$sbaid." 
			and cosano = '2011'
			
		) AS foo
		GROUP BY foo.cosdsc, sbaid, unddid
	";
	//dbg($sql);
	return $obItens->carregar($sql);
}
?>