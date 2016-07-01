<?php
/**
 * Mapeamento dos tipos de dados do WSQualitativo da SOF.
* $Id: QualitativoMap.php 85884 2014-09-01 13:46:31Z maykelbraz $
*/

/**
 * Mapeamento dos dados do WSQualitativo da SOF.
*/
class Spo_Ws_Sof_QualitativoMap
{
	/**
	 * Default class map for wsdl=>php
	 * @access private
	 * @var array
	 */
	public static $classmap = array(
		"obterOrgao" => "obterOrgao",
		"credencialDTO" => "credencialDTO",
		"baseDTO" => "baseDTO",
		"obterOrgaoResponse" => "obterOrgaoResponse",
		"retornoOrgaosDTO" => "retornoOrgaosDTO",
		"retornoDTO" => "retornoDTO",
		"orgaoDTO" => "orgaoDTO",
		"obterUnidadesOrcamentariasOrgao" => "obterUnidadesOrcamentariasOrgao",
		"obterUnidadesOrcamentariasOrgaoResponse" => "obterUnidadesOrcamentariasOrgaoResponse",
		"obterProgramasPorOrgao" => "obterProgramasPorOrgao",
		"obterProgramasPorOrgaoResponse" => "obterProgramasPorOrgaoResponse",
		"retornoProgramasDTO" => "retornoProgramasDTO",
		"programaDTO" => "programaDTO",
		"obterAcoesPorPrograma" => "obterAcoesPorPrograma",
		"obterAcoesPorProgramaResponse" => "obterAcoesPorProgramaResponse",
		"retornoAcoesDTO" => "retornoAcoesDTO",
		"acoes" => "acoes",
		"acaoDTO" => "acaoDTO",
		"localizadores" => "localizadores",
		"localizadorDTO" => "localizadorDTO",
		"obterAcoesPorOrgao" => "obterAcoesPorOrgao",
		"obterAcoesPorOrgaoResponse" => "obterAcoesPorOrgaoResponse",
		"obterIndicadoresPorPrograma" => "obterIndicadoresPorPrograma",
		"obterIndicadoresPorProgramaResponse" => "obterIndicadoresPorProgramaResponse",
		"retornoIndicadoresDTO" => "retornoIndicadoresDTO",
		"indicadorDTO" => "indicadorDTO",
		"obterObjetivosPorPrograma" => "obterObjetivosPorPrograma",
		"obterObjetivosPorProgramaResponse" => "obterObjetivosPorProgramaResponse",
		"retornoObjetivosDTO" => "retornoObjetivosDTO",
		"objetivoDTO" => "objetivoDTO",
		"obterMetasPorObjetivo" => "obterMetasPorObjetivo",
		"obterMetasPorObjetivoResponse" => "obterMetasPorObjetivoResponse",
		"retornoMetasDTO" => "retornoMetasDTO",
		"metaDTO" => "metaDTO",
		"obterRegionalizacoesPorMeta" => "obterRegionalizacoesPorMeta",
		"obterRegionalizacoesPorMetaResponse" => "obterRegionalizacoesPorMetaResponse",
		"retornoRegionalizacoesDTO" => "retornoRegionalizacoesDTO",
		"regionalizacaoDTO" => "regionalizacaoDTO",
		"obterIniciativasPorObjetivo" => "obterIniciativasPorObjetivo",
		"obterIniciativasPorObjetivoResponse" => "obterIniciativasPorObjetivoResponse",
		"retornoIniciativasDTO" => "retornoIniciativasDTO",
		"iniciativaDTO" => "iniciativaDTO",
		"obterMedidaInstitucionalPorIniciativa" => "obterMedidaInstitucionalPorIniciativa",
		"obterMedidaInstitucionalPorIniciativaResponse" => "obterMedidaInstitucionalPorIniciativaResponse",
		"retornoMedidaInstitucionalNormativaDTO" => "retornoMedidaInstitucionalNormativaDTO",
		"medidaInstitucionalNormativaDTO" => "medidaInstitucionalNormativaDTO",
		"obterFinanciamentoExtraOrcamentarioPorIniciativa" => "obterFinanciamentoExtraOrcamentarioPorIniciativa",
		"obterFinanciamentoExtraOrcamentarioPorIniciativaResponse" => "obterFinanciamentoExtraOrcamentarioPorIniciativaResponse",
		"retornoFinanciamentoExtraOrcamentarioDTO" => "retornoFinanciamentoExtraOrcamentarioDTO",
		"financiamentoExtraOrcamentarioDTO" => "financiamentoExtraOrcamentarioDTO",
		"obterAcoesPorIniciativa" => "obterAcoesPorIniciativa",
		"obterAcoesPorIniciativaResponse" => "obterAcoesPorIniciativaResponse",
		"obterLocalizadoresPorAcao" => "obterLocalizadoresPorAcao",
		"obterLocalizadoresPorAcaoResponse" => "obterLocalizadoresPorAcaoResponse",
		"retornoLocalizadoresDTO" => "retornoLocalizadoresDTO",
		"obterOrgaoPorCodigoSiorg" => "obterOrgaoPorCodigoSiorg",
		"obterOrgaoPorCodigoSiorgResponse" => "obterOrgaoPorCodigoSiorgResponse",
		"obterTabelasApoio" => "obterTabelasApoio",
		"obterTabelasApoioResponse" => "obterTabelasApoioResponse",
		"retornoApoioQualitativoDTO" => "retornoApoioQualitativoDTO",
		"baseGeograficaDTO" => "baseGeograficaDTO",
		"esferaDTO" => "esferaDTO",
		"funcaoDTO" => "funcaoDTO",
		"macroDesafioDTO" => "macroDesafioDTO",
		"momentoDTO" => "momentoDTO",
		"perfilDTO" => "perfilDTO",
		"periodicidadeDTO" => "periodicidadeDTO",
		"produtoDTO" => "produtoDTO",
		"regiaoDTO" => "regiaoDTO",
		"subFuncaoDTO" => "subFuncaoDTO",
		"tipoAcaoDTO" => "tipoAcaoDTO",
		"tipoInclusaoDTO" => "tipoInclusaoDTO",
		"tipoProgramaDTO" => "tipoProgramaDTO",
		"unidadeMedidaDTO" => "unidadeMedidaDTO",
		"unidadeMedidaIndicadorDTO" => "unidadeMedidaIndicadorDTO",
		"obterProgramacaoCompleta" => "obterProgramacaoCompleta",
		"obterProgramacaoCompletaResponse" => "obterProgramacaoCompletaResponse",
		"retornoProgramacaoQualitativoDTO" => "retornoProgramacaoQualitativoDTO",
		"agendaSamDTO" => "agendaSamDTO",
		"planoOrcamentarioDTO" => "planoOrcamentarioDTO",
		"obterMomentoCarga" => "obterMomentoCarga",
		"obterMomentoCargaResponse" => "obterMomentoCargaResponse",
		"retornoMomentoDTO" => "retornoMomentoDTO",
		"obterPlanosOrcamentariosPorAcao" => "obterPlanosOrcamentariosPorAcao",
		"obterPlanosOrcamentariosPorAcaoResponse" => "obterPlanosOrcamentariosPorAcaoResponse",
		"retornoPlanoOrcamentarioDTO" => "retornoPlanoOrcamentarioDTO",
		"obterAcaoPorIdentificadorUnico" => "obterAcaoPorIdentificadorUnico",
		"obterAcaoPorIdentificadorUnicoResponse" => "obterAcaoPorIdentificadorUnicoResponse",
	);
}	
	
