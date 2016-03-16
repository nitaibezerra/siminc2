<?php
/**
 * Definição das classes de mapeamento do webservice quantitativo da SOF.
 * $Id: WSQuantitativo.class.inc 94501 2015-02-26 14:28:20Z fellipesantos $
 */

/**
 * 
 */
class obterExecucaoOrcamentariaSam {
  public $credencial; // credencialDTO
  public $anoExercicio; // int
  public $planoInterno; // string
}

class credencialDTO {
  public $perfil; // int
  public $senha; // string
  public $usuario; // string
}

class baseDTO {
}

class obterExecucaoOrcamentariaSamResponse {
  public $return; // retornoExecucaoOrcamentariaSamDTO
}

class retornoExecucaoOrcamentariaSamDTO {
  public $registros; // execucaoOrcamentariaSamDTO
}

class retornoDTO {
  public $mensagensErro; // string
  public $sucesso; // boolean
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
  //public $identificadorUnicoAcao; // int
  public $justificativa; // string
  //public $metaPlanoOrcamentario; // metaPlanoOrcamentarioDTO
  public $quantidadeFisico; // long
  //public $receitas; // receitaDTO
  public $snAtual; // boolean
  public $valorFisico; // long
  public $expansaoFisicaSolicitada; // long
  public $expansaoFisicaConcedida; // long - retorno
  public $justificativaExpansaoSolicitada; // string 2500
  public $justificativaExpansaoConcedida; // -- string 2500 - retorno
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
  public $fonte; // string
  public $idOC; // string
  public $idUso; // string
  public $identificadorPlanoOrcamentario; // int
  public $naturezaDespesa; // string
  public $resultadoPrimarioAtual; // string
  public $resultadoPrimarioLei; // string
  public $valor; // long
  public $expansaoSolicitada; // long
}

