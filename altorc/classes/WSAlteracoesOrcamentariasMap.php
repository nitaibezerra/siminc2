<?php
/**
 * Classes de mapeamento para o acesso ao serviço WSAlteracoesOrcamentarias.
 * $Id: WSAlteracoesOrcamentariasMap.php 99532 2015-06-30 21:18:12Z maykelbraz $
 */

class WSAlteracoesOrcamentariasMap
{
    public static function getClassmap()
    {
        return self::$classmap;
    }

    private static $classmap = array(
        'enviarPedidoAlteracao'=>'EnviarPedidoAlteracao',
        'credencialDTO'=>'CredencialDTO',
        'baseDTO'=>'BaseDTO',
        'enviarPedidoAlteracaoResponse'=>'EnviarPedidoAlteracaoResponse',
        'retornoVerificacaoPedidoAlteracaoDTO'=>'RetornoVerificacaoPedidoAlteracaoDTO',
        'verificacoes'=>'Verificacoes',
        'retornoDTO'=>'RetornoDTO',
        'verificacaoPedidoAlteracaoDTO'=>'VerificacaoPedidoAlteracaoDTO',
        'detalhes'=>'Detalhes',
        'obterPedidosAlteracao'=>'ObterPedidosAlteracao',
        'filtroFuncionalProgramaticaDTO'=>'FiltroFuncionalProgramaticaDTO',
        'obterPedidosAlteracaoResponse'=>'ObterPedidosAlteracaoResponse',
        'retornoPedidoAlteracaoDTO'=>'RetornoPedidoAlteracaoDTO',
        'pedidoAlteracaoDTO'=>'PedidoAlteracaoDTO',
        'fisicoPedidoAlteracaoDTO'=>'FisicoPedidoAlteracaoDTO',
        'financeiroPedidoAlteracaoDTO'=>'FinanceiroPedidoAlteracaoDTO',
        'respostaJustificativaDTO'=>'RespostaJustificativaDTO',
        'obterPedidosAlteracaoPorDescricao'=>'ObterPedidosAlteracaoPorDescricao',
        'obterPedidosAlteracaoPorDescricaoResponse'=>'ObterPedidosAlteracaoPorDescricaoResponse',
        'cadastrarPedidoAlteracao'=>'CadastrarPedidoAlteracao',
        'cadastrarPedidoAlteracaoResponse'=>'CadastrarPedidoAlteracaoResponse',
        'cadastrarPedidoAlteracaoRemanejamentoEmendas'=>'CadastrarPedidoAlteracaoRemanejamentoEmendas',
        'cadastrarPedidoAlteracaoRemanejamentoEmendasResponse'=>'CadastrarPedidoAlteracaoRemanejamentoEmendasResponse',
        'cadastrarPedidoPAC'=>'CadastrarPedidoPAC',
        'cadastrarPedidoPACResponse'=>'CadastrarPedidoPACResponse',
        'retornoPedidoPACDTO'=>'RetornoPedidoPACDTO',
        'obterPedidoAlteracao'=>'ObterPedidoAlteracao',
        'obterPedidoAlteracaoResponse'=>'ObterPedidoAlteracaoResponse',
        'excluirPedidoAlteracao'=>'ExcluirPedidoAlteracao',
        'excluirPedidoAlteracaoResponse'=>'ExcluirPedidoAlteracaoResponse',
        'obterPerguntaJustificativa'=>'ObterPerguntaJustificativa',
        'obterPerguntaJustificativaResponse'=>'ObterPerguntaJustificativaResponse',
        'retornoPerguntaJustificativaDTO'=>'RetornoPerguntaJustificativaDTO',
        'perguntaJustificativaDTO'=>'PerguntaJustificativaDTO',
        'obterPerguntasJustificativa'=>'ObterPerguntasJustificativa',
        'obterPerguntasJustificativaResponse'=>'ObterPerguntasJustificativaResponse',
        'obterTabelasApoioAlteracoesOrcamentarias'=>'ObterTabelasApoioAlteracoesOrcamentarias',
        'obterTabelasApoioAlteracoesOrcamentariasResponse'=>'ObterTabelasApoioAlteracoesOrcamentariasResponse',
        'retornoApoioAlteracoesOrcamentariasDTO'=>'RetornoApoioAlteracoesOrcamentariasDTO',
        'classificacaoAlteracaoDTO'=>'ClassificacaoAlteracaoDTO',
        'situacaoPedidoAlteracaoDTO'=>'SituacaoPedidoAlteracaoDTO',
        'tipoAlteracaoDTO'=>'TipoAlteracaoDTO',
        'tipoFonteRecursoDTO'=>'TipoFonteRecursoDTO',
        'tipoInstrumentoLegalDTO'=>'TipoInstrumentoLegalDTO',
        'verificarPedidoAlteracao'=>'VerificarPedidoAlteracao',
        'verificarPedidoAlteracaoResponse'=>'VerificarPedidoAlteracaoResponse',
        'consultarSituacaoTransmissaoSiafi'=>'ConsultarSituacaoTransmissaoSiafi',
        'consultarSituacaoTransmissaoSiafiResponse'=>'ConsultarSituacaoTransmissaoSiafiResponse',
        'retornoSituacaoTransmissaoSiafiDTO'=>'RetornoSituacaoTransmissaoSiafiDTO',
        'obterSaldosAcoesPAC'=>'ObterSaldosAcoesPAC',
        'obterSaldosAcoesPACResponse'=>'ObterSaldosAcoesPACResponse',
        'retornoSaldosBloqueioPAC'=>'RetornoSaldosBloqueioPAC',
        'saldoBloqueioDotacaoDTO'=>'SaldoBloqueioDotacaoDTO',
        'obterAnalisesEmendas'=>'ObterAnalisesEmendas',
        'filtroAnaliseEmendaDTO'=>'FiltroAnaliseEmendaDTO',
        'codigosUO'=>'CodigosUO',
        'codigosParlamentar'=>'CodigosParlamentar',
        'obterAnalisesEmendasResponse'=>'ObterAnalisesEmendasResponse',
        'retornoAnaliseEmendaDTO'=>'RetornoAnaliseEmendaDTO',
        'analisesEmenda'=>'AnalisesEmenda',
        'analiseEmendaDTO'=>'AnaliseEmendaDTO',
        'codigosImpedimento'=>'CodigosImpedimento',
        'cadastrarAnalisesEmendas'=>'CadastrarAnalisesEmendas',
        'cadastrarAnalisesEmendasResponse'=>'CadastrarAnalisesEmendasResponse',
        'retornoCadastrarAnaliseEmendaDTO'=>'RetornoCadastrarAnaliseEmendaDTO',
        'pendencias'=>'Pendencias',
    	'codigosParlamentares' => 'codigosParlamentares',
    	'obterEmendasAprovadas' => 'obterEmendasAprovadas',
    	'filtroEmendaAprovadaDTO' => 'filtroEmendaAprovadaDTO',
    	'codigosParlamentares' => 'codigosParlamentares',
    	'obterEmendasAprovadasResponse' => 'obterEmendasAprovadasResponse',
    );
}

