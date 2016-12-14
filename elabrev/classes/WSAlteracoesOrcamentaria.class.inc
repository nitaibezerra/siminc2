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
  public $snOrcamentoInvestimento; // boolean
}

class fisicoPedidoAlteracaoDTO {
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFuncao; // string
  public $codigoLocalizador; // string
  public $codigoPrograma; // string
  public $codigoSubFuncao; // string
  public $codigoTipoInclusaoAcao; // int
  public $codigoTipoInclusaoLocalizador; // int
  public $codigoUO; // string
  public $exercicio; // int
  public $listaFinanceiroPedidoAlteracaoDTO; // financeiroPedidoAlteracaoDTO
  public $quantidadeAcrescimo; // long
  public $quantidadeReducao; // long
}

class financeiroPedidoAlteracaoDTO {
  public $codigoFonte; // string
  public $codigoIdOC; // string
  public $codigoIdUso; // string
  public $codigoNatureza; // string
  public $codigoRP; // string
  public $codigorplei; // string
  public $codigoTipoFonteRecurso; // int
  public $planoOrcamentario; //string
  public $valorCancelamento; // long
  public $valorsuplementacao; // long
}

class respostaJustificativaDTO {
  public $codigoPergunta; // int
  public $resposta; // string
}

class obterPedidosAlteracao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $filtroFuncionalProgramatica; // filtroFuncionalProgramaticaDTO
  public $dataHoraUltimaConsulta; // dateTime
}

class filtroFuncionalProgramaticaDTO {
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFuncao; // string
  public $codigoLocalizador; // string
  public $codigoPrograma; // string
  public $codigoSubFuncao; // string
//  public $codigoTipoInclusaoAcao; // int
  public $codigoTipoInclusaoLocalizador; // int
  public $codigoUO; // string
  public $exercicio; // int
}

class obterPedidosAlteracaoResponse {
  public $return; // retornoPedidoAlteracaoDTO
}

class cadastrarPedidoAlteracao {
  public $credencial; // credencialDTO
  public $pedidoAlteracao; // pedidoAlteracaoDTO
}

class cadastrarPedidoAlteracaoResponse {
  public $return; // retornoPedidoAlteracaoDTO

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
  public $classificacoesAlteracaoDTO; // classificacaoAlteracaoDTO
  public $situacoesPedidoAlteracaoDTO; // situacaoPedidoAlteracaoDTO
  public $tiposAlteracaoDTO; // tipoAlteracaoDTO
  public $tiposFonteRecursoDTO; // tipoFonteRecursoDTO
  public $tiposInstrumentoLegalDTO; // tipoInstrumentoLegalDTO
}