class metaPlanoOrcamentarioDTO {
  public $identificadorUnicoPlanoOrcamentario; // int
  public $quantidadeFisico; // long
  public $expansaoFisicaSolicitada; // long
  public $expansaoFisicaConcedida; // long -- retorno
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

class acompanhamentoPlanoOrcamentarioDTO
{
    public $planoOrcamentario; // -- string(4)
    public $realizadoLOA; // -- long
    public $dataApuracaoLOA; // -- dateTime
}

class comentariosLocalizador {
  public $comentarioLocalizador; // comentarioAcompanhamentoOrcamentarioDTO
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

class consultarExecucaoOrcamentariaMensal {
  public $credencial; // credencialDTO
  public $parametros; // parametrosWebExecucaoOrcamentariaDTO
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

class AnaliseAcompanhamentoOrcamentarioDTO {
    public $periodoOrdem;
    public $analise;
    public $nomeUsuario;
    public $ultimaModificacao;
}

/**
 * WSQuantitativo class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class WSQuantitativo extends SoapClient
{
	protected $soapClient;
	private $arrProposta;
	private $tipolog;
	private $db;

    private static $classmap = array(
        'obterExecucaoOrcamentariaSam' => 'obterExecucaoOrcamentariaSam',
        'credencialDTO' => 'credencialDTO',
        'baseDTO' => 'baseDTO',
        'obterExecucaoOrcamentariaSamResponse' => 'obterExecucaoOrcamentariaSamResponse',
        'retornoExecucaoOrcamentariaSamDTO' => 'retornoExecucaoOrcamentariaSamDTO',
        'retornoDTO' => 'retornoDTO',
        'execucaoOrcamentariaSamDTO' => 'execucaoOrcamentariaSamDTO',
        'excluirProposta' => 'excluirProposta',
        'propostaDTO' => 'propostaDTO',
        'financeiroDTO' => 'financeiroDTO',
        'metaPlanoOrcamentarioDTO' => 'metaPlanoOrcamentarioDTO',
        'receitaDTO' => 'receitaDTO',
        'excluirPropostaResponse' => 'excluirPropostaResponse',
        'retornoPropostasDTO' => 'retornoPropostasDTO',
        'cadastrarProposta' => 'cadastrarProposta',
        'cadastrarPropostaResponse' => 'cadastrarPropostaResponse',
        'consultarProposta' => 'consultarProposta',
        'consultarPropostaResponse' => 'consultarPropostaResponse',
        'obterTabelasApoioQuantitativo' => 'obterTabelasApoioQuantitativo',
        'obterTabelasApoioQuantitativoResponse' => 'obterTabelasApoioQuantitativoResponse',
        'retornoApoioQuantitativoDTO' => 'retornoApoioQuantitativoDTO',
        'fonteDTO' => 'fonteDTO',
        'idUsoDTO' => 'idUsoDTO',
        'idOcDTO' => 'idOcDTO',
        'naturezaDespesaDTO' => 'naturezaDespesaDTO',
        'resultadoPrimarioDTO' => 'resultadoPrimarioDTO',
        'obterProgramacaoCompletaQuantitativo' => 'obterProgramacaoCompletaQuantitativo',
        'obterProgramacaoCompletaQuantitativoResponse' => 'obterProgramacaoCompletaQuantitativoResponse',
        'cadastrarAcompanhamentoOrcamentario' => 'cadastrarAcompanhamentoOrcamentario',
        'acompanhamentoOrcamentarioAcaoDTO' => 'acompanhamentoOrcamentarioAcaoDTO',
        'acompanhamentosLocalizadores' => 'acompanhamentosLocalizadores',
        'acompanhamentoOrcamentarioLocalizadorDTO' => 'acompanhamentoOrcamentarioLocalizadorDTO',
        'comentariosLocalizador' => 'comentariosLocalizador',
        'comentarioAcompanhamentoOrcamentarioDTO' => 'comentarioAcompanhamentoOrcamentarioDTO',
        'cadastrarAcompanhamentoOrcamentarioResponse' => 'cadastrarAcompanhamentoOrcamentarioResponse',
        'retornoAcompanhamentoOrcamentarioDTO' => 'retornoAcompanhamentoOrcamentarioDTO',
        'acompanhamentosAcoes' => 'acompanhamentosAcoes',
        'consultarAcompanhamentoOrcamentario' => 'consultarAcompanhamentoOrcamentario',
        'filtroFuncionalProgramaticaDTO' => 'filtroFuncionalProgramaticaDTO',
        'consultarAcompanhamentoOrcamentarioResponse' => 'consultarAcompanhamentoOrcamentarioResponse',
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
        'consultarExecucaoOrcamentariaResponse' => 'consultarExecucaoOrcamentariaResponse',
        'retornoExecucaoOrcamentariaDTO' => 'retornoExecucaoOrcamentariaDTO',
        'execucoesOrcamentarias' => 'execucoesOrcamentarias',
        'execucaoOrcamentariaDTO' => 'execucaoOrcamentariaDTO',
        'consultarExecucaoOrcamentariaEstataisMensal' => 'consultarExecucaoOrcamentariaEstataisMensal',
        'parametrosWebExecucaoOrcamentariaDTO' => 'parametrosWebExecucaoOrcamentariaDTO',
        'consultarExecucaoOrcamentariaEstataisMensalResponse' => 'consultarExecucaoOrcamentariaEstataisMensalResponse',
        'retornoExecucaoOrcamentariaMensalDestDTO' => 'retornoExecucaoOrcamentariaMensalDestDTO',
        'execucaoOrcamentariaMensalDestDTO' => 'execucaoOrcamentariaMensalDestDTO',
        'consultarExecucaoOrcamentariaMensal' => 'consultarExecucaoOrcamentariaMensal',
        'consultarExecucaoOrcamentariaMensalResponse' => 'consultarExecucaoOrcamentariaMensalResponse',
        'retornoExecucaoOrcamentariaMensalDTO' => 'retornoExecucaoOrcamentariaMensalDTO',
        'execucaoOrcamentariaMensalDTO' => 'execucaoOrcamentariaMensalDTO',
        'analisesLocalizador' => 'analisesLocalizador',
        'analiseAcompanhamentoOrcamentarioDTO' => 'analiseAcompanhamentoOrcamentarioDTO',
        'acompanhamentosPlanoOrcamentario' => 'acompanhamentosPlanoOrcamentario',
        'acompanhamentoPlanoOrcamentarioDTO' => 'acompanhamentoPlanoOrcamentarioDTO'
    );

    public function WSQuantitativo($wsdl = "\wsdl2php\mec\WSQuantitativo.xml", $options = array())
    {

        if (('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin'])) {
            $options['proxy_host'] = 'proxy3.mec.gov.br';
            $options['proxy_port'] = '8080';
        }


        foreach (self::$classmap as $key => $value) {
            if (!isset($options['classmap'][$key])) {
                $options['classmap'][$key] = $value;
            }
        }


        $this->arrProposta = '';
        $this->logid = '';
        $this->soapClient = $client;
        $this->tipolog = '';
        $this->db = '';
        parent::__construct($wsdl, $options);
    }

  /**
   *  
   *
   * @param cadastrarAcompanhamentoOrcamentario $cadastrarAcompanhamentoOrcamentario
   * @return cadastrarAcompanhamentoOrcamentarioResponse
   */
  public function cadastrarAcompanhamentoOrcamentario(cadastrarAcompanhamentoOrcamentario $cadastrarAcompanhamentoOrcamentario, $arrProposta) {
  	$this->arrProposta = $arrProposta;
  	$this->tipolog = 'cadastrarAcompanhamentoOrcamentario';
  	
    return $this->__soapCall('cadastrarAcompanhamentoOrcamentario', array($cadastrarAcompanhamentoOrcamentario),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => '', 'compression' => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP
           )
      );
  }

