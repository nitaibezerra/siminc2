<?php
/**
 * Mapeamento dos tipos de dados do WSQuantitativo da SOF.
 * $Id: QuantitativoMap.php 85884 2014-09-01 13:46:31Z maykelbraz $
 */

/**
 * Mapeamento dos dados do WSQuantitativo da SOF.
 */
class Spo_Ws_QuantitativoMap
{
    public static $classmap = array(
        'consultarEmendasLocalizador' => 'consultarEmendasLocalizador',
        'credencialDTO' => 'credencialDTO',
        'baseDTO' => 'baseDTO',
        'localizadorDTO' => 'localizadorDTO',
        'consultarEmendasLocalizadorResponse' => 'consultarEmendasLocalizadorResponse',
        'retornoFinanceiroEmendasDTO' => 'retornoFinanceiroEmendasDTO',
        'financeiros' => 'financeiros',
        'retornoDTO' => 'retornoDTO',
        'financeiroEmendasDTO' => 'financeiroEmendasDTO',
        'emendas' => 'emendas',
        'financeiroDTO' => 'financeiroDTO',
        'emendaDTO' => 'emendaDTO',
        'consultarExecucaoOrcamentaria' => 'consultarExecucaoOrcamentaria',
        'filtroExecucaoOrcamentariaDTO' => 'filtroExecucaoOrcamentariaDTO',
        'acoes' => 'acoes',
        'acompanhamentosPO' => 'acompanhamentosPO',
        'categoriasEconomicas' => 'categoriasEconomicas',
        'detalhesAcompanhamentoPO' => 'detalhesAcompanhamentoPO',
        'elementosDespesa' => 'elementosDespesa',
        'esferas' => 'esferas',
        'fontes' => 'fontes',
        'funcoes' => 'funcoes',
        'gruposNatureza' => 'gruposNatureza',
        'identificadoresAcompanhamentoPO' => 'identificadoresAcompanhamentoPO',
        'idocs' => 'idocs',
        'idusos' => 'idusos',
        'localizadores' => 'localizadores',
        'modalidadesAplicacao' => 'modalidadesAplicacao',
        'naturezasDespesa' => 'naturezasDespesa',
        'planosInternos' => 'planosInternos',
        'planosOrcamentarios' => 'planosOrcamentarios',
        'programas' => 'programas',
        'resultadosPrimariosAtuais' => 'resultadosPrimariosAtuais',
        'resultadosPrimariosLei' => 'resultadosPrimariosLei',
        'subfuncoes' => 'subfuncoes',
        'tematicasPO' => 'tematicasPO',
        'tiposCredito' => 'tiposCredito',
        'tiposApropriacaoPO' => 'tiposApropriacaoPO',
        'unidadesOrcamentarias' => 'unidadesOrcamentarias',
        'unidadesGestorasResponsaveis' => 'unidadesGestorasResponsaveis',
        'selecaoRetornoExecucaoOrcamentariaDTO' => 'selecaoRetornoExecucaoOrcamentariaDTO',
        'paginacaoDTO' => 'paginacaoDTO',
        'consultarExecucaoOrcamentariaResponse' => 'consultarExecucaoOrcamentariaResponse',
        'retornoExecucaoOrcamentariaDTO' => 'retornoExecucaoOrcamentariaDTO',
        'execucoesOrcamentarias' => 'execucoesOrcamentarias',
        'execucaoOrcamentariaDTO' => 'execucaoOrcamentariaDTO',
        'consultarExecucaoOrcamentariaMensal' => 'consultarExecucaoOrcamentariaMensal',
        'consultarExecucaoOrcamentariaMensalResponse' => 'consultarExecucaoOrcamentariaMensalResponse',
        'consultarExecucaoOrcamentariaEstataisMensal' => 'consultarExecucaoOrcamentariaEstataisMensal',
        'parametrosWebExecucaoOrcamentariaDTO' => 'parametrosWebExecucaoOrcamentariaDTO',
        'consultarExecucaoOrcamentariaEstataisMensalResponse' => 'consultarExecucaoOrcamentariaEstataisMensalResponse',
        'retornoExecucaoOrcamentariaMensalDestDTO' => 'retornoExecucaoOrcamentariaMensalDestDTO',
        'execucaoOrcamentariaMensalDestDTO' => 'execucaoOrcamentariaMensalDestDTO',
        'consultarAcompanhamentoFisicoFinanceiro' => 'consultarAcompanhamentoFisicoFinanceiro',
        'consultarAcompanhamentoFisicoFinanceiroResponse' => 'consultarAcompanhamentoFisicoFinanceiroResponse',
        'retornoAcompanhamentoFisicoFinanceiroDTO' => 'retornoAcompanhamentoFisicoFinanceiroDTO',
        'acompanhamentoFisicoFinanceiroDTO' => 'acompanhamentoFisicoFinanceiroDTO',
        'cadastrarProposta' => 'cadastrarProposta',
        'propostaDTO' => 'propostaDTO',
        'metaPlanoOrcamentarioDTO' => 'metaPlanoOrcamentarioDTO',
        'receitaDTO' => 'receitaDTO',
        'cadastrarPropostaResponse' => 'cadastrarPropostaResponse',
        'retornoPropostasDTO' => 'retornoPropostasDTO',
        'consultarProposta' => 'consultarProposta',
        'consultarPropostaResponse' => 'consultarPropostaResponse',
        'excluirProposta' => 'excluirProposta',
        'excluirPropostaResponse' => 'excluirPropostaResponse',
        'obterTabelasApoioQuantitativo' => 'obterTabelasApoioQuantitativo',
        'obterTabelasApoioQuantitativoResponse' => 'obterTabelasApoioQuantitativoResponse',
        'retornoApoioQuantitativoDTO' => 'retornoApoioQuantitativoDTO',
        'idocs' => 'idocs',
        'idusos' => 'idusos',
        'fontes' => 'fontes',
        'resultadosPrimarios' => 'resultadosPrimarios',
        'naturezas' => 'naturezas',
        'idOcDTO' => 'idOcDTO',
        'idUsoDTO' => 'idUsoDTO',
        'fonteDTO' => 'fonteDTO',
        'resultadoPrimarioDTO' => 'resultadoPrimarioDTO',
        'naturezaDespesaDTO' => 'naturezaDespesaDTO',
        'obterProgramacaoCompletaQuantitativo' => 'obterProgramacaoCompletaQuantitativo',
        'obterProgramacaoCompletaQuantitativoResponse' => 'obterProgramacaoCompletaQuantitativoResponse',
        'obterInformacaoCaptacaoPLOA' => 'obterInformacaoCaptacaoPLOA',
        'parametroInformacaoCaptacaoPLOA' => 'parametroInformacaoCaptacaoPLOA',
        'obterInformacaoCaptacaoPLOAResponse' => 'obterInformacaoCaptacaoPLOAResponse',
        'retornoInformacaoCaptacaoPLOADTO' => 'retornoInformacaoCaptacaoPLOADTO',
        'registros' => 'registros',
        'informacaoCaptacaoPLOADTO' => 'informacaoCaptacaoPLOADTO',
        'obterExecucaoOrcamentariaSam' => 'obterExecucaoOrcamentariaSam',
        'obterExecucaoOrcamentariaSamResponse' => 'obterExecucaoOrcamentariaSamResponse',
        'retornoExecucaoOrcamentariaSamDTO' => 'retornoExecucaoOrcamentariaSamDTO',
        'execucaoOrcamentariaSamDTO' => 'execucaoOrcamentariaSamDTO',
        'cadastrarAcompanhamentoOrcamentario' => 'cadastrarAcompanhamentoOrcamentario',
        'acompanhamentoOrcamentarioAcaoDTO' => 'acompanhamentoOrcamentarioAcaoDTO',
        'acompanhamentosLocalizadores' => 'acompanhamentosLocalizadores',
        'acompanhamentoOrcamentarioLocalizadorDTO' => 'acompanhamentoOrcamentarioLocalizadorDTO',
        'analisesLocalizador' => 'analisesLocalizador',
        'comentariosRegionalizacao' => 'comentariosRegionalizacao',
        'acompanhamentosPlanoOrcamentario' => 'acompanhamentosPlanoOrcamentario',
        'analiseAcompanhamentoOrcamentarioDTO' => 'analiseAcompanhamentoOrcamentarioDTO',
        'acompanhamentoPlanoOrcamentarioDTO' => 'acompanhamentoPlanoOrcamentarioDTO',
        'analisesPlanoOrcamentario' => 'analisesPlanoOrcamentario',
        'cadastrarAcompanhamentoOrcamentarioResponse' => 'cadastrarAcompanhamentoOrcamentarioResponse',
        'retornoAcompanhamentoOrcamentarioDTO' => 'retornoAcompanhamentoOrcamentarioDTO',
        'acompanhamentosAcoes' => 'acompanhamentosAcoes',
        'alertas' => 'alertas',
        'pendencias' => 'pendencias',
        'consultarAcompanhamentoOrcamentario' => 'consultarAcompanhamentoOrcamentario',
        'filtroFuncionalProgramaticaDTO' => 'filtroFuncionalProgramaticaDTO',
        'consultarAcompanhamentoOrcamentarioResponse' => 'consultarAcompanhamentoOrcamentarioResponse',
    );
}