class obterEmendasAprovadasResponse {
	public $return; // retornoEmendaAprovadaDTO
}

class retornoEmendaAprovadaDTO {
	public $emendasAprovadas; // emendasAprovadas
}

class EnviarPedidoAlteracao
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $identificadorUnico; // -- int
}

class EnviarPedidoAlteracaoResponse
{
    public $return; // -- RetornoVerificacaoPedidoAlteracaoDTO
}

class RetornoVerificacaoPedidoAlteracaoDTO
{
    public $verificacoes; // -- Verificacoes
}

class Verificacoes
{
    public $verificacao; // -- VerificacaoPedidoAlteracaoDTO
}

class VerificacaoPedidoAlteracaoDTO
{
    public $regra; // -- string
    public $passou; // -- boolean
    public $snInformativa; // -- boolean
    public $snConfirmacaoEnvio; // -- boolean
    public $detalhes; // -- Detalhes
}

class Detalhes
{
    public $detalhe; // -- string
}

class ObterPedidosAlteracao
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $codigoMomento; // -- int
    public $filtroFuncionalProgramatica; // -- FiltroFuncionalProgramaticaDTO
    public $dataHoraUltimaConsulta; // -- dateTime
}

class FiltroFuncionalProgramaticaDTO
{
    public $codigoAcao; // -- string
    public $codigoEsfera; // -- string
    public $codigoFuncao; // -- string
    public $codigoLocalizador; // -- string
    public $codigoPrograma; // -- string
    public $codigoSubFuncao; // -- string
    public $codigoTipoInclusaoAcao; // -- int
    public $codigoTipoInclusaoLocalizador; // -- int
    public $codigoUO; // -- string
    public $exercicio; // -- int
}

