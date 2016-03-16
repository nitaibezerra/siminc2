<?php

if(!isset($_SESSION['inuid']) || $_SESSION['inuid'] == ''){
	echo "<script>document.location.href = 'cte.php?modulo=principal/estrutura_avaliacao&acao=A'</script>";
	die();
}

// colocar mascara para aceitar apenas números
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

// CARREGA DADOS  Já Exite subacao para o municipio
	$subacao = carregaSubacao();
	//$feitoPeloForm = $subacao[0]['sbaformulario'];
	//dbg($subacao[0]['sbaid']);
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
		else if ($_REQUEST['excluirben'] == '2'){
			$vlr2010 = $_REQUEST['vlr2010'];
			if($benid2010){
				$valor2010 = $_REQUEST['ben2010'];
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "UPDATE cte.subacaobeneficiario 
						SET vlrrural = ".$vlr2010."
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
		else if ($_REQUEST['excluirben'] == '2'){
			$vlr2011 = $_REQUEST['vlr2011'];
			if($benid2010){
				$valor2010 = $_REQUEST['ben2011'];
				$obBeneficiarioSub		= new SubacaoBeneficiario();
				$sql = "UPDATE cte.subacaobeneficiario 
						SET vlrrural = ".$vlr2011."
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
	}else{
		// carrega subacao do guia
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
		if($_REQUEST['enviarparamec'] == 'true'){
			$obSubacao->sbasituacaoarvore = '1';
		}else{
			$obSubacao->sbasituacaoarvore = '0';
		}
		$obSubacao->sbaformulario = 'true';
		$sbaid = $obSubacao->salvar();
		if($sbaid){
			$obSubacao->commit();
			$obSubacao->carregarPorId($sbaid);
			$sucessoSub = true;
		}
		
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
			//$obComposicaoSub->cosid  	= $_REQUEST['cosid2010_'.$i];
			$obComposicaoSub->cosdsc 	= $_REQUEST['descritem_'.$i];
			$obComposicaoSub->cosvlruni = $valor; 
			$obComposicaoSub->unddid 	= $_REQUEST['unddid_'.$i];
			$obComposicaoSub->sbaid 	= $obSubacao->sbaid;
			$obComposicaoSub->cosano = '2010';
			$obComposicaoSub->cosqtd = $_REQUEST['cosqtd_2010_'.$i];
			$totalGlobal2010 = $totalGlobal2010 + $obComposicaoSub->cosqtd;
			$obComposicaoSub->salvar();
			$obComposicaoSub->commit();
			$sucessoItens = true;
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
			$obComposicaoSub->salvar();
			$obComposicaoSub->commit();
			$sucessoItens = true;
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
			$obBeneficiarioSub 			= new SubacaoBeneficiario();
			$obBeneficiarioSub->sbaid 		= $obSubacao->sbaid;
			$obBeneficiarioSub->vlrurbano 	= 0;
			$obBeneficiarioSub->vlrrural 	= $_REQUEST['totalBen_'.$i.'2010'];
			$obBeneficiarioSub->sabano 		= $_REQUEST['ano_'.$i.'2010'];
			$obBeneficiarioSub->benid		= $_REQUEST['benid_'.$i.'2010'];
			$sql="INSERT INTO cte.subacaobeneficiario (sbaid,benid,vlrurbano,vlrrural,sabano) 
			VALUES (".$obBeneficiarioSub->sbaid.",".$obBeneficiarioSub->benid.",".$obBeneficiarioSub->vlrurbano.",".$obBeneficiarioSub->vlrrural.",".$obBeneficiarioSub->sabano.")";
			$obBeneficiarioSub->executar($sql);
			$obBeneficiarioSub->commit();
		}
		// Se quantidade de 2011 preenchida insere item em 2011
		if($_REQUEST['ano_'.$i.'2011']){
			$obBeneficiarioSub 			= new SubacaoBeneficiario();
			$obBeneficiarioSub->sbaid 		= $obSubacao->sbaid;
			$obBeneficiarioSub->vlrurbano 	= 0;
			$obBeneficiarioSub->vlrrural 	= $_REQUEST['totalBen_'.$i.'2011'];
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
				window.location.href="cte.php?modulo=principal/programas/formularioOnibus&acao=A";</script>');
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
	
//CARREGA DADOS PARA TELA
if($subacao[0]['sbaid']){
	$dadosSubacaoItens = carregaDadosItensAgrupado($subacao[0]['sbaid']);
	if(is_array($dadosSubacaoItens)){
		foreach($dadosSubacaoItens as $chave=>$dados){
			$dadosSubacaoItens[$chave]['cosdsc'] = iconv("ISO-8859-1", "UTF-8", $dados["cosdsc"]);
		}
		echo '<div id="composicao" style="display:none;"  >'.simec_json_encode($dadosSubacaoItens).'</div>';
	}
}

// CRIA CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
cte_montaTitulo($titulo_modulo, 'Programa Caminho da Escola');
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
				        <td style="padding: 2px; font-weight: bold;">Valor <br> Unitário (estimado)</td>
				        <td style="padding: 2px; font-weight: bold;">Qtd. <br> 2010</td>
				        <td style="padding: 2px; font-weight: bold;">Qtd. <br> 2011</td>
				        <td style="padding: 2px; font-weight: bold;">Valor <br> 2010</td>
				        <td style="padding: 2px; font-weight: bold;">Valor da Contrapartida <br> do Município 2010 (1% em R$)</td>
				        <td style="padding: 2px; font-weight: bold;">Valor <br> 2011</td>
				        <td style="padding: 2px; font-weight: bold;">Valor da Contrapartida <br> do Município 2011 (1% em R$)</td>
				        <td style="padding: 2px; font-weight: bold;">Unidade de Medida </td>
				        <td style="padding: 2px; font-weight: bold;">Valor <br> Total </td>
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
<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<tr>
		<td class="SubTituloCentro">Beneficiário da Zona Rural por Item Solicitado </td>
	</tr>
	<tr style="background-color: #cccccc;">
		<td>
			<div style="margin: 0; padding: 0; height: 80px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
				<table id="beneficiariosSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
					<tr style="text-align: center">
						<td style="padding: 2px; font-weight: bold;">Ano</td>
						<td style="padding: 2px; font-weight: bold;">Descrição do <br> Beneficiário</td>
				        <td style="padding: 2px; font-weight: bold;">Total</td>
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
	var objsDescricao 	= new Object;
	objsDescricao.dados = {
			texto : new Array(	'Selecione',
								'Ônibus convencional pequeno',
								'Ônibus reforçado médio',
								'Ônibus reforçado grande'	
							), 
			valor : new Array('',1,2,3) 
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

	function carregaTipoItem(descricao){
		var index;
		index = objsDescricao.dados.texto.indexOf( descricao );
		return index;

	}

	function salvarDados(tipo){
        var totalLinhasItens         = $('itensComposicaoSubAcao').rows.length -1;
        var totalLinhasBeneficiarios = $('beneficiariosSubAcao').rows.length - 1;
        var alertaQtd;
        var alertaCombo;

    	if(totalLinhasItens == 0){
    		alert('Não existe itens de composição. Insira um plano de trabalho.');
    		return false;
    	}
    	if(totalLinhasItens > 1){
    		alertaCombo = 'Selecione a descrição dos itens de composição.';
    		alertaQtd = 'Obrigatorio informa a quantidade em pelo menos um dos anos \r \n para todos os itens inseridos.';
    	}else{
    		alertaCombo = 'Selecione a descrição do item de composição.';
    		alertaQtd = 'Obrigatorio informa a quantidade em pelo menos um dos anos no item.';
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
    	}

        document.formulario.totallinhasitens.value = totalLinhasItens;
        document.formulario.totallinhasben.value = totalLinhasBeneficiarios;

		if(tipo == 'salva'){
			document.formulario.salvar.value = true;
			document.formulario.enviarparamec.value = false;
		}else if(tipo == 'envia'){
			if(confirm('Após enviar para análise do MEC não é possível alterar seu plano de trabalho. \r \n Deseja continuar?')){
				document.formulario.salvar.value = false;
				document.formulario.enviarparamec.value = true;
				document.formulario.submit();
			}else{
				return false;
			}
		}
		document.formulario.submit();
	}

	function quantidadeBeneficiario(linha){
		var qtd = '0'; 
		var tipoDeItem = document.getElementById( "cosdsc_"+linha ).value;
		switch(Number(tipoDeItem)){
    	case 1: 
	    	qtd = '58'; 
	    	break;
    	case 2: 
    		qtd = '88'; 
	    	break;
    	case 3: 
    		qtd = '118'; 
	    	break;
    	default: 
    		qtd = '0';  
    		break;
		}
		return qtd;
	}
	
	function alterarBeneficiario(ano){
		var tabelaItensComposicaoSubAcao = $('itensComposicaoSubAcao');
	    var qtdLinha = tabelaItensComposicaoSubAcao.rows.length;
	    var valorTotal = 0;
		
		for (var cont=0; cont < qtdLinha-1; cont++){
				valorTotal =  valorTotal + (Number($('cosqtd_'+ano+'_'+cont).value) * Number($('unidade_'+cont).value));
		}
		//var valorAtual2010 = document.getElementById('trbeneficiario_'+ano).cells[3].childNodes[0].value = unidadeItem;
		if(document.getElementById('trbeneficiario_'+ano)){
			var valorAtual = document.getElementById('trbeneficiario_'+ano).cells[2].childNodes[0].value;
			document.getElementById('trbeneficiario_'+ano).cells[2].childNodes[0].value = valorTotal; //Number(valorAtual) + Number(totalBen); //totalBen;
		}
	}

	function adicionarBeneficiario(qtdDoItem, linha, ano)
    {	
        var input_bendsc;
        var input_benid;
	    var qtd = linha;
	    var unidadeItem = quantidadeBeneficiario(linha);
	    var totalBen = qtdDoItem * unidadeItem;

     	// CAMPO DESCRIÇÃO
		input_bendsc    = document.createElement('input');
        input_bendsc.setAttribute('type', 'text');
        input_bendsc.setAttribute('class', 'disabled');
        input_bendsc.setAttribute('readOnly', 'readonly');
        input_bendsc.setAttribute('name', 'bendsc_'+ qtd + ano);
        input_bendsc.setAttribute('id', 'bendsc_'+ qtd + ano);
        input_bendsc.setAttribute('size', '30');
        input_bendsc.setAttribute('value', 'ALUNOS');

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
	    input_benid.setAttribute('value', '76');

     	// CAMPO TOTAL DE ITEM
		input_totalBen    = document.createElement('input');
        input_totalBen.setAttribute('type', 'text');
        input_totalBen.setAttribute('class', 'disabled');
        input_totalBen.setAttribute('readOnly', 'readonly');
        input_totalBen.setAttribute('name', 'totalBen_'+ qtd + ano);
        input_totalBen.setAttribute('id', 'totalBen_'+ qtd + ano);
        input_totalBen.setAttribute('size', '30');
        input_totalBen.setAttribute('value', totalBen);

	    var tabelabeneficiariosSubAcao   = $('beneficiariosSubAcao');
        var novaLinha                    = tabelabeneficiariosSubAcao.insertRow(tabelabeneficiariosSubAcao.rows.length);
        novaLinha.id                     = 'trbeneficiario_'+ano;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

      	//Ano
        var novaCelula1 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula1.appendChild(input_ano);

        //Descricao
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula2.appendChild(input_benid);
        novaCelula2.appendChild(input_bendsc);

        //TOTAL DE BENEFICIARIO
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_totalBen);
    }

    function eventosIE( obj, qtd, tipo, ano ){
        if(obj != 0){
        	var valor = obj.value; 
        	var localOption = obj.selectedIndex;
        }
    	switch( tipo ){
			case(1): carregaValor(valor, qtd, localOption ); break;
			case(2): carregaValorTotal(obj, qtd, ano ); break;
			case(3): selecao(qtd); break;
		}
	}

   	function selecao(cont){
   		var teste = eval('selec'+cont);
   		return teste;

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
		var cosvlruni 	= objetoDeDados.cosvlruni 		!= 0 ? objetoDeDados.cosvlruni : '';
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
        input_cosqtd2010.setAttribute('class', 'normal');
        input_cosqtd2010.setAttribute('name', 'cosqtd_2010_'+ qtd);
        input_cosqtd2010.setAttribute('id', 'cosqtd_2010_'+ qtd);
        input_cosqtd2010.setAttribute('size', '8');
        input_cosqtd2010.setAttribute('value', cosqtd2010);
        input_cosqtd2010.setAttribute('onkeypress', "return somenteNumeros(event);");

     	// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_cosqtd2010.setAttribute( "onchange", "carregaValorTotal(this.value, "+qtd+", '2010' )" );
		}else{ // IE
			input_cosqtd2010.attachEvent( "onchange", function() { eventosIE( document.getElementById( 'cosqtd_2010_' + qtd ).value, qtd, 2, '2010' ); } );
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
     	// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_cosqtd2011.setAttribute( "onchange", "carregaValorTotal(this.value, "+qtd+", '2011' )" );
		}else{ // IE
			input_cosqtd2011.attachEvent( "onchange", function() { eventosIE( document.getElementById( 'cosqtd_2011_' + qtd ).value, qtd, 2, '2011' ); } );
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

		// CAMPO UNIDADE DE MEDIDA
	    input_undddsc   = document.createElement('input');
	    input_undddsc.setAttribute('class', 'CampoEstilo');
	    input_undddsc.setAttribute('name', 'undddsc_' + qtd);
	    input_undddsc.setAttribute('id', 'undddsc_' + qtd);
	    input_undddsc.setAttribute('value', 'VEÍCULO');
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

		// CAMPO POR UNIDADE de beneficiario
		input_unidade    = document.createElement('input');
        input_unidade.setAttribute('type', 'text');
        input_unidade.setAttribute('class', 'disabled');
        input_unidade.setAttribute('readOnly', 'readonly');
        input_unidade.setAttribute('name', 'unidade_'+ qtd);
        input_unidade.setAttribute('id', 'unidade_'+ qtd);
        input_unidade.setAttribute('size', '10');
        input_unidade.setAttribute('value', '');

        
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

        //Quantidade 2011
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_cosqtd2011);
        novaCelula4.appendChild(input_cosid2011);

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

      	//Por unidade
        var novaCelula11 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula11.appendChild(input_unidade);

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


        return true;
	}

	function valorTotalPorLinha(total, linha){
		/*
		var valorfinal = 0;
		valorfinal = valorfinal + total;
		var campo = $('valortotal_'+ linha);
		var campoT = mascaraglobal( '[###.]###,##', valorfinal );
		campo.value = campoT;
		*/
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

	// qtd = quantidade informada de itens / local = qual campo inserir o dado, ano de referencia
	function carregaValorTotal(qtd, local, ano){
		var valorUnitario = replaceAll(replaceAll($('cosvlruni_'+ local).value, ".", ""), ",", "");
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
		
		totalFormatado = mascaraglobal( '[###.]###,##', total );
		contrapartida =  mascaraglobal( '[###.]###,##', contrapartida );
		campo.value = totalFormatado;
		campoContraPartida.value = contrapartida;
	
		// funcoes carrega valor total
		valorTotalPorLinha(total, local);

		//add beneficiario
		if($('trbeneficiario_'+ano) == null){
			adicionarBeneficiario(qtd, local ,ano );
		}else{
			var exiteBeneficiario = $('trbeneficiario_'+ano).id;
			if(exiteBeneficiario){
				alterarBeneficiario(ano);
			}
		}
		return true;
	}
	
	function carregaValor(tipoDeItem, local, localOption){
		//alert(localOption);
		var valor = '0';
		//var objsDescricaoClone = clone(objsDescricao);
		switch(Number(tipoDeItem)){
	    	case 1: 
		    	valor = '123.000,00'; 
		    	break;
	    	case 2: 
		    	valor = '198.500,00'; 
		    	break;
	    	case 3: 
		    	valor = '203.000,00'; 
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
		var unidadeItem = quantidadeBeneficiario(local);
		$('unidade_'+ local).value = unidadeItem;
		
		return true;

	}

	function voltarPag(){
		var url 	= 'cte.php?modulo=principal/estrutura_avaliacao&acao=A';
		window.location = url;
	}

	function removerBeneficiario(linha)
	{	
		var excluir = true;
		var sucesso = false;
		var totalLinhasItens = $('itensComposicaoSubAcao').rows.length -1;
		var existeQtd2010 = false;
		var existeQtd2011 = false;
		var excluirben2010 = '1';// Pode Excluir
		var excluirben2011 = '1';// Pode Excluir
		for(var x = 0; x < totalLinhasItens; x++ ){
			if($('cosqtd_2010_'+x).value){
				existeQtd2010 = true; //Se existe qtd 2010 não apago beneficiario de 2010
				excluirben2010 = '2'; //Não pode excluir;
				break;
			}
		}
		for(var x = 0; x < totalLinhasItens; x++ ){
			if($('cosqtd_2011_'+x).value){
				existeQtd2011 = true; //Se existe qtd 2011 não apago beneficiario de 2011;
				excluirben2011 = '2'; //Não pode excluir;
				break;
			}
		}
		//2010
		if($('benid_'+linha+'2010') != null){
			var benid2010 =  $('benid_'+linha+'2010').value;
			alterarBeneficiario(2010);
			var retira2010 =  $('totalBen_'+linha+'2010').value;
                if (benid2010 != null) {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: '&excluirben2010='+ excluirben2010 +'&ben2010=' + benid2010 + '&sabano2010=2010&vlr2010='+retira2010,
                                                   onComplete: function(r)
                                                   { 
                                                       if (r.responseText != 'null') {
                                                           excluir = false;
                                                           alert('Não foi possível excluir o Beneficiario.');
                                                       }
                                                   }
                                               });
                }
              
                if ( excluir && !existeQtd2010){
                	var linha1 = $('trbeneficiario_2010').rowIndex;
        			document.getElementById('beneficiariosSubAcao').deleteRow(linha1);
        			sucesso = true;
                }else{
                	$('totalBen_'+linha+'2010').value = Number($('totalBen_'+linha+'2010').value) - Number(retira2010) ;
                	sucesso = true;
                }
		}
		// 2011
		if($('benid_'+linha+'2011') != null){
			var benid2011 =  $('benid_'+linha+'2011').value;
			alterarBeneficiario(2011);
			var retira2011 =  $('totalBen_'+linha+'2011').value;
                if (benid2011 != null) {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: '&excluirben2011='+ excluirben2011 +'&ben2011=' + benid2011 + '&sabano2011=2011&vlr2011='+retira2011,
                                                   onComplete: function(r)
                                                   {  
                                                       if (r.responseText != 'null') {
                                                           excluir = false;
                                                           alert('Não foi possível excluir o Beneficiario.');
                                                       }
                                                   }
                                               });
                }

                if (excluir && !existeQtd2011){
                	var linha2 = $('trbeneficiario_2011').rowIndex;
        			document.getElementById('beneficiariosSubAcao').deleteRow(linha2);
        			sucesso = true;
                }else{
                	$('totalBen_'+linha+'2011').value = Number($('totalBen_'+linha+'2011').value) - Number(retira2011) ;
                	sucesso = true;
                }
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
	        var linha = $(linha);
	        linha.style.backgroundColor = 'rgb(255,255,210)';
	        linha.style.backgroundColor = '#DAE0D2';

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
	                                                           	removerBeneficiario(qtd );
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
	                                    	        	     	alterarBeneficiario(2010);
	               	 											alterarBeneficiario(2011);
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
	                    alterarBeneficiario(2010);
	               	 	alterarBeneficiario(2011);
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

	 <?php if($subacao[0]['sbaid']){ 
			if(is_array($dadosSubacaoItens)){
	?>
	 alterarBeneficiario(2010);
	 alterarBeneficiario(2011);
	 <?php 
	 	}
	 }
	 ?>
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
		AND indcod = 7 
		AND ardcod = 1 
		AND dimcod = 4 
		AND ps.ppsid in (1067,1068,1069,15,14)";
		$subacao = $obSubacao->carregar($sql);
		return $subacao;
	}
	
	function carregaSubacaoGuia(){
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
				 AND indcod = 7 
				 AND ardcod = 1 
				 AND dimcod = 4 
				 AND ps.ppsid in (1067,1068,1069,15,14)
				";
			//dbg($sql,1);
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
			//dbg($sql,1);
			return $obItens->carregar($sql);
		}
?>