class ObterUnidadesOrcamentariasOrgao
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoOrgao; // -- string
}

class ObterUnidadesOrcamentariasOrgaoResponse
{
	public $return; // -- RetornoOrgaosDTO
}

class RetornoOrgaosDTO
{
	public $registros; // -- OrgaoDTO
}

class OrgaoDTO
{
	public $codigoOrgao; // -- string
	public $codigoOrgaoPai; // -- string
	public $descricao; // -- string
	public $descricaoAbreviada; // -- string
	public $exercicio; // -- int
	public $orgaoId; // -- int
	public $orgaoSiorg; // -- string
	public $snAtivo; // -- boolean
	public $tipoOrgao; // -- string
}

class ObterProgramasPorOrgao
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoOrgao; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterProgramasPorOrgaoResponse
{
	public $return; // -- RetornoProgramasDTO
}

class RetornoProgramasDTO
{
	public $registros; // -- ProgramaDTO
}

class ProgramaDTO
{
	public $codigoMacroDesafio; // -- int
	public $codigoMomento; // -- int
	public $codigoOrgao; // -- string
	public $codigoPrograma; // -- string
	public $codigoTipoPrograma; // -- string
	public $estrategiaImplementacao; // -- string
	public $exercicio; // -- int
	public $horizonteTemporalContinuo; // -- int
	public $identificadorUnico; // -- int
	public $justificativa; // -- string
	public $objetivo; // -- string
	public $objetivoGoverno; // -- string
	public $objetivoSetorial; // -- string
	public $problema; // -- string
	public $publicoAlvo; // -- string
	public $snExclusaoLogica; // -- boolean
	public $titulo; // -- string
	public $unidadeResponsavel; // -- string
}