class ObterPedidosAlteracaoResponse
{
    public $return; // -- RetornoPedidoAlteracaoDTO
}

class RetornoPedidoAlteracaoDTO
{
    public $registros; // -- PedidoAlteracaoDTO
}

class PedidoAlteracaoDTO
{
    public $identificadorUnico; // int
    public $exercicio; // int
    public $codigoMomento; // int
    public $codigoClassificacaoAlteracao; // int
    public $codigoTipoAlteracao; // string
    public $snOrcamentoInvestimento = false; // boolean
//    public $codigoSituacaoPedidoAlteracao; // int
//    public $codigoInstrumentoLegal; // int
    public $descricao; // string
    public $codigoOrgao; // string
    public $fisicosPedidoAlteracao; // fisicoPedidoAlteracaoDTO
    public $respostasJustificativa; // respostaJustificativaDTO
//    public $nomeUsuarioCriacao; // string
//    public $loginUsuarioCriacao; // string
//    public $nomeUsuarioEfetivacao; // string
//    public $loginUsuarioEfetivacao; // string
//    public $nomeUsuarioEnvio; // string
//    public $loginUsuarioEnvio; // string
//    public $dataCriacao; // dateTime
//    public $dataEfetivacao; // dateTime
//    public $dataEnvio; // dateTime
//    public $snIntegracao; // boolean
//    public $snAtual; // boolean
//    public $snExclusaoLogica; // boolean
//    public $snAgregadora; // boolean
//    public $snEnviadoCongressoNacional; // boolean
//    public $snEmValidacaoExterna; // boolean
//    public $identificadorUnicoPedidoAgregador; // int
//    public $identificadorUnicoPedidoOrigem; // int
}

class FisicoPedidoAlteracaoDTO
{
    public $codigoAcao; // -- string
    public $codigoEsfera; // -- string
    public $codigoFuncao; // -- string
    public $codigoLocalizador; // -- string
    public $codigoPrograma; // -- string
    public $codigoSubFuncao; // -- string
    public $codigoTipoInclusaoAcao; // -- int
    public $codigoTipoInclusaoLocalizador; // -- int
    public $codigoUO; // -- string
    public $exercicio; // -- int
    public $listaFinanceiroPedidoAlteracaoDTO; // -- FinanceiroPedidoAlteracaoDTO
    public $quantidadeAcrescimo; // -- Long
    public $quantidadeReducao; // -- Long
}

class FinanceiroPedidoAlteracaoDTO
{
    public $codigoFonte; // -- string
    public $codigoIdOC; // -- string
    public $codigoIdUso; // -- string
    public $codigoNatureza; // -- string
    public $codigoRP; // -- string
    public $codigoRPLei; // -- string
    public $codigoTipoFonteRecurso; // -- int
    public $planoOrcamentario; // -- string
    public $valorCancelamento; // -- Long
    public $valorSuplementacao; // -- Long
}

class RespostaJustificativaDTO
{
    public $codigoPergunta; // -- int
    public $resposta; // -- string
}

class ObterPedidosAlteracaoPorDescricao
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $codigoMomento; // -- int
    public $descricao; // -- string
}

class ObterPedidosAlteracaoPorDescricaoResponse
{
    public $return; // -- RetornoPedidoAlteracaoDTO
}

class CadastrarPedidoAlteracao
{
    public $credencial; // -- CredencialDTO
    public $pedidoAlteracao; // -- PedidoAlteracaoDTO
}

class CadastrarPedidoAlteracaoResponse
{
    public $return; // -- RetornoPedidoAlteracaoDTO
}

class CadastrarPedidoAlteracaoRemanejamentoEmendas
{
    public $credencial; // -- CredencialDTO
    public $tipoCredito; // -- string
    public $restricao; // -- string
}

class CadastrarPedidoAlteracaoRemanejamentoEmendasResponse
{
    public $return; // -- RetornoPedidoAlteracaoDTO
}

class CadastrarPedidoPAC
{
    public $credencial; // -- CredencialDTO
    public $pedidoAlteracao; // -- PedidoAlteracaoDTO
}

class CadastrarPedidoPACResponse
{
    public $return; // -- RetornoPedidoPACDTO
}

class RetornoPedidoPACDTO
{
    public $ESB; // -- string
    public $fitaGerada; // -- boolean
    public $identificadorUnico; // -- int
    public $pedidoEfetivado; // -- boolean
    public $pedidoSalvo; // -- boolean
}

