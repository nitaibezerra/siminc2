<?php
/**
 * PLANACOMORC
 * base: Programaчуo Orчamentсria
 */
require_once('ClassesDTO.class.inc');
/**
 * WSQualitativo class
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class WSQualitativo extends SoapClient {

  private static $classmap = array(
      'obterOrgao' => 'obterOrgao',
      'credencialDTO' => 'credencialDTO',
      'baseDTO' => 'baseDTO',
      'obterOrgaoResponse' => 'obterOrgaoResponse',
      'retornoOrgaosDTO' => 'retornoOrgaosDTO',
      'retornoDTO' => 'retornoDTO',
      'orgaoDTO' => 'orgaoDTO',
      'obterUnidadesOrcamentariasOrgao' => 'obterUnidadesOrcamentariasOrgao',
      'obterUnidadesOrcamentariasOrgaoResponse' => 'obterUnidadesOrcamentariasOrgaoResponse',
      'obterProgramasPorOrgao' => 'obterProgramasPorOrgao',
      'obterProgramasPorOrgaoResponse' => 'obterProgramasPorOrgaoResponse',
      'retornoProgramasDTO' => 'retornoProgramasDTO',
      'programaDTO' => 'programaDTO',
      'obterAcoesPorPrograma' => 'obterAcoesPorPrograma',
      'obterAcoesPorProgramaResponse' => 'obterAcoesPorProgramaResponse',
      'retornoAcoesDTO' => 'retornoAcoesDTO',
      'acaoDTO' => 'acaoDTO',
      'obterIndicadoresPorPrograma' => 'obterIndicadoresPorPrograma',
      'obterIndicadoresPorProgramaResponse' => 'obterIndicadoresPorProgramaResponse',
      'retornoIndicadoresDTO' => 'retornoIndicadoresDTO',
      'indicadorDTO' => 'indicadorDTO',
      'obterObjetivosPorPrograma' => 'obterObjetivosPorPrograma',
      'obterObjetivosPorProgramaResponse' => 'obterObjetivosPorProgramaResponse',
      'retornoObjetivosDTO' => 'retornoObjetivosDTO',
      'objetivoDTO' => 'objetivoDTO',
      'obterMetasPorObjetivo' => 'obterMetasPorObjetivo',
      'obterMetasPorObjetivoResponse' => 'obterMetasPorObjetivoResponse',
      'retornoMetasDTO' => 'retornoMetasDTO',
      'metaDTO' => 'metaDTO',
      'obterRegionalizacoesPorMeta' => 'obterRegionalizacoesPorMeta',
      'obterRegionalizacoesPorMetaResponse' => 'obterRegionalizacoesPorMetaResponse',
      'retornoRegionalizacoesDTO' => 'retornoRegionalizacoesDTO',
      'regionalizacaoDTO' => 'regionalizacaoDTO',
      'obterIniciativasPorObjetivo' => 'obterIniciativasPorObjetivo',
      'obterIniciativasPorObjetivoResponse' => 'obterIniciativasPorObjetivoResponse',
      'retornoIniciativasDTO' => 'retornoIniciativasDTO',
      'iniciativaDTO' => 'iniciativaDTO',
      'obterMedidaInstitucionalPorIniciativa' => 'obterMedidaInstitucionalPorIniciativa',
      'obterMedidaInstitucionalPorIniciativaResponse' => 'obterMedidaInstitucionalPorIniciativaResponse',
      'retornoMedidaInstitucionalNormativaDTO' => 'retornoMedidaInstitucionalNormativaDTO',
      'medidaInstitucionalNormativaDTO' => 'medidaInstitucionalNormativaDTO',
      'obterFinanciamentoExtraOrcamentarioPorIniciativa' => 'obterFinanciamentoExtraOrcamentarioPorIniciativa',
      'obterFinanciamentoExtraOrcamentarioPorIniciativaResponse' => 'obterFinanciamentoExtraOrcamentarioPorIniciativaResponse',
      'retornoFinanciamentoExtraOrcamentarioDTO' => 'retornoFinanciamentoExtraOrcamentarioDTO',
      'financiamentoExtraOrcamentarioDTO' => 'financiamentoExtraOrcamentarioDTO',
      'obterAcoesPorIniciativa' => 'obterAcoesPorIniciativa',
      'obterAcoesPorIniciativaResponse' => 'obterAcoesPorIniciativaResponse',
      'obterLocalizadoresPorAcao' => 'obterLocalizadoresPorAcao',
      'obterLocalizadoresPorAcaoResponse' => 'obterLocalizadoresPorAcaoResponse',
      'retornoLocalizadoresDTO' => 'retornoLocalizadoresDTO',
      'localizadorDTO' => 'localizadorDTO',
      'obterOrgaoPorCodigoSiorg' => 'obterOrgaoPorCodigoSiorg',
      'obterOrgaoPorCodigoSiorgResponse' => 'obterOrgaoPorCodigoSiorgResponse',
      'obterTabelasApoio' => 'obterTabelasApoio',
      'obterTabelasApoioResponse' => 'obterTabelasApoioResponse',
      'retornoApoioQualitativoDTO' => 'retornoApoioQualitativoDTO',
      'baseGeograficaDTO' => 'baseGeograficaDTO',
      'esferaDTO' => 'esferaDTO',
      'funcaoDTO' => 'funcaoDTO',
      'macroDesafioDTO' => 'macroDesafioDTO',
      'momentoDTO' => 'momentoDTO',
      'perfilDTO' => 'perfilDTO',
      'periodicidadeDTO' => 'periodicidadeDTO',
      'produtoDTO' => 'produtoDTO',
      'regiaoDTO' => 'regiaoDTO',
      'subFuncaoDTO' => 'subFuncaoDTO',
      'tipoAcaoDTO' => 'tipoAcaoDTO',
      'tipoInclusaoDTO' => 'tipoInclusaoDTO',
      'tipoProgramaDTO' => 'tipoProgramaDTO',
      'unidadeMedidaDTO' => 'unidadeMedidaDTO',
      'unidadeMedidaIndicadorDTO' => 'unidadeMedidaIndicadorDTO',
      'obterProgramacaoCompleta' => 'obterProgramacaoCompleta',
      'obterProgramacaoCompletaResponse' => 'obterProgramacaoCompletaResponse',
      'retornoProgramacaoQualitativoDTO' => 'retornoProgramacaoQualitativoDTO',
      'agendaSamDTO' => 'agendaSamDTO',
      'planoOrcamentarioDTO' => 'planoOrcamentarioDTO',
      'obterMomentoCarga' => 'obterMomentoCarga',
      'obterMomentoCargaResponse' => 'obterMomentoCargaResponse',
      'retornoMomentoDTO' => 'retornoMomentoDTO',
      'obterPlanosOrcamentariosPorAcao' => 'obterPlanosOrcamentariosPorAcao',
      'obterPlanosOrcamentariosPorAcaoResponse' => 'obterPlanosOrcamentariosPorAcaoResponse',
      'retornoPlanoOrcamentarioDTO' => 'retornoPlanoOrcamentarioDTO',
      'obterAcaoPorIdentificadorUnico' => 'obterAcaoPorIdentificadorUnico',
      'obterAcaoPorIdentificadorUnicoResponse' => 'obterAcaoPorIdentificadorUnicoResponse');

  private $_overloadedMethods = array(
      'obterProgramacaoCompleta', 'obterTabelasApoio', 'obterProgramasPorOrgao',/*, 'obterAcoes', 'obterOrgao',
      'obterProgramas', 'obterObjetivos', 'obterMetas', 'obterIniciativas', 'obterLocalizadores',
      'obterRegionalizacoes', 'obterUnidadesOrcamentarias', 'obterIndicadores',
      'obterMomentos', 'obterPlanosOrcamentarios'*/);

  /**
   * Estabelece uma conexуo com o webservice SIOP-WSQualitativo.
   * Adiciona o mapeamento de classes рs opчѕes da conexуo para formatar o retorno
   * do webservice.
   * @return SoapClient
   */
  public function WSQualitativo($wsdl = "https://www.siop.planejamento.gov.br/services/WSQualitativo?wsdl", $options = array()) {
    if (('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin'])) {
        $options['proxy_host'] = 'proxy3.mec.gov.br';
        $options['proxy_port'] = '8080';
    }

    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   * Sobrecarga dos mщtodos de chamada ao webservice. Os mщtodos executam um
   * procedimento padrуo e comum.
   * @param string $nome Nome do mщtodo invocado;
   * @param array $parametros Parтmetros enviados na chamada.
   * @return mixed
   */
  public function __call($nome, $parametros) {
    if (!in_array($nome, $this->_overloadedMethods)) {
      trigger_error("Funчуo '{$nome}' nуo estс definida em WSQualitativo nem em WSQualitativo::_overloadedMethods.", E_USER_ERROR);
    }
    // -- O tipo do objeto enviado como parametro para a chamada do ws corresponde
    // -- ao nome do mщtodo chamado.
    if (!($parametros[0] instanceof $nome)) {
      trigger_error("Tipo de parтmetro invсlido. Esperado '{$nome}'. Recebido: " . gettype($parametros[0]), E_USER_ERROR);
    }

    //return simec_json_decode(file_get_contents(APPRAIZ . 'planacomorc/classes/json.txt'));
    
    
    
    
    return $this->__soapCall($nome, array($parametros[0]), array(
        'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
        'soapaction' => ''));
  }



  /**
   *  
   *
   * @param obterAcaoPorIdentificadorUnico $obterAcaoPorIdentificadorUnico
   * @return obterAcaoPorIdentificadorUnicoResponse
   */
  public function obterAcaoPorIdentificadorUnico(obterAcaoPorIdentificadorUnico $obterAcaoPorIdentificadorUnico) {
    return $this->__soapCall('obterAcaoPorIdentificadorUnico', array($obterAcaoPorIdentificadorUnico), array(
        'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
        'soapaction' => ''));
  }

  /**
   *  
   *
   * @param obterAcoesPorIniciativa $obterAcoesPorIniciativa
   * @return obterAcoesPorIniciativaResponse
   */
  public function obterAcoesPorIniciativa(obterAcoesPorIniciativa $obterAcoesPorIniciativa) {
    return $this->__soapCall('obterAcoesPorIniciativa', array($obterAcoesPorIniciativa), array(
        'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
        'soapaction' => ''));
  }

  /**
   *  
   * @param obterObjetivosPorPrograma $obterObjetivosPorPrograma
   * @return obterObjetivosPorProgramaResponse
   */
  public function obterObjetivosPorPrograma(obterObjetivosPorPrograma $obterObjetivosPorPrograma) {
    return $this->__soapCall('obterObjetivosPorPrograma', array($obterObjetivosPorPrograma), array(
        'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
        'soapaction' => ''));
  }

  /**
   *  
   * @param obterLocalizadoresPorAcao $obterLocalizadoresPorAcao
   * @return obterLocalizadoresPorAcaoResponse
   */
  public function obterLocalizadoresPorAcao(obterLocalizadoresPorAcao $obterLocalizadoresPorAcao) {
    return $this->__soapCall('obterLocalizadoresPorAcao', array($obterLocalizadoresPorAcao), array(
        'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
        'soapaction' => ''));
  }
