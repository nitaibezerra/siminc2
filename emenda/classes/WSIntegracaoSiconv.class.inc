<?php
//include_once APPRAIZ."emenda/classes/SoapIntegracaoSiconv.class.inc";
include_once APPRAIZ."emenda/classes/InterfaceSiconvMP.class.inc";

class WSIntegracaoSiconv{
	
	private $urlWsdl;
	private $ptrid;
	private $wsusuario;
	private $wssenha;
	private $arrDadosPTA = array();
	private $arrDadosPag = array();
	private $valorMeta;
	private $valorEtapa;
	private $valorCronograma;
	private $numeroProposta;
	private $statusErro;
	private $tipoMensagem;
	private $valorMetaFisico;
	private $numeroConvenio;
	private $nuSeqMovNE;
	private $exfverifsiafi;
  	private $exfverifcadin;
  	private $cpfPresidente;
  	private $db;
  	private $exfid;
  	private $acacod;
	
	public function __construct( $arrParam = array() ){
		
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//$urlWsdl = 'http://172.20.65.93:8080/IntraSiconvWS/services/SimecWsFacade?wsdl';
			$urlWsdl = 'https://wshom.convenios.gov.br/siconv-siconv-interfaceSiconv-1.0/InterfaceSiconvHandlerBeanImpl?wsdl';
		} else {
			$urlWsdl = 'https://ws.convenios.gov.br/siconv-interfaceSiconv-1.0/InterfaceSiconvHandler/InterfaceSiconvHandlerBeanImpl?wsdl';
		}
		
		$this->db = new cls_banco();
		
		$this->ptrid 		= $arrParam['ptrid'];
		$this->exfid 		= $arrParam['exfid'];
		$this->wsusuario 	= $arrParam['usuario'];
		$this->wssenha 		= $arrParam['senha'];
		$this->urlWsdl 		= ( $arrParam['url'] ? $arrParam['url'] : $urlWsdl);
		$this->valorMeta 	= 0;
		$this->valorEtapa 	= 0;
		$this->valorCronograma = 0;
		$this->tipoMensagem = '';
		$this->valorMetaFisico = 0;
		$this->numeroConvenio = '';
		$this->nuSeqMovNE = '';
		$this->exfverifcadin = $arrParam['post']['verificadoCadin'];
		$this->exfverifsiafi = $arrParam['post']['verificadoSiafi'];
		$this->arrDadosPag = '';
		$this->cpfPresidente =  $this->db->pegaUm("SELECT e.entnumcpfcnpj FROM emenda.concedente c 
													inner join entidade.entidade e on e.entid = c.entid and c.convigente = 't'");
	}
		
