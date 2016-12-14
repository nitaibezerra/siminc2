<?php
class MontaObjetoAlteracao{
	
	private $urlWsdl;
	private $wsusuario; 
	private $wssenha;
	private $certificado;
	private $senha_certificado;
	private $dbws;
	private $post;
	
	public function __construct( $arrParam = array() ){
		global $db;
        
        $urlWsdl = WEB_SERVICE_SIOP_URL. 'WSAlteracoesOrcamentarias?wsdl';
        $this->certificado = WEB_SERVICE_SIOP_CERTIFICADO;
//        $this->senha_certificado = WEB_SERVICE_SIOP_SENHA;
		/*if($db){
			$this->dbws = $db;
		} else {*/
			$this->dbws = $db;
		//}
		
		$this->wsusuario 	= $arrParam['usuario'];
		$this->wssenha 		= $arrParam['senha'];
		$this->urlWsdl 		= $urlWsdl;
		$this->post			= $arrParam['post'];
		$this->acaid		= '';
		$this->valorfisico	= '';
	}
	
	public function cadastrarPedidoAlteracao(){
		$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
		
		$arrPedido = $this->post['pedido_cadastra'];
		$arrPedido = $arrPedido ? $arrPedido : array();
		$arrTipoAlteracao = array();
		$arrCodigoUO = array();
		foreach ($arrPedido as $valor) {
			$arrValor = explode( '_', $valor );
			$arrTipoAlteracao[] = strtolower($arrValor[0]) ;
			$arrCodigoUO[] = $arrValor[1];
		}
		$arrRetorno = array();
		
		foreach ($arrCodigoUO as $key => $codigouo) {

			$arrPedido = $obAlteracaoDAO->carregarPedido( $this->post['mcrid'], $arrTipoAlteracao[$key], $codigouo );
			$obEnvio = $this->getcadastrarPedidoAlteracao($this->post['mcrid'], $arrPedido);

			$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																			'local_cert'	=> $this->certificado, 
																			'passphrase ' 	=> $this->senha_certificado,
																			'exceptions'	=> true,
																	        'trace'			=> true,
																			'encoding'		=> 'ISO-8859-1' )
															);
			
			$retorno = $wsAlteracao->cadastrarPedidoAlteracao( $obEnvio, $arrPedido );
			array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
										   'tipoalteracao' => $arrTipoAlteracao[$key],
										   'codigouo' => $codigouo,
										   'retorno' => $retorno
										));
//			ver($retorno);
		}
		return $arrRetorno;