//  /**
//   *  
//   *
//   * @param obterAcoesPorPrograma $obterAcoesPorPrograma
//   * @return obterAcoesPorProgramaResponse
//   */
//  public function obterAcoesPorPrograma(obterAcoesPorPrograma $obterAcoesPorPrograma) {
//    return $this->__soapCall('obterAcoesPorPrograma', array($obterAcoesPorPrograma),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterFinanciamentoExtraOrcamentarioPorIniciativa $obterFinanciamentoExtraOrcamentarioPorIniciativa
//   * @return obterFinanciamentoExtraOrcamentarioPorIniciativaResponse
//   */
//  public function obterFinanciamentoExtraOrcamentarioPorIniciativa(obterFinanciamentoExtraOrcamentarioPorIniciativa $obterFinanciamentoExtraOrcamentarioPorIniciativa) {
//    return $this->__soapCall('obterFinanciamentoExtraOrcamentarioPorIniciativa', array($obterFinanciamentoExtraOrcamentarioPorIniciativa),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterIndicadoresPorPrograma $obterIndicadoresPorPrograma
//   * @return obterIndicadoresPorProgramaResponse
//   */
//  public function obterIndicadoresPorPrograma(obterIndicadoresPorPrograma $obterIndicadoresPorPrograma) {
//    return $this->__soapCall('obterIndicadoresPorPrograma', array($obterIndicadoresPorPrograma),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterIniciativasPorObjetivo $obterIniciativasPorObjetivo
//   * @return obterIniciativasPorObjetivoResponse
//   */
//  public function obterIniciativasPorObjetivo(obterIniciativasPorObjetivo $obterIniciativasPorObjetivo) {
//    return $this->__soapCall('obterIniciativasPorObjetivo', array($obterIniciativasPorObjetivo),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//
//  /**
//   *  
//   *
//   * @param obterMedidaInstitucionalPorIniciativa $obterMedidaInstitucionalPorIniciativa
//   * @return obterMedidaInstitucionalPorIniciativaResponse
//   */
//  public function obterMedidaInstitucionalPorIniciativa(obterMedidaInstitucionalPorIniciativa $obterMedidaInstitucionalPorIniciativa) {
//    return $this->__soapCall('obterMedidaInstitucionalPorIniciativa', array($obterMedidaInstitucionalPorIniciativa),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterMetasPorObjetivo $obterMetasPorObjetivo
//   * @return obterMetasPorObjetivoResponse
//   */
//  public function obterMetasPorObjetivo(obterMetasPorObjetivo $obterMetasPorObjetivo) {
//    return $this->__soapCall('obterMetasPorObjetivo', array($obterMetasPorObjetivo),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterMomentoCarga $obterMomentoCarga
//   * @return obterMomentoCargaResponse
//   */
//  public function obterMomentoCarga(obterMomentoCarga $obterMomentoCarga) {
//    return $this->__soapCall('obterMomentoCarga', array($obterMomentoCarga),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//
//  /**
//   *  
//   *
//   * @param obterOrgao $obterOrgao
//   * @return obterOrgaoResponse
//   */
//  public function obterOrgao(obterOrgao $obterOrgao) {
//    return $this->__soapCall('obterOrgao', array($obterOrgao),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterOrgaoPorCodigoSiorg $obterOrgaoPorCodigoSiorg
//   * @return obterOrgaoPorCodigoSiorgResponse
//   */
//  public function obterOrgaoPorCodigoSiorg(obterOrgaoPorCodigoSiorg $obterOrgaoPorCodigoSiorg) {
//    return $this->__soapCall('obterOrgaoPorCodigoSiorg', array($obterOrgaoPorCodigoSiorg),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterPlanosOrcamentariosPorAcao $obterPlanosOrcamentariosPorAcao
//   * @return obterPlanosOrcamentariosPorAcaoResponse
//   */
//  public function obterPlanosOrcamentariosPorAcao(obterPlanosOrcamentariosPorAcao $obterPlanosOrcamentariosPorAcao) {
//    return $this->__soapCall('obterPlanosOrcamentariosPorAcao', array($obterPlanosOrcamentariosPorAcao),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }

