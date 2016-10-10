<?php
class obterPedidoAlteracao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $identificadorUnicoPedido; // int
  public $codigoMomento; // int
}

class credencialDTO {
  public $perfil; // int
  public $senha; // string
  public $usuario; // string
}

class baseDTO {
}

class obterPedidoAlteracaoResponse {
  public $return; // retornoPedidoAlteracaoDTO
}

class retornoPedidoAlteracaoDTO {
  public $registros; // pedidoAlteracaoDTO
}

class retornoDTO {
  public $mensagensErro; // string
  public $sucesso; // boolean
}

class pedidoAlteracaoDTO {
  public $codigoClassificacaoAlteracao; // int
  //public $codigoInstrumentoLegal; // int
  public $codigoMomento; // int
  public $codigoOrgao; // string
  //public $codigoSituacaoPedidoAlteracao; // int
  public $codigoTipoAlteracao; // string
  //public $dataCriacao; // dateTime
  //public $dataEfetivacao; // dateTime
  //public $dataEnvio; // dateTime
  public $descricao; // string
  public $exercicio; // int
  public $fisicosPedidoAlteracao; // fisicoPedidoAlteracaoDTO
  //public $identificadorUnico; // int
  /*public $identificadorUnicoPedidoAgregador; // int
  public $identificadorUnicoPedidoOrigem; // int
  public $loginUsuarioCriacao; // string
  public $loginUsuarioEfetivacao; // string
  public $loginUsuarioEnvio; // string
  public $nomeUsuarioCriacao; // string
  public $nomeUsuarioEfetivacao; // string
  public $nomeUsuarioEnvio; // string*/
  public $respostasJustificativa; // respostaJustificativaDTO
  /*public $snAgregadora; // boolean
  public $snAtual; // boolean
  public $snEmValidacaoExterna; // boolean
  public $snEnviadoCongressoNacional; // boolean
  public $snExclusaoLogica; // boolean 
  public $snIntegracao; // boolean*/
  //public $snOrcamentoInvestimento; // boolean
  
  public function __construct($arrPedido = array(), $arrFisico = array(), $arrResposta = array() ){
  	extract($arrPedido);
  	$this->codigoClassificacaoAlteracao 		= $codigoclassificacaoalteracao; //1
	//$this->codigoInstrumentoLegal 				= $codigoinstrumentolegal;
	$this->codigoMomento 						= $codigomomento; //9200
	$this->codigoOrgao 							= $codigoorgao; //42000 CODIGO_ORGAO_SISTEMA
	//$this->codigoSituacaoPedidoAlteracao 		= $codigosituacaopedidoalteracao;
	$this->codigoTipoAlteracao 					= $codigotipoalteracao; //tipocredito
	//$this->dataCriacao 							= $datacriacao;
	//$this->dataEfetivacao 						= $dataefetivacao;
	//$this->dataEnvio 							= $dataenvio;
	$this->descricao 							= $descricao; //'Integração - 01'
	$this->exercicio 							= $exercicio; //2012
	$this->fisicosPedidoAlteracao 				= $arrFisico;
	//$this->identificadorUnico 					= $identificadorunico;
	//$this->identificadorUnicoPedidoAgregador 	= $identificadorunicopedidoagregador;
	//$this->identificadorUnicoPedidoOrigem 		= $identificadorunicopedidoorigem;
	//$this->loginUsuarioCriacao 					= $loginusuariocriacao;
	//$this->loginUsuarioEfetivacao 				= $loginusuarioefetivacao;
	//$this->loginUsuarioEnvio 					= $loginusuarioenvio;
	//$this->nomeUsuarioCriacao 					= $nomeusuariocriacao;
	//$this->nomeUsuarioEfetivacao 				= $nomeusuarioefetivacao;
	//$this->nomeUsuarioEnvio 					= $nomeusuarioenvio;
	$this->respostasJustificativa 				= $arrResposta;
	//$this->snAgregadora 						= $snagregadora;
	//$this->snAtual 								= $snatual;
	//$this->snEmValidacaoExterna 				= $snemvalidacaoexterna;
	//$this->snEnviadoCongressoNacional 			= $snenviadocongressonacional;
	//$this->snExclusaoLogica 					= $snexclusaologica;
	//$this->snIntegracao 						= $snintegracao;
	$this->snOrcamentoInvestimento 				= '';
  }
}

