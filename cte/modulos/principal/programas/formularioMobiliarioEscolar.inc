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
	global $db;
	
	
// VERIFICA O ESTADO A QUAL O MUNICÍPIO PERTENCE
$sql = " SELECT mun_estuf FROM cte.instrumentounidade WHERE inuid='".$_SESSION['inuid']."'";
$estuf = $db->pegaUm($sql);

// CARREGA DADOS  Já Exite subacao para o municipio
	$subacao = carregaSubacao();
	//dbg($subacao[0]['sbaid']);
// Carrega escolas cadastradas. // ou Deleta Escolas
if($_REQUEST['req'] == 'carregarEscolas'){
	header('Content-type: text/html;charset=ISO-8859-1');
	$arEscolas = montarSQLEscolas($subacao[0]['sbaid']);		
       $resp = $db->monta_lista_simples($arEscolas, array('Ações', 'Código INEP', 'Escola', 'Ano'), 
        									 	   5000, 5000, 'N', '100%','N');
    die();
}else if( $_REQUEST['req']   == 'removerEscola' && $_REQUEST['qfaid'] != '' ){
    	
        $sql = '
        		DELETE FROM cte.escolacomposicaosubacao
        		WHERE qfaid = ' . (integer) $_REQUEST['qfaid'] . ';

        		DELETE FROM cte.qtdfisicoano
        		WHERE qfaid = ' . (integer) $_REQUEST['qfaid'];

        $db->executar($sql);
        $db->commit();

} else if($_REQUEST['req'] == 'salvaitenscompo'){ // INCLUIR ITENS DE COMPOSIÇÃO
		$linha 	= $_REQUEST['linha'];
		$ano 	= $_REQUEST['ano'];
		//Formata Dados
		$valor = str_replace(".", "", $_REQUEST['vlr']);
		$valor = str_replace(",", ".", $valor);
		if($ano){
			$obComposicaoSub 			= new ComposicaoSubacao();
			$obComposicaoSub->cosdsc 	= $_REQUEST['desc'];
			$obComposicaoSub->cosvlruni = $valor; 
			$obComposicaoSub->unddid 	= $_REQUEST['unddid'];
			$obComposicaoSub->sbaid 	= $_REQUEST['sbaid'];
			$obComposicaoSub->cosano = $ano;
			$obComposicaoSub->cosqtd = '0';//$_REQUEST['cosqtd_2010_'.$i];
			$cosid = $obComposicaoSub->salvar();
			$obComposicaoSub->commit();
		}
		if($cosid){
			die($cosid);
		}else{
			die('erro');
		}
	
}	

	if($subacao[0]['sbaformulario'] == "t" && $subacao[0]['sbaid'] && $subacao[0]['sbasituacaoarvore'] == '1'){
		if($_REQUEST['pr'] != 'A'){
			die( '<script type="text/javascript"> 
					alert("Os dados já foram enviados para análise do MEC. \r \n Não é possível alterar a demanda de melhoria."); 
					window.location.href="cte.php?modulo=principal/estrutura_avaliacao&acao=A";</script>');
		}
	}	
	
	//EXCLUI ITENS DE COMPOSIÇÃO
	if( $_REQUEST['excluir'] ){
		$excluidoComSucesso10 = false;
		$excluidoComSucesso11 = false;
		$cosid2010 = $_REQUEST['cos2010'];
		$cosid2011 = $_REQUEST['cos2011'];
		if($cosid2010){
			$obComposicaoSub = new ComposicaoSubacao();
			$obComposicaoSub->carregarPorId($cosid2010);
			if($obComposicaoSub->excluir($obComposicaoSub->cosid)){
				$obComposicaoSub->commit();
				$excluidoComSucesso10 = true;
			}else{
				$excluidoComSucesso10 = false;
			}
		}
		if($cosid2011){
			$obComposicaoSub = new ComposicaoSubacao();
			$obComposicaoSub->carregarPorId($cosid2011);
			if($obComposicaoSub->excluir($obComposicaoSub->cosid)){
				$obComposicaoSub->commit();
				$excluidoComSucesso11 = true;
			}else{
				$excluidoComSucesso11 = false;
			}
		}
		if($excluidoComSucesso10 || $excluidoComSucesso11){
			die('null');
		}else{
			die();
		}
		
	}

	
	//EXCLUI BENEFICIARIOS
	//---------------- 2010 ------------------//
	if($_REQUEST['excluirben2010']){
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
						AND sbaid = ". (integer) $subacao[0]['sbaid'];
				
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
			
			$vlr2010Urbano = $_REQUEST['vlr2010ur'];
			$vlr2010Rural = $_REQUEST['vlr2010ru'];
			if($vlr2010Urbano == ''){
				$vlr2010Urbano = 0;
			}
			if($vlr2010Rural == ''){
				$vlr2010Rural = 0;
			}
			if($benid2010){
				$valor2010 = $_REQUEST['ben2010'];
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "UPDATE cte.subacaobeneficiario 
						SET vlrrural = ".$vlr2010Rural.", vlrurbano = ".$vlr2010Urbano."
						WHERE benid = " . (integer) $benid2010 ."
						AND sabano = ". (integer) $sabano2010 ."
						AND sbaid = ". (integer) $subacao[0]['sbaid'];
	
				if($obBeneficiarioSub->executar($sql)){
					$obBeneficiarioSub->commit();
					$excluidoComSucesso = true;
				}else{
					$excluidoComSucesso = false;
				}
			}
		}
		//dbg($excluidoComSucesso,1);
		if($excluidoComSucesso){
			die('null');
		}else{
			die();
		}
	}
	
	//---------------- 2011 ------------------//
	if($_REQUEST['excluirben2011'] ){
		$excluidoComSucesso = false;
		$benid2011 = $_REQUEST['ben2011'];
		$sabano2011 = $_REQUEST['sabano2011'];
		
		
		//Exclui Beneficiario
		//dbg($_REQUEST['excluirben2011'],1);
		if($_REQUEST['excluirben2011'] == '1'){
			if($benid2011){
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "DELETE FROM cte.subacaobeneficiario 
						WHERE benid = " . (integer) $benid2011 ."
						AND sabano = ". (integer) $sabano2011 ."
						AND sbaid = ". (integer) $subacao[0]['sbaid'];
	
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
			$vlr2011Urbano = $_REQUEST['vlr2011ur'];
			$vlr2011Rural = $_REQUEST['vlr2011ru'];
			if($vlr2011Urbano == ''){
				$vlr2011Urbano = 0;
			}
			if($vlr2011Rural == ''){
				$vlr2011Rural = 0;
			}
			if($benid2011){
				$valor2011 = $_REQUEST['ben2011'];
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "UPDATE cte.subacaobeneficiario 
						SET vlrrural = ".$vlr2011Rural.", vlrurbano = ".$vlr2011Urbano."
						WHERE benid = " . (integer) $benid2011 ."
						AND sabano = ". (integer) $sabano2011 ."
						AND sbaid = ". (integer) $subacao[0]['sbaid'];
				
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

// DECLARAÇÃO DE VARIAVEIS
	$totalLinhasItens 			= $_REQUEST['totallinhasitens'];
	$totalLinhasBeneficiario 	= $_REQUEST['totallinhasben'];
	$sucessoSub					= false;
	$sucessoItens				= false;

	if($subacao[0]['sbaid']){
		$composicao = $obComposicaoSub->carregaPorSubacao($subacao[0]['sbaid'], "and cosano in( '2010','2011')");
	}
	//dbg($subacao[0]['sbaid']);
/************************* SALVA DADOS ******************************/	
if( $_REQUEST['salvar'] || $_REQUEST['enviarparamec']  ){
	//SALVA SUBACAO 
	if($subacao[0]['sbaid']){// Se exite a subacao, limpa os dados
		$obSubacao->carregarPorId($subacao[0]['sbaid']);
		$obComposicaoSub->excluirPorSubacao($obSubacao->sbaid, "and cosano in( '2010','2011')");
		$obBeneficiarioSub->excluirPorSubacao($obSubacao->sbaid, "and sabano in( '2010','2011')");
		$obSubParecer->carregarPorId($subacao[0]['sbaid']);
		if($obSubParecer->sptparecer == NULL && $obSubacao->ppsid){
			$obProposicaoSubacao->carregarPorId($obSubacao->ppsid);
		}
		
		if($_REQUEST['enviarparamec'] == 'true'){
			$obSubacao->sbasituacaoarvore = '1';
		}else{
			$obSubacao->sbasituacaoarvore = '0';
		}
		$obSubacao->sbaformulario = 'true';
		$obSubacao->salvar();
		$obSubacao->commit();
	}
	
	//SALVA ITENS 
	$totalGlobal2010 = 0;
	$totalGlobal2011 = 0;
	for($i = 0; $i < $totalLinhasItens;  $i++){
		//Formata Dados
		$valor = str_replace(".", "", $_REQUEST['cosvlruni_'.$i]);
		$valor = str_replace(",", ".", $valor);
		global $db;
		// Se quantidade de 2010 preenchida insere item em 2010
		if($_REQUEST['cosqtd_2010_'.$i]){
			$obComposicaoSub 			= new ComposicaoSubacao();
			$obComposicaoSub->cosdsc 	= $_REQUEST['descritem_'.$i];
			$obComposicaoSub->cosvlruni = $valor; 
			$obComposicaoSub->unddid 	= $_REQUEST['unddid_'.$i];
			$obComposicaoSub->sbaid 	= $obSubacao->sbaid;
			$obComposicaoSub->cosano = '2010';
			$obComposicaoSub->cosqtd = $_REQUEST['cosqtd_2010_'.$i];
			$totalGlobal2010 = $totalGlobal2010 + $obComposicaoSub->cosqtd;
			$cosid2010 = $obComposicaoSub->salvar();
			$obComposicaoSub->commit();
			$sucessoItens = true;
			$obComposicaoSub->carregarPorId($cosid2010);
			
			
			//SALVA QTD DE ESCOLAS POR ITEM
			//Deleta todas as qtd das escolas
			$db->executar(sprintf('DELETE FROM cte.escolacomposicaosubacao WHERE cosid = %d',
                          (integer) $obComposicaoSub->cosid));
            // salva dados
			$_REQUEST['objetoqtdescolas2010_'.$i] = str_replace('\"', '"', $_REQUEST['objetoqtdescolas2010_'.$i]);
			$qtdEscItens = json_decode($_REQUEST['objetoqtdescolas2010_'.$i]);
			$ins = 'INSERT INTO cte.escolacomposicaosubacao(
				        qfaid,
				        cosid,
				        ecsqtd
				    ) VALUES (%d, %d, %f)';
			for($x = 0; $x < count($qtdEscItens);  $x++){
				$ins = 'INSERT INTO cte.escolacomposicaosubacao(
				        qfaid,
				        cosid,
				        ecsqtd
				    ) VALUES ('.$qtdEscItens[$x]->{'id'}.', '.$obComposicaoSub->cosid.', '. str_replace(',', '.', str_replace('.', '', $qtdEscItens[$x]->{'qtd'})).')';
				$db->executar($ins);
				$db->commit();
			}
			
		}
		// Se quantidade de 2010 preenchida insere item em 2011
		if($_REQUEST['cosqtd_2011_'.$i]){
			$obComposicaoSub 			= new ComposicaoSubacao();
			//$obComposicaoSub->cosid  	= $_REQUEST['cosid2011_'.$i];
			$obComposicaoSub->cosdsc 	= $_REQUEST['descritem_'.$i];
			$obComposicaoSub->cosvlruni = $valor; 
			$obComposicaoSub->unddid 	= $_REQUEST['unddid_'.$i];
			$obComposicaoSub->sbaid 	= $obSubacao->sbaid;
			$obComposicaoSub->cosano 	= '2011';
			$obComposicaoSub->cosqtd = $_REQUEST['cosqtd_2011_'.$i];
			$totalGlobal2011 = $totalGlobal2011 + $obComposicaoSub->cosqtd;
			$cosid2011 = $obComposicaoSub->salvar();
			$obComposicaoSub->commit();
			$sucessoItens = true;
			$obComposicaoSub->carregarPorId($cosid2011);
			
			//SALVA QTD DE ESCOLAS POR ITEM
			//Deleta todas as qtd das escolas
			$db->executar(sprintf('DELETE FROM cte.escolacomposicaosubacao WHERE cosid = %d',
                          (integer) $obComposicaoSub->cosid));
            // salva dados
			$_REQUEST['objetoqtdescolas2011_'.$i] = str_replace('\"', '"', $_REQUEST['objetoqtdescolas2011_'.$i]);
			$qtdEscItens = json_decode($_REQUEST['objetoqtdescolas2011_'.$i]);
			$ins = 'INSERT INTO cte.escolacomposicaosubacao(
				        qfaid,
				        cosid,
				        ecsqtd
				    ) VALUES (%d, %d, %f)';
			for($x = 0; $x < count($qtdEscItens);  $x++){
				$db->executar(sprintf($ins,
									  $qtdEscItens[$x]->{'id'},
		                	          $obComposicaoSub->cosid,
		                    	      str_replace(',', '.', str_replace('.', '', $qtdEscItens[$x]->{'qtd'}))));
		        $db->commit();
			}
		}

	}
	
	// SALVA DADOS DO PARECER E CRONOGRAMA
		if($_REQUEST['cosqtd_2010_0']){
			$parecerPadrao = $obSubParecer->sptparecer == NULL ? $obProposicaoSubacao->ppsparecerpadrao  : $obSubParecer->sptparecer;
			$obSubParecerTecnico				= new SubacaoParecerTecnico();
			$obSubParecerTecnico->sbaid 		= $obSubacao->sbaid;
			$obSubParecerTecnico->sptparecer 	= $parecerPadrao;
			$obSubParecerTecnico->sptunt 		= $totalGlobal2010;
			$obSubParecerTecnico->sptano 		= 2010;
			$obSubParecerTecnico->sptinicio 	= 4;
			$obSubParecerTecnico->sptfim 		= 12;
			$obSubParecerTecnico->tppid 		= 4;
			$obSubParecerTecnico->ssuid 		= 1;
			
			$obSubParecerTecnico->salvar();
			$obSubParecerTecnico->commit();
		}
		if($_REQUEST['cosqtd_2011_0']){
			$parecerPadrao = $obSubParecer->sptparecer == NULL ? $obProposicaoSubacao->ppsparecerpadrao  : $obSubParecer->sptparecer;
			$obSubParecerTecnico				= new SubacaoParecerTecnico();
			$obSubParecerTecnico->sbaid 		= $obSubacao->sbaid;
			$obSubParecerTecnico->sptparecer 	= $parecerPadrao;
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
		// Se quantidade de 2010 preenchida insere item em 2011
		if($_REQUEST['ano_'.$i.'2010']){
			$_REQUEST['qtd_urbana_'.$i.'2010'] = $_REQUEST['qtd_urbana_'.$i.'2010'] == NULL ? 0 : $_REQUEST['qtd_urbana_'.$i.'2010'];
			$_REQUEST['qtd_rural_'.$i.'2010'] = $_REQUEST['qtd_rural_'.$i.'2010'] == NULL ? 0 : $_REQUEST['qtd_rural_'.$i.'2010'];
			$obBeneficiarioSub 			= new SubacaoBeneficiario();
			$obBeneficiarioSub->sbaid 		= $obSubacao->sbaid;
			$obBeneficiarioSub->vlrurbano 	= $_REQUEST['qtd_urbana_'.$i.'2010'];
			$obBeneficiarioSub->vlrrural 	= $_REQUEST['qtd_rural_'.$i.'2010'];
			$obBeneficiarioSub->sabano 		= $_REQUEST['ano_'.$i.'2010'];
			$obBeneficiarioSub->benid		= $_REQUEST['benid_'.$i.'2010'];
			$sql="INSERT INTO cte.subacaobeneficiario (sbaid,benid,vlrurbano,vlrrural,sabano) 
			VALUES (".$obBeneficiarioSub->sbaid.",".$obBeneficiarioSub->benid.",".$obBeneficiarioSub->vlrurbano.",".$obBeneficiarioSub->vlrrural.",".$obBeneficiarioSub->sabano.")";
			$obBeneficiarioSub->executar($sql);
			$obBeneficiarioSub->commit();
		}
		// Se quantidade de 2011 preenchida insere item em 2011
		if($_REQUEST['ano_'.$i.'2011']){
			$_REQUEST['qtd_urbana_'.$i.'2011'] = $_REQUEST['qtd_urbana_'.$i.'2011'] == NULL ? 0 : $_REQUEST['qtd_urbana_'.$i.'2011'];
			$_REQUEST['qtd_rural_'.$i.'2011'] = $_REQUEST['qtd_rural_'.$i.'2011'] == NULL ? 0 : $_REQUEST['qtd_rural_'.$i.'2011'];
			$obBeneficiarioSub 			= new SubacaoBeneficiario();
			$obBeneficiarioSub->sbaid 		= $obSubacao->sbaid;
			$obBeneficiarioSub->vlrurbano 	= $_REQUEST['qtd_urbana_'.$i.'2011'];
			$obBeneficiarioSub->vlrrural 	= $_REQUEST['qtd_rural_'.$i.'2011'];
			$obBeneficiarioSub->sabano 		= $_REQUEST['ano_'.$i.'2011'];
			$obBeneficiarioSub->benid		= $_REQUEST['benid_'.$i.'2011'];
			$sql="INSERT INTO cte.subacaobeneficiario (sbaid,benid,vlrurbano,vlrrural,sabano) 
			VALUES (".$obBeneficiarioSub->sbaid.",".$obBeneficiarioSub->benid.",".$obBeneficiarioSub->vlrurbano.",".$obBeneficiarioSub->vlrrural.",".$obBeneficiarioSub->sabano.")";
			$obBeneficiarioSub->executar($sql);
			$obBeneficiarioSub->commit();
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
				window.location.href="cte.php?modulo=principal/programas/formularioMobiliarioEscolar&acao=A";</script>');
		}
		
		
	}
}

//----------------- DELETE DADOS SE OS DADOS CADASTRADOS NÃO FOR DO FORMULARIO.
//Se já existe a sub
	if(($subacao[0]['sbaformulario'] == "f") && ($subacao[0]['sbaid'])){
		// Deleta beneficiario
		$sql = "SELECT sbaid, benid, sabano FROM cte.subacaobeneficiario where sbaid = ".$subacao[0]['sbaid']." and sabano in ('2010', '2011')";
		$dadosBen = $obBeneficiarioSub->carregar($sql);
		if(is_array($dadosBen)){
			foreach($dadosBen as $dados){
				$sql = "DELETE FROM cte.subacaobeneficiario 
							WHERE benid = " . (integer) $dados['benid'] ."
							AND sabano = ". (integer) $dados['sabano'] ."
							AND sbaid = ". (integer) $dados['sbaid'];
				$obBeneficiarioSub->executar($sql);
				$obBeneficiarioSub->commit();
			}
		}
		
		// Deleta Composicao Pessoa
		$sql = "SELECT cspid FROM cte.composicaopessoa where sbaid = ".$subacao[0]['sbaid']." and cspano in ('2010', '2011')";
		$dadosComPessoa = $obComposicaoSub->carregar($sql);
		if(is_array($dadosComPessoa)){
			foreach($dadosComPessoa as $dados){
				$sql = "DELETE FROM cte.composicaopessoa 
							WHERE cspid = " . (integer) $dados['cspid'];
				$obComposicaoSub->executar($sql);
				$obComposicaoSub->commit();
			}
		}
		
		// Deleta Parecer
		$sql = "SELECT sptid FROM cte.subacaoparecertecnico where sbaid = ".$subacao[0]['sbaid']." and sptano in ('2010', '2011')";
		$dadosPar = $obSubParecer->carregar($sql);
		if(is_array($dadosPar)){
			foreach($dadosPar as $dados){
				$sql = "DELETE FROM cte.subacaoparecertecnico 
							WHERE sptid = " . (integer) $dados['sptid'];
				$obSubParecer->executar($sql);
				$obSubParecer->commit();
			}
		}
		
		if($subacao[0]['sbaporescola'] == true){	
			// Delete escolas	
			$sql = "select qfaid from cte.qtdfisicoano where sbaid = ".$subacao[0]['sbaid']." and qfaano in (2010,2011)";
			$dadosQtdF = $obComposicaoSub->carregar($sql);
			if(is_array($dadosQtdF)){
				foreach($dadosQtdF as $dados){
					$sql = "DELETE FROM cte.qtdfisicoano 
							WHERE qfaid = " . (integer) $dados['qfaid'];
					$obComposicaoSub->executar($sql);
					$obComposicaoSub->commit();
				}
			}
			// Deleta Itens
			$sql = "SELECT cosid FROM cte.composicaosubacao where sbaid = ".$subacao[0]['sbaid']." and cosano in ('2010', '2011')";
			$dadosIte = $obComposicaoSub->carregar($sql);
			if(is_array($dadosIte)){
				foreach($dadosIte as $dados){
					//Deleta quantidade por escolas
					$sql = "DELETE FROM cte.escolacomposicaosubacao where cosid = " . (integer) $dados['cosid'];
					$obComposicaoSub->executar($sql);
					$obComposicaoSub->commit();
					//delete item
					$sql = "DELETE FROM cte.composicaosubacao 
								WHERE cosid = " . (integer) $dados['cosid'];
					$obComposicaoSub->executar($sql);
					$obComposicaoSub->commit();
				}
			}
			
		}else{
			// Deleta Itens
			$sql = "SELECT cosid FROM cte.composicaosubacao where sbaid = ".$subacao[0]['sbaid']." and cosano in ('2010', '2011')";
			$dadosIte = $obComposicaoSub->carregar($sql);
			if(is_array($dadosIte)){
				foreach($dadosIte as $dados){
					$sql = "DELETE FROM cte.composicaosubacao 
								WHERE cosid = " . (integer) $dados['cosid'];
					$obComposicaoSub->executar($sql);
					$obComposicaoSub->commit();
				}
			}
		}

	}
	
//CRIA SUBAÇÃO CASO NÃO EXISTA
if(!$subacao[0]['sbaid']){
	$propostaSubacao = carregaSubacaoGuia();
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
	$obSubacaoN						= new SubacaoIndicador();
	$obSubacaoN->aciid 				= $propostaSubacao[0]['aciid'];
	$obSubacaoN->undid 				= $obProposicaoSubacao->undid;
	$obSubacaoN->frmid 				= $obProposicaoSubacao->frmid;
	$obSubacaoN->sbadsc 			= $obProposicaoSubacao->ppsdsc;
	$obSubacaoN->sbastgmpl 			= $obProposicaoSubacao->ppsmetodologia;
	$obSubacaoN->sbadata 			= $obData->dataAtual('Y-m-d:H:m:s');
	$obSubacaoN->usucpf 			= $_SESSION['usucpf']; 
	$obSubacaoN->prgid 				= $obProposicaoSubacao->prgid;
	$obSubacaoN->sbaporescola 		= $obProposicaoSubacao->indqtdporescola;
	$obSubacaoN->ppsid 				= $obProposicaoSubacao->ppsid;
	$obSubacaoN->sbaordem 			= $obProposicaoSubacao->ppsordem;
	$obSubacaoN->sbasituacaoarvore 	= '0'; // Mudar depois.
	$obSubacaoN->sbaformulario 		= 'true';
	$sbaid = $obSubacaoN->salvar();
	if($sbaid){
		$obSubacaoN->commit();
		$obSubacaoN->carregarPorId($sbaid);
	}
}
	
//CARREGA DADOS PARA TELA
if($subacao[0]['sbaid'] || $obSubacaoN ){
	$idSubacao = $subacao[0]['sbaid'] == NULL ? $obSubacaoN->sbaid : $subacao[0]['sbaid'];
	$dadosSubacaoItens = carregaDadosItensAgrupado($idSubacao);
	// ITENS DE COMPOSICAO
	if(is_array($dadosSubacaoItens)){
		foreach($dadosSubacaoItens as $chave=>$dados){
			$dadosSubacaoItens[$chave]['cosdsc'] = iconv("ISO-8859-1", "UTF-8", $dados["cosdsc"]);
			//dbg($dadosSubacaoItens[$chave]['cosid2010']);
			//if($dadosSubacaoItens[$chave]['cosid2010']){
				$sql = "SELECT e.*, c.cosid, c.cosano 
						FROM cte.escolacomposicaosubacao e
						INNER JOIN cte.composicaosubacao c on c.cosid = e.cosid 
						WHERE e.cosid = ".$dadosSubacaoItens[$chave]['cosid2010'];
				$arrQtdItens2010[$chave] = $db->carregar($sql);
				
				$sql = "SELECT e.*, c.cosid, c.cosano 
						FROM cte.escolacomposicaosubacao e
						INNER JOIN cte.composicaosubacao c on c.cosid = e.cosid 
						WHERE e.cosid = ".$dadosSubacaoItens[$chave]['cosid2011'];
				$arrQtdItens2011[$chave] = $db->carregar($sql);

			//}
			/*
			else if($dadosSubacaoItens[$chave]['cosid2011']){
				$sql = "SELECT e.*, c.cosid, c.cosano 
						FROM cte.escolacomposicaosubacao e
						INNER JOIN cte.composicaosubacao c on c.cosid = e.cosid 
						WHERE e.cosid = ".$dadosSubacaoItens[$chave]['cosid2011'];
				$arrQtdItens2011[$chave] = $db->carregar($sql);
			}
			*/
		}
		
		echo '<div id="composicao" style="display:none;"  >'.simec_json_encode($dadosSubacaoItens).'</div>';
		
		// QTD DE ITENS DE COMPOSICAO POR ESCOLA
		echo '<div id="qtditensescolas2010" style="display:none;"  >'.simec_json_encode($arrQtdItens2010).'</div>';
		echo '<div id="qtditensescolas2011" style="display:none;"  >'.simec_json_encode($arrQtdItens2011).'</div>';
	}

	//RECUPERA BENEFICIARIO
	$sql = 'select* from cte.subacaobeneficiario where sbaid = '.$idSubacao.' and sabano in (2010,2011);';
	$dadosBen = $db->carregar($sql);
	echo '<div id="beneficiarioretorno" style="display:none;"  >'.simec_json_encode($dadosBen).'</div>';
}


// CRIA CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
cte_montaTitulo($titulo_modulo, 'Programa Mobiliário Escolar');
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
<input type=hidden name="sbaid" id="sbaid" value="<?php echo $idSubacao; ?>" />
<input type=hidden name="enviarparamec" value="false" />
<input type=hidden name="totallinhasitens" value="0" />
<input type=hidden name="totallinhasben" value="0" />
<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<tr>
		<td class="SubTituloCentro">Cadastro de Escolas</td>
	</tr>
	<tr style="background-color: #cccccc;">
		<td>
			<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="escolasSubAcao"></div>
			<div style="padding: 5px 0px 0px 5px">
				<span style="cursor:pointer;"  id="linkInserirEscolas" onclick="return popupSelecionarEscolas('null');"><img src="../imagens/gif_inclui.gif" align="left" style="border: none" />Editar / Inserir Escolas</span>
			</div>
		</td>
	</tr>
</table>
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
				        <td style="padding: 2px; font-weight: bold;">Valor <br> Unitário</td>
				        <td style="padding: 2px; font-weight: bold;">Qtd. <br> 2010</td>
				        <td style="padding: 2px; font-weight: bold;">Qtd. <br> 2011</td>
				        <td style="padding: 2px; font-weight: bold;">Valor <br> 2010</td>
				        <td style="padding: 2px; font-weight: bold;">Valor da Contrapartida <br> do Município 2010 (1% em R$)</td>
				        <td style="padding: 2px; font-weight: bold;">Valor <br> 2011</td>
				        <td style="padding: 2px; font-weight: bold;">Valor da Contrapartida <br> do Município 2011 (1% em R$)</td>
				        <td style="padding: 2px; font-weight: bold;">Unidade de Medida </td>
				        <td style="padding: 2px; font-weight: bold;">Valor <br> Total </td>
			        </tr>		
				</table>
			</div>
			<div style="padding: 5px 0px 0px 5px">
				<span style="cursor:pointer;"  id="linkInserirItem" onclick="return adicionarItemComposicao('null');"><img src="../imagens/gif_inclui.gif" align="left" style="border: none" />Inserir Itens de Composição</span>
			</div>
		</td>
	</tr>
</table>
<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<tr>
		<td class="SubTituloCentro">Beneficiário por Item Solicitado </td>
	</tr>
	<tr style="background-color: #cccccc;">
		<td>
			<div style="margin: 0; padding: 0; height: 120px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
				<table id="beneficiariosSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
					<tr style="text-align: center">
						<td style="padding: 2px; font-weight: bold;">Ano</td>
						<td style="padding: 2px; font-weight: bold;">Descrição do <br> Beneficiário</td>
				        <td style="padding: 2px; font-weight: bold;">Qtd Zona Urbana</td>
				        <td style="padding: 2px; font-weight: bold;">Qtd Zona Rural</td>
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
	var estuf = '<?php echo $estuf; ?>';
	
	var objsDescricao 	= new Object;
	objsDescricao.dados = {
			texto : new Array(	'Selecione',
								'CJA - 03 conjunto para aluno tamanho 3 - mesa e cadeira. altura do aluno entre 1,19 e 1,42m',
								'CJA - 04 conjunto para aluno tamanho 4 - mesa e cadeira. altura do aluno entre 1,33 e 1,59m',
								'CJA - 06 conjunto para aluno tamanho 6 - mesa e cadeira. altura do aluno entre 1,59 e 1,88m',
								'CJP - 01 conjunto para professor - mesa e cadeira',
								'MA - 01 mesa acessível - pessoa em cadeira de rodas'
	
							), 
			valor : new Array('',1,2,3,4,5) 
	};

	
	function clone(obj){
	       
        if(obj == null || typeof(obj) != 'object')
            return obj;
    
        var temp = obj.constructor(); // breaks
    
        for(var key in obj)
            temp[key] = clone(obj[key]);
    
        return temp;
    
    }

	

	var objsDescricaoBac 	= new Object;
	objsDescricaoBac = clone(objsDescricao);
	
	
	<?php if($subacao[0]['sbaid']){ 
			if(is_array($dadosSubacaoItens)){
	?>
		var arDados 	= new Object;
		var arDados = eval('(' + document.getElementById('composicao').innerHTML + ')');
		for (var x = 0; x < arDados.length; x++) {
			adicionarItemComposicao( arDados[x]);
		}

	<?php } } ?>

	function gerarSubacao(){
		var totalLinhasItens         = $('itensComposicaoSubAcao').rows.length -1;
		var totalLinhasBeneficiarios = $('beneficiariosSubAcao').rows.length - 1;

        document.formulario.totallinhasitens.value = totalLinhasItens;
        document.formulario.totallinhasben.value = totalLinhasBeneficiarios;
		document.formulario.salvar.value = true;
		document.formulario.submit();
	}

	function carregaTipoItem(descricao){
		var index;
		index = objsDescricao.dados.texto.indexOf( descricao );
		return index;

	}

	function salvarDados(tipo){
        var totalLinhasItens         = $('itensComposicaoSubAcao').rows.length -1;
        var totalLinhasBeneficiarios = $('beneficiariosSubAcao').rows.length - 1;
        var vlrItem2010 = 0;
        var vlrItem2011 = 0;
        var vlrBen2010 = 0;
        var vlrBen2011 = 0;
        var totalLinhaItem = 0;
        var totalLinhaBen = 0;
        var podeSalvar = false;

        var alertaQtd;
        var alertaCombo;
        var alertaVlr;

    	if( (totalLinhasItens == 0) || (totalLinhasItens < 1)  ){
    		alert('Não existe itens de composição. Insira um plano de trabalho.');
    		return false;
    	}
    	if(totalLinhasItens > 1){
    		alertaCombo = 'Selecione a descrição dos itens de composição.';
    		alertaQtd = 'Obrigatório informar a quantidade em pelo menos um dos anos \r \n para todos os itens inseridos.';
    	}else{
    		alertaCombo = 'Selecione a descrição do item de composição.';
    		alertaQtd = 'Obrigatório informar a quantidade em pelo menos um dos anos no item.';
 
    	}
    	
    	for(var x = 0; x < totalLinhasItens; x++ ){
    		if($('cosdsc_'+x).value == ''){
    			alert(alertaCombo);
    			return false;
    			break;
    		}else{
    			podeSalvar = true;
    		}
    		if( ($('cosqtd_2010_'+x).value == '') && ($('cosqtd_2011_'+x).value == '') ){
    			alert(alertaQtd);
    			return false;
    			break;
    		}else{
    			podeSalvar = true;
    		}
    	}

        document.formulario.totallinhasitens.value = totalLinhasItens;
        document.formulario.totallinhasben.value = totalLinhasBeneficiarios;

        if(totalLinhasBeneficiarios < 1){
			alert('Insira pelo menos um item de composição');
			return false;
        }else{
        	for (var contBen=1; contBen < totalLinhasBeneficiarios + 1; contBen++){
        		totalLinhaBen = 0;
            	vlrBen2010 = document.getElementById('beneficiariosSubAcao').rows[contBen].cells[2].childNodes[0].value;
            	vlrBen2011 = document.getElementById('beneficiariosSubAcao').rows[contBen].cells[3].childNodes[0].value;
            	totalLinhaBen = vlrBen2010 + vlrBen2011; 
				if(totalLinhaBen == 0){
					alert('A quantidadede Beneficiários não pode ser vazia.');
					return false;
				}else{
					podeSalvar = true;
				}
        	}
        }
		
   		if(podeSalvar){
			if(tipo == 'salva'){
				document.formulario.salvar.value = true;
				document.formulario.enviarparamec.value = false;
				document.formulario.submit();
			}else if(tipo == 'envia'){
				if(confirm('Após enviar para análise do MEC não e possível altera seu plano de trabalho. \r \n deseja continuar?')){
					document.formulario.salvar.value = false;
					document.formulario.enviarparamec.value = true;
					document.formulario.submit();
				}else{
					return false;
				}
			}
   		}
	}
	
	function adicionarBeneficiario(qtdDoItem, linha, ano)
    {	
        var input_bendsc;
        var input_benid;
        var linhaItem = linha;
	   // var qtd = 0 //linha;
	    var tabelaBem = $('beneficiariosSubAcao');
	    var qtdLinhaBen = tabelaBem.rows.length;
	    var qtd = qtdLinhaBen - 1;
	   
	    var unidadeItem = $('cosqtd_'+ ano +'_'+ linha).value; //quantidadeBeneficiario(linha);
	    var totalBen = unidadeItem; //qtdDoItem * unidadeItem;
	    var totalUrbana = '';
	    var totalRural = '';
	    var tipo = '';

     	// CAMPO DESCRIÇÃO
		input_bendsc    = document.createElement('input');
        input_bendsc.setAttribute('type', 'text');
        input_bendsc.setAttribute('class', 'disabled');
        input_bendsc.setAttribute('readOnly', 'readonly');
        input_bendsc.setAttribute('name', 'bendsc_'+ qtd + ano);
        input_bendsc.setAttribute('id', 'bendsc_'+ qtd + ano);
        input_bendsc.setAttribute('size', '30');
        if($('cosdsc_'+linha)[0].value != 4){
        	input_bendsc.setAttribute('value', 'ALUNOS'); // mudar de acordo com o programa
    	}else{
    		input_bendsc.setAttribute('value', 'PROFESSORES'); // mudar de acordo com o programa
    	}
        

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
	    if($('cosdsc_'+linha)[0].value != 4){
	    	input_benid.setAttribute('value', '76'); // mudar de acordo com o programa
	    	tipo = 'aluno';
	    }else{
	    	input_benid.setAttribute('value', '82'); // mudar de acordo com o programa
	    	tipo = 'professor';
	    }

     	// Qtd urbana
		input_qtdUrbana    = document.createElement('input');
        input_qtdUrbana.setAttribute('type', 'text');
        input_qtdUrbana.setAttribute('class', 'normal');
        input_qtdUrbana.setAttribute('name', 'qtd_urbana_'+ qtd + ano);
        input_qtdUrbana.setAttribute('id', 'qtd_urbana_'+ qtd + ano);
        input_qtdUrbana.setAttribute('size', '8');
        input_qtdUrbana.setAttribute('value', totalUrbana);
        input_qtdUrbana.setAttribute('onkeyup', "return validaValorBen(this, 'urbana', "+qtd+", "+ano+", '"+tipo+"', '"+linhaItem+"');");
		
     	// Qtd rural
		input_qtdRural    = document.createElement('input');
        input_qtdRural.setAttribute('type', 'text');
        input_qtdRural.setAttribute('class', 'normal');
        input_qtdRural.setAttribute('name', 'qtd_rural_'+ qtd + ano);
        input_qtdRural.setAttribute('id', 'qtd_rural_'+ qtd + ano);
        input_qtdRural.setAttribute('size', '8');
        input_qtdRural.setAttribute('value', totalRural);
        input_qtdRural.setAttribute('onkeyup', "return validaValorBen(this, 'rural', "+qtd+", "+ano+", '"+tipo+"', '"+linhaItem+"');");

	    var tabelabeneficiariosSubAcao   = $('beneficiariosSubAcao');
        var novaLinha                    = tabelabeneficiariosSubAcao.insertRow(tabelabeneficiariosSubAcao.rows.length);
        novaLinha.id                     = 'trbeneficiario_'+ano+tipo;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

      	//Ano
        var novaCelula1 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula1.appendChild(input_ano);

        //Descricao
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula2.appendChild(input_benid);
        novaCelula2.appendChild(input_bendsc);

        //QTD URBANA
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_qtdUrbana);

      	//QTD Rural
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_qtdRural);
    }

    function eventosIE( obj, qtd, tipo, ano ){
        var valor = obj.value; 
        var localOption = obj.selectedIndex;
    	switch( tipo ){
			case(1): carregaValor(valor, qtd, localOption ); break;
			case(2): carregaValorTotal(obj, qtd, ano ); break;
		}
	}

	function validaValorBen(campo, tipo, linha, ano, tipoItem, linhaItem){
		var valor = campo.value;
		var valorMax = 0;
		var totalFinal = 0;
		var totalLinhasItens         = $('itensComposicaoSubAcao').rows.length -1;

		if(tipoItem == 'aluno'){
			for (cont=0; cont < totalLinhasItens; cont++){
				if($('cosdsc_'+cont)[0].value != 4){
					valorMax = Number(valorMax) + Number($('cosqtd_'+ano+'_'+cont).value);
				}
			}
		}else if(tipoItem == 'professor'){
			valorMax = Number($('cosqtd_'+ano+'_'+linhaItem).value);
		}

		if(tipo == 'urbana'){
			var outroValor = $('qtd_rural_'+ linha + ano).value;
		}else{
			var outroValor = $('qtd_urbana_'+ linha + ano).value;
		}
		if(outroValor == '' || outroValor == null){
			valorMax = valorMax * 3;
			if( valor > valorMax){
				alert('O beneficiário não pode ser 3 vezes maior que a quantidade do item.');
				campo.value = '';
				return false;
			}else{
				return true;
			}
		}else{
			totalFinal = Number(valor) + Number(outroValor);
			valorMax = valorMax * 3;

			if(totalFinal > valorMax){
				alert('O beneficiário não pode ser 3 vezes maior que a quantidade do item.');
				campo.value = '';
				return false;
			}else{
				return true;
			}
		}
		
	}

   
	function adicionarItemComposicao(objetoDeDados)
	{	
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
		var cosvlruni 	= objetoDeDados.cosvlruni 	!= 0 ? objetoDeDados.cosvlruni : '';
		var cosdsc 		= objetoDeDados.cosdsc 			!= null ? objetoDeDados.cosdsc : '';
		var unddid 		= objetoDeDados.unddid    		!= null ? objetoDeDados.unddid : '253'; // Mudar de acordo com o programa
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
	            removerItemComposicao(this.parentNode.parentNode.getAttribute("id"), cosid2010, cosid2011, qtd);
	    }

		// CAMPO DESCRIÇÃO
	    input_cosdsc    = document.createElement('select');
	    input_cosdsc.setAttribute('class', 'CampoEstilo');
	    input_cosdsc.setAttribute('name', 'cosdsc_'+qtd);
	    input_cosdsc.setAttribute('id', 'cosdsc_'+qtd);
	    input_cosdsc.setAttribute('value', '');
			// OnKeyUp
			if(window.addEventListener){ // Mozilla, Netscape, Firefox
				input_cosdsc.setAttribute( "onchange", 'carregaValor(this.value, '+qtd+', this.selectedIndex )' );
			}else{ // IE
				input_cosdsc.attachEvent( "onchange", function() { eventosIE( document.getElementById( 'cosdsc_' + qtd ), qtd,1, 0 ); } );
			}
			//$('cosdsc_'+qtd).options[$('cosdsc_'+qtd).selectedIndex].text
			
	    	// CARREGA LISTA DE ITENS QUE PODEM SER INSERIDOS NO PROGRAMA ÓNIBUS
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
			    
		// CAMPO VALOR UNITÁRIO
		input_cosvlruni    = document.createElement('input');
        input_cosvlruni.setAttribute('type', 'text');
        input_cosvlruni.setAttribute('class', 'disabled');
        input_cosvlruni.setAttribute('readOnly', 'readonly');
      	//  input_cosvlruni.setAttribute('disabled', 'disabled');
        input_cosvlruni.setAttribute('name', 'cosvlruni_'+ qtd);
        input_cosvlruni.setAttribute('id', 'cosvlruni_'+ qtd);
        input_cosvlruni.setAttribute('maxlength', '100');
        input_cosvlruni.setAttribute('size', '30');
        input_cosvlruni.setAttribute('value', cosvlruni);
        
     	// CAMPO QUANTIDADE 2010
		input_cosqtd2010    = document.createElement('input');
        input_cosqtd2010.setAttribute('type', 'text');
        input_cosqtd2010.setAttribute('class', 'disabled');
        input_cosqtd2010.setAttribute('readOnly', 'readonly');
        input_cosqtd2010.setAttribute('name', 'cosqtd_2010_'+ qtd);
        input_cosqtd2010.setAttribute('id', 'cosqtd_2010_'+ qtd);
        input_cosqtd2010.setAttribute('size', '8');
        input_cosqtd2010.setAttribute('value', cosqtd2010);
        if(window.addEventListener){ // Mozilla, Netscape, Firefox
        	input_cosqtd2010.setAttribute('onclick', "popupSelecionarEscolasItemComposicao(this, '2010', "+qtd+")");
        }else{ // se IE
        	input_cosqtd2010.attachEvent("onclick", function corrigeIE(){popupSelecionarEscolasItemComposicao(this, '2010', qtd); } );
        }
        
        
        

     	// CAMPO QUANTIDADE 2011
		input_cosqtd2011    = document.createElement('input');
        input_cosqtd2011.setAttribute('type', 'text');
        input_cosqtd2011.setAttribute('class', 'disabled');
        input_cosqtd2011.setAttribute('readOnly', 'readonly');
        input_cosqtd2011.setAttribute('name', 'cosqtd_2011_'+ qtd);
        input_cosqtd2011.setAttribute('id', 'cosqtd_2011_'+ qtd);
        input_cosqtd2011.setAttribute('size', '8');
        input_cosqtd2011.setAttribute('value', cosqtd2011);
        if(window.addEventListener){ // Mozilla, Netscape, Firefox
        	input_cosqtd2011.setAttribute('onclick', "popupSelecionarEscolasItemComposicao(this, '2011', "+qtd+")");
        }else{ // se IE
        	input_cosqtd2010.attachEvent("onclick", function corrigeIE(){popupSelecionarEscolasItemComposicao(this, '2011', qtd); } );
        }

     	// CAMPO VALOR FINAL 2010
		input_valor_final2010    = document.createElement('input');
        input_valor_final2010.setAttribute('type', 'text');
        input_valor_final2010.setAttribute('class', 'disabled');
        input_valor_final2010.setAttribute('readOnly', 'readonly');
        input_valor_final2010.setAttribute('name', 'valor_final_2010_'+ qtd);
        input_valor_final2010.setAttribute('id', 'valor_final_2010_'+ qtd);
        input_valor_final2010.setAttribute('size', '20');
        input_valor_final2010.setAttribute('value', '');

     	// CAMPO VALOR FINAL 2011
		input_valor_final2011    = document.createElement('input');
        input_valor_final2011.setAttribute('type', 'text');
        input_valor_final2011.setAttribute('class', 'disabled');
        input_valor_final2011.setAttribute('readOnly', 'readonly');
        input_valor_final2011.setAttribute('name', 'valor_final_2011_'+ qtd);
        input_valor_final2011.setAttribute('id', 'valor_final_2011_'+ qtd);
        input_valor_final2011.setAttribute('size', '20');
        input_valor_final2011.setAttribute('value', '');

     	// CAMPO CONTRAPARTIDA 2010
		input_contrapartida_2010    = document.createElement('input');
		input_contrapartida_2010.setAttribute('type', 'text');
		input_contrapartida_2010.setAttribute('class', 'disabled');
		input_contrapartida_2010.setAttribute('readOnly', 'readonly');
		input_contrapartida_2010.setAttribute('name', 'contrapartida_2010_'+ qtd);
		input_contrapartida_2010.setAttribute('id', 'contrapartida_2010_'+ qtd);
		input_contrapartida_2010.setAttribute('size', '20');
		input_contrapartida_2010.setAttribute('value', ''); 

		// CAMPO CONTRAPARTIDA 2011
		input_contrapartida_2011    = document.createElement('input');
		input_contrapartida_2011.setAttribute('type', 'text');
		input_contrapartida_2011.setAttribute('class', 'disabled');
		input_contrapartida_2011.setAttribute('readOnly', 'readonly');
		input_contrapartida_2011.setAttribute('name', 'contrapartida_2011_'+ qtd);
		input_contrapartida_2011.setAttribute('id', 'contrapartida_2011_'+ qtd);
		input_contrapartida_2011.setAttribute('size', '20');
		input_contrapartida_2011.setAttribute('value', '');

        // CAMPO DE ID
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

	    // CAMPO OBJETO DE QUANTIDADE DE ITENS POR ESCOLAS
	    input_objetoqtdescolas2010 = document.createElement('input');
	    input_objetoqtdescolas2010.setAttribute('type', 'hidden');
	    input_objetoqtdescolas2010.setAttribute('name', 'objetoqtdescolas2010_'+ qtd);
	    input_objetoqtdescolas2010.setAttribute('id', 'objetoqtdescolas2010_'+ qtd);
	    input_objetoqtdescolas2010.setAttribute('value', '');
	    
	    input_objetoqtdescolas2011 = document.createElement('input');
	    input_objetoqtdescolas2011.setAttribute('type', 'hidden');
	    input_objetoqtdescolas2011.setAttribute('name', 'objetoqtdescolas2011_'+ qtd);
	    input_objetoqtdescolas2011.setAttribute('id', 'objetoqtdescolas2011_'+ qtd);
	    input_objetoqtdescolas2011.setAttribute('value', '');

		// CAMPO UNIDADE DE MEDIDA
	    input_undddsc   = document.createElement('input');
	    input_undddsc.setAttribute('class', 'CampoEstilo');
	    input_undddsc.setAttribute('name', 'undddsc_' + qtd);
	    input_undddsc.setAttribute('id', 'undddsc_' + qtd);
	    input_undddsc.setAttribute('value', 'MOBILIÁRIO'); // mudar de acordo com o programa
	    input_undddsc.setAttribute('class', 'disabled');
	    input_undddsc.setAttribute('readOnly', 'readonly');

	    input_unddid     = document.createElement('input');
	    input_unddid.setAttribute('type', 'hidden');
	    input_unddid.setAttribute('name', 'unddid_'+ qtd);
	    input_unddid.setAttribute('id', 'unddid_'+ qtd);
	    input_unddid.setAttribute('value', unddid);

	 	// CAMPO TOTAL DOS ANOS 2010 E 2011
		input_valortotal    = document.createElement('input');
		input_valortotal.setAttribute('type', 'text');
		input_valortotal.setAttribute('class', 'disabled');
		input_valortotal.setAttribute('readOnly', 'readonly');
		input_valortotal.setAttribute('name', 'valortotal_'+ qtd);
		input_valortotal.setAttribute('id', 'valortotal_'+ qtd);
		input_valortotal.setAttribute('size', '20');
		input_valortotal.setAttribute('value', '');

        
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
        
        //Valor unitário
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula2.appendChild(input_cosvlruni);

        //Quantidade 2010
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula3.appendChild(input_cosqtd2010);
        novaCelula3.appendChild(input_cosid2010);
        novaCelula3.appendChild(input_objetoqtdescolas2010);

        //Quantidade 2011
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_cosqtd2011);
        novaCelula4.appendChild(input_cosid2011);
        novaCelula4.appendChild(input_objetoqtdescolas2011);

        //Valor Final 2010
        var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula5.appendChild(input_valor_final2010);

        //Valor Contrapartida 2010

        var novaCelula6 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula6.appendChild(input_contrapartida_2010);

        //Valor Final 2011
        var novaCelula7 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula7.appendChild(input_valor_final2011);

        //Valor Contrapartida 2011
        var novaCelula8 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula8.appendChild(input_contrapartida_2011);

        //Unidade de medida
        var novaCelula9 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula9.appendChild(input_undddsc);
        novaCelula9.appendChild(input_unddid);

      	//Valor Total 2010 + 2011
        var novaCelula10 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula10.appendChild(input_valortotal);

        // BUG DO IE PARA CARREGAR O COMBO SELECIONADO
        for (var cont=0; cont < $('cosdsc_'+qtd).length; cont++){
            if($('cosdsc_'+qtd).options[cont].text == cosdsc){
				$('cosdsc_'+qtd).options[cont].selected=true;
            }
        }

       
      	//CARREGA DADOS SE JÁ EXISTIR NO BANCO DE DADOS
        if(carregaBanco == 1){
			carregaValor($('cosdsc_' + qtd).value, qtd, $('cosdsc_' + qtd).selectedIndex );
		}
      	
		if(carregaBanco == 1 && cosqtd2010 != '' && cosvlruni != '' ){
			carregaValorTotal(cosqtd2010, qtd, '2010' );
		}

		if(carregaBanco == 1 && cosqtd2011 != '' && cosvlruni != '' ){
			carregaValorTotal(cosqtd2011, qtd, '2011' );
		}
		
		if(carregaBanco == 1){
			// CARREGA QTD DE ITENS POR ESCOLA.
			var arQtdDadosItens2010 	= new Object;
			var arQtdDadosItens2011 	= new Object;
			var arQtdDadosItens2010 = eval('(' + document.getElementById('qtditensescolas2010').innerHTML + ')');
			var arQtdDadosItens2011 = eval('(' + document.getElementById('qtditensescolas2011').innerHTML + ')');
			var contador2010 = 0;
			var contador2011 = 0;
			var esQtd;
			var arEsQtd10 = new Array(); 
			var arEsQtd11 = new Array(); 
			
			for (var c = 0; c < arQtdDadosItens2010.length; c++) { // item
				for (var v = 0; v < arQtdDadosItens2010[c].length; v++) { // qtd por escola
					//alert(Number(arQtdDadosItens[c][v].cosid));
					if(Number(arQtdDadosItens2010[c][v].cosid) == Number(cosid2010) && cosqtd2010 != '' ){
						arEsQtd10[contador2010] = '{"qtd" : ' + arQtdDadosItens2010[c][v].ecsqtd + ', "id" : ' + arQtdDadosItens2010[c][v].qfaid + '}';
						contador2010 ++;	
					}
				}
				if(cosqtd2010 != ''){
					esQtd = '[' + arEsQtd10.join(', ') + ']';
					$('objetoqtdescolas2010_'+qtd).value = esQtd;
				}
			}

			for (var c = 0; c < arQtdDadosItens2011.length; c++) { // item
				for (var v = 0; v < arQtdDadosItens2011[c].length; v++) { // qtd por escola
					if(Number(arQtdDadosItens2011[c][v].cosid) == Number(cosid2011) && cosqtd2011 != '' ){
						arEsQtd11[contador2011] = '{"qtd" : ' + arQtdDadosItens2011[c][v].ecsqtd + ', "id" : ' + arQtdDadosItens2011[c][v].qfaid + '}';
						contador2011 ++;	
					}
				}
				if(cosqtd2011 != ''){
					esQtd = '[' + arEsQtd11.join(', ') + ']';
					$('objetoqtdescolas2011_'+qtd).value = esQtd;
				}
			}
		}
	 	// FIM CARREGA QTD DE ITENS POR ESCOLA.
		
        return true;
	}

	function valorTotalPorLinha(total, linha){
		var valor2010 = 0;
		var valor2011 = 0;
		
		if($('valor_final_2010_'+linha).value != ''){
			valor2010 = replaceAll(replaceAll($('valor_final_2010_'+linha).value, ".", ""), ",", "");	
		}
		if($('valor_final_2011_'+linha).value != ''){
			valor2011 = replaceAll(replaceAll($('valor_final_2011_'+linha).value, ".", ""), ",", "");
		}
		
		var valorfinal = Number(valor2010) + Number(valor2011);
		var campo = $('valortotal_'+ linha);
		var campoT = mascaraglobal( '[###.]###,##', valorfinal );
		campo.value = campoT;
	}

	function carregaValorTotal(qtd, local, ano){
		var valorUnitario = replaceAll(replaceAll($('cosvlruni_'+ local).value, ".", ""), ",", ".");
		var total = qtd * valorUnitario;
		// valor de contrapartida
		var contrapartida = total * 0.01;
		if(ano == '2010'){
			var campo = $('valor_final_2010_'+ local);
			var campoContraPartida = $('contrapartida_2010_'+ local);
		}else if (ano == '2011'){
			var campo = $('valor_final_2011_'+ local);
			var campoContraPartida = $('contrapartida_2011_'+ local);
		}
		
		totalFormatado = mascaraglobal( '[###.]###,##', total.toFixed(2) );
		contrapartida =  mascaraglobal( '[###.]###,##', contrapartida.toFixed(2) );
		campo.value = totalFormatado;
		campoContraPartida.value = contrapartida;
	
		// funcoes carrega valor total
		valorTotalPorLinha(total, local);

		//add beneficiario
		
		var tipo = '';
		if($('cosdsc_'+local)[0].value != 4){
			tipo = 'aluno'; // mudar de acordo com o programa
    	}else{
    		tipo = 'professor'; // mudar de acordo com o programa
    	}

		if($('trbeneficiario_'+ano+tipo) == null){
			adicionarBeneficiario(qtd, local ,ano );
		}
		
		return true;
	}
	
	function carregaValor(tipoDeItem, local, localOption){
		var valor = '0';
		switch(Number(tipoDeItem)){
	    	case 1: 
		    	if(estuf == 'AC' || estuf == 'AM' || estuf == 'RO' || estuf == 'MT' || estuf == 'MS' || estuf == 'PR'  ){
		    		valor = '164,00'; 
			    }
			    else if (estuf == 'AP' || estuf == 'PA' || estuf == 'RR'){
			    	valor = '170,00'; 
			    }
			    else if (estuf == 'MG' || estuf == 'DF' || estuf == 'GO'){
			    	valor = '145,20'; 
			    }
			    else if (estuf == 'BA'){
			    	valor = '154,00'; 
			    }
			    else if (estuf == 'TO' || estuf == 'MA'){
			    	valor = '140,00'; 
			    }
			    else if (estuf == 'PI' || estuf == 'CE'){
			    	valor = '160,00'; 
			    }
			    else if (estuf == 'RN' || estuf == 'PB'){
			    	valor = '128,00'; 
			    }
			    else if (estuf == 'PE' || estuf == 'AL' || estuf == 'SE'){
			    	valor = '149,93'; 
			    }
			    else if (estuf == 'RJ' || estuf == 'ES' ){
			    	valor = '149,50'; 
			    } 
			    else if (estuf == 'SP'){
			    	valor = '128,89'; 
			    }
			    else if (estuf == 'SC' || estuf == 'RS' ){
			    	valor = '145,00'; 
			    }
		    	break;
		    	
	    	case 2: 
	    		if(estuf == 'AC' || estuf == 'AM' || estuf == 'RO' || estuf == 'MT' || estuf == 'MS' || estuf == 'PR' ){
		    		valor = '165,00'; 
			    }
			    else if (estuf == 'AP' || estuf == 'PA' || estuf == 'RR'){
			    	valor = '174,00'; 
			    }
			    else if (estuf == 'MG' || estuf == 'DF' || estuf == 'GO'){
			    	valor = '145,20'; 
			    }
			    else if (estuf == 'BA'){
			    	valor = '155,50'; 
			    }
			    else if (estuf == 'TO' || estuf == 'MA'){
			    	valor = '147,00'; 
			    }
			    else if (estuf == 'PI' || estuf == 'CE'){
			    	valor = '165,00'; 
			    }
			    else if (estuf == 'RN' || estuf == 'PB'){
			    	valor = '135,00'; 
			    }
			    else if (estuf == 'PE' || estuf == 'AL' || estuf == 'SE'){
			    	valor = '150,70'; 
			    }
			    else if (estuf == 'RJ' || estuf == 'ES' ){
			    	valor = '154,00'; 
			    } 
			    else if (estuf == 'SP'){
			    	valor = '134,89'; 
			    }
			    else if (estuf == 'SC' || estuf == 'RS' ){
			    	valor = '154,00'; 
			    }
		    	break;
	    	case 3: 
	    		if(estuf == 'AC' || estuf == 'AM' || estuf == 'RO' || estuf == 'MT' || estuf == 'MS' || estuf == 'PR'  ){
		    		valor = '165,40'; 
			    }
			    else if (estuf == 'AP' || estuf == 'PA' || estuf == 'RR'){
			    	valor = '175,00'; 
			    }
			    else if (estuf == 'MG' || estuf == 'DF' || estuf == 'GO'){
			    	valor = '145,10'; 
			    }
			    else if (estuf == 'BA'){
			    	valor = '156,50'; 
			    }
			    else if (estuf == 'TO' || estuf == 'MA'){
			    	valor = '140,00'; 
			    }
			    else if (estuf == 'PI' || estuf == 'CE'){
			    	valor = '165,00'; 
			    }
			    else if (estuf == 'RN' || estuf == 'PB'){
			    	valor = '136,00'; 
			    }
			    else if (estuf == 'PE' || estuf == 'AL' || estuf == 'SE'){
			    	valor = '151,80'; 
			    }
			    else if (estuf == 'RJ' || estuf == 'ES' ){
			    	valor = '159,25'; 
			    } 
			    else if (estuf == 'SP'){
			    	valor = '139,88'; 
			    }
			    else if (estuf == 'SC' || estuf == 'RS' ){
			    	valor = '155,00'; 
			    }
		    	break;
	    	case 4:
	    		if(estuf == 'AC' || estuf == 'AM' || estuf == 'RO' || estuf == 'MT' || estuf == 'MS' || estuf == 'PR'  ){
		    		valor = '200,00'; 
			    }
			    else if (estuf == 'AP' || estuf == 'PA' || estuf == 'RR'){
			    	valor = '250,00'; 
			    }
			    else if (estuf == 'MG' || estuf == 'DF' || estuf == 'GO'){
			    	valor = '147,00'; 
			    }
			    else if (estuf == 'BA'){
			    	valor = '198,00'; 
			    }
			    else if (estuf == 'TO' || estuf == 'MA'){
			    	valor = '190,00'; 
			    }
			    else if (estuf == 'PI' || estuf == 'CE'){
			    	valor = '209,00'; 
			    }
			    else if (estuf == 'RN' || estuf == 'PB'){
			    	valor = '175,00'; 
			    }
			    else if (estuf == 'PE' || estuf == 'AL' || estuf == 'SE'){
			    	valor = '175,00'; 
			    }
			    else if (estuf == 'RJ' || estuf == 'ES' ){
			    	valor = '189,00'; 
			    } 
			    else if (estuf == 'SP'){
			    	valor = '159,99'; 
			    }
			    else if (estuf == 'SC' || estuf == 'RS' ){
			    	valor = '174,00'; 
			    }
		    	break;
	    	case 5: 
	    		if(estuf == 'AC' || estuf == 'AM' || estuf == 'RO' || estuf == 'MT' || estuf == 'MS' || estuf == 'PR'  ){
		    		valor = '110,00'; 
			    }
			    else if (estuf == 'AP' || estuf == 'PA' || estuf == 'RR'){
			    	valor = '100,00'; 
			    }
			    else if (estuf == 'MG' || estuf == 'DF' || estuf == 'GO'){
			    	valor = '145,00'; 
			    }
			    else if (estuf == 'BA'){
			    	valor = '135,00'; 
			    }
			    else if (estuf == 'TO' || estuf == 'MA'){
			    	valor = '100,00'; 
			    }
			    else if (estuf == 'PI' || estuf == 'CE'){
			    	valor = '154,00'; 
			    }
			    else if (estuf == 'RN' || estuf == 'PB'){
			    	valor = '100,00'; 
			    }
			    else if (estuf == 'PE' || estuf == 'AL' || estuf == 'SE'){
			    	valor = '102,60'; 
			    }
			    else if (estuf == 'RJ' || estuf == 'ES' ){
			    	valor = '100,00'; 
			    } 
			    else if (estuf == 'SP'){
			    	valor = '99,99'; 
			    }
			    else if (estuf == 'SC' || estuf == 'RS' ){
			    	valor = '102,00'; 
			    }
		    	break;
	    	default: 
			    valor = '0'; 
		    	break;
		}
		var campo = $('cosvlruni_'+ local);
		campo.value = valor;
		var descricaoItem = $('descritem_'+ local);
		descricaoItem.value = $('cosdsc_'+local).options[$('cosdsc_'+local).selectedIndex].text;
		
		// retira do objeto (combo) o item selecionado para os proximo.
		objsDescricao.dados.texto.splice(localOption, 1); 
		objsDescricao.dados.valor.splice(localOption, 1);

		var i;
		for (i = $('cosdsc_'+local).options.length - 1; i>=0; i--) {
			if(Number(tipoDeItem) != $('cosdsc_'+local).options[i].value ){
				$('cosdsc_'+local).remove(i);
			}
		}
		
		return true;

	}

	

	
	function voltarPag(){
		var url 	= 'cte.php?modulo=principal/estrutura_avaliacao&acao=A';
		window.location = url;
	}

	function removerBeneficiario(linhaItem, tipo, banco)
	{	
		var excluir 			= true;
		var sucesso 			= false;
		var totalLinhasItens 	= $('itensComposicaoSubAcao').rows.length -1;
		var existeQtd2010 		= false;
		var existeQtd2011 		= false;
		var excluirben2010 		= '1';// Pode Excluir
		var excluirben2011 		= '1';// Pode Excluir
		var errosalvaben2010 	= false;
		var errosalvaben2011 	= false;
		var tabelaBeneficiario = $('beneficiariosSubAcao');
	    var qtdLinhaBeneficiario = tabelaBeneficiario.rows.length;
	    var qtdBen = qtdLinhaBeneficiario - 1;
	    var tipoDesc = '';
	    if(tipo == 'aluno'){
			tipoDesc = 'ALUNOS';
	    }else if(tipo == 'professor'){
	    	tipoDesc = 'PROFESSORES';
	    }
	    // parte1
		if(tipoDesc == 'ALUNOS'){
			for(var x = 0; x <= totalLinhasItens; x++ ){
				if($('cosqtd_2010_'+x) != null){
					if($('cosqtd_2010_'+x).value){
						existeQtd2010 = true; //Se existe qtd 2010 não apago beneficiario de 2010
						excluirben2010 = '2'; //Não pode excluir;
						break;
					}
				}
			}
			for(var x = 0; x <= totalLinhasItens; x++ ){
				if($('cosqtd_2011_'+x) != null){
					if($('cosqtd_2011_'+x).value){
						existeQtd2011 = true; //Se existe qtd 2011 não apago beneficiario de 2011;
						excluirben2011 = '2'; //Não pode excluir;
						break;
					}
				}
			}
		}
		 
		// parte 2
		var mostraAlert = false;
		for(var t = 0; t < qtdBen; t++ ){
			if($('bendsc_'+t+'2010') != null){
				if(tipoDesc == $('bendsc_'+t+'2010').value) {
					if($('qtd_urbana_'+t+'2010').value != ''  ){
						//$('qtd_urbana_'+t+'2010').value = '';
					}else{
						mostraAlert = true;
					}
					if($('qtd_rural_'+t+'2010').value != ''  ){
						//$('qtd_rural_'+t+'2010').value = '';
					}else{
						mostraAlert = true;
					}
				}
			}
			if($('bendsc_'+t+'2011') != null){
				if(tipoDesc == $('bendsc_'+t+'2011').value) {
					if($('qtd_urbana_'+t+'2011').value != '' || $('qtd_urbana_'+t+'2011').value != null ){
						//$('qtd_urbana_'+t+'2011').value = null;
						
					}else{
						mostraAlert = true;
					}
					if($('qtd_rural_'+t+'2011').value != '' || $('qtd_rural_'+t+'2011').value != null ){
						//$('qtd_rural_'+t+'2011').value = null;
						
					}else{
						mostraAlert = true;
					}
				}
			}
			//2010
			if(banco == true){
				
				if($('benid_'+t+'2010') != null){
					var benid2010 =  $('benid_'+t+'2010').value;
					//alterarBeneficiario(2010, tipo);
					var retira2010Urbano = '0';
					var retira2010Rural = '0';
					
					if($('qtd_urbana_'+t+'2010') != null){
						retira2010Urbano =  $('qtd_urbana_'+t+'2010').value;
					}
					if($('qtd_rural_'+t+'2010') != null){
						retira2010Rural =  $('qtd_rural_'+t+'2010').value;
					}

		            if (benid2010 != null) {
	                    var exc = new Ajax.Request(window.location.href,
	                                               {
	                                                   method: 'post',
	                                                   parameters: '&excluirben2010='+ excluirben2010 +'&ben2010=' + benid2010 + '&sabano2010=2010&vlr2010ur='+retira2010Urbano+'&vlr2010ru='+retira2010Rural,
	                                                   onComplete: function(r)
	                                                   { //alert(r.responseText);
	                                                       if (r.responseText != 'null') {
	                                                           excluir = false;
	                                                           errosalvaben2010 = true;
	                                                           
	                                                       }
	                                                   }
	                                               });
		           }
		             
		                
				}
				
				// 2011
				if($('benid_'+t+'2011') != null){
					var benid2011 =  $('benid_'+t+'2011').value;
					//alterarBeneficiario(2011, tipo);
					var retira2011Urbano = '0';
					var retira2011Rural = '0';
					if($('qtd_urbana_'+t+'2011') != null){
						retira2011Urbano =  $('qtd_urbana_'+t+'2011').value;
					}
					if($('qtd_rural_'+t+'2011') != null){
						retira2011Rural =  $('qtd_rural_'+t+'2011').value;
					}

		                if (benid2011 != null) {
		                    var exc = new Ajax.Request(window.location.href,
		                                               {
		                                                   method: 'post',
		                                                   parameters: '&excluirben2011='+ excluirben2011 +'&ben2011=' + benid2011 + '&sabano2011=2011&vlr2011ur='+retira2011Urbano+'&vlr2011ru='+retira2011Rural,
		                                                   onComplete: function(r)
		                                                   { //alert(r.responseText);
		                                                       if (r.responseText != 'null') {
		                                                           excluir = false;
		                                                           errosalvaben2011 = true;
		                                                       }
		                                                   }
		                                               });
		                }
						/*
		                if (excluir && !existeQtd2011){
		                	var linha2 = $('trbeneficiario_2011'+tipo).rowIndex;
		        			document.getElementById('beneficiariosSubAcao').deleteRow(linha2);
		        			sucesso = true;
		                }else{
		                	$('totalBen_'+t+'2011').value = Number($('totalBen_'+t+'2011').value) - Number(retira2011) ;
		                	sucesso = true;
		                }
		                */
				}
			}
			
		}

		

		if ( excluir && !existeQtd2010){
			if($('trbeneficiario_2010'+tipo) != null){
        		var linha1 = $('trbeneficiario_2010'+tipo).rowIndex;
				document.getElementById('beneficiariosSubAcao').deleteRow(linha1);
			}
			deletar = true;
        }else{
        	deletar = false;
        }
		
		if ( excluir && !existeQtd2011){
			if($('trbeneficiario_2011'+tipo) != null){
        		var linha1 = $('trbeneficiario_2011'+tipo).rowIndex;
				document.getElementById('beneficiariosSubAcao').deleteRow(linha1);
			}
			deletar = true;
        }else{
        	deletar = false;
        }
		
		if( (!deletar) && (tipoDesc == 'ALUNOS') ){
			if(mostraAlert == true){
				alert('As quantidades dos beneficiarios serão removidas. Insira novamente.');
				return true;
			}
		}
		if(errosalvaben2010 == true || errosalvaben2011 == true ){
       		alert('Não foi possível excluir o Beneficiario.');
       }

		
		if(sucesso){
			return true;
		}else{
			return false;
		}
		
	}
	
	 function removerItemComposicao(linha, cosid2010, cosid2011, qtd)
	    { 
		 	var select;
		 	var numItem;
		 	var local = qtd;
	        var linha = $(linha);
	        linha.style.backgroundColor = 'rgb(255,255,210)';
	        linha.style.backgroundColor = '#DAE0D2';
	        
	     	// coloca nome correta na tabela de beneficiario
            var tipo = '';
    		if($('cosdsc_'+local)[0].value != 4){
    			tipo = 'aluno'; // mudar de acordo com o programa
        	}else{
        		tipo = 'professor'; // mudar de acordo com o programa
        	}
    		
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
                            								if(r.responseText == 'null'){
                           										linha.parentNode.removeChild(linha);
	                                                           	linha.style.backgroundColor = '#f0f0f0';
	                                                           	removerBeneficiario(qtd, tipo, true );

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
	                                    	        	     	//alterarBeneficiario(2010, tipo);
	               	 											//alterarBeneficiario(2011, tipo);
	                                    	        	     	$('loader-container').hide();
                            								}else{
																alert("Ocorreu um erro ao tentar excluir o item de composição.");
																 $('loader-container').hide();
																return false;
                            								}  
	                                                   }
	                                               });
	                } else {
	                    linha.parentNode.removeChild(linha);
	                    removerBeneficiario(qtd, tipo, false );
	                    //alterarBeneficiario(2010, tipo);
	               	 	//alterarBeneficiario(2011, tipo);
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

	 function popupSelecionarEscolasItemComposicao(input, qfaano, linha)
	    {

		 var cosid = $('cosid'+qfaano+'_'+linha).value;
		 if(cosid == null || cosid == ''){
			 cosid = 0;
		 }
			
		if(qfaano == '2010' && $('objetoqtdescolas2010_'+linha)){
			var obEscolas2010 = $('objetoqtdescolas2010_'+linha).value;
		}else{
			var obEscolas2010 = '';
		}
		if(qfaano == '2011' && $('objetoqtdescolas2011_'+linha)){
			 var obEscolas2011 = $('objetoqtdescolas2011_'+linha).value;
		}else{
			var obEscolas2011 = '';
		}

		return windowOpen('/cte/cte.php?modulo=principal/programas/escolhaEscolasMobEscolar&acao=A&sbaid=<?php echo $idSubacao; ?>&cosid='+cosid+'&qfaano='+qfaano+'&linha='+linha+'&qtd2010='+obEscolas2010+'&qtd2011='+obEscolas2011,
                'escolherEscolasItemComposicao',
                'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
		
	    }

	    function atualizaItensComposicao(strEscolasQtd, ano, linha, total){
		    if(ano == '2010'){
				$('objetoqtdescolas2010_'+linha).value = strEscolasQtd;//objEscolasQtd;
				$('cosqtd_2010_'+linha).value = total;
				carregaValorTotal(total, linha, '2010' );
		    }else if(ano == '2011'){
				$('objetoqtdescolas2011_'+linha).value = strEscolasQtd;
				$('cosqtd_2011_'+linha).value = total;
				carregaValorTotal(total, linha, '2011' );
		    }
	    }

	 function popupSelecionarEscolas(qfaano)
	    {
	        return windowOpen( '/cte/cte.php?modulo=principal/programas/cadastraEscolasMobEscolar&acao=A&tipo=cadastrarEscolas&sbaid='+$("sbaid").value+'&qfaano=' + qfaano,
	                           'popupSelecionarEscolas',
	                           'height=400, width=800, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
	        return false;
	    }

	 function carregarEscolas()
	    {
			$('escolasSubAcao').style.height = "auto";
	        return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&reqt=<?php echo $reqt; ?>&req=carregarEscolas',
	                                    onComplete: function(res)
	                                    {
	                                        $('escolasSubAcao').innerHTML = res.responseText;
	                                        
	                                        
	                                        var totalEscolas = $('somaTotalEscolas').innerHTML;
	                                        
	                                        if( totalEscolas > 20 ){
		                                        $('escolasSubAcao').style.height = "500px";
	                                        }
	                                        else{
		                                        $('escolasSubAcao').style.height = "auto";
	                                        }
	                                        $('loader-container').hide();
	                                    }
	                                });
	    }

	 function removerEscola(qfaid)
	    {
	        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
	            return false;
	        }
	        return new Ajax.Request(window.location.href,
	                                   {
	                                       method: 'post',
	                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerEscola&qfaid=' + qfaid,
	                                       onComplete: function(res)
	                                       {
	                                           carregarEscolas();
	                                       }
	                                   });
	        return false;
	    }

	 carregarEscolas();
	 
	 // Insere valores de Beneficiário.
	 function addValorBen(vlrBen, x){
			var tipoben = '';
			
			if(vlrBen.benid == 76 ){ // alunos
				tipoben = 'aluno';
			}else if(vlrBen.benid == 82 ){ // professores
				tipoben = 'professor';
			}
			if( vlrBen.vlrrural){
				document.getElementById('trbeneficiario_'+vlrBen.sabano+tipoben).cells[3].childNodes[0].value = vlrBen.vlrrural;
			}

			if(vlrBen.vlrurbano){
				document.getElementById('trbeneficiario_'+vlrBen.sabano+tipoben).cells[2].childNodes[0].value = vlrBen.vlrurbano;
			}
			return true;
	}
		
	// Carrega Beneficiarios existentes no BD.
	 <?php if($subacao[0]['sbaid']){ 
			if(is_array($dadosBen)){
		?>
		var arDadosBen 	= new Object;
		var arDadosBen = eval('(' + document.getElementById('beneficiarioretorno').innerHTML + ')');
		for (var j = 0; j < arDadosBen.length; j++) {
			addValorBen( arDadosBen[j], j);
		}

		<?php } } ?>

	 
	 $('loader-container').hide();

