<?php
/**
 * Detalhamento das classes de mapeamento de resultados do webservice do SIOP.
 * $Id: ClassesDTO.class.inc 90719 2014-11-21 11:26:06Z maykelbraz $
 */

/**
 *
 */
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
  public $snAcompanhamentoOpcional; // bool
  public $localizadores; // Lista de LocalizadorDTO
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
  public $identificadorUnicoAcao; // int
  public $exercicio; // int
  public $codigoMomento; // int
  public $codigoLocalizador; // string
  public $codigoTipoInclusao; // int
//  public $anoInicio; // int // -- Verificar atributo, aparentemente foi substituído
  public $mesAnoInicio; // date
//  public $anoTermino; // int // -- Verificar atributo, aparentemente foi substituído
  public $mesAnoTermino; // date
  public $descricao; // string
  public $justificativaRepercussao; // string
  public $municipio; // string
//  public $regiao; // string // -- Verificar atributo, aparentemente foi substituído
  public $codigoRegiao; // integer
  public $totalFinanceiro; // double
  public $totalFisico; // double
  public $uf; // string
//  public $siglaRegiao; // string // -- Verificar atributo, aparentemente foi substituído
  public $snExclusaoLogica; // boolean
  public $snAcompanhamentoOpcional; // boolean
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

class obterExecucaoOrcamentariaSam {
  public $credencial; // credencialDTO
  public $anoExercicio; // int
  public $planoInterno; // string
}



class obterExecucaoOrcamentariaSamResponse {
  public $return; // retornoExecucaoOrcamentariaSamDTO
}

class retornoExecucaoOrcamentariaSamDTO {
  public $registros; // execucaoOrcamentariaSamDTO
}


class execucaoOrcamentariaSamDTO {
  public $acao; // string
  public $acompanhamentoIntensivo; // string
  public $anoExercicio; // string
  public $descAcao; // string
  public $descFuncao; // string
  public $descLocalizador; // string
  public $descOrgao; // string
  public $descPlanoInterno; // string
  public $descPrograma; // string
  public $descSubfuncao; // string
  public $descUO; // string
  public $descUnidadeGestoraResponsavel; // string
  public $dotAtual; // decimal
  public $dotInicial; // decimal
  public $empLiquidado; // decimal
  public $empenhado; // decimal
  public $esfera; // string
  public $funcao; // string
  public $localizador; // string
  public $orgao; // string
  public $pago; // decimal
  public $planoInterno; // string
  public $programa; // string
  public $rapNaoProcessado; // decimal
  public $rapProcessado; // decimal
  public $subfuncao; // string
  public $ultimaAtualizacao; // dateTime
  public $unidadeGestoraResponsavel; // string
  public $uo; // string
}

class excluirProposta {
  public $credencial; // credencialDTO
  public $proposta; // propostaDTO
}

class propostaDTO {
  public $exercicio; // int
  public $codigoEsfera; // string
  public $codigoOrgao; // string
  public $codigoFuncao; // string
  public $codigoSubFuncao; // string
  public $codigoPrograma; // string
  public $codigoAcao; // string
  public $codigoTipoInclusaoAcao; // int
  public $codigoLocalizador; // string
  public $codigoTipoInclusaoLocalizador; // int
  public $codigoMomento; // int
  public $codigoTipoDetalhamento; // string
  public $snAtual; // boolean
  public $quantidadeFisico; // long
  public $valorFisico; // long
  public $financeiros; // financeiroDTO
  public $receitas; // receitaDTO
  public $metaPlanoOrcamentario; // metaPlanoOrcamentarioDTO
  public $justificativa; // string
  public $identificadorUnicoAcao; // int
  public $expansaoFisicaSolicitada; // long
  public $expansaoFisicaConcedida; // long
  public $justificativaExpansaoSolicitada; // string
  public $justificativaExpansaoConcedida; // string
}