class ObterAcoesPorPrograma
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterAcoesPorProgramaResponse
{
	public $return; // -- RetornoAcoesDTO
}

class ObterOrgao
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoOrgao; // -- string
	public $tipoOrgao; // -- string
	public $dataHoraReferencia; // -- dateTime
}

class ObterOrgaoResponse
{
	public $return; // -- RetornoOrgaosDTO
}

class ObterAcoesPorOrgao
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoOrgao; // -- string
	public $codigoUo; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterAcoesPorOrgaoResponse
{
	public $return; // -- RetornoAcoesDTO
}

class ObterIndicadoresPorPrograma
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterIndicadoresPorProgramaResponse
{
	public $return; // -- RetornoIndicadoresDTO
}

class RetornoIndicadoresDTO
{
	public $registros; // -- IndicadorDTO
}

class IndicadorDTO
{
	public $codigoBaseGeografica; // -- int
	public $codigoIndicador; // -- int
	public $codigoMomento; // -- int
	public $codigoPeriodicidade; // -- int
	public $codigoPrograma; // -- string
	public $codigoUnidadeMedidaIndicador; // -- int
	public $dataApuracao; // -- dateTime
	public $descricao; // -- string
	public $exercicio; // -- int
	public $fonte; // -- string
	public $formula; // -- string
	public $identificadorUnico; // -- int
	public $snApuracaoReferencia; // -- boolean
	public $snExclusaoLogica; // -- boolean
	public $valorReferencia; // -- decimal
}

class ObterObjetivosPorPrograma
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterObjetivosPorProgramaResponse
{
	public $return; // -- RetornoObjetivosDTO
}

class RetornoObjetivosDTO
{
	public $registros; // -- ObjetivoDTO
}

class ObjetivoDTO
{
	public $codigoMomento; // -- int
	public $codigoObjetivo; // -- string
	public $codigoOrgao; // -- string
	public $codigoPrograma; // -- string
	public $enunciado; // -- string
	public $exercicio; // -- int
	public $identificadorUnico; // -- int
	public $snExclusaoLogica; // -- boolean
}

class ObterMetasPorObjetivo
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterMetasPorObjetivoResponse
{
	public $return; // -- RetornoMetasDTO
}

class RetornoMetasDTO
{
	public $registros; // -- MetaDTO
}

class MetaDTO
{
	public $codigoMeta; // -- int
	public $codigoMomento; // -- int
	public $codigoObjetivo; // -- string
	public $codigoPrograma; // -- string
	public $descricao; // -- string
	public $exercicio; // -- int
	public $identificadorUnico; // -- int
}

class ObterRegionalizacoesPorMeta
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoMeta; // -- int
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterRegionalizacoesPorMetaResponse
{
	public $return; // -- RetornoRegionalizacoesDTO
}

