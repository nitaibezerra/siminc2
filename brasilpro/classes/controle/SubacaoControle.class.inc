<?php 
class SubacaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function recuperarTiposObraAtivas( $tooid )
	{
		$obPreTipoObra = new PreTipoObra();
		$arTiposObras = $obPreTipoObra->recuperarTiposObraAtivas( $tooid );
		return $arTiposObras;		
	}
	
	public function recuperarPreObra($preid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarPreObra($preid);				
	}
	
	public function recuperarTiposObraDocumentos()
	{
		$obPreTipoDocumento = new PreTipoDocumento();
		$arPreTipoDocumento = $obPreTipoDocumento->recuperarTiposObraDocumentos();
		return $arPreTipoDocumento;		
	}
	
	public function verificaLatitudePreObra($preid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaLatitudePreObra($preid);		
	}
	
	public function recuperarItensComposicaoCronograma($ptoid, $icoid, $boProinfancia = false, $tipofundacao = null)
	{
		$obPreItensComposicao = new PreItensComposicao();
		$arItens = $obPreItensComposicao->recuperarItensComposicaoCronograma($ptoid, $icoid, $boProinfancia, $tipofundacao);
		return $arItens;
	}
	
	public function recuperarValorTotalItensComposicaoCronograma($ptoid, $icoid, $boProinfancia = false, $tipofundacao = null)
	{
		$obPreItensComposicao = new PreItensComposicao();
		$total = $obPreItensComposicao->recuperarValorTotalItensComposicaoCronograma($ptoid, $icoid, $boProinfancia, $tipofundacao);
		return $total;
	}
	
	public function recuperarValorTotalItensComposicaoPlanilha($ptoid, $icoid)
	{
		$obPreItensComposicao = new PreItensComposicao();
		$total = $obPreItensComposicao->recuperarValorTotalItensComposicaoPlanilha($ptoid, $icoid);
		return $total;
	}
	
	public function recuperarItensComposicaoPlanilha($ptoid, $icoid, $tipoFundacao = null)
	{
		$obPreItensComposicao = new PreItensComposicao();
		$arItens = $obPreItensComposicao->recuperarItensComposicaoPlanilha($ptoid, $icoid, $tipoFundacao);
		return $arItens;
	}
	
	public function recuperarItensComposicaoPlanilhaProInfancia($ptoid, $preid, $tipoFundacao = null)
	{
		$obPreItensComposicao = new PreItensComposicao();
		$arItens = $obPreItensComposicao->recuperarItensComposicaoPlanilhaProInfancia($ptoid, $preid, $tipoFundacao);
		return $arItens;
	}
	
	public function salvarSubacaoItensComposicao($arDados)
	{
		$obSubacaoItensComposicao = new SubacaoItensComposicao();
		$obSubacaoItensComposicao->icoid = $arDados['icoid_'];
		$obSubacaoItensComposicao->icodetalhe = $arDados['icodetalhe'];
		$obSubacaoItensComposicao->salvar();
		$obSubacaoItensComposicao->commit();
	}
	
	public function salvarPreObra($arDados)
	{
		$obPreObra = new PreObra();
		$obPreObra->preid = null;
		$obPreObra->tooid = ORIGEM_OBRA_PAC2;
		$obPreObra->salvar();
		$obPreObra->commit();
	}
	
	public function recuperarPreObraPorIcoid($icoid) 
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarPreObraPorIcoid($icoid);
	}
	
	public function recuperaDescricaoMunicipio($muncod) 
	{
		$obMunicipio = new Municipio();
		return $obMunicipio->descricaoMunicipio($muncod);
	}
	
	public function verificaTipoObra($preidsistema, $sisid = null)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaTipoObra($preidsistema, $sisid);
	}

	public function verificaObraFNDE($preidsistema, $sisid = null)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaObraFNDE($preidsistema, $sisid);
	}
	
	public function verificaClassificacaoObra($preidsistema, $sisid = null)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaClassificacaoObra($preidsistema, $sisid);
	}
	
	public function verificaTipoFundacao($preid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaTipoFundacao($preid);
	}
	
	public function verificaFotosObra($preid, $sisid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaFotosObra($preid, $sisid);
	}
	
	public function verificaDocumentosObra($preid, $sisid, $tipoObra, $tipoA = false)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaDocumentosObra($preid, $sisid, $tipoObra, $tipoA);
	}
	
	public function verificaPlanilhaOrcamentaria($preidsistema, $sisid = null, $preid = null)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaPlanilhaOrcamentaria($preidsistema, $sisid, $preid);
	}
	
	public function salvarDadosPlanilhaOrcamentaria($arDados)
	{
			$obPrePlanilhaOrcamentaria = new PrePlanilhaOrcamentaria();
			$obPrePlanilhaOrcamentaria->excluiItensPlanilhaOrcamentaria($arDados['preid'],$arDados['itcid'] );
			$obPrePlanilhaOrcamentaria->ppoid = null;
			$obPrePlanilhaOrcamentaria->preid = $arDados['preid'];
			$obPrePlanilhaOrcamentaria->itcid = $arDados['itcid'];
			$obPrePlanilhaOrcamentaria->ppovalorunitario = $arDados['ppovalorunitario'];
			$obPrePlanilhaOrcamentaria->salvar();	
			$obPrePlanilhaOrcamentaria->commit();	
	}
	
	public function salvarDadosCronogramaFisicoFinanceiro($arDados)
	{
		$arDados['icoid'] = $arDados['icoid'] ? $arDados['icoid'] : null;
		
//		ver($arDados);
		
		$obPreItensComposicaoObra = new PreItensComposicaoObra();
		$obPreItensComposicaoObra->icoid 				= $arDados['icoid'];
		$obPreItensComposicaoObra->preid 				= $arDados['preid'];
		$obPreItensComposicaoObra->icopercprojperiodo   = $arDados['icopercprojperiodo'];
		$obPreItensComposicaoObra->icopercexecutado 	= $arDados['icopercexecutado'];
		$obPreItensComposicaoObra->icoordem 			= $arDados['icoordem'];
		$obPreItensComposicaoObra->icodtinicioitem 		= $arDados['icodtinicioitem'];
		$obPreItensComposicaoObra->icodterminoitem 		= $arDados['icodterminoitem'];
		$obPreItensComposicaoObra->icostatus 			= $arDados['icostatus'];
		$obPreItensComposicaoObra->icodtinclusao 		= $arDados['icodtinclusao'];
		$obPreItensComposicaoObra->itcid 				= $arDados['itcid'];
		$obPreItensComposicaoObra->salvar();
		$obPreItensComposicaoObra->commit();
		
	}
	
	public function comitar($obj){
		$obj->commit();
	}
	public function salvarDadosPreObra($arDados)
	{

		$latitude  = ($arDados['latitude']) ? implode(".", $arDados['latitude']) : '';
		$longitude = ($arDados['longitude']) ? implode(".", $arDados['longitude']) : '';
		$cep 	   = ($arDados['endcep1']) ? str_replace(array(".","-"),"",$arDados['endcep1']) : '';
		
		if($arDados['icoid']){
			$obPreObra = new PreObra();
			$preid = $obPreObra->recuperarPreObraPorIcoid($arDados['icoid'],true);		
		}
		
		$obPreObra = new PreObra($preid);
		if(!$preid){
			$obPreObra->preid = null;
			$obPreObra->docid = 109494; // TESTE
			$obPreObra->presistema = $_SESSION['sisid'];
			$obPreObra->preidsistema = $arDados['icoid'];
			$obPreObra->predtinclusao = date('Y-m-d H:i:s');
		}
		$obPreObra->ptoid 		   = $arDados['ptoid'];
		$obPreObra->preobservacao  = $arDados['preobservacao'];
		$obPreObra->prelogradouro  = $arDados['endlog'];
		$obPreObra->prenumero 	   = $arDados['endnum'];
		$obPreObra->precomplemento = $arDados['endcom'];
		$obPreObra->prebairro 	   = $arDados['endbai'];
		$obPreObra->estuf 		   = $arDados['estuf'];
		$obPreObra->muncod 		   = $arDados['muncod'];
		$obPreObra->precep 		   = $cep;
		$obPreObra->prelatitude    = $latitude;
		$obPreObra->prelongitude   = $longitude;
		$obPreObra->tooid 		   = ORIGEM_OBRA_PAC2;
		if(!$preid){
			$preid = $obPreObra->salvar();
		}else{
			$obPreObra->salvar();	
		}
		if($obPreObra->commit()){
			echo '<script type="text/javascript"> 
					alert("Operação realizada com sucesso.");
					document.location.href = \'par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=dados&icoid='.$obPreObra->preidsistema.'\';
				  </script>';
			exit;
		}
	}
	
	public function salvarDadosProInfancia($arDados)
	{

		$obPreObra = new PreObra();

		$latitude  = ($arDados['latitude']) ? implode(".", $arDados['latitude']) : '';
		$longitude = ($arDados['longitude']) ? implode(".", $arDados['longitude']) : '';
		$cep 	   = ($arDados['endcep1']) ? str_replace(array(".","-"),"",$arDados['endcep1']) : '';
		
		if($arDados['preid']){
			
//			$preid = $obPreObra->recuperarPreObraPorPreid($arDados['preid'],true);
			$preid = $arDados['preid'];		
		}
		
		$obPreObra = new PreObra($preid);
		if(!$preid){
			$obPreObra->preid = null;
			$obPreObra->presistema = SIS_OBRAS;
			$obPreObra->predtinclusao = date('Y-m-d H:i:s');
		}
		
		//Controle criado por Eduardo em 27/10/2010
		if( ($arDados['ptoid'] == '5') && ($arDados['entcodent'] == '') ){
			$stAba = "&tipoAba=dados";
			echo '<script type="text/javascript"> 
					alert("Escolha uma escola.");
					document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A'.$stAba.'&preid='.$obPreObra->preid.'&ano='.$obPreObra->preano.'\';
				  </script>';
			exit;
		}
		
		//Controle criado por Eduardo em 27/10/2010
		$tipoB = Array(2,7);
		if( in_array( $arDados['ptoid'], $tipoB ) && ($arDados['pretipofundacao'] == '') ){
			$stAba = "&tipoAba=dados";
			echo '<script type="text/javascript"> 
					alert("Escolha um tipo de fundação.");
					window.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A'.$stAba.'&preid='.$obPreObra->preid.'&ano='.$obPreObra->preano.'\';
				  </script>';
			exit;
		}
		
		//Início - Regra definida pelo Daniel em 20/09/10
		// Em caso de mudança de tipo de fundação, deve-se excluir os dados referentes a obra nas tabelas obras.preplanilhaorcamentaria e obras.precronograma
		if($obPreObra->ptoid != $arDados['ptoid']){
			$obPreObra->excluiDadosPreObra($preid);
		}
		//Fim - Regra definida pelo Daniel em 20/09/10
		$obPreObra->ptoid 		   = $arDados['ptoid'];
		
		$obPreObra->preobservacao  = $arDados['preobservacao'];
		$obPreObra->predescricao   = $arDados['predescricao'];
		$obPreObra->prelogradouro  = $arDados['endlog'];
		$obPreObra->prenumero 	   = $arDados['endnum'];
		$obPreObra->precomplemento = $arDados['endcom'];
		$obPreObra->prebairro 	   = $arDados['endbai'];
		$obPreObra->preano 		   = $arDados['preano'];
		$obPreObra->entcodent	   = $arDados['entcodent'];
		$obPreObra->estuf 		   = $arDados['estuf'];
		if($arDados['muncod_']){
			$obPreObra->muncod 		   = $arDados['muncod_'];
		}
		$obPreObra->estufpar 	   = $_SESSION['par']['estuf']  ? $_SESSION['par']['estuf']  : null;
		$obPreObra->muncodpar 	   = $_SESSION['par']['muncod'] ? $_SESSION['par']['muncod'] : null;
		
		//Início - Regra definida pelo Daniel em 20/09/10
		// Em caso de mudança de tipo de fundação, deve-se excluir os dados referentes a obra nas tabelas obras.preplanilhaorcamentaria e obras.precronograma
		if($obPreObra->pretipofundacao != $arDados['pretipofundacao']){
			$obPreObra->excluiDadosPreObra($preid);
		}
		//Fim - Regra definida pelo Daniel em 20/09/10
		
		//Regra definida pelo Daniel em 14/10/10
		$tipoB = Array(2,7);
		if( in_array($obPreObra->ptoid, $tipoB) ){
			$obPreObra->pretipofundacao = $arDados['pretipofundacao'];
		}else{
			$obPreObra->pretipofundacao = '';
		}
		//Fim - Regra definida pelo Daniel em 14/10/10
		
		$obPreObra->precep 		   = $cep;
		$obPreObra->prelatitude    = $latitude;
		$obPreObra->prelongitude   = $longitude;
		$obPreObra->tooid 		   = $arDados['origem'];
		$preid = $obPreObra->salvar();
		
		if($obPreObra->commit()){
			if($arDados['preid'] == ''){
				if(preCriarDocumento($preid, WF_FLUXO_PRONATEC)){
					$obPreObra->commit();
				}
			}else{
				$obPreObra->commit();
			}
			
			$_SESSION['par']['preid'] = $preid;
			
			if($arDados['acao'] == 'proximo'){
				$stAba = "&tipoAba=questionario";
			}
			
			switch ($obPreObra->tooid){
				CASE '1': $programa = 'proinfancia/popupProInfancia';
					break;
				CASE '3': $programa = 'pronatec/popupPronatec';
					break;
			}
				
			echo '<script type="text/javascript"> 
					alert("Operação realizada com sucesso.");
					window.location.href = \'par.php?modulo=principal/programas/'.$programa.'&acao=A'.$stAba.'&preid='.$obPreObra->preid.'&ano='.$obPreObra->preano.'\';
				  </script>';
			exit;
		}
	}
	
	public function listaDocumentos()
	{
		$obPreObraAnexo = new PreObraAnexo();
		$arDados = $obPreObraAnexo->listaDocumentos();
		
		// CABEÇALHO da lista
		$arCabecalho = array("Ação",
                             "Data Inclusão",
                             "Tipo do Documento",
                             "Nome Arquivo",
                             "Tamanho (Mb)",
                             "Descrição Arquivo",
                             "Responsável");
		
		$arParamCol[2] = array("type" => Lista::TYPESTRING, 
							   "style" => "color:blue;",
							   //"html" => "<img src=\"/imagens/fluxodoc.gif\" border=0 title=\"\" style=\"cursor:pointer;\" onclick=\"\">",
							   "html" => "<a style=\"cursor: pointer; \" onclick=\"window.location='?modulo=principal/popupItensComposicao&acao=A&tipoAba=documento&icoid=1&download=S&arqid='+{arqid};\" />{nome}</a>",
							   "align" => "center");
		
		$acao = "<center>
				   <img border=\"0\" src=\"../imagens/excluir.gif\" onclick=\"javascript:excluirAnexo('{arqid}');\" style=\"cursor: pointer\" title=\"Excluir\" >
				 <center>";
				
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:95%;",
						  "totalLinha" => false,
						  "totalRegistro" => true);
		
		$oLista = new Lista($arConfig);
		$oLista->setCabecalho( $arCabecalho );
		$oLista->setCorpo( $arDados, $arParamCol );
		$oLista->setAcao( $acao );
		$oLista->show();
		
	}
	
	public function identacaoRecursiva($arDados)
	{
		$obPreItensComposicao = new PreItensComposicao();
		$arConteudo = array();
		$obPreItensComposicao->identacaoRecursiva($arDados, null, &$arConteudo);
		return $arConteudo;
	}
	
	public function verificaCategoriaObra($preidsistema, $sisid = null)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaCategoriaObra($preidsistema, $sisid);
	}
	
}