class consultarEmendasLocalizador {
  public $credencial; // credencialDTO
  public $localizador; // localizadorDTO
}

class credencialDTO {
  public $perfil; // int
  public $senha; // string
  public $usuario; // string
}

class baseDTO {
}

class localizadorDTO {
  public $codigoLocalizador; // string
  public $codigoMomento; // int
  public $codigoRegiao; // int
  public $codigoTipoInclusao; // int
  public $dataHoraAlteracao; // dateTime
  public $descricao; // string
  public $exercicio; // int
  public $identificadorUnico; // int
  public $identificadorUnicoAcao; // int
  public $justificativaRepercussao; // string
  public $mesAnoInicio; // dateTime
  public $mesAnoTermino; // dateTime
  public $municipio; // string
  public $snExclusaoLogica; // boolean
  public $totalFinanceiro; // double
  public $totalFisico; // double
  public $uf; // string
}

class consultarEmendasLocalizadorResponse {
  public $return; // retornoFinanceiroEmendasDTO
}

class retornoFinanceiroEmendasDTO {
  public $financeiros; // financeiros
}

class financeiros {
  public $financeiroEmendas; // financeiroEmendasDTO
}

class retornoDTO {
  public $mensagensErro; // string
  public $sucesso; // boolean
}

class financeiroEmendasDTO {
  public $emendas; // emendas
}