class RetornoRegionalizacoesDTO
{
	public $registros; // -- RegionalizacaoDTO
}

class RegionalizacaoDTO
{
	public $codigoMeta; // -- int
	public $codigoMomento; // -- int
	public $codigoObjetivo; // -- string
	public $codigoPrograma; // -- string
	public $codigoUnidadeMedida; // -- string
	public $descricao; // -- string
	public $exercicio; // -- int
	public $identificadorUnicoMeta; // -- int
	public $regionalizacaoId; // -- int
	public $sigla; // -- string
	public $valor; // -- decimal
}

class ObterIniciativasPorObjetivo
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterIniciativasPorObjetivoResponse
{
	public $return; // -- RetornoIniciativasDTO
}

class RetornoIniciativasDTO
{
	public $registros; // -- IniciativaDTO
}

class IniciativaDTO
{
	public $codigoIniciativa; // -- string
	public $codigoMomento; // -- int
	public $codigoObjetivo; // -- string
	public $codigoOrgao; // -- string
	public $codigoPrograma; // -- string
	public $exercicio; // -- int
	public $identificadorUnico; // -- int
	public $snExclusaoLogica; // -- boolean
	public $snIndividualizada; // -- boolean
	public $titulo; // -- string
}

class ObterMedidaInstitucionalPorIniciativa
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoIniciativa; // -- string
	public $codigoMomento; // -- int
}

class ObterMedidaInstitucionalPorIniciativaResponse
{
	public $return; // -- RetornoMedidaInstitucionalNormativaDTO
}

class RetornoMedidaInstitucionalNormativaDTO
{
	public $registros; // -- MedidaInstitucionalNormativaDTO
}

class MedidaInstitucionalNormativaDTO
{
	public $codigoMomento; // -- int
	public $codigoOrgaoSiorg; // -- string
	public $descricao; // -- string
	public $exercicio; // -- int
	public $identificadorIniciativa; // -- int
	public $identificadorUnico; // -- int
	public $produto; // -- string
	public $snExclusaoLogica; // -- boolean
}

class ObterFinanciamentoExtraOrcamentarioPorIniciativa
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoIniciativa; // -- string
	public $codigoMomento; // -- int
}

class ObterFinanciamentoExtraOrcamentarioPorIniciativaResponse
{
	public $return; // -- RetornoFinanciamentoExtraOrcamentarioDTO
}

class RetornoFinanciamentoExtraOrcamentarioDTO
{
	public $registros; // -- FinanciamentoExtraOrcamentarioDTO
}

class FinanciamentoExtraOrcamentarioDTO
{
	public $codigoMomento; // -- int
	public $codigoOrgaoSiorg; // -- string
	public $custoTotal; // -- double
	public $dataInicio; // -- dateTime
	public $dataTermino; // -- dateTime
	public $descricao; // -- string
	public $exercicio; // -- int
	public $fonteFinanciamento; // -- string
	public $identificadorIniciativa; // -- int
	public $identificadorUnico; // -- int
	public $outraFonteFinanciamento; // -- string
	public $produto; // -- string
	public $snProjeto; // -- boolean
	public $valorAno1Ppa; // -- double
	public $valorAno2Ppa; // -- double
	public $valorAno3Ppa; // -- double
	public $valorAno4Ppa; // -- double
	public $valorTotal; // -- double
}

class ObterAcoesPorIniciativa
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoPrograma; // -- string
	public $codigoObjetivo; // -- string
	public $codigoIniciativa; // -- string
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterAcoesPorIniciativaResponse
{
	public $return; // -- RetornoAcoesDTO
}

class ObterLocalizadoresPorAcao
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $identificadorUnicoAcao; // -- int
	public $codigoMomento; // -- int
	public $dataHoraReferencia; // -- dateTime
}

class ObterLocalizadoresPorAcaoResponse
{
	public $return; // -- RetornoLocalizadoresDTO
}

class RetornoLocalizadoresDTO
{
	public $registros; // -- LocalizadorDTO
}

class ObterOrgaoPorCodigoSiorg
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoSiorg; // -- string
}