class fisicoPedidoAlteracaoDTO {
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFuncao; // string
  public $codigoLocalizador; // string
  public $codigoPrograma; // string
  public $codigoSubFuncao; // string
  public $codigoTipoInclusaoLocalizador; // int
  public $codigoUO; // string
  public $exercicio; // int
  public $listaFinanceiroPedidoAlteracaoDTO; // financeiroPedidoAlteracaoDTO
  public $quantidadeAcrescimo; // long
  public $quantidadeReducao; // long
  
  /*public function __construct($arrFisico = array(), $arrFinanceiro = array()){
  	extract($arrFisico);
  	$this->codigoAcao 							= $codigoacao;
	$this->codigoEsfera 						= $codigoesfera;
	$this->codigoFuncao 						= $codigofuncao;
	$this->codigoLocalizador 					= $codigolocalizador;
	$this->codigoPrograma 						= $codigoprograma;
	$this->codigoSubFuncao 						= $codigosubfuncao;
	$this->codigoTipoInclusaoLocalizador 		= $codigotipoinclusaolocalizador; //4  #1 - PLOA, 2 - Emenda, 4 - PPA, 5 - Crédito Especial, 6 - Crédito Extraordinário
	$this->codigoUO 							= $codigouo;
	$this->exercicio 							= $exercicio; //2012;
	$this->listaFinanceiroPedidoAlteracaoDTO 	= $arrFinanceiro;
	$this->quantidadeAcrescimo 					= $quantidadeacrescimo; //0
	$this->quantidadeReducao 					= $quantidadereducao; //0
  }*/
}

class financeiroPedidoAlteracaoDTO {
  public $codigoFonte; // string
  public $codigoIdOC; // string
  public $codigoIdUso; // string
  public $codigoNatureza; // string
  public $codigoRP; // string
  public $codigoRPLei; // string
  public $codigoTipoFonteRecurso; // int
  public $valorCancelamento; // long
  public $valorSuplementacao; // long
  
  /*public function __construct($arrFinanceiro = array()){
  	extract($arrFinanceiro);
  	$codigorplei = $codigorp;
  	
  	$this->codigoFonte 				= $codigofonte;
	$this->codigoIdOC 				= $codigoidoc;
	$this->codigoIdUso 				= $codigoiduso;
	$this->codigoNatureza 			= $codigonatureza;
	$this->codigoRP 				= $codigorp;
	$this->codigoRPLei 				= $codigorplei;
	$this->codigoTipoFonteRecurso 	= $codigotipofonterecurso; //defaut 3
	$this->valorCancelamento 		= $valorcancelamento;
	$this->valorSuplementacao 		= $valorsuplementacao;
  }*/
}

class respostaJustificativaDTO {
  public $codigoPergunta; // int
  public $resposta; // string
  
  /*public function __construct($arrResposta = array()){
  	ver($arrResposta,d);
  	foreach ($arrResposta as $resposta) {
  		
  	}
  	
  	$this->codigoPergunta = $codigopergunta;
  	$this->resposta = $resposta;
  }*/
}

class obterPedidosAlteracao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $FiltroFuncionalProgramaticaDTO; // filtroFuncionalProgramaticaDTO
  public $dataHoraUltimaConsulta; // dateTime
}

class filtroFuncionalProgramaticaDTO {
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFuncao; // string
  public $codigoLocalizador; // string
  public $codigoPrograma; // string
  public $codigoSubFuncao; // string
  public $codigoTipoInclusaoAcao; // int
  public $codigoTipoInclusaoLocalizador; // int
  public $codigoUO; // string
}

class obterPedidosAlteracaoResponse {
  public $return; // retornoPedidoAlteracaoDTO
}

class cadastrarPedidoAlteracao {
  public $credencial; // credencialDTO
  public $pedidoAlteracaoDTO; // pedidoAlteracaoDTO
}

class cadastrarPedidoAlteracaoResponse {
  public $return; // retornoPedidoAlteracaoDTO
}

class cadastrarPedidoPAC {
  public $credencial; // credencialDTO
  public $pedidoAlteracaoDTO; // pedidoAlteracaoDTO
}