class emendas {
  public $emenda; // emendaDTO
}

class financeiroDTO {
//  public $codigoPlanoOrcamentario; // string
  public $expansaoConcedida; // long
  public $expansaoSolicitada; // long
  public $fonte; // string
  public $idOC; // string
  public $idUso; // string
  public $identificadorPlanoOrcamentario; // int
  public $naturezaDespesa; // string
  public $resultadoPrimarioAtual; // string
  public $resultadoPrimarioLei; // string
  public $valor; // long
}

class emendaDTO {
  public $exercicio; // int
  public $codAutor; // int
  public $decisaoParecerId; // int
  public $emendaNumero; // int
  public $emenda; // int
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
  public $anoExercicio; // int
  public $anoReferencia; // int
  public $categoriasEconomicas; // categoriasEconomicas
  public $detalhesAcompanhamentoPO; // detalhesAcompanhamentoPO
  public $elementosDespesa; // elementosDespesa
  public $esferas; // esferas
  public $estatalDependente; // boolean
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
  public $estatalDependente; // boolean
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
  public $numeroptres; // boolean
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
}

class paginacaoDTO {
  public $pagina; // int
  public $registrosPorPagina; // int
}

class consultarExecucaoOrcamentariaResponse {
  public $return; // retornoExecucaoOrcamentariaDTO
}