class ObterOrgaoPorCodigoSiorgResponse
{
	public $return; // -- RetornoOrgaosDTO
}

class ObterTabelasApoio
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $retornarMomentos; // -- boolean
	public $retornarEsferas; // -- boolean
	public $retornarTiposInclusao; // -- boolean
	public $retonarFuncoes; // -- boolean
	public $retornarSubFuncoes; // -- boolean
	public $retornarTiposAcao; // -- boolean
	public $retornarProdutos; // -- boolean
	public $retornarUnidadesMedida; // -- boolean
	public $retornarRegioes; // -- boolean
	public $retornarPerfis; // -- boolean
	public $retornarTiposPrograma; // -- boolean
	public $retornarMacroDesafios; // -- boolean
	public $retornarUnidadesMedidaIndicador; // -- boolean
	public $retornarPeriodicidades; // -- boolean
	public $retornarBasesGeograficas; // -- boolean
	public $dataHoraReferencia; // -- dateTime
}

class ObterTabelasApoioResponse
{
	public $return; // -- RetornoApoioQualitativoDTO
}

class RetornoApoioQualitativoDTO
{
	public $basesGeograficasDTO; // -- BaseGeograficaDTO
	public $esferasDTO; // -- EsferaDTO
	public $funcoesDTO; // -- FuncaoDTO
	public $macroDesafiosDTO; // -- MacroDesafioDTO
	public $momentosDTO; // -- MomentoDTO
	public $perfisDTO; // -- PerfilDTO
	public $periodicidadesDTO; // -- PeriodicidadeDTO
	public $produtosDTO; // -- ProdutoDTO
	public $regioesDTO; // -- RegiaoDTO
	public $subFuncoesDTO; // -- SubFuncaoDTO
	public $tiposAcaoDTO; // -- TipoAcaoDTO
	public $tiposInclusaoDTO; // -- TipoInclusaoDTO
	public $tiposProgramaDTO; // -- TipoProgramaDTO
	public $unidadesMedidaDTO; // -- UnidadeMedidaDTO
	public $unidadesMedidaIndicadorDTO; // -- UnidadeMedidaIndicadorDTO
}

class BaseGeograficaDTO
{
	public $codigoBaseGeografica; // -- int
	public $descricao; // -- string
	public $snAtivo; // -- boolean
	public $snExclusaoLogica; // -- boolean
}

class EsferaDTO
{
	public $codigoEsfera; // -- string
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $descricaoAbreviada; // -- string
	public $snAtivo; // -- boolean
	public $snValorizacao; // -- boolean
}

class FuncaoDTO
{
	public $codigoFuncao; // -- string
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $descricaoAbreviada; // -- string
	public $exercicio; // -- int
	public $snAtivo; // -- boolean
}

class MacroDesafioDTO
{
	public $codigoMacroDesafio; // -- int
	public $descricao; // -- string
	public $titulo; // -- string
}

class MomentoDTO
{
	public $codigoMomento; // -- int
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $snAtivo; // -- boolean
}

class PerfilDTO
{
	public $descricao; // -- string
	public $perfilId; // -- int
}

class PeriodicidadeDTO
{
	public $codigoPeriodicidade; // -- int
	public $descricao; // -- string
	public $snAtivo; // -- boolean
	public $snExclusaoLogica; // -- boolean
}

class ProdutoDTO
{
	public $codigoProduto; // -- int
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $snAtivo; // -- boolean
}

class RegiaoDTO
{
	public $codigoRegiao; // -- int
	public $descricao; // -- string
	public $sigla; // -- string
}

class SubFuncaoDTO
{
	public $codigoFuncao; // -- string
	public $codigoSubFuncao; // -- string
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $descricaoAbreviada; // -- string
	public $exercicio; // -- int
	public $snAtivo; // -- boolean
}

class TipoAcaoDTO
{
	public $codigoTipoAcao; // -- string
	public $descricao; // -- string
	public $snAtivo; // -- boolean
}

class TipoInclusaoDTO
{
	public $codigoTipoInclusao; // -- int
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $snAtivo; // -- boolean
}