class cadastrarPedidoPACResponse {
  public $return; // retornoPedidoPACDTO
}

class retornoPedidoPACDTO {
  public $identificadorUnico; // int
  public $saldoBloqueioDotacao; // saldoBloqueioDotacaoDTO
}

class saldoBloqueioDotacaoDTO {
  public $bloqueioAtual; // decimal
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFonte; // string
  public $codigoFuncao; // string
  public $codigoIdOC; // string
  public $codigoIdUso; // string
  public $codigoLocalizador; // string
  public $codigoNatureza; // string
  public $codigoPrograma; // string
  public $codigoRP; // string
  public $codigoRPLei; // string
  public $codigoSubFuncao; // string
  public $codigoTipoInclusaoAcao; // int
  public $codigoTipoInclusaoLocalizador; // int
  public $codigoUnidadeOrcamentaria; // string
  public $dotacaoAtual; // decimal
  public $exercicio; // int
}

class excluirPedidoAlteracao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $identificadorUnico; // int
}

class excluirPedidoAlteracaoResponse {
  public $return; // retornoPedidoAlteracaoDTO
}

class obterPerguntaJustificativa {
  public $credencial; // credencialDTO
  public $codigoPergunta; // int
}

class obterPerguntaJustificativaResponse {
  public $return; // retornoPerguntaJustificativaDTO
}

class retornoPerguntaJustificativaDTO {
  public $registros; // perguntaJustificativaDTO
}

class perguntaJustificativaDTO {
  public $codigoPergunta; // int
  public $pergunta; // string
}

class obterPerguntasJustificativa {
  public $credencial; // credencialDTO
  public $orcamentoInvestimento; // boolean
}

class obterPerguntasJustificativaResponse {
  public $return; // retornoPerguntaJustificativaDTO
}

class obterTabelasApoioAlteracoesOrcamentarias {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $retornarClassificacoesAlteracao; // boolean
  public $retornarTiposAlteracao; // boolean
  public $retornarSituacoesPedidoAlteracao; // boolean
  public $retornarTiposInstrumentoLegal; // boolean
  public $retornarTiposFonteRecurso; // boolean
  public $dataHoraReferencia; // dateTime
}

class obterTabelasApoioAlteracoesOrcamentariasResponse {
  public $return; // retornoApoioAlteracoesOrcamentariasDTO
}

class retornoApoioAlteracoesOrcamentariasDTO {
  public $classificacoesAlteracaoDTO; // classificaoAlteracaoDTO
  public $situacoesPedidoAlteracaoDTO; // situacaoPedidoAlteracaoDTO
  public $tiposAlteracaoDTO; // tipoAlteracaoDTO
  public $tiposFonteRecursoDTO; // tipoFonteRecursoDTO
  public $tiposInstrumentoLegalDTO; // tipoInstrumentoLegalDTO
}

class classificaoAlteracaoDTO {
  public $codigoClassificacaoAlteracao; // int
  public $descricao; // string
  public $snAtivo; // boolean
  public $snTipoCredito; // boolean
}

class situacaoPedidoAlteracaoDTO {
  public $codigoSituacaoPedidoAlteracao; // int
  public $descricao; // string
  public $snAtivo; // boolean
}

class tipoAlteracaoDTO {
  public $baseLegal; // string
  public $codigoClassificacaoAlteracao; // int
  public $codigoTipoAlteracao; // string
  public $descricao; // string
  public $exercicio; // int
  public $snOrcamentoInvestimento; // boolean
}

class tipoFonteRecursoDTO {
  public $codigoTipoFonteRecurso; // int
  public $descricao; // string
}

class tipoInstrumentoLegalDTO {
  public $codigoTipoInstrumentoLegal; // int
  public $descricao; // string
  public $snAtivo; // boolean
}

class consultarSituacaoTransmissaoSiafi {
  public $credencial; // credencialDTO
  public $identificadorPedidoAlteracao; // int
}

class consultarSituacaoTransmissaoSiafiResponse {
  public $return; // retornoSituacaoTransmissaoSiafiDTO
}

class retornoSituacaoTransmissaoSiafiDTO {
  public $situacaoTransmissaoSiafi; // string
}

