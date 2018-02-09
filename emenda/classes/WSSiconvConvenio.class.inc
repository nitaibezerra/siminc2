<?php
require_once APPRAIZ."emenda/classes/InterfaceSiconvConvenios.class.inc";
require_once APPRAIZ."emenda/classes/InterfaceSiconvMP.class.inc";

class WSSiconvConvenio{
	private $urlWsdl;
	private $ptrid;
	private $exfid;
	private $exfverifcadin;
	private $exfverifsiafi;
	private $wsusuario;
	private $wssenha;
	private $arrDadosEmp = array();
	
	public function __construct( $arrParam = array() ){
		//MyIp: 200.130.2.5		
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			if( $arrParam['service'] == 'siconv' ){
				$urlWsdl = 'https://wshom.convenios.gov.br/siconv-interfaceSiconv-1.0/InterfaceSiconvHandler/InterfaceSiconvHandlerBeanImpl?wsdl';
			} else {
				$urlWsdl = 'https://wshom.convenios.gov.br/siconv-interfaceConvenio-1.0/InterfaceConvenioHandler/InterfaceConvenioHandlerBeanImpl?wsdl';
			}
		} else {
			if( $arrParam['service'] == 'siconv' ){
				$urlWsdl = 'https://ws.convenios.gov.br/siconv-interfaceSiconv-1.0/InterfaceSiconvHandler/InterfaceSiconvHandlerBeanImpl?wsdl';
			} else {
				$urlWsdl = 'https://ws.convenios.gov.br/siconv-interfaceConvenio-1.0/InterfaceConvenioHandler/InterfaceConvenioHandlerBeanImpl?wsdl';
			}
		}
		
