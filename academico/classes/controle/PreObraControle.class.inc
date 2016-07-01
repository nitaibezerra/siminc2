<?php 
class PreObraControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function recuperarQuantidadePac($muncod)
	{
		$obPreObraPac = new PreObraPac();
		return $obPreObraPac->recuperarQuantidadePac($muncod);
	}
	
	public function recuperarEscolasQuadra($preid)
	{
		$obPreEscolasQuadraEsporte = new PreEscolasQuadraEsporte();
		return $obPreEscolasQuadraEsporte->recuperarEscolasQuadra($preid);
	}
	
	public function recuperarSqlEscolasQuadra($preid)
	{
		$obPreEscolasQuadraEsporte = new PreEscolasQuadraEsporte();
		return $obPreEscolasQuadraEsporte->recuperarSqlEscolasQuadra($preid);
	}
	
	public function verificaEscolasQuadraSelecionadas($muncod)
	{
		$obPreEscolasQuadraEsporte = new PreEscolasQuadraEsporte();
		return $obPreEscolasQuadraEsporte->verificaEscolasQuadraSelecionadas($muncod);
	}
	
	public function descricaoMunicipio($muncod, $boUF = true)
	{
		$obMunicipio = new Municipio();
		return $obMunicipio->descricaoMunicipio($muncod, $boUF);
	}
	
	public function recuperarUF($muncod)
	{
		$obMunicipio = new Municipio();
		return $obMunicipio->recuperarUF($muncod);
	}
	
	public function recuperarPreObraPorMuncodSemQuadraCobertura($campo, $sisid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarPreObraPorMuncodSemQuadraCobertura($campo, $sisid);
	}
	
	public function recuperarPreObraPorMuncod($campo, $sisid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarPreObraPorMuncod($campo, $sisid);
	}
	
	public function recuperarPreObraPorMuncodComQuadraCobertura($campo, $sisid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarPreObraPorMuncodComQuadraCobertura($campo, $sisid);
	}
	
	public function recuperarDescricaoPreObraPorId($preid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarDescricaoPreObraPorId($preid);
	}
	
	public function recuperarDocumentosAnexoPorPodid($podid, $preid)
	{
		$obPreTipoDocumento = new PreTipoDocumento();
		return $obPreTipoDocumento->recuperarDocumentosAnexoPorPodid($podid, $preid);
	}
	
	public function recuperarTiposObraDocumentosProInfancia($preid, $tipoA = false)
	{
		$oSubacaoControle 	= new SubacaoControle();
		$obPreTipoDocumento = new PreTipoDocumento();
		
		$tipoObra 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);		
		$arPreTipoDocumento = $obPreTipoDocumento->recuperarTiposObraDocumentosProInfancia($preid, $tipoObra, $tipoA);
		
		return $arPreTipoDocumento;		
	}

	public function recuperarArTiposObraDocumentos($preid, $tipoA = false)
	{
		$oSubacaoControle 	= new SubacaoControle();
		$obPreTipoDocumento = new PreTipoDocumento();
		
		$tipoObra 			= $oSubacaoControle->verificaTipoObra($preid, 23);		
		$arPreTipoDocumento = $obPreTipoDocumento->recuperarArTiposObraDocumentos($preid, $tipoObra, $tipoA);
		
		return $arPreTipoDocumento;		
	}
	
	public function verificaDocumentoDominialidade($preid)
	{
		$obPreTipoDocumento = new PreTipoDocumento();
		return $obPreTipoDocumento->verificaDocumentoDominialidade($preid);
	}
	
	public function documentoDownloadAnexo($arqid)
	{		
		$file = new FilesSimec(null,null,'obras');
		return $file->getDownloadArquivo($arqid);
	}
	
	public function salvarDocumentosPreObra($arDados)
	{
		$campos	= array(
					"preid" 	    => $_SESSION['par']['preid'],
					"datainclusao" 	=> "now()" ,
					"usucpf"    	=> $_SESSION['usucpf'],
					"poadescricao"  => "'".$arDados['poadescricao']."'",
					"podid"     	=> $arDados['podid']
					);	
		$file = new FilesSimec("preobraanexo", $campos ,"obras");
		$arquivoSalvo = $file->setUpload( $arDados['poadescricao']);		
		if($arquivoSalvo){
			echo '<script type="text/javascript"> 
					alert("Operação realizada com sucesso.");
					document.location.href = \''.$_SERVER['HTTP_REFERER'].'\';
				  </script>';
			exit;
		}
	}
	
	public function excluirDocumentosPreObra($arqid, $preid, $tipoA = false)
	{
		$obPreObraAnexo = new PreObraAnexo();
		$obPreObraAnexo->excluirDocumento($arqid);
	    unset($arqid);
	    
	    if($tipoA){
	    	$stAba = "documentoTipoA";
	    }else{
	    	$stAba = "documento";
	    }
		
		echo '<script type="text/javascript"> 
				alert("Operação realizada com sucesso.");
				document.location.href = "'.str_replace("&arqidDel={$_GET['arqidDel']}","",$_SERVER['REQUEST_URI']).'";
			  </script>';
		exit;
	}
	
	public function recuperarPrefeitoMunicipio($muncod, $funid = 2)
	{
		$obPreObraAnexo = new PreObraAnexo();
		return $obPreObraAnexo->recuperarPrefeitoMunicipio($muncod, $funid);
	}

	public function recuperarSecretarioMunicipio($estuf, $funid = 2)
	{
		$obPreObraAnexo = new PreObraAnexo();
		return $obPreObraAnexo->recuperarSecretarioMunicipio($estuf, $funid);
	}
	
	public function excluirPreObra($preid)
	{
		global $db;
		
		$sql = "UPDATE obras.preobra SET  prestatus = 'I' WHERE preid = '{$preid}'";
		$db->executar($sql);
		/*
		$sql = "DELETE FROM obras.preobrafotos WHERE preid = '{$preid}'";
		$db->executar($sql);
		
		$sql = "DELETE FROM obras.preplanilhaorcamentaria WHERE preid = '{$preid}'";
		$db->executar($sql);
		
		$sql = "DELETE FROM obras.precronograma WHERE preid = '{$preid}'";
		$db->executar($sql);
		
		$sql = "DELETE FROM obras.preobraanexo WHERE preid = '{$preid}'";
		$db->executar($sql);
		
		$sql = "DELETE FROM obras.preobra WHERE preid = '{$preid}'";
		$db->executar($sql);
		*/
		return $db->commit();
	}
	
	public function verificaQuestionario($qrpid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaQuestionario($qrpid);
	}
	
	public function verificaCronograma($preid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaCronograma($preid);
	}
	
	public function verificaFotosObras($preid)
	{
		$obPreObra = new PreObra();	
		return $obPreObra->verificaFotosObras($preid);
	}
	
	public function verificaAnexoObras($preid)
	{
		$obPreObra = new PreObra();	
		return $obPreObra->verificaAnexoObras($preid);
	}
	
	public function recuperarListaObras($post = null)
	{
		$obPreObra = new PreObra();
		return $obPreObra->recuperarListaObras($post);
	}
	
	public function verificaGrupoMunicipioTipoObra_A($muncod)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaGrupoMunicipioTipoObra_A($muncod);
	}
	
	public function verificaPrazoExpiraAdesao($muncod)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaPrazoExpiraAdesao($muncod);
	}
	
	public function verificaQtdObrasEstado($dados)
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaQtdObrasEstado($dados);
	}
	
	public function recuperarDadosAnaliseEngenharia($preid)
	{
		$obPreObraAnalise = new PreObraAnalise();		
		return $obPreObraAnalise->recuperarDadosAnaliseEngenharia($preid);
	}
	
	public function recuperarTodasQuestoes()
	{
		$obPreQuestaoAnalise = new PreQuestaoAnalise();
		return $obPreQuestaoAnalise->recuperarTodasQuestoes(); 
	}
	
	public function recuperarRespostasQuestionario($qrpid)
	{
		$obPreObraAnalise = new PreObraAnalise();
		return $obPreObraAnalise->recuperarRespostasQuestionario($qrpid); 
	}
	
	public function recuperarDadosPorPreid($preid)
	{
		$obPreObraAnalise = new PreObraAnalise();
		return $obPreObraAnalise->recuperarDadosPorPreid($preid);
	}
	
	public function salvarObraAnalise($arDados)
	{
		$obPreObraAnalise = new PreObraAnalise();
		$obPreObraAnalise->poaid 			 = $arDados['poaid'];
		$obPreObraAnalise->preid 			 = $arDados['preid'];
		if(!$arDados['poaid']){
			$obPreObraAnalise->poadataanalise 	 = $arDados['poadataanalise'];
		}
		$obPreObraAnalise->poastatus 		 = $arDados['poastatus'];
		$obPreObraAnalise->poausucpfinclusao = $arDados['poausucpfinclusao'];
//		$obPreObraAnalise->qrpid 			 = $arDados['qrpid'];
		$obPreObraAnalise->poaindeferido 	 = $arDados['poaindeferido'];
		$obPreObraAnalise->poajustificativa  = $arDados['poajustificativa'];
		
		$id = $obPreObraAnalise->salvar();
		
		$obPreObraAnalise->commit();
		
		return $id; 
	}
	
	public function validaPermissaoFinalizarQuestionario( $qrpid )
	{
		$obPreObra = new PreObra();
		return $obPreObra->validaPermissaoFinalizarQuestionario( $qrpid );		
	}
	
	public function verificaAlertaArquivoCorrompido( $preid )
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaAlertaArquivoCorrompido( $preid );		
	}
	
	public function verificaAlertaArquivoSubstituido( $preid )
	{
		$obPreObra = new PreObra();
		return $obPreObra->verificaAlertaArquivoSubstituido( $preid );		
	}
	
	public function pegaMuncodPorPreid($preid)
	{
		$obPreObra = new PreObra();
		return $obPreObra->pegaMuncodPorPreid($preid); 
	}
	
	public function verificaPodeEditarAnalise($preid)
	{
		$obPreObraAnalise = new PreObraAnalise();
		return $obPreObraAnalise->verificaPodeEditarAnalise($preid);
	}
	
	public function alteraPodeEditar($arDados)
	{
		$obPreObraAnalise = new PreObraAnalise();
		
		$obPreObraAnalise->poaid 		= $arDados['poaid'];
		$obPreObraAnalise->bopodeeditar = $arDados['bopodeeditar'];
		$obPreObraAnalise->salvar();
		
		if($obPreObraAnalise->commit()){
			return true;
		}else{
			return false;
		}
	}
	
	public function recuperarNomeAnalistaObra($preid)
	{
		$obPreObraAnalise = new PreObraAnalise();
		return $obPreObraAnalise->recuperarNomeAnalistaObra($preid);
	}
	
	public function verificaIndeferimentoQuestionario($preid)
	{
		$obPreObraAnalise = new PreObraAnalise();
		return $obPreObraAnalise->verificaIndeferimentoQuestionario($preid);
	}
	
	public function recuperarConsideracoesFinais($preid)
	{
		$obPreObraAnalise = new PreObraAnalise();
		return $obPreObraAnalise->recuperarConsideracoesFinais($preid);
	}
}