class retornoExecucaoOrcamentariaDTO {
  public $dataHoraUltimaCarga; // dateTime
  public $execucoesOrcamentarias; // execucoesOrcamentarias
  public $paginacao; // paginacaoDTO
}

class execucoesOrcamentarias {
  public $execucaoOrcamentaria; // execucaoOrcamentariaDTO
}

class execucaoOrcamentariaDTO {
  public $acao; // string
  public $acompanhamento; // string
  public $acompanhamentoPO; // string
  public $anoExercicio; // int
  public $anoReferencia; // int
  public $autorizado; // decimal
  public $bloqueadoRemanejamento; // decimal
  public $bloqueadoSOF; // decimal
  public $categoriaEconomica; // string
  public $credito; // string
  public $creditoContidoSOF; // decimal
  public $detalheAcompanhamento; // string
  public $detalheAcompanhamentoPO; // string
  public $disponivel; // decimal
  public $dotAntecipada; // decimal
  public $dotAtual; // decimal
  public $dotInicial; // decimal
  public $dotInicialSiafi; // decimal
  public $dotacaoAntecipada; // decimal
  public $dotacaoInicial; // decimal
  public $dotacaoOriginal; // decimal
  public $elementoDespesa; // string
  public $empALiquidar; // decimal
  public $empLiqInscrRapNp; // string
  public $empLiquidado; // decimal
  public $empenhadoALiquidar; // decimal
  public $esfera; // string
  public $executadoPorInscricaoDeRAP; // decimal
  public $fonte; // string
  public $funcao; // string
  public $grupoNaturezaDespesa; // string
  public $idOc; // string
  public $idUso; // string
  public $identificadorAcompanhamento; // string
  public $identificadorAcompanhamentoPO; // string
  public $indisponivel; // decimal
  public $localizador; // string
  public $mes; // int
  public $modalidadeAplicacao; // string
  public $natureza; // string
  public $numeroptres; // string
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
  public $rpAtual; // string
  public $rpLei; // string
  public $subElementoDespesa; // string
  public $subFuncao; // string
  public $tematica; // string
  public $tematicaPO; // string
  public $tipoApropriacao; // string
  public $tipoApropriacaoPO; // string
  public $tipoCredito; // string
  public $unidadeGestoraResponsavel; // string
  public $unidadeOrcamentaria; // string
}

class consultarExecucaoOrcamentariaMensal {
  public $credencial; // credencialDTO
  public $filtro; // filtroExecucaoOrcamentariaDTO
  public $selecaoRetorno; // selecaoRetornoExecucaoOrcamentariaDTO
  public $mes; // int
  public $paginacao; // paginacaoDTO
}

class consultarExecucaoOrcamentariaMensalResponse {
  public $return; // retornoExecucaoOrcamentariaDTO
}

class consultarExecucaoOrcamentariaEstataisMensal {
  public $credencial; // credencialDTO
  public $parametros; // parametrosWebExecucaoOrcamentariaDTO
  public $paginacao; // paginacaoDTO
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
  public $paginacao; // paginacaoDTO
  public $registros; // execucaoOrcamentariaMensalDestDTO
}

