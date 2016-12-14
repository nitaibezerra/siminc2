<?php
class obterOrgao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoOrgao; // string
  public $tipoOrgao; // string
  public $dataHoraReferencia; // dateTime
}

class credencialDTO {
  public $perfil; // int
  public $senha; // string
  public $usuario; // string
}

class baseDTO {
}

class obterOrgaoResponse {
  public $return; // retornoOrgaosDTO
}

class retornoOrgaosDTO {
  public $registros; // orgaoDTO
}

class retornoDTO {
  public $mensagensErro; // string
  public $sucesso; // boolean
}

class orgaoDTO {
  public $codigoOrgao; // string
  public $codigoOrgaoPai; // string
  public $descricao; // string
  public $descricaoAbreviada; // string
  public $exercicio; // int
  public $orgaoId; // int
  public $orgaoSiorg; // string
  public $snAtivo; // boolean
  public $tipoOrgao; // string
}

class obterUnidadesOrcamentariasOrgao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoOrgao; // string
}

class obterUnidadesOrcamentariasOrgaoResponse {
  public $return; // retornoOrgaosDTO
}

class obterProgramasPorOrgao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoOrgao; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterProgramasPorOrgaoResponse {
  public $return; // retornoProgramasDTO
}

class retornoProgramasDTO {
  public $registros; // programaDTO
}

class programaDTO {
  public $codigoMacroDesafio; // int
  public $codigoMomento; // int
  public $codigoOrgao; // string
  public $codigoPrograma; // string
  public $codigoTipoPrograma; // string
  public $estrategiaImplementacao; // string
  public $exercicio; // int
  public $horizonteTemporalContinuo; // int
  public $identificadorUnico; // int
  public $justificativa; // string
  public $objetivo; // string
  public $objetivoGoverno; // string
  public $objetivoSetorial; // string
  public $problema; // string
  public $publicoAlvo; // string
  public $snExclusaoLogica; // boolean
  public $titulo; // string
  public $unidadeResponsavel; // string
}