  /**
   *  
   *
   * @param cadastrarProposta $cadastrarProposta
   * @return cadastrarPropostaResponse
   */
  public function cadastrarProposta(cadastrarProposta $cadastrarProposta, $arrProposta) {
  	$this->arrProposta = $arrProposta;
  	$this->tipolog = 'cadastrarProposta';
  	
    return $this->__soapCall('cadastrarProposta', array($cadastrarProposta),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }
  
  /**
   *  
   *
   * @param consultarAcompanhamentoOrcamentario $consultarAcompanhamentoOrcamentario
   * @return consultarAcompanhamentoOrcamentarioResponse
   */
  public function consultarAcompanhamentoOrcamentario(consultarAcompanhamentoOrcamentario $consultarAcompanhamentoOrcamentario, $arrProposta) {
  	$this->arrProposta = $arrProposta;
  	$this->tipolog = 'consultarAcompanhamentoOrcamentario';
  	
    return $this->__soapCall('consultarAcompanhamentoOrcamentario', array($consultarAcompanhamentoOrcamentario),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarExecucaoOrcamentaria $consultarExecucaoOrcamentaria
   * @return consultarExecucaoOrcamentariaResponse
   */
  public function consultarExecucaoOrcamentaria(consultarExecucaoOrcamentaria $consultarExecucaoOrcamentaria) {
    return $this->__soapCall('consultarExecucaoOrcamentaria', array($consultarExecucaoOrcamentaria),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }
  
  /**
   *  
   *
   * @param consultarExecucaoOrcamentariaEstataisMensal $consultarExecucaoOrcamentariaEstataisMensal
   * @return consultarExecucaoOrcamentariaEstataisMensalResponse
   */
  public function consultarExecucaoOrcamentariaEstataisMensal(consultarExecucaoOrcamentariaEstataisMensal $consultarExecucaoOrcamentariaEstataisMensal) {
    return $this->__soapCall('consultarExecucaoOrcamentariaEstataisMensal', array($consultarExecucaoOrcamentariaEstataisMensal),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarProposta $consultarProposta
   * @return consultarPropostaResponse
   */
  public function consultarProposta(consultarProposta $consultarProposta, $arrProposta) {
  	$this->arrProposta = $arrProposta;
  	$this->tipolog = 'consultarProposta';
  	
    return $this->__soapCall('consultarProposta', array($consultarProposta),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param excluirProposta $excluirProposta
   * @return excluirPropostaResponse
   */
  public function excluirProposta(excluirProposta $excluirProposta, $arrProposta) {
  	$this->arrProposta = $arrProposta;
  	$this->tipolog = 'excluirProposta';
  	
    return $this->__soapCall('excluirProposta', array($excluirProposta),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterExecucaoOrcamentariaSam $obterExecucaoOrcamentariaSam
   * @return obterExecucaoOrcamentariaSamResponse
   */
  public function obterExecucaoOrcamentariaSam(obterExecucaoOrcamentariaSam $obterExecucaoOrcamentariaSam) {
    return $this->__soapCall('obterExecucaoOrcamentariaSam', array($obterExecucaoOrcamentariaSam),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterProgramacaoCompletaQuantitativo $obterProgramacaoCompletaQuantitativo
   * @return obterProgramacaoCompletaQuantitativoResponse
   */
  public function obterProgramacaoCompletaQuantitativo(obterProgramacaoCompletaQuantitativo $obterProgramacaoCompletaQuantitativo) {
  	$this->tipolog = 'obterProgramacaoCompletaQuantitativo';
    return $this->__soapCall('obterProgramacaoCompletaQuantitativo', array($obterProgramacaoCompletaQuantitativo),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param obterTabelasApoioQuantitativo $obterTabelasApoioQuantitativo
   * @return obterTabelasApoioQuantitativoResponse
   */
  public function obterTabelasApoioQuantitativo(obterTabelasApoioQuantitativo $obterTabelasApoioQuantitativo) {
  	$this->tipolog = 'obterTabelasApoioQuantitativo';
    return $this->__soapCall('obterTabelasApoioQuantitativo', array($obterTabelasApoioQuantitativo),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }
  
	function __doRequest($request, $location, $action, $version, $one_way = 0) 
	    {
	    	$this->log($request, $location, $action, $version, 'request');
	         	
	         $response = SoapClient::__doRequest($request, $location, $action, $version, $one_way);
	         
	         $this->log($response, $location, $action, $version, 'response');
	        
	         return $response;
	    }
	
	    public function log($request, $location, $action, $version, $tipo)
	    {
	        // here you could add filterings to log only items, e.g.
	            // code to log item
	        global $db;
	        
	        if($db){
	    		$this->db = $db;			
			} else {
				$this->db = new cls_banco();
			}
			//if( $tipo == 'response' )ver(simec_htmlentities($request),d);
			
			$request = str_ireplace( "'", '"', $request );
	         if( $this->tipolog != 'naograva'  ){
		         if( $tipo == 'request' ){
			         $sql = "INSERT INTO elabrev.logerrows(logtipo, acacod, unicod, prgcod, loccod, prgano, usucpf, logdatainclusao, acaid, xmlrequest, xmlresponse) 
							VALUES ('{$this->tipolog}', '{$this->arrProposta['acacod']}', '{$this->arrProposta['unicod']}', '{$this->arrProposta['prgcod']}', '{$this->arrProposta['loccod']}', '{$_SESSION['exercicio']}', '{$_SESSION['usucpf']}', now(), ".($this->arrProposta['acaid'] ? "'".$this->arrProposta['acaid']."'" : 'null').", '$request', null) returning logid";
					 $this->logid = $this->db->pegaUm( $sql );
					 $_SESSION['elabrev']['logid'] = $this->logid; 
					 $_SESSION['elabrev']['logidarr'][]= $this->logid;
		         } else {
		         	$sql = "UPDATE elabrev.logerrows SET 
							  	logtipo = '{$this->tipolog}',
							  	acacod = '{$this->arrProposta['acacod']}',
							  	unicod = '{$this->arrProposta['unicod']}',
							  	prgcod = '{$this->arrProposta['prgcod']}',
							  	loccod = '{$this->arrProposta['loccod']}',
							 	prgano = '{$_SESSION['exercicio']}',
							  	acaid = ".($this->arrProposta['acaid'] ? "'".$this->arrProposta['acaid']."'" : 'null').",
							  	xmlresponse = '$request'
							WHERE 
							  	logid = ".$this->logid;
					$this->db->executar($sql);
		         }
		         $this->db->commit();
	         }
	    }
	
	    // route all other method calls directly to soapClient
	    public function __call($method, $args)
	    {
	        // you could also add method_exists check here
	        return call_user_func_array(array($this->soapClient, $method), $args);
	    }

}

?>