class obterSaldosAcoesPAC {
  public $credencial; // credencialDTO
  public $exercicio; // int
}

class obterSaldosAcoesPACResponse {
  public $return; // retornoSaldosBloqueioPAC
}

class retornoSaldosBloqueioPAC {
  public $saldoBloqueioDotacao; // saldoBloqueioDotacaoDTO
}


/**
 * WSAlteracoesOrcamentarias class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class WSAlteracoesOrcamentarias extends SoapClient {

  private static $classmap = array(
                                    'obterPedidoAlteracao' => 'obterPedidoAlteracao',
                                    'credencialDTO' => 'credencialDTO',
                                    'baseDTO' => 'baseDTO',
                                    'obterPedidoAlteracaoResponse' => 'obterPedidoAlteracaoResponse',
                                    'retornoPedidoAlteracaoDTO' => 'retornoPedidoAlteracaoDTO',
                                    'retornoDTO' => 'retornoDTO',
                                    'pedidoAlteracaoDTO' => 'pedidoAlteracaoDTO',
                                    'fisicoPedidoAlteracaoDTO' => 'fisicoPedidoAlteracaoDTO',
                                    'financeiroPedidoAlteracaoDTO' => 'financeiroPedidoAlteracaoDTO',
                                    'respostaJustificativaDTO' => 'respostaJustificativaDTO',
                                    'obterPedidosAlteracao' => 'obterPedidosAlteracao',
                                    'filtroFuncionalProgramaticaDTO' => 'filtroFuncionalProgramaticaDTO',
                                    'obterPedidosAlteracaoResponse' => 'obterPedidosAlteracaoResponse',
                                    'cadastrarPedidoAlteracao' => 'cadastrarPedidoAlteracao',
                                    'cadastrarPedidoAlteracaoResponse' => 'cadastrarPedidoAlteracaoResponse',
                                    'cadastrarPedidoPAC' => 'cadastrarPedidoPAC',
                                    'cadastrarPedidoPACResponse' => 'cadastrarPedidoPACResponse',
                                    'retornoPedidoPACDTO' => 'retornoPedidoPACDTO',
                                    'saldoBloqueioDotacaoDTO' => 'saldoBloqueioDotacaoDTO',
                                    'excluirPedidoAlteracao' => 'excluirPedidoAlteracao',
                                    'excluirPedidoAlteracaoResponse' => 'excluirPedidoAlteracaoResponse',
                                    'obterPerguntaJustificativa' => 'obterPerguntaJustificativa',
                                    'obterPerguntaJustificativaResponse' => 'obterPerguntaJustificativaResponse',
                                    'retornoPerguntaJustificativaDTO' => 'retornoPerguntaJustificativaDTO',
                                    'perguntaJustificativaDTO' => 'perguntaJustificativaDTO',
                                    'obterPerguntasJustificativa' => 'obterPerguntasJustificativa',
                                    'obterPerguntasJustificativaResponse' => 'obterPerguntasJustificativaResponse',
                                    'obterTabelasApoioAlteracoesOrcamentarias' => 'obterTabelasApoioAlteracoesOrcamentarias',
                                    'obterTabelasApoioAlteracoesOrcamentariasResponse' => 'obterTabelasApoioAlteracoesOrcamentariasResponse',
                                    'retornoApoioAlteracoesOrcamentariasDTO' => 'retornoApoioAlteracoesOrcamentariasDTO',
                                    'classificaoAlteracaoDTO' => 'classificaoAlteracaoDTO',
                                    'situacaoPedidoAlteracaoDTO' => 'situacaoPedidoAlteracaoDTO',
                                    'tipoAlteracaoDTO' => 'tipoAlteracaoDTO',
                                    'tipoFonteRecursoDTO' => 'tipoFonteRecursoDTO',
                                    'tipoInstrumentoLegalDTO' => 'tipoInstrumentoLegalDTO',
                                    'consultarSituacaoTransmissaoSiafi' => 'consultarSituacaoTransmissaoSiafi',
                                    'consultarSituacaoTransmissaoSiafiResponse' => 'consultarSituacaoTransmissaoSiafiResponse',
                                    'retornoSituacaoTransmissaoSiafiDTO' => 'retornoSituacaoTransmissaoSiafiDTO',
                                    'obterSaldosAcoesPAC' => 'obterSaldosAcoesPAC',
                                    'obterSaldosAcoesPACResponse' => 'obterSaldosAcoesPACResponse',
                                    'retornoSaldosBloqueioPAC' => 'retornoSaldosBloqueioPAC',
                                   );

  public function WSAlteracoesOrcamentarias($wsdl = "https://homologacao.siop.planejamento.gov.br/services/WSAlteracoesOrcamentarias?wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   *  
   *
   * @param cadastrarPedidoAlteracao $cadastrarPedidoAlteracao
   * @return cadastrarPedidoAlteracaoResponse
   */
  public function cadastrarPedidoAlteracao(cadastrarPedidoAlteracao $cadastrarPedidoAlteracao) {
    return $this->__soapCall('cadastrarPedidoAlteracao', array($cadastrarPedidoAlteracao),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param cadastrarPedidoPAC $cadastrarPedidoPAC
   * @return cadastrarPedidoPACResponse
   */
  public function cadastrarPedidoPAC(cadastrarPedidoPAC $cadastrarPedidoPAC) {
    return $this->__soapCall('cadastrarPedidoPAC', array($cadastrarPedidoPAC),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarSituacaoTransmissaoSiafi $consultarSituacaoTransmissaoSiafi
   * @return consultarSituacaoTransmissaoSiafiResponse
   */
  public function consultarSituacaoTransmissaoSiafi(consultarSituacaoTransmissaoSiafi $consultarSituacaoTransmissaoSiafi) {
    return $this->__soapCall('consultarSituacaoTransmissaoSiafi', array($consultarSituacaoTransmissaoSiafi),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param excluirPedidoAlteracao $excluirPedidoAlteracao
   * @return excluirPedidoAlteracaoResponse
   */
  public function excluirPedidoAlteracao(excluirPedidoAlteracao $excluirPedidoAlteracao) {
    return $this->__soapCall('excluirPedidoAlteracao', array($excluirPedidoAlteracao),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterPedidoAlteracao $obterPedidoAlteracao
   * @return obterPedidoAlteracaoResponse
   */
  public function obterPedidoAlteracao(obterPedidoAlteracao $obterPedidoAlteracao) {
    return $this->__soapCall('obterPedidoAlteracao', array($obterPedidoAlteracao),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterPedidosAlteracao $obterPedidosAlteracao
   * @return obterPedidosAlteracaoResponse
   */
  public function obterPedidosAlteracao(obterPedidosAlteracao $obterPedidosAlteracao) {
    return $this->__soapCall('obterPedidosAlteracao', array($obterPedidosAlteracao),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterPerguntaJustificativa $obterPerguntaJustificativa
   * @return obterPerguntaJustificativaResponse
   */
  public function obterPerguntaJustificativa(obterPerguntaJustificativa $obterPerguntaJustificativa) {
    return $this->__soapCall('obterPerguntaJustificativa', array($obterPerguntaJustificativa),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterPerguntasJustificativa $obterPerguntasJustificativa
   * @return obterPerguntasJustificativaResponse
   */
  public function obterPerguntasJustificativa(obterPerguntasJustificativa $obterPerguntasJustificativa) {
    return $this->__soapCall('obterPerguntasJustificativa', array($obterPerguntasJustificativa),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterSaldosAcoesPAC $obterSaldosAcoesPAC
   * @return obterSaldosAcoesPACResponse
   */
  public function obterSaldosAcoesPAC(obterSaldosAcoesPAC $obterSaldosAcoesPAC) {
    return $this->__soapCall('obterSaldosAcoesPAC', array($obterSaldosAcoesPAC),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterTabelasApoioAlteracoesOrcamentarias $obterTabelasApoioAlteracoesOrcamentarias
   * @return obterTabelasApoioAlteracoesOrcamentariasResponse
   */
  public function obterTabelasApoioAlteracoesOrcamentarias(obterTabelasApoioAlteracoesOrcamentarias $obterTabelasApoioAlteracoesOrcamentarias) {
    return $this->__soapCall('obterTabelasApoioAlteracoesOrcamentarias', array($obterTabelasApoioAlteracoesOrcamentarias),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

}

?>