//  /**
//   *  
//   *
//   * @param obterProgramasPorOrgao $obterProgramasPorOrgao
//   * @return obterProgramasPorOrgaoResponse
//   */
//  public function obterProgramasPorOrgao(obterProgramasPorOrgao $obterProgramasPorOrgao) {
//    return $this->__soapCall('obterProgramasPorOrgao', array($obterProgramasPorOrgao),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//
//  /**
//   *  
//   *
//   * @param obterRegionalizacoesPorMeta $obterRegionalizacoesPorMeta
//   * @return obterRegionalizacoesPorMetaResponse
//   */
//  public function obterRegionalizacoesPorMeta(obterRegionalizacoesPorMeta $obterRegionalizacoesPorMeta) {
//    return $this->__soapCall('obterRegionalizacoesPorMeta', array($obterRegionalizacoesPorMeta),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }
//

//
//  /**
//   *  
//   *
//   * @param obterUnidadesOrcamentariasOrgao $obterUnidadesOrcamentariasOrgao
//   * @return obterUnidadesOrcamentariasOrgaoResponse
//   */
//  public function obterUnidadesOrcamentariasOrgao(obterUnidadesOrcamentariasOrgao $obterUnidadesOrcamentariasOrgao) {
//    return $this->__soapCall('obterUnidadesOrcamentariasOrgao', array($obterUnidadesOrcamentariasOrgao),       array(
//            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
//            'soapaction' => ''
//           )
//      );
//  }

}
?>