class TipoProgramaDTO
{
	public $codigoTipoPrograma; // -- string
	public $descricao; // -- string
	public $exercicio; // -- int
	public $snAtivo; // -- boolean
}

class UnidadeMedidaDTO
{
	public $codigoUnidadeMedida; // -- string
	public $dataHoraAlteracao; // -- dateTime
	public $descricao; // -- string
	public $snAtivo; // -- boolean
}

class UnidadeMedidaIndicadorDTO
{
	public $codigoUnidadeMedidaIndicador; // -- int
	public $descricao; // -- string
	public $exercicio; // -- int
	public $snAtivo; // -- boolean
}

class ObterProgramacaoCompleta
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoMomento; // -- int
	public $retornarOrgaos; // -- boolean
	public $retornarProgramas; // -- boolean
	public $retornarIndicadores; // -- boolean
	public $retornarObjetivos; // -- boolean
	public $retornarIniciativas; // -- boolean
	public $retornarAcoes; // -- boolean
	public $retornarLocalizadores; // -- boolean
	public $retornarMetas; // -- boolean
	public $retornarRegionalizacoes; // -- boolean
	public $retornarPlanosOrcamentarios; // -- boolean
	public $retornarAgendaSam; // -- boolean
	public $retornarMedidasInstitucionaisNormativas; // -- boolean
	public $dataHoraReferencia; // -- dateTime
}

class ObterProgramacaoCompletaResponse
{
	public $return; // -- RetornoProgramacaoQualitativoDTO
}

class RetornoProgramacaoQualitativoDTO
{
	public $acoesDTO; // -- AcaoDTO
	public $agendasSamDTO; // -- AgendaSamDTO
	public $indicadoresDTO; // -- IndicadorDTO
	public $iniciativasDTO; // -- IniciativaDTO
	public $localizadoresDTO; // -- LocalizadorDTO
	public $medidasInstitucionaisNormativasDTO; // -- MedidaInstitucionalNormativaDTO
	public $metasDTO; // -- MetaDTO
	public $objetivosDTO; // -- ObjetivoDTO
	public $orgaosDTO; // -- OrgaoDTO
	public $planosOrcamentariosDTO; // -- PlanoOrcamentarioDTO
	public $programasDTO; // -- ProgramaDTO
	public $regionalizacoesDTO; // -- RegionalizacaoDTO
}

class AgendaSamDTO
{
	public $agendaSam; // -- string
	public $codigoAgendaSam; // -- int
	public $descricao; // -- string
}

class PlanoOrcamentarioDTO
{
	public $codigoIndicadorPlanoOrcamentario; // -- string
	public $codigoMomento; // -- int
	public $codigoProduto; // -- int
	public $codigoUnidadeMedida; // -- string
	public $dataHoraAlteracao; // -- dateTime
	public $detalhamento; // -- string
	public $exercicio; // -- int
	public $identificadorUnico; // -- int
	public $identificadorUnicoAcao; // -- int
	public $planoOrcamentario; // -- string
	public $snAtual; // -- boolean
	public $titulo; // -- string
}

class ObterMomentoCarga
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
}

class ObterMomentoCargaResponse
{
	public $return; // -- RetornoMomentoDTO
}

class RetornoMomentoDTO
{
	public $momento; // -- MomentoDTO
}

class ObterPlanosOrcamentariosPorAcao
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoMomento; // -- int
	public $identificadorUnicoAcao; // -- int
}

class ObterPlanosOrcamentariosPorAcaoResponse
{
	public $return; // -- RetornoPlanoOrcamentarioDTO
}

class RetornoPlanoOrcamentarioDTO
{
	public $registros; // -- PlanoOrcamentarioDTO
}

class ObterAcaoPorIdentificadorUnico
{
	public $credencial; // -- CredencialDTO
	public $exercicio; // -- int
	public $codigoMomento; // -- int
	public $identificadorUnico; // -- int
}

class ObterAcaoPorIdentificadorUnicoResponse
{
	public $return; // -- RetornoAcoesDTO
}