	public function enviaPropostaWS(){
		
		$this->arrDadosPTA = $this->carregaDadosPTA();
		
		$sql = "select
					vf.acacod
				from
					emenda.ptemendadetalheentidade pte
				    inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = ve.acaid
				where
					pte.ptrid = {$this->arrDadosPTA['ptrid']}   
				    and ve.edestatus = 'A'
				    and vf.acastatus = 'A'
				    and vf.prgano = '{$_SESSION['exercicio']}'";
		$acacod = $this->db->pegaUm($sql);
		$this->acacod = $acacod;
		
		//ver($this->getCronogramaDesembolsoPropostaWS(), $this->getCronogramaFisicoPropostaWS(),d);
		
		$sql = "SELECT distinct
					esp.espid,
				    esp.espnome
				FROM 
				    emenda.ptiniciativa pti
				    inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
				    inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
				    inner join emenda.especificacao esp on esp.espid = ice.espid
				WHERE 
				    pti.ptrid = {$this->ptrid}
				    and ice.icestatus = 'A'
                    and esp.ssiid is null";
        $arrNatureza = $this->db->carregar($sql);
        $arrNatureza = ($arrNatureza ? $arrNatureza : array());
        
		$msg = '';
        if( $arrNatureza ){
        	$msg = "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>SIMEC : É necessário vincular a especificação a natureza de despesa:<br>";
	        foreach ($arrNatureza as $arr) {
	        	$msg .= '<a href="#" onclick="abreEspecificacao('.$arr['espid'].')" style="cursor: pointer;">'.$arr['espnome'].'</a><br>';
	        }
	        $msg .= "</span>";
	        die($msg);
		}
		$obEnvio = $this->getEnviarProposta();
		//ver($obEnvio,d); 
		$wsProposta = new SimecWsInterfaceService($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
												
		$wsProposta->__setUsernameToken($this->wsusuario, md5($this->wssenha) );
		/*$obEnvio = $this->getExportaProponente();
		$retorno = $wsProposta->exportaProponente( $obEnvio, $this->arrDadosPTA );*/
		
		$retorno = $wsProposta->enviaPropostaWS( $obEnvio, $this->arrDadosPTA );
		
		$numeroProposta = substr($retorno->return, -11);
		
		if($numeroProposta) {
			$arProposta = explode('/', $numeroProposta);			
			$sql = "update emenda.planotrabalho set ptrpropostasiconv = '{$arProposta['0']}', ptrpropostasiconvano = '{$arProposta['1']}' where ptrid = {$this->ptrid}";
			$this->db->executar( $sql );
			$this->db->commit();
		}
		return $retorno->return;
	}
		
	public function getEnviarProposta(){
		$ObjetoWS = new enviaProposta();
		$ObjetoWS->arg0	= $this->getProposta(); // PropostaWS
		
		return $ObjetoWS;
	}
	
	public function solicitarNotaEmpenhoWS(){
		$this->arrDadosEmpenho = $this->carregaDadosEmpenho();
		
		$wsProposta = new SimecWsInterfaceService($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
		
		ver( $this->getFuncoes($wsProposta), $this->getTipos($wsProposta),d );
		
		$dadosconvenio = $this->consultaConvenioWS();
		if( $dadosconvenio->consultarConvenioReturn->numeroConvenio ){
			$this->numeroConvenio = $dadosconvenio->consultarConvenioReturn->numeroConvenio;
			#insere os dados do convenio
			$this->insereDadosConvenio( $this->numeroConvenio );
			
			$obEnvio = $this->getsolicitarNotaEmpenho();

			$wsProposta = new SimecWsInterfaceService($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
												
			$wsProposta->__setUsernameToken($this->wsusuario, md5($this->wssenha) );			
			$retorno = $wsProposta->solicitarNotaEmpenho( $obEnvio, $this->arrDadosEmpenho );
			ver($retorno,d);
			
			$this->nuSeqMovNE = $retorno->solicitarNotaEmpenhoReturn->nuSeqMovNE;
			$this->insereRetornoNotaEmepenho();		
		}else{
			return 'Não encontrou dados de convênio.';
		}
	}
	
	public function solicitarOrdemBancaria(){
		$this->arrDadosPag = $this->carregarDadosPagamento();
		$obEnvio = $this->getSolicitarOrdemBancaria();
		
		$wsProposta = new SimecWsInterfaceService($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
		$retorno = $wsProposta->solicitarOrdemBancaria( $obEnvio, $this->arrDadosPag );
		
	}
	
	public function consultaConvenioWS(){
		$this->arrDadosEmpenho = $this->carregaDadosEmpenho();
		
		$obEnvio = $this->getConsultarConvenio();
		//ver($obEnvio,d);
		$urlWsdl = 'http://www.fnde.gov.br/IntraSiconvWS/services/SimecWsFacade?wsdl';
		$wsProposta = new SimecWsInterfaceService($urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
		$retorno = $wsProposta->consultarConvenio( $obEnvio, $this->arrDadosEmpenho );
		return $retorno; 
	}
	
	public function consultarEmpenhoWS(){
		$arrDados = $this->carregarDadosConsultaEmpenho();
		$wsProposta = new SimecWsInterfaceService($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);

		$obEnvio = $this->getConsultarEmpenho( $arrDados );
		$retorno = $wsProposta->consultarEmpenho( $obEnvio, $arrDados );
		
		$logresponse = $this->db->pegaUm("SELECT logresponse FROM emenda.logerroswsdl WHERE logid='".$_SESSION['emenda']['logid']."'");

		$logresponse = str_ireplace('<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">', "", $logresponse);
		$logresponse = str_ireplace('<soapenv:Body>', "<Body>", $logresponse);
		$logresponse = str_ireplace('<consultarEmpenhoResponse xmlns="http://simec.intraSiconvWS.sistema.fnde.gov.br">', "<consultarEmpenhoResponse>", $logresponse);
		$logresponse = str_ireplace(' xsi:nil="true"', "", $logresponse);
		$logresponse = str_ireplace('</soapenv:Body>', "</Body>", $logresponse);
		$logresponse = str_ireplace('</soapenv:Envelope>', "", $logresponse);
		
		$string = <<<XML
$logresponse
XML;

		$objXML = simplexml_load_string($string);
		
		$arrDados = $objXML->consultarEmpenhoResponse->consultarEmpenhoReturn->listaEmpenhos;
		if( $arrDados ){
			if($this->insereHistoricoEmpenhoSiconv($arrDados)){
				return false;
			} else {
				return $_SESSION['emenda']['logid'];
			}
		} else {
			return $_SESSION['emenda']['logid'];
		}
	}
		
	public function exportaProponente(){
		
		$obEnvio = $this->getExportaProponente();
		//ver($obEnvio,d);		
		$wsConvenio = new SimecWsInterfaceService($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
												
		$wsConvenio->__setUsernameToken( $this->wsusuario, md5($this->wssenha) );		
		$objRetorno = $wsConvenio->exportaProponente( $obEnvio, $this->arrDadosEmp );
		ver($objRetorno,d);
	}
	
	public function getConsultarEmpenho( $arrDados ){
		
		$ObjetoWS = new consultarEmpenho();
  		$ObjetoWS->consultaEmpenhoDTO 	= $this->getConsultaEmpenhoDTO( $arrDados ); // ConsultaEmpenhoDTO
  		$ObjetoWS->login				= $this->getLoginWS(); // LoginWS
  		
  		return $ObjetoWS;
	}
	
	public function getConsultarConvenio(){
		$ObjetoWS = new consultarConvenio();
		$ObjetoWS->consultaConvenioDTO	= $this->getConsultaConvenioDTO(); // ConsultaConvenioDTO
	  	$ObjetoWS->login				= $this->getLoginWS(); // LoginWS
	  	
	  	return $ObjetoWS;
	}
	
	public function getConsultaConvenioDTO(){
		
		$ObjetoWS = new ConsultaConvenioDTO();
	 	$ObjetoWS->anExercicio		= $_SESSION['exercicio']; // string
	 	$ObjetoWS->coProgramaFnde	= '03'; // string 
	 	
	 	return $ObjetoWS;
	}
	
	public function getConsultaEmpenhoDTO( $arrDados ){
		
		$ObjetoWS = new ConsultaEmpenhoDTO();
  		$ObjetoWS->anConvenio		= $arrDados['ptranoconvenio']; // string
  		$ObjetoWS->anExercicio		= $arrDados['ptrexercicio']; // string
  		$ObjetoWS->nuConvenio		= '900623'; //$arrDados['ptrnumconvenio']; // string
  		$ObjetoWS->nuProcesso		= '23400002829200986'; //$arrDados['ptrnumprocessoempenho']; // string
  		$ObjetoWS->nuPropostaSiconv	= '000448/2009'; //$arrDados['specodigosiconv']; // string
  		
  		return $ObjetoWS;
	}
	
	public function getExportaProponente(){
		$ObjetoWS = new exportaProponente();
		$ObjetoWS->arg0 = $this->proponenteExportacaoWS();
		
		return $ObjetoWS;
	}
	
	public function proponenteExportacaoWS(){
		$ObjetoWS = new proponenteExportacaoWS();
		
		$ObjetoWS->id = '';
		$ObjetoWS->identificacao = '34972366320';
		$ObjetoWS->login = $this->getLoginWS('true');
		$ObjetoWS->tipoIdentificacao = 1;
		
		return $ObjetoWS;
	}
	
	public function getProposta(){
		
		
		
		$ObjetoWS = new propostaWS();
		$ObjetoWS->ano 									= $this->arrDadosPTA['ptrexercicio'];
		//$ObjetoWS->atribuicaoRespAnalise 				= $atribuicaoRespAnalise;
		$ObjetoWS->bensServicoWS 						= $this->getBensServicosPropostaWS(); // ArrayOf_tns1_BensServicosPropostaWS
		//$ObjetoWS->capacidadeTecnica 					= '';
		$ObjetoWS->contaBancariaWS 						= $this->getContaBancariaWS(); // ContaBancariaWS
		$ObjetoWS->cronogramaDesembolsoWS 				= $this->getCronogramaDesembolsoPropostaWS(); // CronogramaDesembolsoPropostaWS		
		$ObjetoWS->cronogramaFisicoWS 					= $this->getCronogramaFisicoPropostaWS(); // CronogramaFisicoPropostaWS		
		/*$ObjetoWS->dataAprovacaoDePropostaWS 			= '';
		$ObjetoWS->dataAprovacaoPlanoTrabalhoWS 		= '';
		$ObjetoWS->dataEntregaProjetoBasicoWS 			= '';
		$ObjetoWS->dataLimiteComplementacaoPbWS 		= '';
		$ObjetoWS->dataLimiteEntregaProjetoBasicoWS 	= '';*/
		$ObjetoWS->dataPropostaWS 						= date('Y-m-d').'T00:00:00'; //$this->getDataPropostaWS();
		/*$ObjetoWS->dataUltimoEnvioContratoRepasseWS 	= '';
		$ObjetoWS->dataVersaoWS 						= '';
		$ObjetoWS->despesaAdministrativa				= $this->getDespesaAdministrativaWS(); // ArrayOf_tns1_DespesaAdministrativaWS
		$ObjetoWS->empenhoPublicacao 					= '';
		$ObjetoWS->executorWS							= $this->getParticipantesPropostaWS(); // ArrayOf_tns1_ParticipantesPropostaWS
		*/
		$ObjetoWS->fimExecucao 							= $this->arrDadosPTA['fimexecucao'].'T00:00:00'; //($this->arrDadosPTA['vigdatafim'] ? $this->arrDadosPTA['vigdatafim'] : null); //$this->getFimExecucaoWS();
		$ObjetoWS->gestao 								= '15253';
		/*$ObjetoWS->gestaoContratoPublicacao 			= '';
		$ObjetoWS->historico 							= '';
		$ObjetoWS->id 									= '';*/
		$ObjetoWS->inicioExecucao 						= $this->arrDadosPTA['inicioexecucao'].'T00:00:00'; // ($this->arrDadosPTA['vigdatafim'] ? $this->arrDadosPTA['vigdatafim'] : null); //$this->getInicioExecucaoWS();
		//$ObjetoWS->instituicaoMandatariaWS				= $this->getOrgaoAdministrativoWS(); // OrgaoAdministrativoWS
		//$ObjetoWS->intervenenteWS						= $this->getParticipantesPropostaWS(); // ArrayOf_tns1_ParticipantesPropostaWS		
		$ObjetoWS->justificativa 						= $this->arrDadosPTA['ptrjustificativa'];
		$ObjetoWS->login 								= $this->getLoginWS('true'); // LoginWS
		//$ObjetoWS->mandatarioWS							= $this->getParticipantesPropostaWS(); // ParticipantesPropostaWS
		$ObjetoWS->membroParticipanteWS					= $this->getMembroParticipanteWS( $this->arrDadosPTA['cpfdirigente'] ); // MembroParticipanteWS
		$ObjetoWS->modalidadePropostaWS					= $this->getModalidadePropostaWS(); // ModalidadePropostaWS
		//$ObjetoWS->motivosNaoAcatamentoContratoRepasse	= $this->getMotivoNaoAcatamentoContratoRepasseWS(); // ArrayOf_tns1_MotivoNaoAcatamentoContratoRepasseWS
		$ObjetoWS->objetoConvenio 						= 'objeto';
		$ObjetoWS->orgaoAdministrativoWS				= $this->getOrgaoAdministrativoWS(); // OrgaoAdministrativoWS
		$ObjetoWS->orgaoConcedenteWS					= $this->getOrgaoConcedenteWS(); // OrgaoAdministrativoWS
		$ObjetoWS->orgaoExecutorWS						= $this->getOrgaoAdministrativoWS(); // OrgaoAdministrativoWS
		//$ObjetoWS->parecerPlanoTrabalho					= $this->getParecerPlanoTrabalhoWS(); // ArrayOf_tns1_ParecerPlanoTrabalhoWS
		$ObjetoWS->parecerProposta						= $this->getParecerPropostaConvenioWS(); // ArrayOf_tns1_ParecerPropostaConvenioWS
		/*$ObjetoWS->permiteAdiarEntregaProjBasico 		= '';
		$ObjetoWS->permiteLiberarPrimeiroRepasse 		= '';
		$ObjetoWS->permiteProrrogarEntregaProjBasico 	= '';
		$ObjetoWS->prazoEntregaProjBasico 				= '';*/
		$ObjetoWS->proponenteWS							= $this->getParticipantesPropostaWS(); // ParticipantesPropostaWS
		//$ObjetoWS->propostaParticipeMembroWS			= $this->getResponsaveisConvenioWS(); // ArrayOf_tns1_ResponsaveisConvenioWS
		$ObjetoWS->propostaProgramaWS					= $this->getPropostaProgramaArrayWS(); // ArrayOf_tns1_PropostaProgramaWS
		$ObjetoWS->repasseExercicioFuturoPropostaWS		= $this->getRepasseExercicioFuturoPropostaWS(); // ArrayOf_tns1_RepasseExercicioFuturoPropostaWS
		if( !empty($this->arrDadosPTA['ptrpropostasiconv']) ){
			$ObjetoWS->sequencial 						= $this->arrDadosPTA['ptrpropostasiconv'].'/'.$this->arrDadosPTA['ptrpropostasiconvano'];
		}
		//$ObjetoWS->situacaoEnvioContratoRepasseWS		= $this->getSituacaoEnvioContratoRepasseWS(); // SituacaoEnvioContratoRepasseWS
		//$ObjetoWS->situacaoLegado 						= '';
		//$ObjetoWS->situacaoProjetoBasicoWS				= $this->getSituacaoProjetoBasicoWS(); // SituacaoProjetoBasicoWS
		//$ObjetoWS->situacaoPropostaWS					= $this->getSituacaoPropostaWS(); // SituacaoPropostaWS
		//$ObjetoWS->tipoNaoAcatamentoContratoRepasseWS	= $this->getTipoNaoAcatamentoContratoRepasseWS(); // TipoNaoAcatamentoContratoRepasseWS
		//$ObjetoWS->tipoProjetoBasicoWS					= $this->getTipoProjetoBasicoWS(); // TipoProjetoBasicoWS
		$ObjetoWS->valorContraPartida 					= $this->arrDadosPTA['ptrvalorproponente'];
		$ObjetoWS->valorContrapartidaBensServicos 		= '0.0';
		$ObjetoWS->valorContrapartidaFinanceira 		= $this->arrDadosPTA['ptrvalorproponente'];
		$ObjetoWS->valorGlobal 							= $this->arrDadosPTA['ptivalortotal']; //$this->arrDadosPTA['ptivalortotal'];
		$ObjetoWS->valorRepasse 						= $this->arrDadosPTA['ptrvalorconcedente']; //$this->arrDadosPTA['vrlglobal'];
		
		return $ObjetoWS;
	}
	
	public function getSolicitarOrdemBancaria(){
		
		$ObjetoWS = new solicitarOrdemBancaria();
		$ObjetoWS->movimentoPagamentoDTO = $this->getMovimentoPagamentoDTO();
		$ObjetoWS->login = $this->getLoginWS();
		
		return $ObjetoWS;
	}
	
	public function getMovimentoPagamentoDTO(){
		
		$ObjetoWS = new MovimentoPagamentoDTO();
		$ObjetoWS->anConvenioOriginal 			= '';
		$ObjetoWS->anReferencia 				= '';
		$ObjetoWS->coGestaoEmitente 			= '';
		$ObjetoWS->coProgramaFnde 				= '';
		$ObjetoWS->coSituacaoDocSiafi 			= '';
		$ObjetoWS->coTipoDocumento 				= '';
		$ObjetoWS->coTipoMovPagamento 			= '';
		$ObjetoWS->coTpAvaliador 				= '';
		$ObjetoWS->coUnidadeGestoraEmitente 	= '';
		$ObjetoWS->idSolicitante 				= '';
		$ObjetoWS->movimentoPagDocumentoDTO		= $this->getMovimentoPagDocumentoDTO(); // ArrayOf_tns1_MovimentoPagDocumentoDTO
		$ObjetoWS->nuAgencia 					= '';
		$ObjetoWS->nuBanco 						= '';
		$ObjetoWS->nuCgcFavorecido 				= '';
		$ObjetoWS->nuContaCorrenteFavorecido 	= '';
		$ObjetoWS->nuConvenioOriginal 			= '';
		$ObjetoWS->nuConvenioSiafi 				= '';
		$ObjetoWS->nuCpfFavorecido 				= '';
		$ObjetoWS->nuIdSistemaSgb 				= '';
		$ObjetoWS->nuProcesso 					= '';
		$ObjetoWS->nuProposta_siconv 			= '';
		$ObjetoWS->nuSeqApostilamentoOriginal	= '';
		$ObjetoWS->nuSeqContaCorrenteDavorec 	= '';
		$ObjetoWS->nuSeqEntidadeFavorecido 		= '';
		$ObjetoWS->nuSeqTermoAditivoOriginal 	= '';
		$ObjetoWS->stDarf 						= '';
		$ObjetoWS->stGps 						= '';
		$ObjetoWS->stGpsPatronal 				= '';
		$ObjetoWS->vlBruto 						= '';
		$ObjetoWS->vlCapital 					= '';
		$ObjetoWS->vlCusteio 					= '';
		$ObjetoWS->vlPagamento 					= '';
		
		return $ObjetoWS;
	}
	
	public function getMovimentoPagDocumentoDTO(){
		
		$ObjetoWS = new MovimentoPagDocumentoDTO();
		$ObjetoWS->anExercicio 			= '';
		$ObjetoWS->anParcela 			= '';
		$ObjetoWS->coFonteRecurso 		= '';
		$ObjetoWS->coNaturezaDespesa	= '';
		$ObjetoWS->coProgramaFnde 		= '';
		$ObjetoWS->coPtres 				= '';
		$ObjetoWS->coTipoDocumentoNe 	= '';
		$ObjetoWS->nuMes 				= '';
		$ObjetoWS->nuParcela 			= '';
		$ObjetoWS->nuSeqDocSiafiNe 		= '';
		$ObjetoWS->nuSeqMovPag 			= '';
		$ObjetoWS->nuSeqMovPagDoc 		= '';
		$ObjetoWS->vlParcela 			= '';
		
		return $ObjetoWS;
	}
	
	public function getBensServicosPropostaWS() {
		
		$sql = "SELECT DISTINCT
					enb.enbcep,
				    esp.espnome,
				    esp.espid,
				    enb.enblog,
				    esp.ssiid,
				    pte.ptequantidade,
    				pte.ptevalorunitario,
    				enb.muncod
				FROM 
				    emenda.planotrabalho ptr
				    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
				    inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
				    inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
				    inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
				    inner join emenda.especificacao esp on esp.espid = ice.espid
				WHERE 
				    ptr.ptrid = {$this->ptrid}
				    and ice.icestatus = 'A'";
		$arrDados = $this->db->carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		
		$ObjetoWS = array();
		foreach ($arrDados as $key => $v) {		
		
			$$key = new bensServicosPropostaWS();
			$$key->cep 							= $v['enbcep'];
			$$key->descricao 					= $v['espnome'];
//			$$key->despesaCompartilhada 		= '';
			$$key->endereco 					= $v['enblog'];
//			$$key->id 							= '';
//			$$key->justificativaAnalista 		= '';
			$$key->municipioWS 					= $this->getMunicipioWS($v['muncod']); // MunicipioWS
			$$key->naturezaAquisicaoWS 			= $this->getNaturezaDespesaWS($v, false); // NaturezaAquisicaoWS
			$$key->naturezaDespesaSubItemWS 	= $this->getNaturezaDespesaSubItemWS($v['espid']); // NaturezaDespesaSubItemWS
			$$key->observacao 					= 'BENS/SERVIÇOS DA PROPOSTA';
			$$key->propostaProgramaWS 			= $this->getPropostaProgramaWS( $this->arrDadosPTA ); // PropostaProgramaWS
			$$key->quantidade 					= $v['ptequantidade'];
			$$key->statusBensServicosPropostaWS	= $this->getStatusBensServicosPropostaWS(); // StatusBensServicosPropostaWS
			$$key->tipoDespesaWS 				= 'OUTROS';
			$$key->unidadeFornecimento 			= 'UN';
			$$key->valorUnitario 				= $v['ptevalorunitario'];
			
			$ObjetoWS[$key] = $$key;
		}
		
		return $ObjetoWS;
	}
	
	public function getPropostaProgramaWS($arrDados){
		
		$ObjetoWS = new propostaProgramaWS();
		$ObjetoWS->objetoWS							= $this->getObjetoWS(); // ArrayOf_tns1_ObjetoWS
		$ObjetoWS->origemRecursoPropProgramaWS		= $this->getOrigemRecursoPropProgramaWS(); // ArrayOf_tns1_OrigemRecursoPropProgramaWS
		$ObjetoWS->programaWS						= $this->getProgramaWS(); // ProgramaWS
		$ObjetoWS->regrasWS							= $this->getRegrasWS(); // RegrasWS
		$ObjetoWS->valorContrapartida 				= $arrDados['ptrvalorproponente'];
		$ObjetoWS->valorContrapartidaBensServicos 	= '0.0';
		$ObjetoWS->valorContrapartidaFinanceira 	= $arrDados['ptrvalorproponente'];
		$ObjetoWS->valorGlobal 						= $arrDados['ptivalortotal']; //$arrDados['ptivalortotal'];
				
		return $ObjetoWS;
	}
	
	public function getPropostaProgramaArrayWS(){
		
		$ObjetoWS = new PropostaProgramaWS();
		//$ObjetoWS->id = '';
		$ObjetoWS->objetoWS							= $this->getObjetoWS(); // ArrayOf_tns1_ObjetoWS
		$ObjetoWS->origemRecursoPropProgramaWS		= $this->getOrigemRecursoPropProgramaWS(); // ArrayOf_tns1_OrigemRecursoPropProgramaWS
		$ObjetoWS->programaWS						= $this->getProgramaWS(); // ProgramaWS
		//$ObjetoWS->qualificacaoProponenteWS			= $this->getQualificacaoProponenteWS(); // QualificacaoProponenteWS
		$ObjetoWS->regrasWS							= $this->getRegrasWS(); // RegrasWS
		$ObjetoWS->valorContrapartida 				= $this->arrDadosPTA['ptrvalorproponente'];
		$ObjetoWS->valorContrapartidaBensServicos 	= '0.0';
		$ObjetoWS->valorContrapartidaFinanceira 	= $this->arrDadosPTA['ptrvalorproponente'];
		$ObjetoWS->valorGlobal 						= $this->arrDadosPTA['ptivalortotal'];
				
		return array($ObjetoWS);
	}
	
	public function getObjetoWS(){
		
		$ObjetoWS = new objetoWS();
		$ObjetoWS->ehObjetoPadronizado	= true; // boolean
		$ObjetoWS->id					= 1;
		$ObjetoWS->justificativa		= 1;
		
		if( $this->acacod == '0048' ){
			$ObjetoWS->nome = 'Apoio a Emendas Parlamentares - Acao 0048';
		} else {		
  			$ObjetoWS->nome	= 'Atendimento ao Ensino Superior';
		}
  		
  		return array($ObjetoWS);
	}
	
	public function getOrigemRecursoPropProgramaWS(){		
		$ObjetoWS = new origemRecursoPropProgramaWS();
		$ObjetoWS->cnpjBeneficiarioEspecificoWS = $this->getCnpjBeneficiarioEspecificoWS();
		$ObjetoWS->cnpjProgramaEmendaWS 		= $this->getCnpjProgramaEmendaWS();
		$ObjetoWS->valorRepasse 				= $this->arrDadosPTA['ptrvalorconcedente']; //$this->arrDadosPTA['vrlglobal'];
		
		return array($ObjetoWS);
	}
	
	public function getLoginWS($hash = false){
		
		$ObjetoWS = new LoginWS();
//		$ObjetoWS->id 		= 32;
		$ObjetoWS->usuario 	= $this->wsusuario;
		if (!$hash)
			$ObjetoWS->senha 	= $this->wssenha;//$this->wssenha;
		else 
			$ObjetoWS->senha 	= md5($this->wssenha);
		
		return $ObjetoWS;
	}
	
	public function getsolicitarNotaEmpenho(){
		
		$ObjetoWS = new solicitarNotaEmpenho();		
		$ObjetoWS->empenhoDTO	= $this->getEmpenhoDTO(); // EmpenhoDTO
  		$ObjetoWS->login		= $this->getLoginWS(); // LoginWS
  		
  		return $ObjetoWS;
	}
	
	public function getEmpenhoDTO(){
		
		$ObjetoWS = new EmpenhoDTO();		
		$ObjetoWS->anExercicioOriginal 			= $this->arrDadosEmpenho['exfanooriginal'];
		$ObjetoWS->coCentroGestaoSolic 			= '61700000000';
		$ObjetoWS->coDescricaoEmpenho 			= '0010';
		$ObjetoWS->coEsferaOrcamentariaSolic	= '1';//1- Fiscal , 2-Seguridade
		$ObjetoWS->coEspecieEmpenho 			= $this->arrDadosEmpenho['exfespecieempenho'];
		$ObjetoWS->coFonteRecursoSolic 			= $this->arrDadosEmpenho['fonterecursos'];
		$ObjetoWS->coGestaoEmitente 			= '15253';
		$ObjetoWS->coNaturezaDespesaSolic 		= $this->arrDadosEmpenho['naturezadespesa'];
		$ObjetoWS->coObservacao 				= '02'; // int
		$ObjetoWS->coPlanoInternoSolic 			= $this->arrDadosEmpenho['planointerno'];
		$ObjetoWS->coProgramaFnde 				= $this->arrDadosEmpenho['programafnde'];
		$ObjetoWS->coPtresSolic 				= $this->arrDadosEmpenho['ptres'];
		$ObjetoWS->coTipoEmpenho 				= '5';
		$ObjetoWS->coUnidadeGestoraEmitente 	= '153173';
		$ObjetoWS->coUnidadeOrcamentariaSolic 	= $this->arrDadosEmpenho['unicod'];
		$ObjetoWS->nuCgcFavorecido 				= $this->arrDadosEmpenho['enbcnpj'];
		$ObjetoWS->anConvenio 					= $_SESSION['exercicio'];		
		$ObjetoWS->nuConveio 					= $this->numeroConvenio;
		$ObjetoWS->nuEmpenhoOriginal 			= $this->arrDadosEmpenho['exfnumempenhooriginal'];
		$ObjetoWS->nuIdSistema 					= '2';
		$ObjetoWS->nuProcesso 					= $this->arrDadosEmpenho['ptrnumprocessoempenho'];
		$ObjetoWS->nuPropostaSiconv 			= $this->arrDadosEmpenho['ptrpropostasiconv'].'/'.$this->arrDadosEmpenho['ptrpropostasiconvano'];
		$ObjetoWS->vlEmpenho 					= $this->arrDadosEmpenho['exfvalor'];
		
		return $ObjetoWS;
	}
	
	public function getDirigenteWS(){
		
		$ObjetoWS = new DirigenteWS();
		$ObjetoWS->cargo 			= $this->arrDadosPTA['funcaodirigente']; // string
		$ObjetoWS->cpf 				= $this->arrDadosPTA['cpfdirigente']; // string
//		$ObjetoWS->id 				= ''; // long
		$ObjetoWS->nome 			= $this->arrDadosPTA['nomedirigente']; // string
		$ObjetoWS->orgaoExpedidor 	= $this->arrDadosPTA['orgaodirigente']; // string
		$ObjetoWS->profissao		= $this->arrDadosPTA['funcaodirigente']; // string
		$ObjetoWS->rg				= $this->arrDadosPTA['rgdirigente'];
		
		return $ObjetoWS;
	}
	
	public function getUnidadeFederativaWS( $estuf ){
		
		$ObjetoWS = new unidadeFederativaWS();
		//$ObjetoWS->id	 = '';
		$ObjetoWS->sigla = $estuf;
		
		return $ObjetoWS;
	}
	
	public function getMunicipioWS( $muncod ){
		
		$sql = "SELECT 
					mun.muncodsiafi, 
				    mun.mundescricao,
				    mun.estuf 
				FROM territorios.municipio mun 
				WHERE 
					mun.muncod = '$muncod'";
					
		$arrDados = $this->db->pegaLinha( $sql );
		
		$ObjetoWS = new municipioWS();
		$ObjetoWS->codigo 				= $arrDados['muncodsiafi'];
		//$ObjetoWS->id					= '';
		$ObjetoWS->nome					= $arrDados['mundescricao'];
		$ObjetoWS->unidadeFederativaWS	= $this->getUnidadeFederativaWS( $arrDados['estuf'] );
		
		return $ObjetoWS;
	}
	
	public function getNaturezaDespesaWS( $dados, $boSubItem = true ){
		$sql = "SELECT 
				  sndcodigonaturezadespesa,
				  snddescricaonaturezadespesa
				FROM 
				  emenda.siconvnaturezadespesa sic
                  inner join emenda.siconvsubitens sii on sii.sndid = sic.sndid 
                  WHERE sii.ssiid = ".$dados['ssiid'];
        
		$arrDados = $this->db->pegaLinha( $sql );
		
		
		$ObjetoWS = new naturezaAquisicaoWS();
		$ObjetoWS->codigo 		= (!$boSubItem ? 1 : $arrDados['sndcodigonaturezadespesa']); //'449051'; //$arrDados['sndcodigonaturezadespesa'];
		$ObjetoWS->descricao 	= $arrDados['snddescricaonaturezadespesa'];
//		$ObjetoWS->id			= '';
		
		return $ObjetoWS;
	}
	
	public function getNaturezaDespesaSubItemWS( $espid ){
		$sql = "SELECT
					scs.ssidescricaosubitem,
				    scs.ssiid,
                    esp.espid,
				    scs.ssicodigosubitem
				FROM
				    emenda.especificacao esp
				    left join emenda.siconvsubitens scs on scs.ssiid = esp.ssiid
				WHERE
					esp.espid = $espid";
				  
		$arrDados = $this->db->pegaLinha( $sql );
		
		$ObjetoWS = array();
		$ObjetoWS = new naturezaDespesaSubItemWS();
		$ObjetoWS->descricaoSubItem		= $arrDados['ssidescricaosubitem'];
//		$ObjetoWS->id					= '';
		$ObjetoWS->naturezaDespesaWS	= $this->getNaturezaDespesaWS( $arrDados );
//		$ObjetoWS->observacao			= '';
		$ObjetoWS->subItem				= $arrDados['ssicodigosubitem']; //$arrDados['ssicodigosubitem'];
		
		return $ObjetoWS;
	}
	
	public function getProgramaWS(){
		
		/*if($this->arrDadosPTA['mdeid'] == '1'){
			$programa = '';
		}elseif($this->arrDadosPTA['mdeid'] == '2'){
			$programa = '';
		}elseif($this->arrDadosPTA['mdeid'] == '3'){
			$programa = '';
		}elseif($this->arrDadosPTA['mdeid'] == '4'){
			$programa = '';
		}elseif($this->arrDadosPTA['mdeid'] == '5'){
			$programa = '';
		}else{
			$programa = '2629820120004';
		}*/
		
		if( $this->acacod == '0048' || $this->acacod == '2032' ){
			$programa = '2629820140001';
			$descricao = 'APOIO AO ENSINO SUPERIOR - EMENDA PARLAMENTAR';
			
			if( $this->acacod == '0048' ){
				$programa = '2629820150002';
				$descricao = 'Apoio a Emendas Parlamentares - Acao 0048';
			}
		}elseif( $this->acacod == '2030' || $this->acacod == '20RF' ){
			$programa = '2629820140002';
			$descricao = 'SESU - Teste';
		} else {
			$programa = '2629820120004';
			$descricao = 'SESU - Teste';
		}
		
		$ObjetoWS = new ProgramaWS();		
		/*$ObjetoWS->acaoOrcamentaria 						= '';
		$ObjetoWS->aceitaDespesaAdministrativa 				= '';
		$ObjetoWS->aceitaPropostaDeProponenteNaoCadastrado 	= '';
		$ObjetoWS->chamamentoPublico 						= '';
		$ObjetoWS->cnpjBeneficiarioEspecificoWS				= $this->getCnpjBeneficiarioEspecificoWS();*/
		$ObjetoWS->cnpjProgramaEmendaWS						= $this->getCnpjProgramaEmendaWS();
		$ObjetoWS->codigo 									= $programa;
		$ObjetoWS->nome 									= $descricao;
		/*$ObjetoWS->concedente								= '';
		$ObjetoWS->criteriosDeSelecao 						= '';
		$ObjetoWS->dataDisponibilizacao 					= '';
		$ObjetoWS->dataFimBeneficiarioEspecifico 			= '';
		$ObjetoWS->dataFimEmendaParlamentar 				= '';
		$ObjetoWS->dataFimPropostaVoluntaria 				= '';
		$ObjetoWS->dataInicioBeneficiarioEspecifico 		= '';
		$ObjetoWS->dataInicioEmendaParlamentar 				= '';
		$ObjetoWS->dataInicioPropostaVoluntaria 			= '';
		$ObjetoWS->dataPublicacaoImprensa 					= '';
		$ObjetoWS->descricao 								= '';
		$ObjetoWS->estadosHabilitados						= '';
		$ObjetoWS->executor									= '';
		$ObjetoWS->id										= '';
		$ObjetoWS->modalidades								= '';
		$ObjetoWS->naturezasJuridicas						= '';
		
		$ObjetoWS->numeroDocumento 							= '';*/
		//$ObjetoWS->objetoWS									= $this->getObjetoWS();
		/*$ObjetoWS->obrigaPlanoTrabalho 						= '';
		$ObjetoWS->observacao 								= '';
		$ObjetoWS->publicadoImprensa 						= '';
		$ObjetoWS->qualificacaoBeneficiarioEmendaParlamentar= '';
		$ObjetoWS->qualificacaoBeneficiarioEspecifico 		= '';
		$ObjetoWS->qualificacaoPropostaVoluntaria 			= '';
		$ObjetoWS->regrasWS									= $this->getRegrasWS();
		$ObjetoWS->status 									= '';*/
		
		return $ObjetoWS;
	}
	
	public function getCnpjBeneficiarioEspecificoWS(){
		
		$ObjetoWS = new cnpjBeneficiarioEspecificoWS();
		$ObjetoWS->cnpj					= $this->arrDadosPTA['enbcnpj'];
//		$ObjetoWS->id					= '';
		$ObjetoWS->valorRepasseProposta = $this->arrDadosPTA['ptrvalorconcedente']; //$this->arrDadosPTA['vrlglobal'];
		
		return array($ObjetoWS);
	}
	
	public function getCnpjProgramaEmendaWS(){

		$sql = "SELECT
					enb.enbcnpj,
					vede.emecod,
				    vede.edevalor
				FROM
					emenda.planotrabalho ptr
				    inner join emenda.ptemendadetalheentidade pde on pde.ptrid = ptr.ptrid
				    inner join emenda.v_emendadetalheentidade vede on vede.edeid = pde.edeid
				    inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
				WHERE
					ptr.ptrid = {$this->ptrid}
				    and vede.edestatus = 'A'";
		$arrDados = $this->db->carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		
		foreach ($arrDados as $key => $v) {
			$$key = new CnpjProgramaEmendaWS();
			$$key->cnpj 					= $v['enbcnpj'];
			$$key->numeroEmendaParlamentar 	= $v['emecod'];
			$$key->valorRepasseProposta		= $v['edevalor'];
			
			$ObjetoWS[$key] = $$key;
		}
		return $ObjetoWS;
	}
	
	public function getOrgaoAdministrativoWS(){
		
		$ObjetoWS = new OrgaoAdministrativoWS();
		$ObjetoWS->codigo 			= '26298'; //codigo do FNDE
		//$ObjetoWS->id				= '';
		//$ObjetoWS->responsavelWS	= $this->getMembroParticipanteWS();
		
		return $ObjetoWS;
	}
	
	public function getOrgaoConcedenteWS(){
		
		$ObjetoWS = new OrgaoAdministrativoWS();
		$ObjetoWS->codigo 			= '26298'; //codigo do FNDE
		$ObjetoWS->responsavelWS	= $this->getResponsaveisConvenioWS( $this->cpfPresidente );
		
		return $ObjetoWS;
	}
	
	public function getResponsaveisConvenioWS( $usucpf ){
		
		$ObjetoWS = new ResponsaveisConvenioWS();
//		$ObjetoWS->id 			= '';
		$ObjetoWS->membroWS 	= $this->getMembroParticipanteResponsavelWS( $usucpf );
//		$ObjetoWS->participeWS 	= $this->getParticipantesPropostaWS();
		
		return $ObjetoWS;
	}
	
	public function getMembroParticipanteResponsavelWS( $usucpf ){
		
		$sql = "SELECT
					ent.entnumcpfcnpj, ent.entnome, ent.entemail, ent.entnumcomercial,
				    ent.entorgaoexpedidor, ent.entnumrg
				FROM
					entidade.entidade ent
				WHERE
					ent.entnumcpfcnpj = '$usucpf'";
					
		$arrDados = $this->db->pegaLinha( $sql );
		
		$ObjetoWS = new MembroParticipanteWS();
		$ObjetoWS->cpf = $usucpf; //''; //$arrDados['entnumcpfcnpj'];
				
		return $ObjetoWS;
	}
	public function getMembroParticipanteWS( $usucpf ){
		$sql = "SELECT
					ent.entnumcpfcnpj, ent.entnome, ende.endlog, ende.endcep, ent.entemail, ent.entnumcomercial,
				    fun.fundsc, ent.entorgaoexpedidor, ent.entnumrg, mun.muncod, mun.muncodsiafi, mun.mundescricao, mun.estuf
				FROM
					entidade.entidade ent
				    left join entidade.endereco ende on ende.entid = ent.entid
				    left join territorios.municipio mun on mun.muncod = ende.muncod
				    left join entidade.funcaoentidade fud on fud.entid = ent.entid and fud.fuestatus = 'A'
				    left join entidade.funcao fun on fun.funid = fud.funid
				WHERE
					ent.entnumcpfcnpj = '$usucpf'";
					
		$arrDados = $this->db->pegaLinha( $sql );
		
		$ObjetoWS = new MembroParticipanteWS();
		$ObjetoWS->cpf					= $usucpf; //$arrDados['entnumcpfcnpj'];
		$ObjetoWS->ativoNoSistema 		= 'true';
		$ObjetoWS->cargoFuncao			= $arrDados['fundsc'];
		$ObjetoWS->cep					= $arrDados['endcep'];
		$ObjetoWS->email				= $arrDados['entemail'];
		$ObjetoWS->endereco				= $arrDados['endlog'];
		$ObjetoWS->nome					= $arrDados['entnome'];
		$ObjetoWS->orgaoExpedidor		= $arrDados['entorgaoexpedidor'];
		$ObjetoWS->rg					= $arrDados['entnumrg'];
		$ObjetoWS->telefone				= $arrDados['entnumcomercial'];
				
		return $ObjetoWS;
	}
	
	public function getParticipantesPropostaWS(){
		
		
		
		$ObjetoWS = new ParticipantesPropostaWS();
		$ObjetoWS->agencia 						= $this->arrDadosPTA['ptragenciabancaria'];
		//$ObjetoWS->alteracaoEstatuaria 			= $this->getAlteracaoEstatuariaWS(); // AlteracaoEstatuariaWS
		//$ObjetoWS->aprovado 					= '';
		//$ObjetoWS->areaAtuacaoEntidadePrivadaWS = $this->getAreaAtuacaoEntidadePrivadaWS(); // AreaAtuacaoEntidadePrivadaWS
		//$ObjetoWS->bairroDistrito 				= '';
		$ObjetoWS->cep 							= $this->arrDadosPTA['enbcep'];
		//$ObjetoWS->certidaoAprovada 			= '';
		//$ObjetoWS->certidaoWS 					= $this->getCertidaoWS(); // CertidaoWS
		//$ObjetoWS->cienteDirigenteNaoRemunerado = '';
		//$ObjetoWS->cnaePrimario 				= '';
		//$ObjetoWS->cnaePrimarioWS 				= $this->getCnaeWS(); // CnaeWS
		$ObjetoWS->codigoBanco 					= $this->arrDadosPTA['bcocod'];
		/*$ObjetoWS->codigoErroSiafi 				= '';
		$ObjetoWS->consorcioPublico 			= '';
		$ObjetoWS->contaCorrente 				= '';
		$ObjetoWS->cpfUsuarioAprovou 			= '';
		$ObjetoWS->declaracaoAprovado 			= '';
		$ObjetoWS->declaracaoNaoDividaWS 		= $this->getDeclaracaoNaoDividaWS(); // DeclaracaoNaoDividaWS
		$ObjetoWS->email 						= '';*/
		$ObjetoWS->endereco 					= $this->arrDadosPTA['enblog'];
		/*$ObjetoWS->entidadesVinculadas 			= '';
		$ObjetoWS->erroSiafi 					= '';
		$ObjetoWS->esferaAdministrativaWS 		= $this->getEsferaAdministrativaWS(); // EsferaAdministrativaWS
		$ObjetoWS->estatutoAprovado 			= '';
		$ObjetoWS->estatutoWS 					= $this->getEstatutoWS(); // EstatutoWS
		$ObjetoWS->executor 					= '';
		$ObjetoWS->id 							= '';*/
		$ObjetoWS->identificacao 				= $this->arrDadosPTA['enbcnpj'];
		/*$ObjetoWS->inscricaoEstadual 			= '';
		$ObjetoWS->inscricaoMunicipal 			= '';
		$ObjetoWS->interveniente 				= '';
		$ObjetoWS->login 						= $this->getLoginWS(); // LoginWS
		$ObjetoWS->mandatario 					= '';*/
		$ObjetoWS->municWS 						= $this->getMunicipioWS( $this->arrDadosPTA['muncod'] ); // MunicipioWS
		/*$ObjetoWS->naturezaJuridica 			= '';
		$ObjetoWS->nome 						= '';
		$ObjetoWS->nomeFantasia 				= '';
		$ObjetoWS->proponente 					= '';
		$ObjetoWS->quadroDirigenteAprovado 		= '';*/
		$ObjetoWS->representanteWS 				= $this->getMembroParticipanteWS( $this->arrDadosPTA['cpfdirigente'] ); // MembroParticipanteWS
		$ObjetoWS->respExercicioWS 				= $this->getMembroParticipanteWS( $this->arrDadosPTA['cpfdirigente'] ); // MembroParticipanteWS
		$ObjetoWS->responsavelWS 				= $this->getMembroParticipanteWS( $this->arrDadosPTA['cpfdirigente'] ); // MembroParticipanteWS
		/*$ObjetoWS->respostaEnvioSiafi 			= '';
		$ObjetoWS->statusEnvioSiafi 			= '';
		$ObjetoWS->telefone 					= '';
		$ObjetoWS->telexFaxCaixaPostal 			= '';
		$ObjetoWS->tipoIdentificacaoWS 			= $this->getTipoIdentificadorParticipeWS(); // TipoIdentificadorParticipeWS
		$ObjetoWS->transcricaoEstatutoONG 		= '';*/
		
		return $ObjetoWS;
	}
	
	public function getRegrasWS(){
		
		$ObjetoWS = new RegrasWS();
		$ObjetoWS->aceitaContrapartidaBens 	= false;
		if( $this->acacod == '0048' ){
			$ObjetoWS->descricao 			= 'Contrapartida financeira.';
		} else {
			$ObjetoWS->descricao 			= 'Contrapartida financeira';
		}
		/*$ObjetoWS->id 									= '';
		$ObjetoWS->percentualMaximoContrapartidaBens 	= $this->arrDadosPTA['ptrcontrapartida'];
		$ObjetoWS->percentualMinimoContrapartida 		= $this->arrDadosPTA['ptrcontrapartida'];*/
		
		return $ObjetoWS;
	}
	
	public function getStatusBensServicosPropostaWS(){
		
		$ObjetoWS = new StatusBensServicosPropostaWS();
		$ObjetoWS->codigo 	= 'APROVADO';
//		$ObjetoWS->id 		= '';
		
		return $ObjetoWS;
	}
	
	public function getContaBancariaWS(){
				
		$digitoVerificadorAgencia = substr($this->arrDadosPTA['ptragenciabancaria'], strpos($this->arrDadosPTA['ptragenciabancaria'], '-') + 1, 5);
		
		$ObjetoWS = new ContaBancariaWS();
		$ObjetoWS->agencia 								= substr($this->arrDadosPTA['ptragenciabancaria'],0,4);
		$ObjetoWS->bancoWS 								= $this->getBancoWS($this->arrDadosPTA['bcocod']); // BancoWS
		$ObjetoWS->digitoVerificadorAgencia 			= $digitoVerificadorAgencia;
		$ObjetoWS->situacao 							= 'CADASTRADO'; //$this->arrDadosPTA['cocsituacao_conta'];
		
		return $ObjetoWS;
	}
	
	public function getBancoWS($codigo){
		
		$ObjetoWS = new BancoWS();
		$ObjetoWS->codigo 	= $this->arrDadosPTA['bcocod'];
		
		return $ObjetoWS;
	}
	
	public function getCronogramaDesembolsoPropostaWS(){
		
		$ObjetoWS = new CronogramaDesembolsoPropostaWS();
		$ObjetoWS->parcelaCronogramaDesembolsoWS 	= $this->getParcelaCronogramaDesembolsoWS(); // ParcelaCronogramaDesembolsoWS
		
		return $ObjetoWS;
	}
	
	public function getParcelaCronogramaDesembolsoWS(){
		
		$sql = "SELECT DISTINCT 
				  	ptp.ptrid,
				  	ptp.prddata,
				  	ptp.prdid,
				  	ptp.prdminuta,
				  	ptp.prdtipo
				FROM 
				  	emenda.ptparceladesembolso ptp
				WHERE
					ptp.ptrid = {$this->ptrid}
				    and ptp.prdminuta = 'P'";
		
		$arrDados = $this->db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$ObjetoWS = array();
		foreach ($arrDados as $key => $v) {
			$arrData = explode( '-', $v['prddata'] );
			$ano = $arrData[0];
			$mes = $arrData[1];
			
			$tipo = ($v['prdtipo'] == 'C' ? '1'  : '2');
			
			$$key = new ParcelaCronogramaDesembolsoWS();
			$$key->ano 						= $ano;
			$$key->mes 						= $mes;
			$$key->numero 					= 1;
			$$key->parcelaMetaWS 			= $this->getParcelaMetaWS( $v['prdid'], $tipo ); // ParcelaMetaWS
			$$key->tipoParticipanteWS 		= $this->getTipoParticipanteWS( $tipo ); // TipoParticipanteWS
			$$key->valor 					= $this->valorCronograma;
			$this->valorCronograma			= 0;
			$ObjetoWS[$key] = $$key;
		}
		
		return $ObjetoWS;
	}
	
	public function getParcelaMetaWS( $prdid, $tipo ){
		//iniciativa
		
		$sql = "SELECT
				  pti.ptivalortotal,
				  pti.ptivalorconcedente,
				  pti.ptivalorproponente,
				  pti.ptiid
				FROM
				  emenda.v_ptiniciativa pti
				WHERE
				  pti.ptrid = {$this->ptrid} and pti.ptiid in (select ptiid from emenda.ptparcelainiciativa where prdid = $prdid)";
		
		$arrDados = $this->db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();

		$ObjetoWS = array();		
		foreach ($arrDados as $key => $valor) {
		
			$$key = new ParcelaMetaWS();
			$$key->idMetaCronogramaFisico 	= $valor['ptiid'];
			$$key->parcelaEtapaWS 			= $this->getParcelaEtapaWS( $valor['ptiid'], $tipo ); // ParcelaEtapaWS
			$$key->valor 					= $this->valorEtapa; //$valor;			
			$this->valorCronograma 		   += $this->valorEtapa;
			$this->valorEtapa 				= 0;						
			$ObjetoWS[$key] 				= $$key;
		}
		
		return $ObjetoWS;
	}
	
	public function getParcelaEtapaWS( $ptiid, $tipo ){
		//especificacao
		
		/*$sql = "SELECT
				  pte.pteid,
				  pte.ptevalortotal,
				  pte.ptevalorconcedente,
                  pte.ptevalorproponente
				FROM
				  emenda.ptiniciativa pti 
                  inner join emenda.v_ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid
				WHERE
				  pti.ptrid = {$this->ptrid} 
                  and pti.ptiid = $ptiid
                  and pte.ptestatus = 'A'";*/
		
		$sql = "SELECT distinct
				    esp.espnome,
				    esp.espid,
				    pte.pteid,
				    pte.ptequantidade,
				    pte.ptevalorunitario,
                    pte.ptevalorproponente,
                    --(pte.ptequantidade * pte.ptevalorunitario) as concedente
                    ( CASE WHEN pte.ptevalorproponente is null THEN (pte.ptequantidade * pte.ptevalorunitario)
					         ELSE ((pte.ptequantidade * pte.ptevalorunitario) - pte.ptevalorproponente) END) as concedente
				FROM 
				    emenda.ptiniciativa pti
				    inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid
				    inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
				    inner join emenda.especificacao esp on esp.espid = ice.espid and esp.espstatus = 'A'
				WHERE 
				    pti.ptrid = {$this->ptrid} 
				    and pti.ptiid = $ptiid
				    and ice.icestatus = 'A'
				    and pte.ptestatus = 'A'";
		
		$arrDados = $this->db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$ObjetoWS = array();
		foreach ($arrDados as $key => $v) {
		
			$$key = new ParcelaEtapaWS();
			$$key->idEtapaCronogramaFisico 	= $v['pteid'];
			$$key->valor 					= ( ($tipo == 1) ? $v['concedente'] : $v['ptevalorproponente'] ); //$v['ptevalortotal'];
			$this->valorEtapa 			   += ( ($tipo == 1) ? $v['concedente'] : $v['ptevalorproponente'] );
			
			$ObjetoWS[$key] = $$key; 
		}
		
		return $ObjetoWS;
	}
	
	public function getTipoParticipanteWS( $tipo ){
		
		$ObjetoWS = new TipoParticipanteWS();
		$ObjetoWS->codigo 		= $tipo;
//		$ObjetoWS->descricao 	= '';
//		$ObjetoWS->id 			= '';
		
		return $ObjetoWS;
	}
	
	public function getCronogramaFisicoPropostaWS(){
		
		$ObjetoWS = new CronogramaFisicoPropostaWS();
		//$ObjetoWS->id 						= '';
		$ObjetoWS->metaCronogramaFisicoWS 	= $this->getMetaCronogramaFisicoWS(); // MetaCronogramaFisicoWS
		
		return $ObjetoWS;
	}
	
	public function getMetaCronogramaFisicoWS(){
		
		$sql = "SELECT
				    ini.ininome,
				    pti.ptiid,
				    min(pte.ptedatainicio) as datainicio,
				    max(pte.ptedatafim) as datafim,
				    sum((ptequantidade * ptevalorunitario)) as ptivalortotal,
				    sum(pte.ptequantidade) as ptequantidade,
                    sum(pte.ptevalorproponente) as ptrvalorproponente,
                    (sum(pte.ptevalorproponente) + sum((ptequantidade * ptevalorunitario))) as vrlglobal
				FROM 
				    emenda.ptiniciativa pti
				    inner join emenda.iniciativa ini on ini.iniid = pti.iniid
                    inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid
                    inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
                    inner join emenda.especificacao esp on esp.espid = ice.espid
				WHERE 
				    pti.ptrid = {$this->ptrid}
				    and ice.icestatus = 'A'
				    and pte.ptestatus = 'A'
                group by
                	ini.ininome,
                	pti.ptiid";
				    
		$arrDados = $this->db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$ObjetoWS = array();
		foreach ($arrDados as $key => $v) {
			//$$key
			$$key = new MetaCronogramaFisicoWS();
			$$key->dataFim 					= ($v['datafim'] ? $v['datafim']."T00:00:00" : null);
			$$key->dataInicio 				= ($v['datainicio'] ? $v['datainicio']."T00:00:00" : null);
			$$key->especificacao 			= $v['ininome'];
			$$key->etapaCronogramaFisicoWS 	= $this->getEtapaCronogramaFisicoWS($v['ptiid']); // EtapaCronogramaFisicoWS
			$$key->id 						= $v['ptiid']; //default
			$$key->municipioWS 				= $this->getMunicipioWS( $this->arrDadosPTA['muncod'] ); // MunicipioWS
			$$key->propostaProgramaWS 		= $this->getPropostaProgramaWS( $v ); // PropostaProgramaWS
			$$key->quantidade 				= $v['ptequantidade'];
			$$key->tipoMeta 				= 'NORMAL';
			$$key->unidadeFornecimento 		= 'UN';
			$$key->valor 					= $v['ptivalortotal'];
			$this->valorMetaFisico 			= 0;
			
			$ObjetoWS[$key] = $$key;
		}		
		return $ObjetoWS;
	}
		
	public function getEtapaCronogramaFisicoWS( $ptiid ){
		
		$sql = "SELECT distinct
				    esp.espnome,
				    esp.espid,
				    esp.ssiid,
				    pte.pteid,
				    pte.ptequantidade,
				    pte.ptevalorunitario,
				    pte.ptedatainicio,
				    pte.ptedatafim,
				    ( CASE WHEN pte.ptevalorproponente is null THEN (pte.ptequantidade * pte.ptevalorunitario)
					         ELSE ((pte.ptequantidade * pte.ptevalorunitario) - pte.ptevalorproponente) END) as concedente
				FROM 
				    emenda.ptiniciativa pti
				    inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid
				    inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
				    inner join emenda.especificacao esp on esp.espid = ice.espid and esp.espstatus = 'A'
				WHERE 
				    pti.ptrid = {$this->ptrid}
				    and pti.ptiid = {$ptiid}
				    and ice.icestatus = 'A'
				    and pte.ptestatus = 'A'";
		
		$arrDados = $this->db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		
		foreach ($arrDados as $key => $v) {
			$$key = new EtapaCronogramaFisicoWS();
			$$key->dataFim 				= ($v['ptedatafim'] ? $v['ptedatafim']."T00:00:00" : null);
			$$key->dataInicio 			= ($v['ptedatainicio'] ? $v['ptedatainicio']."T00:00:00" : null);
			$$key->especificacao 		= $v['espnome'];
			$$key->id 					= $v['pteid']; //$arrDados['sndcodigonaturezadespesa'];
			$$key->quantidade 			= $v['ptequantidade'];
			$$key->unidadeFornecimento 	= 'UN';
			$$key->valor 				= ((int)$v['ptequantidade'] * $v['ptevalorunitario']);
			$this->valorMetaFisico		+= $v['concedente'];

			$ObjetoWS[$key] = $$key;
		}				 
		return $ObjetoWS;
	}
	
	public function getModalidadePropostaWS(){
		
		$ObjetoWS = new ModalidadePropostaWS();
		//$ObjetoWS->id 		= '';
		$ObjetoWS->value 	= '1';
		
		return $ObjetoWS;
	}
	
	public function getParecerPropostaConvenioWS(){
		
		$sql = "SELECT 
				    usu.usunome,
				    usu.usucpf,
				    case when ana.anaparecersiconv is not null then ana.anaparecersiconv else ana.anaparecer end as anaparecer,
				    to_char(anadataconclusao, 'YYYY-MM-DD') as dataparecer
				FROM 
				    emenda.analise ana
                    inner join seguranca.usuario usu on usu.usucpf = ana.usucpf 
				WHERE 
				    ana.anatipo in ('T', 'M')
				    and ana.ptrid = {$this->ptrid}
				    and ana.anastatus = 'A'
				    and ana.analote = (SELECT max(analote) 
				                    FROM emenda.analise 
				                    WHERE anatipo = ana.anatipo
				                        and anastatus = 'A' 
				                        and ptrid = ana.ptrid)";
		$arrDados = $this->db->carregar( $sql );					                        
		$arrDados = $arrDados ? $arrDados : array();
		foreach ($arrDados as $key => $v) {
			$anaparecersiconv = strip_tags($v['anaparecer']);
			$anaparecersiconv = trim(str_ireplace('&nbsp;', '', $anaparecersiconv));
								                        
			$$key = new ParecerPropostaConvenioWS();
			$$key->atribuicaoResponsavel	= 'ANALISTA_TECNICO_CONCEDENTE';
			$$key->dataWS 					= ($v['dataparecer'] ? $v['dataparecer'].'T00:00:00' : null);
			$$key->funcaoResponsavel 		= 'Técnico';
			$$key->parecer 					= str_ireplace('&#13;', '', simec_htmlentities($anaparecersiconv));
			$$key->responsavelWS 			= $this->getMembroParticipanteWS( $v['usucpf'] ); // MembroParticipanteWS
			$$key->tipoParticipanteWS 		= 1; //$this->getTipoParticipanteWS();
			
			$ObjetoWS[$key] = $$key;
		} 
		
		return $ObjetoWS;
	}
	
	public function getRepasseExercicioFuturoPropostaWS(){
		
		$ObjetoWS = new RepasseExercicioFuturoPropostaWS();
		$ObjetoWS->ano 			= $_SESSION['exercicio']; // int
		$ObjetoWS->historico 	= 'false';
		//$ObjetoWS->id 			= '';
		$ObjetoWS->valor 		= $this->arrDadosPTA['ptrvalorconcedente'];
		$arrDados = array();
		
		return array($ObjetoWS) ;
	}
	
	public function insereDadosConvenio($ptrnumconvenio){
		
		$sql = "UPDATE emenda.planotrabalho SET 
					ptrnumconvenio = '$ptrnumconvenio',
					ptranoconvenio = '".date('Y')."'
				WHERE 
  					ptrid = $this->ptrid";
  					
  		$this->db->executar($sql);
  		return $this->db->commit();
	}
	
	public function insereRetornoNotaEmepenho(){
		
		if( $this->nuSeqMovNE ){

			$sql = "UPDATE emenda.execucaofinanceira SET 
					  exfnumsolempenho = {$this->nuSeqMovNE},
					  exfespecieempenho = '01',
					  semid = 1,
					  exfdataalteracao = '".date('Y-m-d H:i:s')."',
					  exfanooriginal = '".date('Y')."',
					  exfverifsiafi = '{$this->exfverifsiafi}',
					  exfverifcadin = '{$this->exfverifcadin}',
					  exfdatainclusao = '".date('Y-m-d H:i:s')."'					 
					WHERE 
					  exfid = {$this->arrDadosEmpenho['exfid']}";
			$this->db->executar( $sql );
			
			$this->nuSeqMovNE = '';
			$this->exfverifcadin = '';
			$this->exfverifsiafi = '';
			
			return $this->db->commit();
		} else {
			$this->numeroConvenio = '';
			return false;
		}
	}
	
	public function insereHistoricoEmpenhoSiconv( $arrDados = array() ){
		
		$this->db->pegaUm("DELETE FROM emenda.sicempenhohistorico WHERE ptrid = $this->ptrid and exfid = $this->exfid");
		
		foreach ($arrDados->children() as $v) {
			$v = (array)$v;
			$sql = "INSERT INTO emenda.sicempenhohistorico(ptrid, exfid, anconvenio, anexercicio, cocentrogestaoaprov, coespecieempenho, cofonterecursoaprov,
  						cogestaoemitente, conaturezadespesaaprov, coplanointernoaprov, coprogramafnde, coptresaprov, cosituacaodocsiafi, cotipodocumento,
  						counidadegestoraemitente, dsusernamemovimento, dtmovimento, nucgcfavorecido, nuconvenio, nuconveniosiconv, nuempenhooriginal,
  						nuempenhosiafi, nuidsistema, nuprocesso, nupropostasiconv, nuseqdocsiafi, nuseqmovne, vlempenho) 
					VALUES ($this->ptrid, $this->exfid, '{$v['anConvenio']}', '{$v['anExercicio']}', '{$v['coCentroGestaoAprov']}', '{$v['coEspecieEmpenho']}', '{$v['coFonteRecursoAprov']}',
  						'{$v['coGestaoEmitente']}', '{$v['coNaturezaDespesaAprov']}', '{$v['coPlanoInternoAprov']}', '{$v['coProgramaFnde']}', '{$v['coPtresAprov']}', '{$v['coSituacaoDocSiafi']}', '{$v['coTipoDocumento']}', 
  						'{$v['coUnidadeGestoraEmitente']}', '{$v['dsUsernameMovimento']}', '{$v['dtMovimento']}', '{$v['nuCgcFavorecido']}', '{$v['nuConvenio']}', '{$v['nuConvenioSiconv']}', '{$v['nuEmpenhoOriginal']}', 
  						'{$v['nuEmpenhoSiafi']}', '{$v['nuIdSistema']}', '{$v['nuProcesso']}', '{$v['nuPropostaSiconv']}', '{$v['nuSeqDocSiafi']}', '{$v['nuSeqMovNe']}', '{$v['vlEmpenho']}')";
  			$this->db->executar($sql);
		}
		return $this->db->commit();		
	}
		
	public function carregaDadosPTA(){
		
		$sql = "SELECT pti.vrlglobal, pti.ptivalortotal, ptr.ptrvalorproponente, sum(ped.pedvalor) as ptrvalorconcedente, ptr.ptrid, ptr.ptrcod, ptr.ptridpai, ent.entnumcpfcnpj as cpfdirigente, ent.entnome as nomedirigente,
				    fun.fundsc as funcaodirigente, ent.entorgaoexpedidor as orgaodirigente, ent.entnumrg as rgdirigente, ende.endlog, ende.endcep, ent.entemail, ent.entnumcomercial,
				    mun.muncod, mun.muncodsiafi, mun.mundescricao, mun.estuf, enb.enblog, enb.enbcep, enb.enbcnpj, 
				    ptr.ptrexercicio, ptr.ptragenciabancaria, ptr.ptrjustificativa, ptr.ptrcontrapartida,
				    cc.cocid, cc.cocnu_seq_mov_conta, cc.cocseq_conta, cc.cocds_razao_social, cc.cocandamento,
					cc.cocnu_banco, cc.cocnu_agencia, cc.coccnpj, cc.cocsituacao_conta, cc.cocdatainclusao,
					cc.cocdsproblema, cc.cocnu_conta_corrente, bc.bcocod, bc.bconome, ptr.mdeid,
				    (select max(ptedatainicio) from  emenda.ptiniciativaespecificacao p1 
                    		inner join emenda.ptiniciativa p2 on p2.ptiid = p1.ptiid where p2.ptrid = ptr.ptrid and p1.ptestatus = 'A') as inicioexecucao,
				    (select max(ptedatafim) from  emenda.ptiniciativaespecificacao p1 
                    		inner join emenda.ptiniciativa p2 on p2.ptiid = p1.ptiid where p2.ptrid = ptr.ptrid and p1.ptestatus = 'A') as fimexecucao,
                    ptr.ptrpropostasiconv, ptr.ptrpropostasiconvano
				FROM 
				    emenda.planotrabalho ptr 
                    inner join emenda.ptemendadetalheentidade ped on ped.ptrid = ptr.ptrid
                    inner join emenda.ptiniciativa pt on pt.ptrid = ptr.ptrid
				    left join entidade.entidade ent on ent.entid = ptr.entiddirigente
				    left join emenda.cargos car 
				        inner join entidade.funcao fun on fun.funid = car.funid
				    on car.carid = ptr.carid
				    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
				    inner join territorios.municipio mun on mun.muncod = enb.muncod
				    inner join 
				              (SELECT 
				                  ptrid,
				                  sum(ptivalortotal) as ptivalortotal,
				                  sum(ptivalorconcedente) as vrlglobal
				               FROM emenda.v_ptiniciativa 
				               GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
				    left join emenda.contacorrente cc on cc.ptrid = ptr.ptrid
				    left join emenda.banco ban on ban.bcoid = ptr.bcoid                    
				    left join financeiro.banco bc on bc.bcocod= ban.bcocod
				    left join entidade.endereco ende on ende.entid = ent.entid
				WHERE
					ptr.ptrid = {$this->ptrid}
                group by
                	ptr.ptrid, ptr.ptrcod, ptr.ptridpai, ent.entnumcpfcnpj, ent.entnome,
				    fun.fundsc, ent.entorgaoexpedidor, ent.entnumrg,
				    mun.muncod, mun.muncodsiafi, mun.mundescricao, mun.estuf, enb.enblog, enb.enbcep, enb.enbcnpj, 
				    pti.ptivalortotal, ptr.ptrexercicio, ptr.ptragenciabancaria, ptr.ptrjustificativa, ptr.ptrcontrapartida,
				    cc.cocid, cc.ptrid, cc.cocnu_seq_mov_conta, cc.cocseq_conta, cc.cocds_razao_social, cc.cocandamento,
					cc.cocnu_banco, cc.cocnu_agencia, cc.coccnpj, cc.cocsituacao_conta, cc.cocdatainclusao,
					cc.cocdsproblema, cc.cocnu_conta_corrente, bc.bcocod, bc.bconome, ptr.ptrvalorproponente,
					ende.endlog, ende.endcep, ent.entemail, ent.entnumcomercial, pt.ptiid, ptr.mdeid, pti.vrlglobal,
					ptr.ptrpropostasiconv, ptr.ptrpropostasiconvano";
					
		$arrDados = $this->db->pegaLinha( $sql );
		
		$arrDados = $arrDados ? $arrDados : array();
		$arrDados['nomedirigente'] 		= utf8_decode($arrDados['nomedirigente']);
		return $arrDados;
	}
	
	public function carregaDadosEmpenho(){
		
		if( $_SESSION['exercicio'] > 2014 )	$filtroImp = ' and ei.edeid = vede.edeid';
		
		$sql = "SELECT DISTINCT
					vede.gndcod,
					ptr.ptrnumprocessoempenho,
					mun.muncodsiafi,
					mun.estuf, 
					mun.muncod, 
					mun.mundescricao,
					exf.exfvalor,
					exf.exfid,
					ptr.ptrid,
					ptr.ptranoconvenio,
					ptr.ptrnumconvenio,
					ptr.ptrnumprocessoempenho,
					ptr.ptrpropostasiconv,                                  
					ptr.ptrpropostasiconvano,
					exf.exfanooriginal,
					(CASE WHEN vede.mapcod = '90' THEN '0' -- federal
				    	  WHEN vede.mapcod = '30' THEN '1' -- estadual
				          WHEN vede.mapcod = '40' THEN '2' -- Municipal
				          WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN '3' --Particular
				     ELSE '' END) as esferaadm,
					esf.esfcod as esferaorcamentaria,
					esf.esfdsc,
					exf.exfespecieempenho,
					exf.exfcodfontesiafi as fonterecursos,
					(CASE WHEN vede.gndcod = '3' THEN '33'||vede.mapcod||'4100'
						  WHEN vede.gndcod = '4' THEN '44'||vede.mapcod||'4200'
					  END) as naturezadespesa,
					exf.plicod as planointerno,
					'03' programafnde,
					exf.ptres,
					fup.unicod,
					ende.enbcnpj,
					exf.exfnumempenhooriginal,
					(exf.exfvalor - coalesce(ei.edivalor, 0)) as exfvalor
				FROM emenda.execucaofinanceira exf 
				--inner join monitora.pi_planointerno pli on pli.plicod = exf.plicod and pli.pliano = '".$_SESSION['exercicio']."'
				--left join monitora.ptres pt on pt.ptres = exf.ptres
				inner join emenda.planotrabalho ptr on ptr.ptrid = exf.ptrid and exf.exfstatus = 'A'
				inner join emenda.ptemendadetalheentidade ped on ped.pedid = exf.pedid and ped.ptrid = exf.ptrid
				inner join emenda.ptiniesprecurso per on per.pedid = ped.pedid
				inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
				inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
				inner join monitora.pi_planointerno pli on pli.plicod = exf.plicod 
														/*and pli.pliano >= '".$_SESSION['exercicio']."'*/ 
														and cast(pli.pliano as integer) = vede.emeano 
														and pli.plistatus = 'A'
                left join monitora.ptres pt on pt.ptres = exf.ptres
				inner join public.esfera esf on fup.esfcod = esf.esfcod  
				inner join emenda.entidadebeneficiada ende on ende.enbid = ptr.enbid
				inner join territorios.municipio mun on ende.muncod = mun.muncod 
				left join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid and ei.edistatus = 'A' $filtroImp 
				/*inner join public.unidadegestora ung on ung.unicod = fup.unicod
				inner join public.unidade uni on uni.unicod = fup.unicod*/
				where 
					exf.ptrid = {$this->ptrid} 
					and exf.exfid = {$this->exfid}
					AND vede.emeano = '".$_SESSION['exercicio']."'     
					AND exf.exfnumsolempenho is null                                         
				GROUP BY
				vede.gndcod,
					ptr.ptrnumprocessoempenho, mun.muncodsiafi, exf.exfvalor, ptr.ptranoconvenio, exf.exfanooriginal, esf.esfcod, esf.esfdsc,
					exf.exfespecieempenho, exf.exfcodfontesiafi, vede.mapcod, exf.plicod, exf.ptres, fup.unicod, ende.enbcnpj, ptr.ptrnumconvenio,
					exf.exfnumempenhooriginal, ptr.ptrnumprocessoempenho, ptr.ptrpropostasiconv, ptr.ptrpropostasiconvano, exf.exfid, ptr.ptrid,
					mun.estuf, mun.muncod, mun.mundescricao, ei.edivalor";
		
		$arrDados = $this->db->pegaLinha( $sql );		
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
	
	public function carregarDadosConsultaEmpenho(){
		
		$sql = "SELECT
					ptr.ptrid,
                    exf.exfid,
					ptr.ptranoconvenio,
				    ptr.ptrnumconvenio,
				    ptr.ptrexercicio,
				    ptr.ptrnumprocessoempenho,
				    ptr.ptrpropostasiconv,
				    ptr.ptrpropostasiconvano
				FROM
					emenda.planotrabalho ptr
                    inner join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid
				WHERE
				    ptr.ptrid = {$this->ptrid}
                    and exf.exfid = {$this->exfid}
                    and exf.exfstatus = 'A'";
				    
		$arrDados = $this->db->pegaLinha( $sql );
		return $arrDados;
	}
	
	public function carregarDadosPagamento(){
		$sql = "";
	}
	
	public function getFuncoes(SimecWsInterfaceService $wsProposta){
		echo '<pre>';
		print_r($wsProposta->__getFunctions());
	}
	
	public function getTipos(SimecWsInterfaceService $wsProposta){
		echo '<pre>';
		print_r($wsProposta->__getTypes());
	}
}
?>