class ObterPedidoAlteracao
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $identificadorUnicoPedido; // -- int
    public $codigoMomento; // -- int
}

class ObterPedidoAlteracaoResponse
{
    public $return; // -- RetornoPedidoAlteracaoDTO
}

class ExcluirPedidoAlteracao
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $identificadorUnico; // -- int
}

class ExcluirPedidoAlteracaoResponse
{
    public $return; // -- RetornoPedidoAlteracaoDTO
}

class ObterPerguntaJustificativa
{
    public $credencial; // -- CredencialDTO
    public $codigoPergunta; // -- int
}

class ObterPerguntaJustificativaResponse
{
    public $return; // -- RetornoPerguntaJustificativaDTO
}

class RetornoPerguntaJustificativaDTO
{
    public $registros; // -- PerguntaJustificativaDTO
}

class PerguntaJustificativaDTO
{
    public $codigoPergunta; // -- int
    public $pergunta; // -- string
}

class ObterPerguntasJustificativa
{
    public $credencial; // -- CredencialDTO
    public $orcamentoInvestimento; // -- boolean
}

class ObterPerguntasJustificativaResponse
{
    public $return; // -- RetornoPerguntaJustificativaDTO
}

class ObterTabelasApoioAlteracoesOrcamentarias
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $retornarClassificacoesAlteracao; // -- boolean
    public $retornarTiposAlteracao; // -- boolean
    public $retornarSituacoesPedidoAlteracao; // -- boolean
    public $retornarTiposInstrumentoLegal; // -- boolean
    public $retornarTiposFonteRecurso; // -- boolean
    public $dataHoraReferencia; // -- dateTime
}

class ObterTabelasApoioAlteracoesOrcamentariasResponse
{
    public $return; // -- RetornoApoioAlteracoesOrcamentariasDTO
}

class RetornoApoioAlteracoesOrcamentariasDTO
{
    public $classificacoesAlteracaoDTO; // -- ClassificacaoAlteracaoDTO
    public $situacoesPedidoAlteracaoDTO; // -- SituacaoPedidoAlteracaoDTO
    public $tiposAlteracaoDTO; // -- TipoAlteracaoDTO
    public $tiposFonteRecursoDTO; // -- TipoFonteRecursoDTO
    public $tiposInstrumentoLegalDTO; // -- TipoInstrumentoLegalDTO
}

class ClassificacaoAlteracaoDTO
{
    public $codigoClassificacaoAlteracao; // -- int
    public $descricao; // -- string
    public $snAtivo; // -- boolean
    public $snTipoCredito; // -- boolean
}

class SituacaoPedidoAlteracaoDTO
{
    public $codigoSituacaoPedidoAlteracao; // -- int
    public $descricao; // -- string
    public $snAtivo; // -- boolean
}

class TipoAlteracaoDTO
{
    public $baseLegal; // -- string
    public $codigoClassificacaoAlteracao; // -- int
    public $codigoTipoAlteracao; // -- string
    public $descricao; // -- string
    public $exercicio; // -- int
    public $snOrcamentoInvestimento; // -- boolean
}

class TipoFonteRecursoDTO
{
    public $codigoTipoFonteRecurso; // -- int
    public $descricao; // -- string
}

class TipoInstrumentoLegalDTO
{
    public $codigoTipoInstrumentoLegal; // -- int
    public $descricao; // -- string
    public $snAtivo; // -- boolean
}

class VerificarPedidoAlteracao
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $identificadorUnico; // -- int
}

class VerificarPedidoAlteracaoResponse
{
    public $return; // -- RetornoVerificacaoPedidoAlteracaoDTO
}

class ConsultarSituacaoTransmissaoSiafi
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
    public $identificadorUnico; // -- int
}

class ConsultarSituacaoTransmissaoSiafiResponse
{
    public $return; // -- RetornoSituacaoTransmissaoSiafiDTO
}

class RetornoSituacaoTransmissaoSiafiDTO
{
    public $codigoSituacao; // -- int
    public $descricaoSituacao; // -- string
    public $ESB; // -- string
}

class ObterSaldosAcoesPAC
{
    public $credencial; // -- CredencialDTO
    public $exercicio; // -- int
}

class ObterSaldosAcoesPACResponse
{
    public $return; // -- RetornoSaldosBloqueioPAC
}

class RetornoSaldosBloqueioPAC
{
    public $saldoBloqueioDotacao; // -- SaldoBloqueioDotacaoDTO
}