class obterAcoesPorPrograma {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterAcoesPorProgramaResponse {
  public $return; // retornoAcoesDTO
}

class retornoAcoesDTO {
  public $registros; // acaoDTO
}

class acaoDTO {
  public $baseLegal; // string
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFuncao; // string
  public $codigoIniciativa; // string
  public $codigoMomento; // int
  public $codigoObjetivo; // string
  public $codigoOrgao; // string
  public $codigoProduto; // int
  public $codigoPrograma; // string
  public $codigoSubFuncao; // string
  public $codigoTipoAcao; // string
  public $codigoTipoInclusaoAcao; // int
  public $codigoUnidadeMedida; // string
  public $descricao; // string
  public $detalhamentoImplementacao; // string
  public $especificacaoProduto; // string
  public $exercicio; // int
  public $finalidade; // string
  public $formaAcompanhamento; // string
  public $identificacaoSazonalidade; // string
  public $identificadorUnico; // int
  public $insumosUtilizados; // string
  public $snDescentralizada; // boolean
  public $snDireta; // boolean
  public $snExclusaoLogica; // boolean
  public $snLinhaCredito; // boolean
  public $snTransferenciaObrigatoria; // boolean
  public $snTransferenciaVoluntaria; // boolean
  public $titulo; // string
  public $unidadeResponsavel; // string
}

class obterIndicadoresPorPrograma {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterIndicadoresPorProgramaResponse {
  public $return; // retornoIndicadoresDTO
}

class retornoIndicadoresDTO {
  public $registros; // indicadorDTO
}

class indicadorDTO {
  public $codigoBaseGeografica; // int
  public $codigoIndicador; // int
  public $codigoMomento; // int
  public $codigoPeriodicidade; // int
  public $codigoPrograma; // string
  public $codigoUnidadeMedidaIndicador; // int
  public $dataApuracao; // dateTime
  public $descricao; // string
  public $exercicio; // int
  public $fonte; // string
  public $formula; // string
  public $identificadorUnico; // int
  public $snApuracaoReferencia; // boolean
  public $snExclusaoLogica; // boolean
  public $valorReferencia; // decimal
}

class obterObjetivosPorPrograma {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterObjetivosPorProgramaResponse {
  public $return; // retornoObjetivosDTO
}

class retornoObjetivosDTO {
  public $registros; // objetivoDTO
}

class objetivoDTO {
  public $codigoMomento; // int
  public $codigoObjetivo; // string
  public $codigoOrgao; // string
  public $codigoPrograma; // string
  public $enunciado; // string
  public $exercicio; // int
  public $identificadorUnico; // int
  public $snExclusaoLogica; // boolean
}

class obterMetasPorObjetivo {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoObjetivo; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterMetasPorObjetivoResponse {
  public $return; // retornoMetasDTO
}

class retornoMetasDTO {
  public $registros; // metaDTO
}

class metaDTO {
  public $codigoMeta; // int
  public $codigoMomento; // int
  public $codigoObjetivo; // string
  public $codigoPrograma; // string
  public $descricao; // string
  public $exercicio; // int
  public $identificadorUnico; // int
}

class obterRegionalizacoesPorMeta {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoObjetivo; // string
  public $codigoMeta; // int
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterRegionalizacoesPorMetaResponse {
  public $return; // retornoRegionalizacoesDTO
}

class retornoRegionalizacoesDTO {
  public $registros; // regionalizacaoDTO
}

class regionalizacaoDTO {
  public $codigoMeta; // int
  public $codigoMomento; // int
  public $codigoObjetivo; // string
  public $codigoPrograma; // string
  public $codigoUnidadeMedida; // string
  public $descricao; // string
  public $exercicio; // int
  public $identificadorUnicoMeta; // int
  public $regionalizacaoId; // int
  public $sigla; // string
  public $valor; // decimal
}

class obterIniciativasPorObjetivo {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoObjetivo; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterIniciativasPorObjetivoResponse {
  public $return; // retornoIniciativasDTO
}

class retornoIniciativasDTO {
  public $registros; // iniciativaDTO
}

class iniciativaDTO {
  public $codigoIniciativa; // string
  public $codigoMomento; // int
  public $codigoObjetivo; // string
  public $codigoOrgao; // string
  public $codigoPrograma; // string
  public $exercicio; // int
  public $identificadorUnico; // int
  public $snExclusaoLogica; // boolean
  public $snIndividualizada; // boolean
  public $titulo; // string
}

class obterMedidaInstitucionalPorIniciativa {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoObjetivo; // string
  public $codigoIniciativa; // string
  public $codigoMomento; // int
}

class obterMedidaInstitucionalPorIniciativaResponse {
  public $return; // retornoMedidaInstitucionalNormativaDTO
}

class retornoMedidaInstitucionalNormativaDTO {
  public $registros; // medidaInstitucionalNormativaDTO
}

class medidaInstitucionalNormativaDTO {
  public $codigoMomento; // int
  public $codigoOrgaoSiorg; // string
  public $descricao; // string
  public $exercicio; // int
  public $identificadorIniciativa; // int
  public $identificadorUnico; // int
  public $produto; // string
  public $snExclusaoLogica; // boolean
}

class obterFinanciamentoExtraOrcamentarioPorIniciativa {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoObjetivo; // string
  public $codigoIniciativa; // string
  public $codigoMomento; // int
}

class obterFinanciamentoExtraOrcamentarioPorIniciativaResponse {
  public $return; // retornoFinanciamentoExtraOrcamentarioDTO
}

class retornoFinanciamentoExtraOrcamentarioDTO {
  public $registros; // financiamentoExtraOrcamentarioDTO
}

class financiamentoExtraOrcamentarioDTO {
  public $codigoMomento; // int
  public $codigoOrgaoSiorg; // string
  public $custoTotal; // double
  public $dataInicio; // dateTime
  public $dataTermino; // dateTime
  public $descricao; // string
  public $exercicio; // int
  public $fonteFinanciamento; // string
  public $identificadorIniciativa; // int
  public $identificadorUnico; // int
  public $outraFonteFinanciamento; // string
  public $produto; // string
  public $snProjeto; // boolean
  public $valorAno1Ppa; // double
  public $valorAno2Ppa; // double
  public $valorAno3Ppa; // double
  public $valorAno4Ppa; // double
  public $valorTotal; // double
}

class obterAcoesPorIniciativa {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoPrograma; // string
  public $codigoObjetivo; // string
  public $codigoIniciativa; // string
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterAcoesPorIniciativaResponse {
  public $return; // retornoAcoesDTO
}

class obterLocalizadoresPorAcao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $identificadorUnicoAcao; // int
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterLocalizadoresPorAcaoResponse {
  public $return; // retornoLocalizadoresDTO
}

class retornoLocalizadoresDTO {
  public $registros; // localizadorDTO
}

class localizadorDTO {
  public $anoInicio; // int
  public $anoTermino; // int
  public $codigoLocalizador; // string
  public $codigoMomento; // int
  public $codigoTipoInclusao; // int
  public $descricao; // string
  public $exercicio; // int
  public $identificadorUnicoAcao; // int
  public $justificativaRepercussao; // string
  public $municipio; // string
  public $regiao; // string
  public $siglaRegiao; // string
  public $snExclusaoLogica; // boolean
  public $totalFinanceiro; // double
  public $totalFisico; // double
  public $uf; // string
}

class obterOrgaoPorCodigoSiorg {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoSiorg; // string
}

class obterOrgaoPorCodigoSiorgResponse {
  public $return; // retornoOrgaosDTO
}

class obterTabelasApoio {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $retornarMomentos; // boolean
  public $retornarEsferas; // boolean
  public $retornarTiposInclusao; // boolean
  public $retonarFuncoes; // boolean
  public $retornarSubFuncoes; // boolean
  public $retornarTiposAcao; // boolean
  public $retornarProdutos; // boolean
  public $retornarUnidadesMedida; // boolean
  public $retornarRegioes; // boolean
  public $retornarPerfis; // boolean
  public $retornarTiposPrograma; // boolean
  public $retornarMacroDesafios; // boolean
  public $retornarUnidadesMedidaIndicador; // boolean
  public $retornarPeriodicidades; // boolean
  public $retornarBasesGeograficas; // boolean
  public $dataHoraReferencia; // dateTime
}

class obterTabelasApoioResponse {
  public $return; // retornoApoioQualitativoDTO
}

class retornoApoioQualitativoDTO {
  public $basesGeograficasDTO; // baseGeograficaDTO
  public $esferasDTO; // esferaDTO
  public $funcoesDTO; // funcaoDTO
  public $macroDesafiosDTO; // macroDesafioDTO
  public $momentosDTO; // momentoDTO
  public $perfisDTO; // perfilDTO
  public $periodicidadesDTO; // periodicidadeDTO
  public $produtosDTO; // produtoDTO
  public $regioesDTO; // regiaoDTO
  public $subFuncoesDTO; // subFuncaoDTO
  public $tiposAcaoDTO; // tipoAcaoDTO
  public $tiposInclusaoDTO; // tipoInclusaoDTO
  public $tiposProgramaDTO; // tipoProgramaDTO
  public $unidadesMedidaDTO; // unidadeMedidaDTO
  public $unidadesMedidaIndicadorDTO; // unidadeMedidaIndicadorDTO
}

class baseGeograficaDTO {
  public $codigoBaseGeografica; // int
  public $descricao; // string
  public $snAtivo; // boolean
  public $snExclusaoLogica; // boolean
}

class esferaDTO {
  public $codigoEsfera; // string
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $descricaoAbreviada; // string
  public $snAtivo; // boolean
  public $snValorizacao; // boolean
}

class funcaoDTO {
  public $codigoFuncao; // string
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $descricaoAbreviada; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class macroDesafioDTO {
  public $codigoMacroDesafio; // int
  public $descricao; // string
  public $titulo; // string
}

class momentoDTO {
  public $codigoMomento; // int
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $snAtivo; // boolean
}

class perfilDTO {
  public $descricao; // string
  public $perfilId; // int
}

class periodicidadeDTO {
  public $codigoPeriodicidade; // int
  public $descricao; // string
  public $snAtivo; // boolean
  public $snExclusaoLogica; // boolean
}

class produtoDTO {
  public $codigoProduto; // int
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $snAtivo; // boolean
  public $snExclusaoLogica; // boolean
}

class regiaoDTO {
  public $codigoRegiao; // int
  public $descricao; // string
  public $sigla; // string
}

class subFuncaoDTO {
  public $codigoFuncao; // string
  public $codigoSubFuncao; // string
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $descricaoAbreviada; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class tipoAcaoDTO {
  public $codigoTipoAcao; // string
  public $descricao; // string
  public $snAtivo; // boolean
}

class tipoInclusaoDTO {
  public $codigoTipoInclusao; // int
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $snAtivo; // boolean
}

class tipoProgramaDTO {
  public $codigoTipoPrograma; // string
  public $descricao; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class unidadeMedidaDTO {
  public $codigoUnidadeMedida; // string
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $snAtivo; // boolean
}

class unidadeMedidaIndicadorDTO {
  public $codigoUnidadeMedidaIndicador; // int
  public $descricao; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class obterProgramacaoCompleta {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $retornarOrgaos; // boolean
  public $retornarProgramas; // boolean
  public $retornarIndicadores; // boolean
  public $retornarObjetivos; // boolean
  public $retornarIniciativas; // boolean
  public $retornarAcoes; // boolean
  public $retornarLocalizadores; // boolean
  public $retornarMetas; // boolean
  public $retornarRegionalizacoes; // boolean
  public $retornarPlanosOrcamentarios; // boolean
  public $retornarAgendaSam; // boolean
  public $retornarMedidasInstitucionaisNormativas; // boolean
  public $dataHoraReferencia; // dateTime
}

class obterProgramacaoCompletaResponse {
  public $return; // retornoProgramacaoQualitativoDTO
}

class retornoProgramacaoQualitativoDTO {
  public $acoesDTO; // acaoDTO
  public $agendasSamDTO; // agendaSamDTO
  public $indicadoresDTO; // indicadorDTO
  public $iniciativasDTO; // iniciativaDTO
  public $localizadoresDTO; // localizadorDTO
  public $medidasInstitucionaisNormativasDTO; // medidaInstitucionalNormativaDTO
  public $metasDTO; // metaDTO 
  public $objetivosDTO; // objetivoDTO
  public $orgaosDTO; // orgaoDTO
  public $planosOrcamentariosDTO; // planoOrcamentarioDTO
  public $programasDTO; // programaDTO
  public $regionalizacoesDTO; // regionalizacaoDTO
}

class agendaSamDTO {
  public $agendaSam; // string
  public $codigoAgendaSam; // int
  public $descricao; // string
}

class planoOrcamentarioDTO {
  public $codigoIndicadorPlanoOrcamentario; // string
  public $codigoMomento; // int
  public $codigoProduto; // int
  public $codigoUnidadeMedida; // string
  public $dataHoraAlteracao; // dateTime
  public $detalhamento; // string
  public $exercicio; // int
  public $identificadorUnico; // int
  public $identificadorUnicoAcao; // int
  public $planoOrcamentario; // string
  public $titulo; // string
}

class obterMomentoCarga {
  public $credencial; // credencialDTO
  public $exercicio; // int
}

class obterMomentoCargaResponse {
  public $return; // retornoMomentoDTO
}

class retornoMomentoDTO {
  public $momento; // momentoDTO
}

class obterPlanosOrcamentariosPorAcao {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $identificadorUnicoAcao; // int
}

class obterPlanosOrcamentariosPorAcaoResponse {
  public $return; // retornoPlanoOrcamentarioDTO
}

class retornoPlanoOrcamentarioDTO {
  public $registros; // planoOrcamentarioDTO
}

class obterAcaoPorIdentificadorUnico {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $identificadorUnico; // int
}

class obterAcaoPorIdentificadorUnicoResponse {
  public $return; // retornoAcoesDTO
}


/**
 * WSQualitativo class
 * 
 *  
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
                                    'obterAcaoPorIdentificadorUnicoResponse' => 'obterAcaoPorIdentificadorUnicoResponse',
                                   );

  public function WSQualitativo($wsdl = NULL, $options = array()) {
    $wsdl = $wsdl?$wsdl: WEB_SERVICE_SIOP_URL. 'WSQualitativo?wsdl';
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
   * @param obterAcaoPorIdentificadorUnico $obterAcaoPorIdentificadorUnico
   * @return obterAcaoPorIdentificadorUnicoResponse
   */
  public function obterAcaoPorIdentificadorUnico(obterAcaoPorIdentificadorUnico $obterAcaoPorIdentificadorUnico) {
    return $this->__soapCall('obterAcaoPorIdentificadorUnico', array($obterAcaoPorIdentificadorUnico),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterAcoesPorIniciativa $obterAcoesPorIniciativa
   * @return obterAcoesPorIniciativaResponse
   */
  public function obterAcoesPorIniciativa(obterAcoesPorIniciativa $obterAcoesPorIniciativa) {
    return $this->__soapCall('obterAcoesPorIniciativa', array($obterAcoesPorIniciativa),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterAcoesPorPrograma $obterAcoesPorPrograma
   * @return obterAcoesPorProgramaResponse
   */
  public function obterAcoesPorPrograma(obterAcoesPorPrograma $obterAcoesPorPrograma) {
    return $this->__soapCall('obterAcoesPorPrograma', array($obterAcoesPorPrograma),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterFinanciamentoExtraOrcamentarioPorIniciativa $obterFinanciamentoExtraOrcamentarioPorIniciativa
   * @return obterFinanciamentoExtraOrcamentarioPorIniciativaResponse
   */
  public function obterFinanciamentoExtraOrcamentarioPorIniciativa(obterFinanciamentoExtraOrcamentarioPorIniciativa $obterFinanciamentoExtraOrcamentarioPorIniciativa) {
    return $this->__soapCall('obterFinanciamentoExtraOrcamentarioPorIniciativa', array($obterFinanciamentoExtraOrcamentarioPorIniciativa),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterIndicadoresPorPrograma $obterIndicadoresPorPrograma
   * @return obterIndicadoresPorProgramaResponse
   */
  public function obterIndicadoresPorPrograma(obterIndicadoresPorPrograma $obterIndicadoresPorPrograma) {
    return $this->__soapCall('obterIndicadoresPorPrograma', array($obterIndicadoresPorPrograma),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterIniciativasPorObjetivo $obterIniciativasPorObjetivo
   * @return obterIniciativasPorObjetivoResponse
   */
  public function obterIniciativasPorObjetivo(obterIniciativasPorObjetivo $obterIniciativasPorObjetivo) {
    return $this->__soapCall('obterIniciativasPorObjetivo', array($obterIniciativasPorObjetivo),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterLocalizadoresPorAcao $obterLocalizadoresPorAcao
   * @return obterLocalizadoresPorAcaoResponse
   */
  public function obterLocalizadoresPorAcao(obterLocalizadoresPorAcao $obterLocalizadoresPorAcao) {
    return $this->__soapCall('obterLocalizadoresPorAcao', array($obterLocalizadoresPorAcao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterMedidaInstitucionalPorIniciativa $obterMedidaInstitucionalPorIniciativa
   * @return obterMedidaInstitucionalPorIniciativaResponse
   */
  public function obterMedidaInstitucionalPorIniciativa(obterMedidaInstitucionalPorIniciativa $obterMedidaInstitucionalPorIniciativa) {
    return $this->__soapCall('obterMedidaInstitucionalPorIniciativa', array($obterMedidaInstitucionalPorIniciativa),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterMetasPorObjetivo $obterMetasPorObjetivo
   * @return obterMetasPorObjetivoResponse
   */
  public function obterMetasPorObjetivo(obterMetasPorObjetivo $obterMetasPorObjetivo) {
    return $this->__soapCall('obterMetasPorObjetivo', array($obterMetasPorObjetivo),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterMomentoCarga $obterMomentoCarga
   * @return obterMomentoCargaResponse
   */
  public function obterMomentoCarga(obterMomentoCarga $obterMomentoCarga) {
    return $this->__soapCall('obterMomentoCarga', array($obterMomentoCarga),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterObjetivosPorPrograma $obterObjetivosPorPrograma
   * @return obterObjetivosPorProgramaResponse
   */
  public function obterObjetivosPorPrograma(obterObjetivosPorPrograma $obterObjetivosPorPrograma) {
    return $this->__soapCall('obterObjetivosPorPrograma', array($obterObjetivosPorPrograma),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterOrgao $obterOrgao
   * @return obterOrgaoResponse
   */
  public function obterOrgao(obterOrgao $obterOrgao) {
    return $this->__soapCall('obterOrgao', array($obterOrgao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterOrgaoPorCodigoSiorg $obterOrgaoPorCodigoSiorg
   * @return obterOrgaoPorCodigoSiorgResponse
   */
  public function obterOrgaoPorCodigoSiorg(obterOrgaoPorCodigoSiorg $obterOrgaoPorCodigoSiorg) {
    return $this->__soapCall('obterOrgaoPorCodigoSiorg', array($obterOrgaoPorCodigoSiorg),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterPlanosOrcamentariosPorAcao $obterPlanosOrcamentariosPorAcao
   * @return obterPlanosOrcamentariosPorAcaoResponse
   */
  public function obterPlanosOrcamentariosPorAcao(obterPlanosOrcamentariosPorAcao $obterPlanosOrcamentariosPorAcao) {
    return $this->__soapCall('obterPlanosOrcamentariosPorAcao', array($obterPlanosOrcamentariosPorAcao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterProgramacaoCompleta $obterProgramacaoCompleta
   * @return obterProgramacaoCompletaResponse
   */
  public function obterProgramacaoCompleta(obterProgramacaoCompleta $obterProgramacaoCompleta) {
    return $this->__soapCall('obterProgramacaoCompleta', array($obterProgramacaoCompleta),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterProgramasPorOrgao $obterProgramasPorOrgao
   * @return obterProgramasPorOrgaoResponse
   */
  public function obterProgramasPorOrgao(obterProgramasPorOrgao $obterProgramasPorOrgao) {
    return $this->__soapCall('obterProgramasPorOrgao', array($obterProgramasPorOrgao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterRegionalizacoesPorMeta $obterRegionalizacoesPorMeta
   * @return obterRegionalizacoesPorMetaResponse
   */
  public function obterRegionalizacoesPorMeta(obterRegionalizacoesPorMeta $obterRegionalizacoesPorMeta) {
    return $this->__soapCall('obterRegionalizacoesPorMeta', array($obterRegionalizacoesPorMeta),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterTabelasApoio $obterTabelasApoio
   * @return obterTabelasApoioResponse
   */
  public function obterTabelasApoio(obterTabelasApoio $obterTabelasApoio) {
    return $this->__soapCall('obterTabelasApoio', array($obterTabelasApoio),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterUnidadesOrcamentariasOrgao $obterUnidadesOrcamentariasOrgao
   * @return obterUnidadesOrcamentariasOrgaoResponse
   */
  public function obterUnidadesOrcamentariasOrgao(obterUnidadesOrcamentariasOrgao $obterUnidadesOrcamentariasOrgao) {
    return $this->__soapCall('obterUnidadesOrcamentariasOrgao', array($obterUnidadesOrcamentariasOrgao),       array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

}
?>