//		$retorno = $obAlteracaoDAO->manterCadastrarPedidoAlteracaoResponse( $cadastrarPedidoAlteracaoResponse, $documento, $obCadastrarPedidoAlteracao, $strTotal );
	}
	
	public function excluirPedidoAlteracao(){		
		//$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
		$arrPedido = $this->post['pedido_exclui'];
		$arrPedido = $arrPedido ? $arrPedido : array();
		$arrRetorno = array();
		foreach ($arrPedido as $identificador) {
			$arrValor = explode( '_', $identificador );
			$tipoAlteracao = strtolower($arrValor[0]) ;
			$identificador = $arrValor[1];
			
			$obEnvio = $this->getExcluirPedidoAlteracao( $identificador );			
			//ver($obEnvio,d);
			$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																			'local_cert'	=> $this->certificado, 
																			'passphrase ' 	=> $this->senha_certificado,
																			'exceptions'	=> true,
																	        'trace'			=> true,
																			'encoding'		=> 'ISO-8859-1' )
															);
			
			$retorno = $wsAlteracao->excluirPedidoAlteracao( $obEnvio );
			array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
										   'tipoalteracao' => $tipoAlteracao,
										   'codigouo' => $identificador,
										   'retorno' => $retorno
										));
			
		}
		return $arrRetorno;
	}
	
	public function obterPedidoAlteracao(){		
		$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
		
		$arrPedido = $this->post['pedido_obterpedido'];
		$arrPedido = $arrPedido ? $arrPedido : array();
		
		$arrRetorno = array();
		foreach ($arrPedido as $identificador) {
			$arrValor = explode( '_', $identificador );
			$tipoAlteracao = strtolower($arrValor[0]) ;
			$identificador = $arrValor[1];
			
			$arrDados = $obAlteracaoDAO->carregarPedidoIdentificador( $this->post['mcrid'], $tipoAlteracao, $identificador );
			
			$obEnvio = $this->getObterPedidoAlteracao( $identificador );			
			//ver($obEnvio,d);
			$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																			'local_cert'	=> $this->certificado, 
																			'passphrase ' 	=> $this->senha_certificado,
																			'exceptions'	=> true,
																	        'trace'			=> true,
																			'encoding'		=> 'ISO-8859-1' )
															);
			
			$retorno = $wsAlteracao->obterPedidoAlteracao( $obEnvio, $arrDados );
			array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
										   'tipoalteracao' => $tipoAlteracao,
										   'codigouo' => $identificador,
										   'retorno' => $retorno
										));
		}
		return $arrRetorno;
	}
	
	public function obterPedidosAlteracao(){		
		$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
		
		$arrPedido = $this->post['pedido_obterpedidos'];
		$arrPedido = $arrPedido ? $arrPedido : array();
		$arrTipoAlteracao = array();
		$arrCodigoUO = array();
		foreach ($arrPedido as $valor) {
			$arrValor = explode( '_', $valor );
			$arrTipoAlteracao[] = strtolower($arrValor[0]) ;
			$arrCodigoUO[] = $arrValor[1];
		}
		
		$arrRetorno = array();
		foreach ($arrCodigoUO as $key => $codigouo) {
			//$arrPedido = $obAlteracaoDAO->carregarPedido( $this->post['mcrid'], $arrTipoAlteracao[$key], $codigouo );
			$arrFisico = $obAlteracaoDAO->carregarFisico( $this->post['mcrid'], $codigouo, $arrTipoAlteracao[$key] );
			//ver($arrFisico,d);
			if( $arrFisico ){
				foreach ($arrFisico as $fisico) {			
					$obEnvio = $this->getObterPedidosAlteracao($fisico);
					//ver($obEnvio,d);
					$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																					'local_cert'	=> $this->certificado, 
																					'passphrase ' 	=> $this->senha_certificado,
																					'exceptions'	=> true,
																			        'trace'			=> true,
																					'encoding'		=> 'ISO-8859-1' )
																	);
					
					$retorno = $wsAlteracao->obterPedidosAlteracao( $obEnvio, $fisico );				
					array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
												   'tipoalteracao' => $arrTipoAlteracao[$key],
												   'codigouo' => $codigouo,
												   'retorno' => $retorno
											));
				}
			} else {
				array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
											   'tipoalteracao' => $arrTipoAlteracao[$key],
											   'codigouo' => $codigouo,
											   'retorno' => 'erro'
										));
			}
		}
		return $arrRetorno;
	}
	
	public function obterPerguntaJustificativa(){		
				
		$pergunta = $this->post['pedido_obterpergunta'];
		$obEnvio = $this->getObterPerguntaJustificativa($pergunta);
		//ver($obEnvio,d);
		$arrRetorno = array();
		$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
		
		$retorno = $wsAlteracao->obterPerguntaJustificativa( $obEnvio, $pergunta );				
		array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
									   'tipoalteracao' => '',
									   'codigouo' => $pergunta,
									   'retorno' => $retorno
								));
				
		return $arrRetorno;
	}
	
	public function obterPerguntasJustificativa(){		
				
		$obEnvio = $this->getObterPerguntasJustificativa();
		//ver($obEnvio,d);
		$arrRetorno = array();
		$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
		//ver($wsAlteracao,d);
		$retorno = $wsAlteracao->obterPerguntasJustificativa( $obEnvio );
		ver($retorno,d);				
		array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
									   'tipoalteracao' => 'null',
									   'codigouo' => 'null',
									   'retorno' => $retorno
								));
				
		return $arrRetorno;
	}
	
	public function obterSaldosAcoesPAC(){		
				
		$obEnvio = $this->getObterSaldosAcoesPAC();
		//ver($obEnvio,d);
		$arrRetorno = array();
		$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
		
		$retorno = $wsAlteracao->obterSaldosAcoesPAC( $obEnvio );
		array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
									   'tipoalteracao' => 'null',
									   'codigouo' => 'null',
									   'retorno' => $retorno
								));
				
		return $arrRetorno;
	}
	
	public function obterAnalisesEmendas(){		
				
		$obEnvio = $this->getObterAnalisesEmendas();
		//ver($obEnvio, d);
		$arrRetorno = array();
		$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
		
		$retorno = $wsAlteracao->obterAnalisesEmendas( $obEnvio );
		ver($retorno,d);
				
		return $arrRetorno;
	}
	
	public function cadastrarAnalisesEmendas(){		
				
		$obEnvio = $this->getCadastrarAnalisesEmendas();
		//ver($obEnvio, d);
		
		$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
		
		$retorno = $wsAlteracao->cadastrarAnalisesEmendas( $obEnvio );
		ver($retorno,d);
				
		return $arrRetorno;
	}
	
	public function obterTabelasApoioAlteracoesOrcamentarias(){		
				
		$obEnvio = $this->getObterTabelasApoioAlteracoesOrcamentarias();
		//ver($obEnvio,d);
		$arrRetorno = array();
		$wsAlteracao = new WSAlteracoesOrcamentarias($this->urlWsdl, array(
																		'local_cert'	=> $this->certificado, 
																		'passphrase ' 	=> $this->senha_certificado,
																		'exceptions'	=> true,
																        'trace'			=> true,
																		'encoding'		=> 'ISO-8859-1' )
														);
		
		$retorno = $wsAlteracao->obterTabelasApoioAlteracoesOrcamentarias( $obEnvio );
		array_push($arrRetorno, array( 'mcrid' => $this->post['mcrid'], 
									   'tipoalteracao' => 'null',
									   'codigouo' => 'null',
									   'retorno' => $retorno
								));
				
		return $arrRetorno;
	}
	
	private function getcadastrarPedidoAlteracao( $mcrid, $pedido = array() ){
		$arrFisicoPedido = array();
		$arrFinanceiro 	 = array();
		$strTotal = 0;
		$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
		
		//foreach ($arrPedido as $pedido) {
			$arrFisico = $obAlteracaoDAO->carregarFisico($mcrid, $pedido['codigouo'], $pedido['codigotipoalteracao'] );
			$strTotal++;
			$obFisico = array();
			$arrRespostaDTO = array();
			foreach ($arrFisico as $chave => $fisico) {
				
				$fisico['codigotipoinclusaolocalizador'] = $obAlteracaoDAO->getTipoInclusaoLocalizador($pedido, $fisico);
				$fisico['codigotipoinclusaoacao'] = $obAlteracaoDAO->getTipoInclusaoAcao($pedido, $fisico);
				
				$arrFinanceiro = $obAlteracaoDAO->carregarFinanceiro($mcrid, $fisico['codigouo'], $fisico['codigotipoalteracao'], $fisico['codigoacao'], $fisico['codigolocalizador']);
				
				$obFinaceiro = array();
				$totalAcrescicmo = 0;
				$totalReducao = 0;				
				foreach ($arrFinanceiro as $key => $financeiro) {
					if( $financeiro['codigotipofonterecurso'] == '2' || $financeiro['codigotipofonterecurso'] == '1' ){
							
							if( $financeiro['codigotipofonterecurso'] == '1' ){
								$financeiro['valorcancelamento'] = $financeiro['valordespesa'];
								$financeiro['valorsuplementacao'] = '0';
								$totalReducao++;
							}else{
								$financeiro['valorcancelamento'] = '0';
								$financeiro['valorsuplementacao'] = $financeiro['valordespesa'];
								$totalAcrescicmo++;
							}				
							$financeiro['codigotipofonterecurso'] = '1';
							
					}
					if( $financeiro['codigotipofonterecurso'] == '3' ){
						$financeiro['codigotipofonterecurso'] = '2';
						$financeiro['valorcancelamento'] = '0';
						$financeiro['valorsuplementacao'] = $financeiro['valordespesa'];
						$totalAcrescicmo++;
					}
					if( $financeiro['codigotipofonterecurso'] == '4' ){
						$financeiro['codigotipofonterecurso'] = '3';
						$financeiro['valorcancelamento'] = '0';
						$financeiro['valorsuplementacao'] = $financeiro['valordespesa'];
						$totalAcrescicmo++;
					}
					$$key = new financeiroPedidoAlteracaoDTO();
					$$key->codigoFonte = $financeiro['codigofonte'];
					$$key->codigoIdOC = $financeiro['codigoidoc'];
					$$key->codigoIdUso = $financeiro['codigoiduso'];
					$$key->codigoNatureza = $financeiro['codigonatureza'];
					$$key->codigoRP = $financeiro['codigorp'];
					$$key->codigoRPLei = $financeiro['codigorp'];
					$$key->codigoTipoFonteRecurso = $financeiro['codigotipofonterecurso'];
					$$key->planoOrcamentario = $financeiro['plocod'];
					$$key->valorCancelamento = str_replace( '-', '', $financeiro['valorcancelamento'] );
					$$key->valorSuplementacao = $financeiro['valorsuplementacao'];
					
					$obFinaceiro[$strChaveFinanceiro.$key] = $$key;
				}
				$fisico['quantidadeacrescimo'] 	= $totalAcrescicmo;
				$fisico['quantidadereducao'] 	= $totalReducao;
				
				$$chave = new fisicoPedidoAlteracaoDTO();
				$$chave->codigoAcao = $fisico['codigoacao'];
				$$chave->codigoEsfera = $fisico['codigoesfera'];
				$$chave->codigoFuncao = $fisico['codigofuncao'];
				$$chave->codigoLocalizador = $fisico['codigolocalizador'];
				$$chave->codigoPrograma = $fisico['codigoprograma'];
				$$chave->codigoSubFuncao = $fisico['codigosubfuncao'];
				$$chave->codigoTipoInclusaoAcao = $fisico['codigotipoinclusaoacao'];
				$$chave->codigoTipoInclusaoLocalizador = $fisico['codigotipoinclusaolocalizador'];
				$$chave->codigoUO = $fisico['codigouo'];
				$$chave->exercicio = $fisico['exercicio'];
				$$chave->listaFinanceiroPedidoAlteracaoDTO = $obFinaceiro;
				$$chave->quantidadeAcrescimo = 0; //$fisico['quantidadeacrescimo'];
				$$chave->quantidadeReducao = 0; //$fisico['quantidadereducao'];
				$obFisico[$strChaveFisico.$chave] = $$chave;
			}
			
			$arrResposta = $obAlteracaoDAO->carregarPerguntaJustificativa( $pedido );
			
			$obrespostaJustificativaDTO = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO->codigoPergunta = '114';
			$obrespostaJustificativaDTO->resposta 		= ($arrResposta['jsccaracterizacao'] ? $arrResposta['jsccaracterizacao'] : '-');			
			$arrRespostaDTO[] = $obrespostaJustificativaDTO;
			
			$obrespostaJustificativaDTO1 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO1->codigoPergunta = '116';
			$obrespostaJustificativaDTO1->resposta 		 = ($arrResposta['jscconsequencia'] ? $arrResposta['jscconsequencia'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO1;
			
			$obrespostaJustificativaDTO2 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO2->codigoPergunta = '115';
			$obrespostaJustificativaDTO2->resposta 		 = ($arrResposta['jscreflexo'] ? $arrResposta['jscreflexo'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO2;
			
			$obrespostaJustificativaDTO3 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO3->codigoPergunta = '117';
			$obrespostaJustificativaDTO3->resposta 		 = ($arrResposta['jscrepercussao'] ? $arrResposta['jscrepercussao'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO3;
			
			$obrespostaJustificativaDTO4 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO4->codigoPergunta = '118';
			$obrespostaJustificativaDTO4->resposta 		 = ($arrResposta['jscmemoria'] ? $arrResposta['jscmemoria'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO4;
			
			$obCadastrarPedidoAlteracao = new cadastrarPedidoAlteracao();
			$obCadastrarPedidoAlteracao->credencial = $this->getCredencial();
			$obCadastrarPedidoAlteracao->pedidoAlteracao = $this->getpedidoAlteracaoDTO($pedido, $obFisico, $arrRespostaDTO);			
		//}
		return $obCadastrarPedidoAlteracao;
	}
	
	private function getpedidoAlteracaoDTO( $arrPedido = array(), $arrFisico = array(), $arrResposta = array() ){
		extract($arrPedido);
		
		$ObjetoWS = new pedidoAlteracaoDTO();
		$ObjetoWS->codigoClassificacaoAlteracao = $codigoclassificacaoalteracao; // int
  		$ObjetoWS->codigoMomento 				= $codigomomento; // int
  		$ObjetoWS->codigoOrgao 					= $codigoorgao; // string
  		$ObjetoWS->codigoTipoAlteracao 			= $codigotipoalteracao; // string
  		$ObjetoWS->descricao 					= $descricao; // string
  		$ObjetoWS->exercicio 					= $exercicio; // int
  		$ObjetoWS->fisicosPedidoAlteracao 		= $arrFisico;
  		$ObjetoWS->respostasJustificativa 		= $arrResposta;
  		$ObjetoWS->snOrcamentoInvestimento 		= 0;
  		
  		return $ObjetoWS;
	}
	
	private function getCredencial(){
		
		$ObjetoWS = new credencialDTO();
		$ObjetoWS->perfil 	= 32;
		$ObjetoWS->senha  	= md5($this->wssenha);
		$ObjetoWS->usuario	= $this->wsusuario;
		
		return $ObjetoWS;
	}
	
	private function getExcluirPedidoAlteracao( $identificadorUnico ){
		
		$ObjetoWS = new excluirPedidoAlteracao();
		$ObjetoWS->credencial 			= $this->getCredencial();
		$ObjetoWS->exercicio 			= $_SESSION['exercicio'];
		$ObjetoWS->identificadorUnico 	= $identificadorUnico;
		
		return $ObjetoWS;
	}
	
	private function getObterPedidoAlteracao( $identificadorUnico ){
		
		$ObjetoWS = new obterPedidoAlteracao();
		$ObjetoWS->codigoMomento			= 9200;
		$ObjetoWS->credencial 				= $this->getCredencial();
		$ObjetoWS->exercicio 				= $_SESSION['exercicio'];
		$ObjetoWS->identificadorUnicoPedido	= $identificadorUnico;
		
		return $ObjetoWS;
	}
	
	private function getObterPedidosAlteracao( $fisico ){
		
		$ObjetoWS = new obterPedidosAlteracao();
		$ObjetoWS->codigoMomento				= 9200;
		$ObjetoWS->credencial 					= $this->getCredencial();
		//$ObjetoWS->dataHoraUltimaConsulta		= $_SESSION['exercicio'];
		$ObjetoWS->exercicio 					= $_SESSION['exercicio'];
		$ObjetoWS->filtroFuncionalProgramatica	= $this->getFiltroFuncionalProgramatica( $fisico );
		
		return $ObjetoWS;
	}
	
	private function getFiltroFuncionalProgramatica( $fisico ){
				
		$ObjetoWS = new filtroFuncionalProgramaticaDTO();
		$ObjetoWS->codigoAcao						= $fisico['codigoacao']; // string
  		$ObjetoWS->codigoEsfera						= $fisico['codigoesfera']; // string
  		$ObjetoWS->codigoFuncao						= $fisico['codigofuncao']; // string
  		$ObjetoWS->codigoLocalizador				= $fisico['codigolocalizador']; // string
  		$ObjetoWS->codigoPrograma					= $fisico['codigoprograma']; // string
  		$ObjetoWS->codigoSubFuncao					= $fisico['codigosubfuncao']; // string
  		//$ObjetoWS->codigoTipoInclusaoAcao			= $fisico['codigotipoinclusaolocalizador']; // int
  		$ObjetoWS->codigoTipoInclusaoLocalizador	= $fisico['codigotipoinclusaolocalizador']; // int
  		$ObjetoWS->codigoUO							= $fisico['codigouo'];
  		
  		return $ObjetoWS;
	}
	
	private function getObterPerguntaJustificativa( $pergunta ){
				
		$ObjetoWS = new obterPerguntaJustificativa();
		$ObjetoWS->codigoPergunta	= $pergunta;
  		$ObjetoWS->credencial		= $this->getCredencial();
  		
  		return $ObjetoWS;
	}
	
	private function getObterPerguntasJustificativa(){
				
		$ObjetoWS = new obterPerguntasJustificativa();
		$ObjetoWS->credencial				= $this->getCredencial();
  		$ObjetoWS->orcamentoInvestimento 	= 1;
  		
  		return $ObjetoWS;
	}
	
	private function getObterSaldosAcoesPAC(){
				
		$ObjetoWS = new obterSaldosAcoesPAC();
		$ObjetoWS->credencial	= $this->getCredencial();
  		$ObjetoWS->exercicio	= $_SESSION['exercicio'];
  		
  		return $ObjetoWS;
	}
	
	private function getObterTabelasApoioAlteracoesOrcamentarias(){
				
		$ObjetoWS = new obterTabelasApoioAlteracoesOrcamentarias();
		$ObjetoWS->credencial						= $this->getCredencial();
  		//$ObjetoWS->dataHoraReferencia				= '';
  		$ObjetoWS->exercicio						= $_SESSION['exercicio'];
  		$ObjetoWS->retornarClassificacoesAlteracao 	= 1;
  		$ObjetoWS->retornarSituacoesPedidoAlteracao	= 1;
  		$ObjetoWS->retornarTiposAlteracao			= 1;
  		$ObjetoWS->retornarTiposFonteRecurso		= 1;
  		$ObjetoWS->retornarTiposInstrumentoLegal	= 1;
  		
  		return $ObjetoWS;
	}
	
	private function getObterAnalisesEmendas(){
				
		$ObjetoWS = new obterAnalisesEmendas();
		$ObjetoWS->CredencialDTO			= $this->getCredencial();
		$ObjetoWS->FiltroAnaliseEmendaDTO	= $this->getFiltroAnaliseEmendaDTO();
  		
  		return $ObjetoWS;
	}
	
	private function getCadastrarAnalisesEmendas(){
				
		$ObjetoWS = new cadastrarAnalisesEmendas();
		$ObjetoWS->CredencialDTO	= $this->getCredencial();
		$ObjetoWS->Integer			= $_SESSION['exercicio'];
		$ObjetoWS->AnaliseEmendaDTO = $this->getAnaliseEmendaDTO();
  		
  		return $ObjetoWS;
	}
	
	private function getFiltroAnaliseEmendaDTO(){
		global $db;
		
		$arrEmendas = $this->carregarDetalheEmenda();
				
		foreach ($arrEmendas as $key => $v) {
			$$key = new filtroAnaliseEmendaDTO();
			$$key->exercicio			= $_SESSION['exercicio'];
			$$key->codigoOrgao			= '12000';
			//$$key->codigosUO			=  $this->getCodigosUO( $v['unicod'] ); /*$db->pegaUm("select ungcod from public.unidadegestora where unicod = '{$v['unicod']}'");*/
			$$key->codigosParlamentar	= $this->getCodigosParlamentar($v['emeid']);
			$$key->codigoMomento		= '1000'; 				#1000Ц Unidade Orзamentбria; 2000 Ц ”rgгo Central; 2500 Ц SRI; 3000 Ц SOF; 5000 ЦFormalizaзгo
			$$key->indicadorImpedimento	= $v['impedimento']; 	#'N' Ц Nгo hб impedimento; 'P' Ц Impedimento Parcial; e 'T' Ц Impedimento Total
			/*$$$key->snAtual				= '';
			$$key->snValidado			= '';*/
			$ObjetoWS[$key] = $$key;
		}
  		return $ObjetoWS;
	}
	
	private function getAnaliseEmendaDTO(){
		global $db;
		
		$arrEmendas = $this->carregarDetalheEmenda();
		
		foreach ($arrEmendas as $key => $v) {
		
			$$key = new analiseEmendaDTO();
			$$key->identificadorUnicoLocalizador 	= '';
			$$key->esfera 					= $v['esfcod']; 		#'10' Ц Fiscal, '20' Ц Seguridade Social e '30' Ц Investimento
			$$key->codigoUO 				= $db->pegaUm("select ungcod from public.unidadegestora where unicod = '{$v['unicod']}'");;
			$$key->codigoPrograma 			= $v['prgcod'];
			$$key->codigoFuncao 			= $v['funcod'];
			$$key->codigoSubFuncao 			= $v['sfucod'];
			$$key->codigoAcao 				= $v['acacod'];
			$$key->codigoLocalizador 		= $v['loccod'];
			$$key->naturezaDespesa 			= '';
			$$key->resultadoPrimario 		= '';
			$$key->fonte 					= $v['foncod'];
			$$key->idUso 					= '';
			$$key->codigoParlamentar 		= $v['autcod'];
			$$key->numeroEmenda 			= $v['emecod'];
			$$key->siglaPartido 			= $v['parsigla'];
			$$key->ufParlamentar 			= $v['estuf'];
			$$key->valorAtual 				= $v['emdvalor'];
			$$key->codigoMomento 			= '1000'; 				#1000 Ц Unidade Orзamentбria; 2000 Ц ”rgгo Central; 2500 Ц SRI; 3000 Ц SOF; 5000 Ц Formalizaзгo
			$$key->indicadorImpedimento 	= $v['impedimento']; 	#'N' Ц Nгo hб impedimento; 'P' Ц Impedimento Parcial; e 'T' Ц Impedimento Total
			$$key->snValidado 				= '';
			$$key->snAtual 					= '';
			$$key->valorImpedimento 		= $v['edivalor'];
			$$key->codigosImpedimento		= $this->getCodigosImpedimento( $v['ediid'] ); // codigosImpedimento
			$$key->justificativa 			= $v['edidescricao'];
			
			$ObjetoWS[$key] = $$key;
		}
		return $ObjetoWS;
	}
	
	private function getCodigosUO( $unicod ){
		global $db;
		
		$arrGestora = $db->carregarColuna("select ungcod from public.unidadegestora where unicod = '$unicod'");
		$arrGestora = $arrGestora ? $arrGestora : array();
		
		/*foreach ($arrGestora as $key => $ungcod) {
			$$key = new codigosUO();
			$$key->codigoUO = $ungcod;
			
			$ObjetoWS[$key] = $$key;
		}*/	
		
		$ObjetoWS = new codigosUO();
		$ObjetoWS->codigoUO = $arrGestora[0];
		
		return $ObjetoWS;
	}
	
	private function getCodigosImpedimento( $ediid ){
		global $db;
		
		$arrImped = $db->carregarColuna("select coalesce(ei.emiid, 0) from emenda.emendaimpositivo_tiposjust eit
												inner join emenda.emendaimpositivo ei on ei.emiid = eit.emiid
											where
												eit.ediid = $ediid");
		$arrImped = $arrImped ? $arrImped : array();
		if( $arrImped ){
			foreach ($arrImped as $key => $emiid) {
				$$key = new codigosImpedimento();
				$$key->codigoImpedimento = $emiid;
				
				$ObjetoWS[$key] = $$key;
			}
			return $ObjetoWS;
		} else {
			$ObjetoWS = new codigosImpedimento();
			$ObjetoWS->codigoImpedimento = '';
			return array($ObjetoWS);
		}
	}
	
	private function getCodigosParlamentar( $emeid ){
		global $db;
		
		$arrAutor = $db->carregarColuna("select autcod from emenda.autor a 
											inner join emenda.emenda e on e.autid = a.autid 
										where e.emeid = '$emeid' and autstatus = 'A'");
		$arrAutor = $arrAutor ? $arrAutor : array();
		
		/*foreach ($arrAutor as $key => $autor) {
			$$key = new codigosParlamentar();
			$$key->codigoParlamentar = $autor;
			
			$ObjetoWS[$key] = $$key;
		}*/
		
		$ObjetoWS = new codigosParlamentar();
		$ObjetoWS->codigoParlamentar = $arrAutor[0];
		
		return $ObjetoWS;
	}
		
	private function carregarDetalheEmenda(){
		global $db;
		
		$arEmdid = implode(', ',  $this->post['chk']);
		
		if( $_SESSION['exercicio'] > 2014 ){
			$filtroImp = ' and edi.edeid = ede.edeid';
			$joinImp = " inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A' ";
		}
		
		$sql = "select 
				    vf.unicod,
				    e.emeid,
				    case when edi.ediimpositivo = 'NH' then 'N'
				        when edi.ediimpositivo = 'PA' then 'P'
				        when edi.ediimpositivo = 'TO' then 'T'
				       else '' end as impedimento,
				    vf.esfcod,
				    vf.prgcod,
				    vf.funcod,
				    vf.sfucod,
				    vf.acacod,
				    vf.loccod,
				    ed.foncod,
				    a.autcod,
				    e.emecod,
				    p.parsigla,
				    a.estuf,
				    ed.emdvalor,
				    edi.edivalor,
				    edi.ediid,
				    edi.edidescricao
				from
				    emenda.emenda e
				    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				    $joinImp
				    inner join emenda.autor a on a.autid = e.autid
				    inner join emenda.partido p on p.parid = a.parid
				    inner join emenda.v_funcionalprogramatica vf on vf.acaid = e.acaid
				    inner join emenda.emendadetalheimpositivo edi on edi.emdid = ed.emdid $filtroImp
				where 
				    e.emeano = '{$_SESSION['exercicio']}'
				    and ed.emdid in (".$arEmdid.")";
		$arrEmendas = $db->carregar($sql);
		$arrEmendas = $arrEmendas ? $arrEmendas : array();
		return $arrEmendas;
	}
}
?>