		$this->ptrid 		= $arrParam['ptrid'];
		$this->exfid 		= $arrParam['exfid'];
		$this->wsusuario 	= $arrParam['usuario'];
		$this->wssenha 		= $arrParam['senha'];
		$this->urlWsdl 		= $urlWsdl;
		$this->arrDadosEmp 	= array();
		$this->exfverifcadin = $arrParam['post']['verificadoCadin'];
		$this->exfverifsiafi = $arrParam['post']['verificadoSiafi'];
	}
	
	public function enviarNotaEmpenho(){
		global $db;
		$this->arrDadosEmp = $this->carregaDadosEmpenho();
				
		$obEnvio = $this->getEnviarNotaEmpenho();
		//ver($this->urlWsdl, $obEnvio,d);
		$wsConvenio = new SimecWsSiconvConvenio($this->urlWsdl, array(
																'exceptions'	=> true,
																'trace'			=> true,
																'encoding'		=> 'ISO-8859-1',
																"proxy_host" => "",
																"proxy_port" => '',
																)
												);
												
		$wsConvenio->__setUsernameToken( $this->wsusuario, md5($this->wssenha) );		
		$objRetorno = $wsConvenio->enviarNotaEmpenho( $obEnvio, $this->arrDadosEmp );
		
		$retorno = $objRetorno->return;
		
		$pos = strripos($retorno, 'Convenio:');
		$convenio = trim(substr($retorno, ((int)$pos + 9)));
		
		if( !empty($convenio) ){
			$arconvenio = explode('/', $convenio);
			$ptrnumconvenio = $arconvenio[0]; 
			$ptranoconvenio = $arconvenio[1];
			 
			$sql = "UPDATE emenda.planotrabalho SET 
		  				ptrnumconvenio = '{$ptrnumconvenio}',
		  				ptranoconvenio = '{$ptranoconvenio}'
					WHERE 
		  				ptrid = {$this->ptrid}";
			$db->executar($sql);
			$db->commit();
		}
		
		return $retorno;
	}
	
	public function exportaConvenio(){
		global $db;
		
		$this->arrDadosEmp = $this->carregaDadosEmpenho();
		
		$obEnvio = $this->getExportaConvenio();
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
		$objRetorno = $wsConvenio->exportaConvenio( $obEnvio, $this->arrDadosEmp );
		ver($objRetorno,d);
	}
	
	private function getExportaConvenio(){		
		$ObjetoWS = new exportaConvenio();
		$ObjetoWS->arg0 = $this->getConvenioExportacaoWS();
		
		return $ObjetoWS;
	}
	
	private function getConvenioExportacaoWS(){
		
		$ObjetoWS = new convenioExportacaoWS();
		$ObjetoWS->login 				 = $this->getLoginWS(true);
		$ObjetoWS->ano 					 = $this->arrDadosEmp['ptranoconvenio'];
		$ObjetoWS->sequencial 			 = $this->arrDadosEmp['ptrnumconvenio'];
		$ObjetoWS->codigoOrgaoConcedente = '20113'; #MINISTERIO DO PLANEJAMENTO, ORCAMENTO E GESTAO 
		#FNDE - 26298
		
		return $ObjetoWS;
	}
	
	private function getLoginWS($hash = false){
		
		$ObjetoWS = new loginWS();
//		$ObjetoWS->id 		= 32;
		$ObjetoWS->usuario 	= $this->wsusuario;
		if (!$hash)
			$ObjetoWS->senha 	= $this->wssenha;//$this->wssenha;
		else 
			$ObjetoWS->senha 	= md5($this->wssenha);
		
		return $ObjetoWS;
	}
	
	private function getEnviarNotaEmpenho(){
		$ObjetoWS = new enviarNotaEmpenho();
		$ObjetoWS->arg0	= $this->getConvenioWS(); // convenioWS
		
		return $ObjetoWS;
	}
	
	private function getConvenioWS(){
				
		$ObjetoWS = new convenioWS();
		$ObjetoWS->ano 									= $_SESSION['exercicio']; // int
//		$ObjetoWS->dataAssinatura 						= ''; // dateTime
//		$ObjetoWS->dataPrestacaoContas 					= ''; // dateTime
//		$ObjetoWS->dataPublicacao 						= ''; // dateTime
		$ObjetoWS->empenhado 							= false; // boolean
//		$ObjetoWS->fimExecucao 							= ''; // dateTime
//		$ObjetoWS->fundamentoLegal 						= ''; // string
//		$ObjetoWS->id 									= ''; // long
//		$ObjetoWS->idHash 								= ''; // string
//		$ObjetoWS->inadimplente 						= ''; // boolean
//		$ObjetoWS->inicioExecucao 						= ''; // dateTime
//		$ObjetoWS->justificativa 						= ''; // string
//		$ObjetoWS->justificativaAlteracaoAdimplencia 	= ''; // string
		$ObjetoWS->login 								= $this->getLoginWS(true); // loginWS
		$ObjetoWS->notaEmpenhoWS 						= $this->getNotaEmpenhoWS(); // notaEmpenhoWS
//		$ObjetoWS->notaLancamentoWS 					= $this->getNotaLancamentoWS(); // notaLancamentoWS
//		$ObjetoWS->numeroControleExterno 				= ''; // string
		$ObjetoWS->numeroInterno 						= '001'; // string
		$ObjetoWS->numeroProcesso 						= $this->arrDadosEmp['ptrnumprocessoempenho']; // string
//		$ObjetoWS->permiteAjustesCronFisico 			= ''; // boolean
//		$ObjetoWS->prazoPrestacaoContas 				= ''; // int
//		$ObjetoWS->prazoProrrogacaoPrestacaoContas 		= ''; // int
		$ObjetoWS->propostaConvenioWS 					= $this->getPropostaConvenioWS(); // propostaConvenioWS
//		$ObjetoWS->propostaWS 							= ''; // propostaWS
		//$ObjetoWS->prorrogasOficio 						= $this->getProrrogaOficioWS(); // prorrogaOficioWS
//		$ObjetoWS->publicado 							= ''; // boolean
//		$ObjetoWS->sequencial 							= ''; // int
//		$ObjetoWS->situacaoPublicacaoWS 				= ''; // string
//		$ObjetoWS->statusConvenioWS 					= ''; // string
		//$ObjetoWS->termosAditivosWS 					= $this->getTermoAditivoWS(); // termoAditivoWS
//		$ObjetoWS->tipoBloqueioWS 						= ''; // string
//		$ObjetoWS->tipoConvenioWS 						= ''; // string
		
		return $ObjetoWS;
	}
	
	private function getNotaEmpenhoWS(){
				
		$ObjetoWS = new notaEmpenhoWS();
//		$ObjetoWS->amparoLegal 				= ''; // string
		$ObjetoWS->ano 						= $_SESSION['exercicio']; // string
//		$ObjetoWS->codigoErroSiafi 			= ''; // string
		$ObjetoWS->cronogramaEmpenhoWS 		= $this->getCronogramaEmpenhoWS(); // cronogramaEmpenhoWS
		$ObjetoWS->dataEmissao 				= date('Y-m-d').'T00:00:00'; // dateTime
//		$ObjetoWS->dataEnvioXml 			= ''; // dateTime
//		$ObjetoWS->dataPagamento 			= ''; // dateTime
//		$ObjetoWS->erroSiafi 				= ''; // string
		$ObjetoWS->esferaOrcamentaria 		= $this->arrDadosEmp['esferaadm']; // string
		$ObjetoWS->especieEmpenho 			= $this->getTipoNotaEmpenhoWS(); // tipoNotaEmpenhoWS
//		$ObjetoWS->favorecido 				= ''; // string
		$ObjetoWS->fonteRecurso 			= $this->arrDadosEmp['fonterecursos']; // string
		$ObjetoWS->gestaoEmitente 			= '15253'; // string
//		$ObjetoWS->gestaoEmpenhoOriginal 	= ''; // string
//		$ObjetoWS->gestaoFavorecida 		= ''; // string
//		$ObjetoWS->gestaoReferencia 		= ''; // string
//		$ObjetoWS->id 						= ''; // long
//		$ObjetoWS->idHash 					= ''; // string
//		$ObjetoWS->inciso					= ''; // string
//		$ObjetoWS->indicadorContraEntregaWS	= ''; // string
//		$ObjetoWS->indicadorResultado 		= ''; // string
//		$ObjetoWS->indicadorTipoCredito 	= ''; // string
		$ObjetoWS->itensEmpenhoWS 			= $this->getItemEmpenhoWS(); // itemEmpenhoWS
//		$ObjetoWS->modalidadeLicitacao 		= ''; // long
//		$ObjetoWS->municipioBeneficiado 	= ''; // string
//		$ObjetoWS->numero 					= ''; // string
//		$ObjetoWS->numeroConvenio 			= $this->arrDadosEmp['ptrnumconvenio']; // string
//		$ObjetoWS->numeroEmpenhoOriginal 	= ''; // string
//		$ObjetoWS->numeroInterno 			= ''; // long
//		$ObjetoWS->numeroInternoConcedente 	= ''; // string
//		$ObjetoWS->numeroLista 				= ''; // string
//		$ObjetoWS->objeto 					= ''; // string
		$ObjetoWS->observacao 				= '02'; // string
//		$ObjetoWS->origemMaterial 			= ''; // string
		$ObjetoWS->planoInterno 			= $this->arrDadosEmp['planointerno']; // string
//		$ObjetoWS->programaDeTrabalho 		= ''; // string
		$ObjetoWS->ptres 					= $this->arrDadosEmp['ptres']; // string
//		$ObjetoWS->situacaoWS 				= ''; // string
//		$ObjetoWS->taxaCambio 				= ''; // double
		$ObjetoWS->tipoEmpenhoWS 			= $this->getTipoEmpenhoWS(); // tipoEmpenhoWS
		$ObjetoWS->ugEmitente 				= '153173'; #FNDE // string
//		$ObjetoWS->ugEmpenhoOriginal 		= ''; // string
//		$ObjetoWS->ugReferencia 			= ''; // string
//		$ObjetoWS->ugr 						= ''; // string
		$ObjetoWS->unidadeFederativaWS 		= $this->getUnidadeFederativaWS(); // unidadeFederativaWS
		$ObjetoWS->unidadeOrcamentaria 		= $this->arrDadosEmp['unicod']; // string
		$ObjetoWS->valorEmpenho 			= $this->arrDadosEmp['exfvalor']; // double
		
		return $ObjetoWS;
	}
	
	private function getCronogramaEmpenhoWS(){
		global $db;
		
		$sql = "SELECT
					pte.ptedatainicio||'T00:00:00' as inicial,
					pte.ptedatafim||'T00:00:00' as final
				FROM
					emenda.ptiniciativaespecificacao pte
				INNER JOIN
					emenda.ptiniciativa pti ON pti.ptiid = pte.ptiid
										   AND pti.ptrid = ".$this->ptrid."
				WHERE
					ptestatus = 'A'";
		
		$arDatas = $db->pegaLinha($sql);
		
		$sql = "SELECT DISTINCT 
				  	ppd.prddata,
				  	ppd.prdtipo,
                    sum(ppi.privalor) as valor
				FROM 
				  	emenda.ptparceladesembolso ppd
                    inner join emenda.ptparcelainiciativa ppi on ppd.prdid = ppi.prdid
				WHERE
					ppd.ptrid = {$this->ptrid}
				    and ppd.prdminuta = 'P'
                GROUP BY	
                	ppd.prddata,
				  	ppd.prdtipo
				ORDER BY ppd.prdtipo";
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		//ver($sql, $arrDados, $cronogramaExecucao,d);
		
		$ObjetoWS = array();
		foreach ($arrDados as $key => $v) {
			$$key = new cronogramaEmpenhoWS();
			
			$tipo = ($v['prdtipo'] == 'C' ? '1'  : '2');
			
			$$key->dataRecebimento	= $arDatas['inicial']; // dateTime
			$$key->dataVencimento	= $arDatas['final']; // dateTime
			$$key->id				= $tipo; // long
			//$$key->idHash			= ''; // string
			$$key->valor			= $v['valor']; // double
			
			$ObjetoWS[$key] = $$key;
		}
		
		return $ObjetoWS;
	}
	
	private function getTipoNotaEmpenhoWS(){
		
		/*
		01 - EMPENHO_DE_DESPESA
		02 - REFORCO_DE_EMPENHO
		03 - ANULACAO_DE_EMPENHO
		04 - CANCELAMENTO_EMPENHO
		05 - ESTORNO_ANULACAO_NOTA_EMPENHO
		06 - ESTORNO_CANCELAMENTO_NOTA_EMPENHO
		07 - EMPENHO_DE_DESPESA_PRE_EMPENHADA
		08 - REFORCO_DE_EMPENHO_DE_DESPESA_PRE_EMPENHADA
		09 - ANULACAO_DE_DESPESA_PRE_EMPENHADA
		10 - CANCELAMENTO_EMPENHO_DESPESA_PRE_EMPENHADA
		11 - ESTORNO_ANULACAO_EMPENHO_DESPESA_PRE_EMPENHADA
		12 - ESTORNO_CANCELAMENTO_EMPENHO_DESPESA_PRE_EMPENHADA
		13 - OUTROS_CANCELAMENTOS_RP
		14 - CANCELAMENTO_RP_INSUF_REC
		15 - ESTORNO_OUTROS_CANCELAMENTOS_RP
		16 - ESTORNO_CANCELAMENTO_RP_INSUF_REC
		1 - PRE_EMPENHO
		2 - REFORCO_PRE_EMPENHO
		3 - ANULACAO_PRE_EMPENHO
		4 - PRE_EMPENHO_OUTRA_UG
		5 - REFORCO_PRE_EMPENHO_OUTRA_UG
		6 - ANULACAO_PRE_EMPENHO_PELA_UG
		7 - ANULACAO_PRE_EMPENHO_OUTRA_UG
		*/
		
		$ObjetoWS = new tipoNotaEmpenhoWS();
		$ObjetoWS->codigo	= '01'; // string
		//$ObjetoWS->id		= '01'; // long
		
		return $ObjetoWS;
	}
	
	private function getItemEmpenhoWS(){
		global $db;
		
		$sql = "SELECT DISTINCT
				    esp.espnome,
				    esp.espid,
				    esp.ssiid,
				    pte.ptequantidade,
    				(pte.ptevalorunitario - coalesce(pte.ptevalorproponente, 0)) as valor,
                    ssi.ssicodigosubitem,
                    ssi.ssidescricaosubitem,
                    snd.sndcodigonaturezadespesa,
                    snd.snddescricaonaturezadespesa
				FROM 
				    emenda.planotrabalho ptr
				    inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
				    inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
				    inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
				    inner join emenda.especificacao esp on esp.espid = ice.espid
                    inner join emenda.siconvsubitens ssi on ssi.ssiid = esp.ssiid
                    inner join emenda.siconvnaturezadespesa snd on snd.sndid = ssi.sndid
				WHERE 
				    ptr.ptrid = {$this->ptrid}
				    and ice.icestatus = 'A'";
		
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		//ver($sql, $arrDados, $cronogramaExecucao,d);
		
		$ObjetoWS = array();
		foreach ($arrDados as $key => $v) {
		
			$$key = new itemEmpenhoWS();
			$$key->descricao				= trim($v['espnome']); // string
			$$key->especie					= '01'; // int
			//$$key->id						= $v['sndcodigonaturezadespesa']; // long
			//$$key->idHash					= $v['']; // string
			$$key->naturezaDespesaSubItem	= trim($v['ssicodigosubitem'].$v['sndcodigonaturezadespesa']); // string
			$$key->quantidade				= $v['ptequantidade']; // double
			//$$key->referencia				= $v['']; // string
			$$key->valorUnitario			= $v['valor']; // double
			$ObjetoWS[$key] = $$key;
		}
		
		return $ObjetoWS;
	}
	
	private function getTipoEmpenhoWS(){
		/*
		1 - ORDINARIO
		3 - ESTIMATIVO
		5 - GLOBAL
		*/
		$ObjetoWS = new tipoEmpenhoWS();
		$ObjetoWS->codigo	= '1'; // string
		//$ObjetoWS->id		= '1'; // long
		//$ObjetoWS->idHash	= ''; // string
		
		return $ObjetoWS;
	}		
		
	private function getNotaLancamentoWS(){
		
		$ObjetoWS = new notaLancamentoWS();
		$ObjetoWS->ano 							= ''; // string
		$ObjetoWS->codErroSiafi 				= ''; // string
		$ObjetoWS->dataEmissao 					= ''; // dateTime
		$ObjetoWS->dataEnvioXml 				= ''; // dateTime
		$ObjetoWS->dataValorizacao 				= ''; // dateTime
		$ObjetoWS->dataVencimentoTituloCredito 	= ''; // dateTime
		$ObjetoWS->erroSiafi 					= ''; // string
		$ObjetoWS->eventoNotaLancamentoWS 		= $this->getEventoNotaLancamentoWS(); // eventoNotaLancamentoWS
		$ObjetoWS->favorecido 					= ''; // string
		$ObjetoWS->gestaoEmitente 				= ''; // string
		$ObjetoWS->gestaoFavorecida 			= ''; // string
		$ObjetoWS->id 							= ''; // long
		$ObjetoWS->idHash 						= ''; // string
		$ObjetoWS->inverteSaldo 				= ''; // string
		$ObjetoWS->numConvenio 					= ''; // string
		$ObjetoWS->numeroControleExterno 		= ''; // string
		$ObjetoWS->numeroDocumento 				= ''; // string
		$ObjetoWS->numeroInterno 				= ''; // long
		$ObjetoWS->numeroInternoConcedente 		= ''; // string
		$ObjetoWS->observacao 					= ''; // string
		$ObjetoWS->selecionadoDh 				= ''; // string
		$ObjetoWS->situacao 					= ''; // string
		$ObjetoWS->taxaCambio 					= ''; // double
		$ObjetoWS->tituloCredito 				= ''; // string
		$ObjetoWS->uasgEmitente 				= ''; // string
		$ObjetoWS->ugEmitente 					= ''; // string
		
		return $ObjetoWS;
	}
	
	private function getEventoNotaLancamentoWS(){
		
		$ObjetoWS = new eventoNotaLancamentoWS();
		$ObjetoWS->classificacao1 	= ''; // string
		$ObjetoWS->classificacao2 	= ''; // string
		$ObjetoWS->id 				= ''; // long
		$ObjetoWS->idHash 			= ''; // string
		$ObjetoWS->inscricao1 		= ''; // string
		$ObjetoWS->inscricao2 		= ''; // string
		$ObjetoWS->numero 			= ''; // string
		$ObjetoWS->valorLancamento 	= ''; // double
		
		return $ObjetoWS;
	}
	
	private function getPropostaConvenioWS(){
		
		$ObjetoWS = new propostaConvenioWS();
		$ObjetoWS->ano			= '2014'; //$this->arrDadosEmp['ptrpropostasiconvano']; // int
		/*$ObjetoWS->id			= ''; // long
		$ObjetoWS->idHash		= ''; // string*/
		$ObjetoWS->sequencial	= '000150'; //$this->arrDadosEmp['ptrpropostasiconv']; // int
		
		return $ObjetoWS;
	}
	
	private function getProrrogaOficioWS(){
		
		$ObjetoWS = new propostaConvenioWS();
		$ObjetoWS->assinado 					= ''; // boolean
		$ObjetoWS->cpfResponsavelAssinatura 	= ''; // string
		$ObjetoWS->dataAssinatura 				= ''; // dateTime
		$ObjetoWS->dataCadastro 				= ''; // dateTime
		$ObjetoWS->dataDisponibilizacao 		= ''; // dateTime
		$ObjetoWS->dataFimVigencia 				= ''; // dateTime
		$ObjetoWS->dataPublicacao 				= ''; // dateTime
		$ObjetoWS->diasProrrogacao 				= ''; // int
		$ObjetoWS->finalizado 					= ''; // boolean
		$ObjetoWS->id 							= ''; // long
		$ObjetoWS->idHash 						= ''; // string
		$ObjetoWS->nomeResponsavelAssinatura 	= ''; // string
		$ObjetoWS->numero 						= ''; // string
		$ObjetoWS->publicado 					= ''; // boolean
		$ObjetoWS->publicar 					= ''; // boolean
		$ObjetoWS->responsavelRegistroWS 		= $this->getMembroParticipanteWS(); // membroParticipanteWS
		$ObjetoWS->statusWS 					= ''; // string
		
		return $ObjetoWS;
	}
	
	private function getMunicipioWS(){
		
		$ObjetoWS = new municipioWS();
		$ObjetoWS->codigo				= $this->arrDadosEmp['muncod']; // string
		$ObjetoWS->id					= ''; // long
		$ObjetoWS->idHash				= ''; // string
		$ObjetoWS->nome					= $this->arrDadosEmp['mundescricao']; // string
		$ObjetoWS->unidadeFederativaWS 	= $this->getUnidadeFederativaWS(); // unidadeFederativaWS
		
		return $ObjetoWS;
	}
	
	private function getUnidadeFederativaWS(){
		
		$ObjetoWS = new unidadeFederativaWS();
		//$ObjetoWS->id		= ''; // long
		//$ObjetoWS->idHash	= ''; // string
		$ObjetoWS->sigla	= $this->arrDadosEmp['estuf']; // string
		
		return $ObjetoWS;
	}
	
	private function getTermoAditivoWS(){
		
		$ObjetoWS = new termoAditivoWS();
		$ObjetoWS->ano 								= ''; // int
		$ObjetoWS->assinado 						= ''; // boolean
		$ObjetoWS->codigoEnvioSiafi 				= ''; // long
		$ObjetoWS->codigoEnvioSiafiContrapartida	= ''; // long
		$ObjetoWS->codigoErroSiafi 					= ''; // string
		$ObjetoWS->data 							= ''; // dateTime
		$ObjetoWS->dataAssinatura 					= ''; // dateTime
		$ObjetoWS->dataEnvioXml 					= ''; // dateTime
		$ObjetoWS->dataFimVigencia 					= ''; // dateTime
		$ObjetoWS->dataInicioVigencia 				= ''; // dateTime
		$ObjetoWS->dataPublicacao 					= ''; // dateTime
		$ObjetoWS->descricaoOutros 					= ''; // string
		$ObjetoWS->erroSiafi 						= ''; // string
		$ObjetoWS->finalizado 						= ''; // boolean
		$ObjetoWS->fundamentoLegal 					= ''; // string
		$ObjetoWS->id 								= ''; // long
		$ObjetoWS->idHash 							= ''; // string
		$ObjetoWS->justificativa 					= ''; // string
		$ObjetoWS->numeroNS 						= ''; // string
		$ObjetoWS->numeroNSContrapartida 			= ''; // string
		$ObjetoWS->objetoAlteracao 					= ''; // string
		$ObjetoWS->permiteAlteracoes 				= ''; // boolean
		$ObjetoWS->permitirEdicaoConvenente 		= ''; // boolean
		$ObjetoWS->publicado 						= ''; // boolean
		$ObjetoWS->publicar 						= ''; // boolean
		$ObjetoWS->repassesExercicioFuturosTAWS 	= $this->getRepasseExercicioFuturoTermoAditivoWS(); // repasseExercicioFuturoTermoAditivoWS
		$ObjetoWS->respConcedenteWS 				= $this->getMembroParticipanteWS(); // membroParticipanteWS
		$ObjetoWS->respRegistroWS 					= $this->getMembroParticipanteWS(); // membroParticipanteWS
		$ObjetoWS->sequencial 						= ''; // int
		$ObjetoWS->situacaoWS 						= ''; // string
		$ObjetoWS->statusWS 						= ''; // string
		$ObjetoWS->textoAmpliacaoObjeto 			= ''; // string
		$ObjetoWS->tiposTAWS 						= $this->getTipoTaWS(); // tipoTaWS
		$ObjetoWS->valorContrapartida 				= ''; // double
		$ObjetoWS->valorContrapartidaBensServicos 	= ''; // double
		$ObjetoWS->valorContrapartidaFinanceira 	= ''; // double
		$ObjetoWS->valorGlobal 						= ''; // double
		$ObjetoWS->valorRepasse 					= ''; // double
		
		return $ObjetoWS;
	}
	
	private function getRepasseExercicioFuturoTermoAditivoWS(){
		
		$ObjetoWS = new repasseExercicioFuturoTermoAditivoWS();
		$ObjetoWS->ano		= ''; // int
		$ObjetoWS->id		= ''; // long
		$ObjetoWS->idHash	= ''; // string
		$ObjetoWS->valor	= ''; // double
		
		return $ObjetoWS;
	}
	
	private function getMembroParticipanteWS(){
		
		$ObjetoWS = new membroParticipanteWS();
		$ObjetoWS->ativoNoSistema 		= ''; // boolean
		$ObjetoWS->cargoFuncao 			= ''; // string
		$ObjetoWS->cep 					= ''; // string
		$ObjetoWS->cpf 					= ''; // string
		$ObjetoWS->email 				= ''; // string
		$ObjetoWS->endereco 			= ''; // string
		$ObjetoWS->id 					= ''; // long
		$ObjetoWS->idHash 				= ''; // string
		$ObjetoWS->matricula 			= ''; // string
		$ObjetoWS->municipioMembroWS 	= $this->getMunicipioWS(); // municipioWS
		$ObjetoWS->municipioWS 			= $this->getMunicipioWS(); // municipioWS
		$ObjetoWS->nome 				= ''; // string
		$ObjetoWS->orgaoExpedidor 		= ''; // string
		$ObjetoWS->rg 					= ''; // string
		$ObjetoWS->senha 				= ''; // string
		$ObjetoWS->telefone 			= ''; // string
		
		return $ObjetoWS;
	}
	
	private function getTipoTaWS(){
		
		$ObjetoWS = new tipoTaWS();
		$ObjetoWS->codigoTipo	= ''; // string
		$ObjetoWS->id			= ''; // long
		$ObjetoWS->idHash		= ''; // string
		
		return $ObjetoWS;
	}
	
	private function carregaDadosEmpenho(){
		global $db;
		
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
				left join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid and ei.edistatus = 'A'  $filtroImp
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
		
		$arrDados = $db->pegaLinha( $sql );
		
		$arrDados = $arrDados ? $arrDados : array();
		return $arrDados;
	}
}
?>