class propostaDTOMeta {
  public $codigoAcao; // string
  public $codigoEsfera; // string
  public $codigoFuncao; // string
  public $codigoLocalizador; // string
  public $codigoMomento; // int
  public $codigoOrgao; // string
  public $codigoPrograma; // string
  public $codigoSubFuncao; // string
  public $codigoTipoDetalhamento; // string
  public $codigoTipoInclusaoAcao; // int
  public $codigoTipoInclusaoLocalizador; // int
  public $exercicio; // int
  public $financeiros; // financeiroDTO
  public $justificativa; // string
  public $metaPlanoOrcamentario; // metaPlanoOrcamentarioDTO
  public $quantidadeFisico; // long
  //public $receitas; // receitaDTO
  public $snAtual; // boolean
  public $valorFisico; // long
}

class financeiroDTO {
  public $idUso; // string
  public $idOC; // string
  public $naturezaDespesa; // string
  public $fonte; // string
  public $resultadoPrimarioLei; // string
  public $resultadoPrimarioAtual; // string
  public $identificadorPlanoOrcamentario; // int
  public $valor; // long
}

class metaPlanoOrcamentarioDTO {
  public $identificadorUnicoPlanoOrcamentario; // int
  public $quantidadeFisico; // long
  public $expansaoFisicaSolicitada; // long
  public $expansaoFisicaConcedida; // -- long
}

class receitaDTO {
  public $naturezaReceita; // string
  public $valor; // long
}

class excluirPropostaResponse {
  public $return; // retornoPropostasDTO
}

class retornoPropostasDTO {
  public $proposta; // propostaDTO
}

class cadastrarProposta {
  public $credencial; // credencialDTO
  public $proposta; // propostaDTO
}

class cadastrarPropostaResponse {
  public $return; // retornoPropostasDTO
}

class consultarProposta {
  public $credencial; // credencialDTO
  public $proposta; // propostaDTO
}

class consultarPropostaResponse {
  public $return; // retornoPropostasDTO
}

class obterTabelasApoioQuantitativo {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $retornarNaturezas; // boolean
  public $retornarIdOcs; // boolean
  public $retornarIdUsos; // boolean
  public $retornarFontes; // boolean
  public $retornarRPs; // boolean
  public $dataHoraReferencia; // dateTime
}

class obterTabelasApoioQuantitativoResponse {
  public $return; // retornoApoioQuantitativoDTO
}

class retornoApoioQuantitativoDTO {
  public $fonte; // fonteDTO
  public $idUso; // idUsoDTO
  public $idoc; // idOcDTO
  public $natureza; // naturezaDespesaDTO
  public $resultadoPrimario; // resultadoPrimarioDTO
}