class SaldoBloqueioDotacaoDTO
{
    public $anoExercicio; // -- int
    public $anoReferencia; // -- int
    public $bloqueioAtual; // -- decimal
    public $categoriaEconomica; // -- string
    public $celula; // -- string
    public $codigoAcao; // -- string
    public $codigoEsfera; // -- string
    public $codigoFonte; // -- string
    public $codigoFuncao; // -- string
    public $codigoIdOC; // -- string
    public $codigoIdUso; // -- string
    public $codigoLocalizador; // -- string
    public $codigoPrograma; // -- string
    public $codigoRP; // -- string
    public $codigoRPLei; // -- string
    public $codigoSubFuncao; // -- string
    public $codigoUnidadeOrcamentaria; // -- string
    public $dataGeracao; // -- dateTime
    public $dotacaoAtual; // -- decimal
    public $grupoNaturezaDespesa; // -- string
    public $indicadorFuncionalPac; // -- boolean
    public $indicadorRap; // -- boolean
    public $modalidadeDeAplicacao; // -- string
    public $planoOrcamentario; // -- string
    public $saldo; // -- decimal
    public $tipoCredito; // -- string
}

class obterEmendasAprovadas {
  public $CredencialDTO; // credencialDTO
  public $FiltroEmendaAprovadaDTO; // filtroEmendaAprovadaDTO
}

class filtroEmendaAprovadaDTO {
  public $exercicio; // int
  public $codigosUO; // codigosUO
  public $codigosParlamentares; // codigosParlamentares
}

class codigosParlamentares {
  public $codigoParlamentar; // int
}

class ObterAnalisesEmendas
{
    public $CredencialDTO; // -- CredencialDTO
    public $FiltroAnaliseEmendaDTO; // -- FiltroAnaliseEmendaDTO
}

class FiltroAnaliseEmendaDTO
{
    public $exercicio; // -- int
    public $codigoOrgao; // -- string
    public $codigosUO; // -- CodigosUO
    public $codigosParlamentar; // -- CodigosParlamentar
    public $codigoMomento; // -- int
    public $indicadorImpedimento; // -- string
    public $snAtual; // -- boolean
    public $snValidado; // -- boolean
}

class CodigosUO
{
    public $codigoUO; // -- string
}

class CodigosParlamentar
{
    public $codigoParlamentar; // -- string
}

class ObterAnalisesEmendasResponse
{
    public $return; // -- RetornoAnaliseEmendaDTO
}

class RetornoAnaliseEmendaDTO
{
    public $analisesEmenda; // -- AnalisesEmenda
}

class AnalisesEmenda
{
    public $analiseEmenda; // -- AnaliseEmendaDTO
}

class AnaliseEmendaDTO
{
    public $identificadorUnicoLocalizador; // -- int
    public $esfera; // -- string
    public $codigoUO; // -- string
    public $codigoPrograma; // -- string
    public $codigoFuncao; // -- string
    public $codigoSubFuncao; // -- string
    public $codigoAcao; // -- string
    public $codigoLocalizador; // -- string
    public $naturezaDespesa; // -- string
    public $resultadoPrimario; // -- string
    public $fonte; // -- string
    public $idUso; // -- string
    public $codigoParlamentar; // -- int
    public $numeroEmenda; // -- int
    public $siglaPartido; // -- string
    public $ufParlamentar; // -- string
    public $valorAtual; // -- Long
    public $codigoMomento; // -- int
    public $indicadorImpedimento; // -- string
    public $snValidado; // -- boolean
    public $snAtual; // -- boolean
    public $valorImpedimento; // -- Long
    public $codigosImpedimento; // -- CodigosImpedimento
    public $justificativa; // -- string
}

class CodigosImpedimento
{
    public $codigoImpedimento; // -- int
}

class CadastrarAnalisesEmendas
{
    public $CredencialDTO; // -- CredencialDTO
    public $Integer; // -- int
    public $AnaliseEmendaDTO; // -- AnaliseEmendaDTO
}

class CadastrarAnalisesEmendasResponse
{
    public $return; // -- RetornoCadastrarAnaliseEmendaDTO
}

class RetornoCadastrarAnaliseEmendaDTO
{
    public $analiseEmendaDTO; // -- AnaliseEmendaDTO
    public $pendencias; // -- Pendencias
}

class Pendencias
{
    public $pendencia; // -- string
}