class classificacaoAlteracaoDTO {
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

class saldoBloqueioDotacaoDTO {
  public $anoExercicio; // int
  public $anoReferencia; // int
  public $bloqueioAtual; // decimal
  public $categoriaEconomica; // string
  public $celula; // string
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFonte; // string
  public $codigoFuncao; // string
  public $codigoIdOC; // string
  public $codigoIdUso; // string
  public $codigoLocalizador; // string
  public $codigoPrograma; // string
  public $codigoRP; // string
  public $codigoRPLei; // string
  public $codigoSubFuncao; // string
  public $codigoUnidadeOrcamentaria; // string
  public $dataGeracao; // dateTime
  public $dotacaoAtual; // decimal
  public $grupoNaturezaDespesa; // string
  public $indicadorFuncionalPac; // boolean
  public $indicadorRap; // boolean
  public $modalidadeDeAplicacao; // string
  public $planoOrcamentario; // string
  public $saldo; // decimal
  public $tipoCredito; // string
}

class obterAnalisesEmendas {
  public $CredencialDTO; // credencialDTO
  public $FiltroAnaliseEmendaDTO; // filtroAnaliseEmendaDTO
}

class cadastrarAnalisesEmendas {
  public $CredencialDTO; // credencialDTO
  public $Integer; // int
  public $AnaliseEmendaDTO; // analiseEmendaDTO
}

class analiseEmendaDTO {
  public $identificadorUnicoLocalizador; // int
  public $esfera; // string
  public $codigoUO; // string
  public $codigoPrograma; // string
  public $codigoFuncao; // string
  public $codigoSubFuncao; // string
  public $codigoAcao; // string
  public $codigoLocalizador; // string
  public $naturezaDespesa; // string
  public $resultadoPrimario; // string
  public $fonte; // string
  public $idUso; // string
  public $codigoParlamentar; // int
  public $numeroEmenda; // int
  public $siglaPartido; // string
  public $ufParlamentar; // string
  public $valorAtual; // long
  public $codigoMomento; // int
  public $indicadorImpedimento; // string
  public $snValidado; // boolean
  public $snAtual; // boolean
  public $valorImpedimento; // long
  public $codigosImpedimento; // codigosImpedimento
  public $justificativa; // string
}

class filtroAnaliseEmendaDTO {
  public $exercicio; // int
  public $codigoOrgao; // string
  public $codigosUO; // codigosUO
  public $codigosParlamentar; // codigosParlamentar
  public $codigoMomento; // int
  public $indicadorImpedimento; // string
  public $snAtual; // boolean
  public $snValidado; // boolean
}

class consultarSituacaoTransmissaoSiafi {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $identificadorUnico; // int
}

class consultarSituacaoTransmissaoSiafiResponse {
  public $return; // retornoSituacaoTransmissaoSiafiDTO
}

class retornoSituacaoTransmissaoSiafiDTO {
  public $codigoSituacao; // int
  public $descricaoSituacao; // string
  public $ESB; // string
}

class codigosUO {
  public $codigoUO; // string
}

class codigosParlamentar {
  public $codigoParlamentar; // string
}

class obterAnalisesEmendasResponse {
  public $return; // retornoAnaliseEmendaDTO
}

class retornoAnaliseEmendaDTO {
  public $analisesEmenda; // analisesEmenda
}

class analisesEmenda {
  public $analiseEmenda; // analiseEmendaDTO
}

class codigosImpedimento {
  public $codigoImpedimento; // int
}

class cadastrarAnalisesEmendasResponse {
  public $return; // retornoCadastrarAnaliseEmendaDTO
}

class retornoCadastrarAnaliseEmendaDTO {
  public $analiseEmendaDTO; // analiseEmendaDTO
  public $pendencias; // pendencias
}

class pendencias {
  public $pendencia; // string
}

class verificarPedidoAlteracao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $identificadorUnico; // int
}

class verificarPedidoAlteracaoResponse {
  public $return; // retornoVerificacaoPedidoAlteracaoDTO
}

class retornoVerificacaoPedidoAlteracaoDTO {
  public $verificacoes; // verificacoes
}

class verificacoes {
  public $verificacao; // verificacaoPedidoAlteracaoDTO
}

class verificacaoPedidoAlteracaoDTO {
  public $regra; // string
  public $passou; // boolean
  public $snInformativa; // boolean
  public $snConfirmacaoEnvio; // boolean
  public $detalhes; // detalhes
}

class detalhes {
  public $detalhe; // string
}

class obterPedidosAlteracaoPorDescricao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $descricao; // string
}

class obterPedidosAlteracaoPorDescricaoResponse {
  public $return; // retornoPedidoAlteracaoDTO
}

class cadastrarPedidoPAC {
  public $credencial; // credencialDTO
  public $pedidoAlteracao; // pedidoAlteracaoDTO
}

class cadastrarPedidoPACResponse {
  public $return; // retornoPedidoPACDTO
}

class retornoPedidoPACDTO {
  public $ESB; // string
  public $fitaGerada; // boolean
  public $identificadorUnico; // int
  public $pedidoEfetivado; // boolean
  public $pedidoSalvo; // boolean
}

class enviarPedidoAlteracao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $identificadorUnico; // int
}

class enviarPedidoAlteracaoResponse {
  public $return; // retornoVerificacaoPedidoAlteracaoDTO
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
  protected $soapClient;
  private $tipolog;
  private $arrPedido;
  private $logid;
  private $dbws;