</script>
<?php 
	function carregaSubacao(){
		$obSubacao 	= new SubacaoIndicador();
		$sql = "SELECT s.sbaid, sbaformulario, sbaporescola, sbasituacaoarvore 
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
		--  MUDAR DE ACORDO COM O PROGRAMA
		AND indcod = 5
		AND ardcod = 1 
		AND dimcod = 4  
		AND ps.ppsid in (1227,1228,1229,1230,1231)"; // MUDAR DE ACORDO COM O PROGRAMA
		$subacao = $obSubacao->carregar($sql);
		return $subacao;
	}
	
function carregaSubacaoGuia(){
	$obProposicaoSubacao  	= new ProposicaoSubacao();
	$sql = "SELECT ps.ppsid, a.aciid, p.ppaid, p.crtid, p.ppadsc, pa.ptoid
			FROM cte.proposicaosubacao 		ps  
		 		INNER JOIN cte.proposicaoacao 	p ON p.ppaid = ps.ppaid
				INNER JOIN cte.criterio 		c on c.crtid = p.crtid
				INNER JOIN cte.pontuacao 		pa ON pa.crtid = c.crtid
				INNER JOIN cte.indicador 		i ON i.indid = c.indid
				INNER JOIN cte.areadimensao 	ad ON ad.ardid = i.ardid
				INNER JOIN cte.dimensao 		d ON d.dimid = ad.dimid 
		 		LEFT JOIN cte.acaoindicador 	a ON a.ppaid = p.ppaid and pa.ptoid = a.ptoid
		 	WHERE inuid = ".$_SESSION['inuid']."
		 		AND ptostatus = 'A'    
		 		--  MUDAR DE ACORDO COM O PROGRAMA
		 		AND indcod = 5 
		 		AND ardcod = 1 
		 		AND dimcod = 4 
		 		AND ps.ppsid in (1227,1228,1229,1230,1231) "; // MUDAR DE ACORDO COM O PROGRAMA
	$propostaSubacao = $obProposicaoSubacao->carregar($sql);
	return $propostaSubacao;
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
			sum(cosvlruni) as cosvlruni
		FROM (
			SELECT  c1.cosdsc, c1.cosid as cosid1, c1.cosano as cosano1, 0 as cosid2, 0 as cosano2, sbaid, cosqtd as cosqtd1, 0 as cosqtd2,	
			c1.cosvlruni as cosvlruni, unddid	
			FROM cte.composicaosubacao c1 
			WHERE sbaid = ".$sbaid."
			and cosano = '2010'
		UNION ALL
			SELECT  c2.cosdsc, 0 as cosid1, 0 as cosano1,  c2.cosid as cosid2,  c2.cosano as cosano2, sbaid	, 0 as cosqtd1, cosqtd as cosqtd2,
			c2.cosvlruni as cosvlruni, unddid	
			FROM cte.composicaosubacao c2 
			WHERE sbaid = ".$sbaid." 
			and cosano = '2011'
			
		) AS foo
		GROUP BY foo.cosdsc, sbaid, unddid
	";
	
	return $obItens->carregar($sql);
}
		
function montarSQLEscolas($sub){
	global $db;
	$sql = ' SELECT
	            \'<center><img id="removeEscolas" name="removeEscolas" onclick="return removerEscola(\' || q.qfaid || \')" src="/imagens/excluir.gif" style="border:none" title="Excluir" alt="Excluir item" /></center>\' as qfaid,
	            e.entcodent,
	            e.entnome,
	            q.qfaano
	        FROM cte.qtdfisicoano q
	        	INNER JOIN entidade.entidade e on q.entid = e.entid
	        WHERE q.qfaano in (2010, 2011)
	            AND q.sbaid  = ' . $sub  . '
	        ORDER BY entnome, q.qfaano';
		$arEscolas = $db->carregar($sql);	
	
		if( $arEscolas ){
	        $arTotais["qfaid"] = "";
	        $arTotais["entcodent"] = ""; 
	        $arTotais["entnome"] = "<b>Total de Escolas: &nbsp;&nbsp;<span id='somaTotalEscolas'>". count($arEscolas)."</span></b>";
	        $arEscolas[] = $arTotais;
		}else{
			$arEscolas = $sql;
		}
	
	return $arEscolas; 	
}
?>