class fonteDTO {
  public $codigoFonte; // string
  public $descricao; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class idUsoDTO {
  public $codigoIdUso; // string
  public $descricao; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class idOcDTO {
  public $codigoIdOc; // string
  public $descricao; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class naturezaDespesaDTO {
  public $codigoNatureza; // string
  public $descricao; // string
  public $descricaoAbreviada; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class resultadoPrimarioDTO {
  public $codigoResultadoPrimario; // string
  public $descricao; // string
  public $exercicio; // int
}

class obterProgramacaoCompletaQuantitativo {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
}

class obterProgramacaoCompletaQuantitativoResponse {
  public $return; // retornoPropostasDTO
}

class cadastrarAcompanhamentoOrcamentario {
  public $credencial; // credencialDTO
  public $acompanhamentoAcao; // acompanhamentoOrcamentarioAcaoDTO
}

class acompanhamentoOrcamentarioAcaoDTO {
  public $periodoOrdem; // int
  public $exercicio; // int
  public $codigoMomento; // int
  public $esfera; // string
  public $unidadeOrcamentaria; // string
  public $funcao; // string
  public $subFuncao; // string
  public $programa; // string
  public $acao; // string
  public $codigoTipoInclusaoAcao; // int
  public $snPendencia; // boolean
  public $dataHoraAlteracao; // dateTime
  public $acompanhamentosLocalizadores; // acompanhamentosLocalizadores
}

class acompanhamentosLocalizadores {
  public $acompanhamentoLocalizador; // acompanhamentoOrcamentarioLocalizadorDTO
}

class acompanhamentoOrcamentarioLocalizadorDTO {
  public $localizador; // string
  public $codigoTipoInclusaoLocalizador; // int
  public $meta; // long
  public $reprogramado; // long
  public $realizadoLOA; // long
  public $dataApuracaoLOA; // dateTime
  public $dotacaoAtual; // long
  public $limite; // long
  public $empenhado; // double
  public $liquidado; // double
  public $realizadoRAP; // long
  public $dataApuracaoRAP; // dateTime
  public $rapInscritoLiquido; // double
  public $rapLiquidadoAPagar; // double
  public $rapPago; // double
  public $justificativa; // string
  public $comentariosLocalizador; // comentariosLocalizador
  public $analisesLocalizador; // -- lista de AnaliseAcompanhamentoOrcamentarioDTO
  public $acompanhamentosPlanoOrcamentario; // -- lista de AcompanhamentoPlanoOrcamentarioDTOf
}

class comentariosLocalizador {
  public $comentarioLocalizador; // comentarioAcompanhamentoOrcamentarioDTO
}

class acompanhamentoPlanoOrcamentarioDTO
{
    public $planoOrcamentario; // -- string(4)
    public $realizadoLOA; // -- long
    public $dataApuracaoLOA; // -- dateTime
    public $analisesPlanoOrcamentario; // -- AnaliseAcompanhamentoOrcamentarioDTO
}

class comentarioAcompanhamentoOrcamentarioDTO {
  public $comentario; // string
  public $comentarioId; // int
  public $nomeUsuario; // string
  public $periodoOrdem; // int
  public $ultimaModificacao; // dateTime
}

class cadastrarAcompanhamentoOrcamentarioResponse {
  public $return; // retornoAcompanhamentoOrcamentarioDTO
}

class retornoAcompanhamentoOrcamentarioDTO {
  public $acompanhamentosAcoes; // acompanhamentosAcoes
}

class acompanhamentosAcoes {
  public $acompanhamentoAcao; // acompanhamentoOrcamentarioAcaoDTO
}

class consultarAcompanhamentoOrcamentario {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $periodoOrdem; // int
  public $codigoMomento; // int
  public $filtro; // filtroFuncionalProgramaticaDTO
  public $dataHoraReferencia; // dateTime
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

class consultarAcompanhamentoOrcamentarioResponse {
  public $return; // retornoAcompanhamentoOrcamentarioDTO
}

class consultarExecucaoOrcamentaria {
  public $credencial; // credencialDTO
  public $filtro; // filtroExecucaoOrcamentariaDTO
  public $selecaoRetorno; // selecaoRetornoExecucaoOrcamentariaDTO
  public $paginacao; // paginacaoDTO
}

class filtroExecucaoOrcamentariaDTO {
  public $acoes; // acoes
  public $acompanhamentosPO; // acompanhamentosPO
  public $anoReferencia; // int
  public $categoriasEconomicas; // categoriasEconomicas
  public $detalhesAcompanhamentoPO; // detalhesAcompanhamentoPO
  public $elementosDespesa; // elementosDespesa
  public $esferas; // esferas
  public $fontes; // fontes
  public $funcoes; // funcoes
  public $gruposNatureza; // gruposNatureza
  public $identificadoresAcompanhamentoPO; // identificadoresAcompanhamentoPO
  public $idocs; // idocs
  public $idusos; // idusos
  public $localizadores; // localizadores
  public $modalidadesAplicacao; // modalidadesAplicacao
  public $naturezasDespesa; // naturezasDespesa
  public $planosInternos; // planosInternos
  public $planosOrcamentarios; // planosOrcamentarios
  public $programas; // programas
  public $resultadosPrimariosAtuais; // resultadosPrimariosAtuais
  public $resultadosPrimariosLei; // resultadosPrimariosLei
  public $subfuncoes; // subfuncoes
  public $tematicasPO; // tematicasPO
  public $tiposCredito; // tiposCredito
  public $tiposApropriacaoPO; // tiposApropriacaoPO
  public $unidadesOrcamentarias; // unidadesOrcamentarias
  public $unidadesGestorasResponsaveis; // unidadesGestorasResponsaveis
}

class acoes {
  public $acao; // string
}

class acompanhamentosPO {
  public $acompanhamentoPO; // string
}

class categoriasEconomicas {
  public $categoriaEconomicas; // string
}

class detalhesAcompanhamentoPO {
  public $detalheAcompanhamentoPO; // string
}

class elementosDespesa {
  public $elementoDespesa; // string
}

class esferas {
  public $esfera; // string
}

class fontes {
  public $fonte; // string
}

class funcoes {
  public $funcao; // string
}

class gruposNatureza {
  public $grupoNatureza; // string
}

class identificadoresAcompanhamentoPO {
  public $identificadorAcompanhamentoPO; // string
}

class idocs {
  public $idoc; // string
}

class idusos {
  public $iduso; // string
}

class localizadores {
  public $localizador; // string
}

class modalidadesAplicacao {
  public $modalidadeAplicacao; // string
}

class naturezasDespesa {
  public $naturezaDespesa; // string
}

class planosInternos {
  public $planoInterno; // string
}

class planosOrcamentarios {
  public $planoOrcamentario; // string
}

class programas {
  public $programa; // string
}

class resultadosPrimariosAtuais {
  public $resultadoPrimarioAtual; // string
}

class resultadosPrimariosLei {
  public $resultadoPrimarioLei; // string
}

class subfuncoes {
  public $subfuncao; // string
}

class tematicasPO {
  public $tematicaPO; // string
}

class tiposCredito {
  public $tipoCredito; // string
}

class tiposApropriacaoPO {
  public $tipoApropriacaoPO; // string
}

class unidadesOrcamentarias {
  public $unidadeOrcamentaria; // string
}

class unidadesGestorasResponsaveis {
  public $unidadeGestoraResponsavel; // string
}

class selecaoRetornoExecucaoOrcamentariaDTO {
  public $acao; // boolean
  public $acompanhamentoPO; // boolean
  public $anoExercicio; // boolean
  public $anoReferencia; // boolean
  public $autorizado; // boolean
  public $bloqueadoRemanejamento; // boolean
  public $bloqueadoSOF; // boolean
  public $categoriaEconomica; // boolean
  public $creditoContidoSOF; // boolean
  public $detalheAcompanhamentoPO; // boolean
  public $disponivel; // boolean
  public $dotAtual; // boolean
  public $dotInicialSiafi; // boolean
  public $dotacaoAntecipada; // boolean
  public $dotacaoInicial; // boolean
  public $elementoDespesa; // boolean
  public $empLiquidado; // boolean
  public $empenhadoALiquidar; // boolean
  public $esfera; // boolean
  public $executadoPorInscricaoDeRAP; // boolean
  public $fonte; // boolean
  public $funcao; // boolean
  public $grupoNaturezaDespesa; // boolean
  public $identificadorAcompanhamentoPO; // boolean
  public $idoc; // boolean
  public $iduso; // boolean
  public $indisponivel; // boolean
  public $localizador; // boolean
  public $modalidadeAplicacao; // boolean
  public $natureza; // boolean
  public $origem; // boolean
  public $pago; // boolean
  public $planoInterno; // boolean
  public $planoOrcamentario; // boolean
  public $programa; // boolean
  public $projetoLei; // boolean
  public $rapAPagarNaoProcessado; // boolean
  public $rapAPagarProcessado; // boolean
  public $rapCanceladosNaoProcessados; // boolean
  public $rapCanceladosProcessados; // boolean
  public $rapExerciciosAnteriores; // boolean
  public $rapInscritoNaoProcessado; // boolean
  public $rapInscritoProcessado; // boolean
  public $rapNaoProcessadoALiquidar; // boolean
  public $rapNaoProcessadoBloqueado; // boolean
  public $rapNaoProcessadoLiquidadoAPagar; // boolean
  public $rapPagoNaoProcessado; // boolean
  public $rapPagoProcessado; // boolean
  public $resultadoPrimarioAtual; // boolean
  public $resultadoPrimarioLei; // boolean
  public $subElementoDespesa; // boolean
  public $subFuncao; // boolean
  public $tematicaPO; // boolean
  public $tipoApropriacaoPO; // boolean
  public $tipoCredito; // boolean
  public $unidadeGestoraResponsavel; // boolean
  public $unidadeOrcamentaria; // boolean
  public $numeroptres; // string
}

class consultarExecucaoOrcamentariaResponse {
  public $return; // retornoExecucaoOrcamentariaDTO
}

class retornoExecucaoOrcamentariaDTO {
  public $dataHoraUltimaCarga; // dateTime
  public $execucoesOrcamentarias; // execucoesOrcamentarias
}

class execucoesOrcamentarias {
  public $execucaoOrcamentaria; // execucaoOrcamentariaDTO
}

class execucaoOrcamentariaDTO {
  public $acao; // string
  public $acompanhamentoPO; // string
  public $anoExercicio; // int
  public $anoReferencia; // int
  public $autorizado; // decimal
  public $bloqueadoRemanejamento; // decimal
  public $bloqueadoSOF; // decimal
  public $categoriaEconomica; // string
  public $creditoContidoSOF; // decimal
  public $detalheAcompanhamentoPO; // string
  public $disponivel; // decimal
  public $dotAtual; // decimal
  public $dotInicialSiafi; // decimal
  public $dotacaoAntecipada; // decimal
  public $dotacaoInicial; // decimal
  public $elementoDespesa; // string
  public $empLiquidado; // decimal
  public $empenhadoALiquidar; // decimal
  public $esfera; // string
  public $executadoPorInscricaoDeRAP; // decimal
  public $fonte; // string
  public $funcao; // string
  public $grupoNaturezaDespesa; // string
  public $identificadorAcompanhamentoPO; // string
  public $idoc; // string
  public $iduso; // string
  public $indisponivel; // decimal
  public $localizador; // string
  public $modalidadeAplicacao; // string
  public $natureza; // string
  public $origem; // string
  public $pago; // decimal
  public $planoInterno; // string
  public $planoOrcamentario; // string
  public $programa; // string
  public $projetoLei; // decimal
  public $rapAPagarNaoProcessado; // decimal
  public $rapAPagarProcessado; // decimal
  public $rapCanceladosNaoProcessados; // decimal
  public $rapCanceladosProcessados; // decimal
  public $rapExerciciosAnteriores; // decimal
  public $rapInscritoNaoProcessado; // decimal
  public $rapInscritoProcessado; // decimal
  public $rapNaoProcessadoALiquidar; // decimal
  public $rapNaoProcessadoBloqueado; // decimal
  public $rapNaoProcessadoLiquidadoAPagar; // decimal
  public $rapPagoNaoProcessado; // decimal
  public $rapPagoProcessado; // decimal
  public $resultadoPrimarioAtual; // string
  public $resultadoPrimarioLei; // string
  public $subElementoDespesa; // string
  public $subFuncao; // string
  public $tematicaPO; // string
  public $tipoApropriacaoPO; // string
  public $tipoCredito; // string
  public $unidadeGestoraResponsavel; // string
  public $unidadeOrcamentaria; // string
  public $numeroptres; // string
}

class consultarExecucaoOrcamentariaMensal extends consultarExecucaoOrcamentaria {
    public $mes;
//  public $credencial; // credencialDTO
//  public $parametros; // parametrosWebExecucaoOrcamentariaDTO
}

class consultarExecucaoOrcamentariaMensalResponse {
  public $return; // retornoExecucaoOrcamentariaMensalDTO
}

class retornoExecucaoOrcamentariaMensalDTO {
  public $registros; // execucaoOrcamentariaMensalDTO
}

class execucaoOrcamentariaMensalDTO {
  public $acao; // string
  public $descricaoFuncao; // string
  public $descricaoSubfuncao; // string
  public $exercicio; // int
  public $funcao; // string
  public $lei; // decimal
  public $leiMaisCreditos; // decimal
  public $localizador; // string
  public $ppipac; // boolean
  public $programa; // string
  public $realizadaAbril; // decimal
  public $realizadaAgosto; // decimal
  public $realizadaDezembro; // decimal
  public $realizadaFevereiro; // decimal
  public $realizadaJaneiro; // decimal
  public $realizadaJulho; // decimal
  public $realizadaJunho; // decimal
  public $realizadaMaio; // decimal
  public $realizadaMarco; // decimal
  public $realizadaNovembro; // decimal
  public $realizadaOutubro; // decimal
  public $realizadaSetembro; // decimal
  public $regiao; // string
  public $subfuncao; // string
  public $tituloAcao; // string
  public $tituloLocalizador; // string
  public $tituloPrograma; // string
  public $uf; // string
  public $unidadeOrcamentaria; // string
}






class consultarExecucaoOrcamentariaEstataisMensal {
  public $credencial; // credencialDTO
  public $parametros; // parametrosWebExecucaoOrcamentariaDTO
}

class parametrosWebExecucaoOrcamentariaDTO {
  public $acao; // string
  public $esfera; // string
  public $exercicio; // int
  public $funcao; // string
  public $localizador; // string
  public $orgao; // string
  public $programa; // string
  public $subFuncao; // string
  public $unidadeOrcamentaria; // string
}

class consultarExecucaoOrcamentariaEstataisMensalResponse {
  public $return; // retornoExecucaoOrcamentariaMensalDestDTO
}

class retornoExecucaoOrcamentariaMensalDestDTO {
  public $registros; // execucaoOrcamentariaMensalDestDTO
}

class execucaoOrcamentariaMensalDestDTO {
  public $acao; // string
  public $descricaoFuncao; // string
  public $descricaoSubfuncao; // string
  public $exercicio; // int
  public $funcao; // string
  public $lei; // decimal
  public $leiMaisCreditos; // decimal
  public $localizador; // string
  public $ppipac; // boolean
  public $programa; // string
  public $realizadaAbril; // decimal
  public $realizadaAgosto; // decimal
  public $realizadaDezembro; // decimal
  public $realizadaFevereiro; // decimal
  public $realizadaJaneiro; // decimal
  public $realizadaJulho; // decimal
  public $realizadaJunho; // decimal
  public $realizadaMaio; // decimal
  public $realizadaMarco; // decimal
  public $realizadaNovembro; // decimal
  public $realizadaOutubro; // decimal
  public $realizadaSetembro; // decimal
  public $regiao; // string
  public $subfuncao; // string
  public $tituloAcao; // string
  public $tituloLocalizador; // string
  public $tituloPrograma; // string
  public $uf; // string
  public $unidadeOrcamentaria; // string
}

class paginacaoDTO {
    public $pagina; // int
    public $registrosPorPagina; // int
}

class AnaliseAcompanhamentoOrcamentarioDTO {
    public $periodoOrdem;
    public $analise;
    public $nomeUsuario;
    public $ultimaModificacao;
}

class obterAcoesDisponiveisAcompanhamentoOrcamentario {
    public $credencial;
    public $exercicio;
    public $periodo;
}