class execucaoOrcamentariaMensalDestDTO {
  public $acao; // string
  public $descricaoFuncao; // string
  public $descricaoSubfuncao; // string
  public $esfera; // string
  public $estatalDependente; // boolean
  public $estatalIndependente; // boolean
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

class consultarAcompanhamentoFisicoFinanceiro {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $periodo; // int
  public $momentoId; // int
  public $tipoCaptacao; // string
  public $paginacao; // paginacaoDTO
}

class consultarAcompanhamentoFisicoFinanceiroResponse {
  public $return; // retornoAcompanhamentoFisicoFinanceiroDTO
}

class retornoAcompanhamentoFisicoFinanceiroDTO {
  public $dataHoraUltimaCargaSiafi; // dateTime
  public $paginacao; // paginacaoDTO
  public $registros; // acompanhamentoFisicoFinanceiroDTO
}

class acompanhamentoFisicoFinanceiroDTO {
  public $acao; // string
  public $codigoPO; // string
  public $descricaoFuncao; // string
  public $descricaoLocalizador; // string
  public $descricaoOrgao; // string
  public $descricaoProduto; // string
  public $descricaoProdutoPO; // string
  public $descricaoSiorg; // string
  public $descricaoSubFuncao; // string
  public $descricaoUnidadeMedida; // string
  public $descricaoUnidadeMedidaPO; // string
  public $descricaoUo; // string
  public $dotacaoAtual; // decimal
  public $dotacaoAtualPO; // decimal
  public $dotacaoInicial; // decimal
  public $dotacaoInicialPO; // decimal
  public $esfera; // string
  public $exercicio; // int
  public $funcao; // string
  public $liquidado; // decimal
  public $liquidadoPO; // decimal
  public $liquidadoRAP; // decimal
  public $liquidadoRAPPO; // decimal
  public $localizador; // string
  public $momento; // string
  public $momentoId; // int
  public $orgao; // string
  public $orgaoSiorg; // string
  public $pago; // decimal
  public $pagoPO; // decimal
  public $periodo; // string
  public $produto; // int
  public $produtoPO; // int
  public $programa; // string
  public $quantidadeMetaLOA; // decimal
  public $quantidadeMetaLOAPO; // decimal
  public $realizadoLOA; // decimal
  public $realizadoPO; // decimal
  public $realizadoRAP; // decimal
  public $reprogramadoFinanceiro; // decimal
  public $reprogramadoFisico; // decimal
  public $subfuncao; // string
  public $tipoCaptacao; // string
  public $tituloAcao; // string
  public $tituloPO; // string
  public $tituloPrograma; // string
  public $unidadeMedida; // string
  public $unidadeMedidaPO; // string
  public $uo; // string
}

class cadastrarProposta {
  public $credencial; // credencialDTO
  public $proposta; // propostaDTO
}

class propostaDTO {
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
  public $expansaoFisicaConcedida; // long
  public $expansaoFisicaSolicitada; // long
  public $financeiros; // financeiroDTO
  public $identificadorUnicoAcao; // int
  public $justificativa; // string
  public $justificativaExpansaoConcedida; // string
  public $justificativaExpansaoSolicitada; // string
  public $metaPlanoOrcamentario; // metaPlanoOrcamentarioDTO
  public $quantidadeFisico; // long
//  public $receitas; // receitaDTO
  public $snAtual; // boolean
  public $valorFisico; // long
}

class metaPlanoOrcamentarioDTO {
  public $expansaoFisicaConcedida; // long
  public $expansaoFisicaSolicitada; // long
  public $identificadorUnicoPlanoOrcamentario; // int
  public $quantidadeFisico; // long
}

class receitaDTO {
  public $naturezaReceita; // string
  public $valor; // long
}

class cadastrarPropostaResponse {
  public $return; // retornoPropostasDTO
}

class retornoPropostasDTO {
  public $proposta; // propostaDTO
}

class consultarProposta {
  public $credencial; // credencialDTO
  public $proposta; // propostaDTO
}

class consultarPropostaResponse {
  public $return; // retornoPropostasDTO
}

class excluirProposta {
  public $credencial; // credencialDTO
  public $proposta; // propostaDTO
}

class excluirPropostaResponse {
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
  public $idocs; // idocs
  public $idusos; // idusos
  public $fontes; // fontes
  public $resultadosPrimarios; // resultadosPrimarios
  public $naturezas; // naturezas
}

class resultadosPrimarios {
  public $resultadoPrimario; // resultadoPrimarioDTO
}

class naturezas {
  public $natureza; // naturezaDespesaDTO
}

class idOcDTO {
  public $codigoIdOc; // string
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

class fonteDTO {
  public $codigoFonte; // string
  public $descricao; // string
  public $exercicio; // int
  public $snAtivo; // boolean
}

class resultadoPrimarioDTO {
  public $codigoResultadoPrimario; // string
  public $descricao; // string
  public $exercicio; // int
}

class naturezaDespesaDTO {
  public $codigoNatureza; // string
  public $elementoDescricao; // string
  public $elementoDescricaoAbreviada; // string
  public $exercicio; // int
  public $subElementoDescricao; // string
  public $subElementoDescricaoAbreviada; // string
}

class obterProgramacaoCompletaQuantitativo {
  public $credencial; // credencialDTO
  public $exercicio; // int
  public $codigoMomento; // int
  public $dataHoraReferencia; // dateTime
  public $paginacao; // paginacaoDTO
}

class obterProgramacaoCompletaQuantitativoResponse {
  public $return; // retornoPropostasDTO
}

class obterInformacaoCaptacaoPLOA {
  public $credencial; // credencialDTO
  public $parametro; // parametroInformacaoCaptacaoPLOA
}

class parametroInformacaoCaptacaoPLOA {
  public $exercicio; // int
  public $codigoMomento; // int
  public $codigoTipoDetalhamento; // string
  public $codigoOrgao; // string
  public $codigoUnidadeOrcamentaria; // string
  public $captados; // boolean
  public $captaveis; // boolean
}

class obterInformacaoCaptacaoPLOAResponse {
  public $return; // retornoInformacaoCaptacaoPLOADTO
}

class retornoInformacaoCaptacaoPLOADTO {
  public $registros; // registros
}

class registros {
  public $registro; // informacaoCaptacaoPLOADTO
}

class informacaoCaptacaoPLOADTO {
  public $codigoMomentoAcao; // int
  public $codigoMomentoJanelaAtual; // int
  public $codigoMomentoLocalizador; // int
  public $codigoMomentoPropostaAtual; // int
  public $codigoTipoDetalhamento; // string
  public $exercicio; // int
  public $funcional; // string
  public $identificadorUnicoAcao; // int
  public $identificadorUnicoLocalizador; // int
  public $podeCaptar; // boolean
  public $porQueNaoPodeCaptar; // string
  public $propostaValida; // boolean
  public $temJanela; // boolean
  public $temProposta; // boolean
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
  public $analisesLocalizador; // analisesLocalizador
  public $comentariosRegionalizacao; // comentariosRegionalizacao
  public $acompanhamentosPlanoOrcamentario; // acompanhamentosPlanoOrcamentario
}

class analisesLocalizador {
  public $analiseLocalizador; // analiseAcompanhamentoOrcamentarioDTO
}

class comentariosRegionalizacao {
  public $comentarioRegionalizacao; // analiseAcompanhamentoOrcamentarioDTO
}

class acompanhamentosPlanoOrcamentario {
  public $acompanhamentoPlanoOrcamentario; // acompanhamentoPlanoOrcamentarioDTO
}

class analiseAcompanhamentoOrcamentarioDTO {
  public $analise; // string
  public $comentarioId; // int
  public $nomeUsuario; // string
  public $periodoOrdem; // int
  public $ultimaModificacao; // dateTime
}

class acompanhamentoPlanoOrcamentarioDTO {
  public $planoOrcamentario; // string
  public $realizadoLOA; // long
  public $dataApuracaoLOA; // dateTime
  public $analisesPlanoOrcamentario; // analisesPlanoOrcamentario
}

class analisesPlanoOrcamentario {
  public $analisePlanoOrcamentario; // analiseAcompanhamentoOrcamentarioDTO
}

class cadastrarAcompanhamentoOrcamentarioResponse {
  public $return; // retornoAcompanhamentoOrcamentarioDTO
}

class retornoAcompanhamentoOrcamentarioDTO {
  public $acompanhamentosAcoes; // acompanhamentosAcoes
  public $alertas; // alertas
  public $pendencias; // pendencias
}

class acompanhamentosAcoes {
  public $acompanhamentoAcao; // acompanhamentoOrcamentarioAcaoDTO
}

class alertas {
  public $alerta; // string
}

class pendencias {
  public $pendencia; // string
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
  public $exercicio; // int
}

class consultarAcompanhamentoOrcamentarioResponse {
  public $return; // retornoAcompanhamentoOrcamentarioDTO
}