  private static $classmap = array(
                                    'obterPerguntaJustificativa' => 'obterPerguntaJustificativa',
                                    'credencialDTO' => 'credencialDTO',
                                    'baseDTO' => 'baseDTO',
                                    'obterPerguntaJustificativaResponse' => 'obterPerguntaJustificativaResponse',
                                    'retornoPerguntaJustificativaDTO' => 'retornoPerguntaJustificativaDTO',
                                    'retornoDTO' => 'retornoDTO',
                                    'perguntaJustificativaDTO' => 'perguntaJustificativaDTO',
                                    'obterPerguntasJustificativa' => 'obterPerguntasJustificativa',
                                    'obterPerguntasJustificativaResponse' => 'obterPerguntasJustificativaResponse',
                                    'obterTabelasApoioAlteracoesOrcamentarias' => 'obterTabelasApoioAlteracoesOrcamentarias',
                                    'obterTabelasApoioAlteracoesOrcamentariasResponse' => 'obterTabelasApoioAlteracoesOrcamentariasResponse',
                                    'retornoApoioAlteracoesOrcamentariasDTO' => 'retornoApoioAlteracoesOrcamentariasDTO',
                                    'classificacaoAlteracaoDTO' => 'classificacaoAlteracaoDTO',
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
                                    'saldoBloqueioDotacaoDTO' => 'saldoBloqueioDotacaoDTO',
                                    'obterAnalisesEmendas' => 'obterAnalisesEmendas',
                                    'filtroAnaliseEmendaDTO' => 'filtroAnaliseEmendaDTO',
                                    'codigosUO' => 'codigosUO',
                                    'codigosParlamentar' => 'codigosParlamentar',
                                    'obterAnalisesEmendasResponse' => 'obterAnalisesEmendasResponse',
                                    'retornoAnaliseEmendaDTO' => 'retornoAnaliseEmendaDTO',
                                    'analisesEmenda' => 'analisesEmenda',
                                    'analiseEmendaDTO' => 'analiseEmendaDTO',
                                    'codigosImpedimento' => 'codigosImpedimento',
                                    'cadastrarAnalisesEmendas' => 'cadastrarAnalisesEmendas',
                                    'cadastrarAnalisesEmendasResponse' => 'cadastrarAnalisesEmendasResponse',
                                    'retornoCadastrarAnaliseEmendaDTO' => 'retornoCadastrarAnaliseEmendaDTO',
                                    'pendencias' => 'pendencias',
                                    'verificarPedidoAlteracao' => 'verificarPedidoAlteracao',
                                    'verificarPedidoAlteracaoResponse' => 'verificarPedidoAlteracaoResponse',
                                    'retornoVerificacaoPedidoAlteracaoDTO' => 'retornoVerificacaoPedidoAlteracaoDTO',
                                    'verificacoes' => 'verificacoes',
                                    'verificacaoPedidoAlteracaoDTO' => 'verificacaoPedidoAlteracaoDTO',
                                    'detalhes' => 'detalhes',
                                    'obterPedidosAlteracao' => 'obterPedidosAlteracao',
                                    'filtroFuncionalProgramaticaDTO' => 'filtroFuncionalProgramaticaDTO',
                                    'obterPedidosAlteracaoResponse' => 'obterPedidosAlteracaoResponse',
                                    'retornoPedidoAlteracaoDTO' => 'retornoPedidoAlteracaoDTO',
                                    'pedidoAlteracaoDTO' => 'pedidoAlteracaoDTO',
                                    'fisicoPedidoAlteracaoDTO' => 'fisicoPedidoAlteracaoDTO',
                                    'financeiroPedidoAlteracaoDTO' => 'financeiroPedidoAlteracaoDTO',
                                    'respostaJustificativaDTO' => 'respostaJustificativaDTO',
                                    'obterPedidosAlteracaoPorDescricao' => 'obterPedidosAlteracaoPorDescricao',
                                    'obterPedidosAlteracaoPorDescricaoResponse' => 'obterPedidosAlteracaoPorDescricaoResponse',
                                    'cadastrarPedidoAlteracao' => 'cadastrarPedidoAlteracao',
                                    'cadastrarPedidoAlteracaoResponse' => 'cadastrarPedidoAlteracaoResponse',
                                    'cadastrarPedidoPAC' => 'cadastrarPedidoPAC',
                                    'cadastrarPedidoPACResponse' => 'cadastrarPedidoPACResponse',
                                    'retornoPedidoPACDTO' => 'retornoPedidoPACDTO',
                                    'obterPedidoAlteracao' => 'obterPedidoAlteracao',
                                    'obterPedidoAlteracaoResponse' => 'obterPedidoAlteracaoResponse',
                                    'excluirPedidoAlteracao' => 'excluirPedidoAlteracao',
                                    'excluirPedidoAlteracaoResponse' => 'excluirPedidoAlteracaoResponse',
                                    'enviarPedidoAlteracao' => 'enviarPedidoAlteracao',
                                    'enviarPedidoAlteracaoResponse' => 'enviarPedidoAlteracaoResponse',
                                   );

  public function WSAlteracoesOrcamentarias($wsdl = NULL, $options = array()) {
    $wsdl = $wsdl?$wsdl: WEB_SERVICE_SIOP_URL. "WSAlteracoesOrcamentarias?wsdl";
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    
    $this->tipolog = '';
    $this->arrPedido = array();
    $this->logid = '';
    $this->soapClient = $client;
    
    parent::__construct($wsdl, $options);
  }

  /**
   *  
   *
   * @param cadastrarPedidoAlteracao $cadastrarPedidoAlteracao
   * @return cadastrarPedidoAlteracaoResponse
   */
  public function cadastrarPedidoAlteracao(cadastrarPedidoAlteracao $cadastrarPedidoAlteracao, $arrPedido) {
    $this->arrPedido = $arrPedido;
  	$this->tipolog = 'cadastrarPedidoAlteracao';
    return $this->__soapCall('cadastrarPedidoAlteracao', array($cadastrarPedidoAlteracao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
            'uri' => WEB_SERVICE_SIOP_URL,
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
            'uri' => WEB_SERVICE_SIOP_URL,
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
  public function obterPedidoAlteracao(obterPedidoAlteracao $obterPedidoAlteracao, $arrPedido) {
  	$this->arrPedido = $arrPedido;
  	$this->tipolog = 'obterPedidoAlteracao';
  	
    return $this->__soapCall('obterPedidoAlteracao', array($obterPedidoAlteracao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
  public function obterPedidosAlteracao(obterPedidosAlteracao $obterPedidosAlteracao, $arrPedido) {
  	$this->arrPedido = $arrPedido;
  	$this->tipolog = 'obterPedidosAlteracao';
  	
    return $this->__soapCall('obterPedidosAlteracao', array($obterPedidosAlteracao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
  public function obterPerguntaJustificativa(obterPerguntaJustificativa $obterPerguntaJustificativa, $pergunta) {
  	$this->arrPedido = $pergunta;
  	$this->tipolog = 'obterPerguntaJustificativa';
  	
    return $this->__soapCall('obterPerguntaJustificativa', array($obterPerguntaJustificativa),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
  	$this->tipolog = 'naograva';
  	
    return $this->__soapCall('obterPerguntasJustificativa', array($obterPerguntasJustificativa),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
  	$this->tipolog = 'naograva';
  	
    return $this->__soapCall('obterSaldosAcoesPAC', array($obterSaldosAcoesPAC),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
  	$this->tipolog = 'naograva';
  	
    return $this->__soapCall('obterTabelasApoioAlteracoesOrcamentarias', array($obterTabelasApoioAlteracoesOrcamentarias),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }
  
  /**
   *  
   *
   * @param obterAnalisesEmendas $obterAnalisesEmendas
   * @return obterAnalisesEmendasResponse
   */
  public function obterAnalisesEmendas(obterAnalisesEmendas $obterAnalisesEmendas) {
  	$this->tipolog = 'obterAnalisesEmendas';
  	
    return $this->__soapCall('obterAnalisesEmendas', array($obterAnalisesEmendas),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param cadastrarAnalisesEmendas $cadastrarAnalisesEmendas
   * @return cadastrarAnalisesEmendasResponse
   */
  public function cadastrarAnalisesEmendas(cadastrarAnalisesEmendas $cadastrarAnalisesEmendas) {
  	$this->tipolog = 'cadastrarAnalisesEmendas';
  	
    return $this->__soapCall('cadastrarAnalisesEmendas', array($cadastrarAnalisesEmendas),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param enviarPedidoAlteracao $enviarPedidoAlteracao
   * @return enviarPedidoAlteracaoResponse
   */
  public function enviarPedidoAlteracao(enviarPedidoAlteracao $enviarPedidoAlteracao) {
    return $this->__soapCall('enviarPedidoAlteracao', array($enviarPedidoAlteracao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterPedidosAlteracaoPorDescricao $obterPedidosAlteracaoPorDescricao
   * @return obterPedidosAlteracaoPorDescricaoResponse
   */
  public function obterPedidosAlteracaoPorDescricao(obterPedidosAlteracaoPorDescricao $obterPedidosAlteracaoPorDescricao) {
    return $this->__soapCall('obterPedidosAlteracaoPorDescricao', array($obterPedidosAlteracaoPorDescricao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param verificarPedidoAlteracao $verificarPedidoAlteracao
   * @return verificarPedidoAlteracaoResponse
   */
  public function verificarPedidoAlteracao(verificarPedidoAlteracao $verificarPedidoAlteracao) {
    return $this->__soapCall('verificarPedidoAlteracao', array($verificarPedidoAlteracao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  
  	function __doRequest($request, $location, $action, $version, $one_way = 0){
  		
    	$this->log($request, $location, $action, $version, 'request');
         
    	$response = SoapClient::__doRequest($request, $location, $action, $version, $one_way);
         
    	$this->log($response, $location, $action, $version, 'response');
        
    	return $response;
  	}
	
	public function log($request, $location, $action, $version, $tipo){
		global $db;
	        
		//ver( simec_htmlentities($request) );
		$request = str_ireplace( "'", '"', $request );
         if( $this->tipolog != 'naograva'  ){
	         if( $tipo == 'request' ){
	         	if( is_array( $this->arrPedido ) ){
			         $sql = "INSERT INTO elabrev.logerrows(logtipo, acacod, unicod, prgcod, loccod, prgano, usucpf, logdatainclusao, acaid, identificadorunico, xmlrequest, xmlresponse) 
							VALUES ('{$this->tipolog}', '{$this->arrPedido['codigoacao']}', '{$this->arrPedido['codigouo']}', '{$this->arrPedido['codigoprograma']}', 
									'{$this->arrPedido['codigolocalizador']}', '{$_SESSION['exercicio']}', '{$_SESSION['usucpf']}', now(), null, '{$this->arrPedido['identificadorunico']}', '$request', null) returning logid";
					
					 $this->logid = $db->pegaUm( $sql );
	         	} else {
	         		$sql = "INSERT INTO elabrev.logerrows(logtipo, acacod, unicod, prgcod, loccod, prgano, usucpf, logdatainclusao, acaid, identificadorunico, xmlrequest, xmlresponse) 
							VALUES ('{$this->tipolog}', '{$this->arrPedido}', null, null, null, null, null, now(), null, null, '$request', null) returning logid";
					$this->logid = $db->pegaUm( $sql );
	         	}
	         } else {
	         	$sql = "UPDATE elabrev.logerrows SET xmlresponse = '$request' WHERE logid = ".$this->logid;
				$db->executar($sql);
	         }
         } else {
         	if( $this->tipolog ){
	         	if( $tipo == 'request' ){
	         		$sql = "INSERT INTO elabrev.logerrows(logtipo, acacod, unicod, prgcod, loccod, prgano, usucpf, logdatainclusao, acaid, identificadorunico, xmlrequest, xmlresponse) 
						VALUES ('{$this->tipolog}', null, null, null, null, null, null, now(), null, null, '$request', null) returning logid";
						$this->logid = $db->pegaUm( $sql );
	         	} else {
	         		$sql = "UPDATE elabrev.logerrows SET xmlresponse = '$request' WHERE logid = ".$this->logid;
					$db->executar($sql);
	         	}
         	}
         }
         $db->commit();
	}
	
	// route all other method calls directly to soapClient
	public function __call($method, $args){
        // you could also add method_exists check here
        return call_user_func_array(array($this->soapClient, $method), $args);
	}
}

?>
