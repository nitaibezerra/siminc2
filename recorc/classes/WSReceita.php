<?php
/**
 * Classe de abstração do webservice WSReceita da SOF.
 * @version $Id: WSReceita.php 88415 2014-10-14 17:23:37Z maykelbraz $
 */

/**
 * Base dos webservices da Sof.
 * @see Spo_Ws_Sof
 */
require(APPRAIZ . 'spo/ws/Sof.php');

require(dirname(__FILE__) . '/WSReceitaMap.php');

class WSReceita extends Spo_Ws_Sof
{
    /**
     * Indicação do ambiente de conexão.
     * @param int $env Ambiente de conexão do webservice.
     */
    public function __construct($env = null)
    {
        parent::__construct('recorc', $env);
    }

    protected function loadURL()
    {
        $this->urlWSDL = WEB_SERVICE_SIOP_URL. 'WSReceita?wsdl';
        return $this;
    }

    public function captarBaseExterna($codigoCaptacaoBaseExterna, $descricao, array $detalhesBaseExterna)
    {
        $captacaoBaseExterna = new CaptacaoBaseExternaDTO();
        $captacaoBaseExterna->codigoCaptacaoBaseExterna = $codigoCaptacaoBaseExterna;
        $captacaoBaseExterna->descricao = $descricao;
        $captacaoBaseExterna->detalhesBaseExterna = $this->criarDetalhesBaseExterna($detalhesBaseExterna);

        $captarBaseExterna = new CaptarBaseExterna();
        $captarBaseExterna->credencial = $this->credenciais;
        $captarBaseExterna->captacaoBaseExterna = $captacaoBaseExterna;

        return $this->soapClient->call('captarBaseExterna', array($captarBaseExterna));
    }

    protected function criarDetalhesBaseExterna(array $detalhesBaseExterna)
    {
        $retorno = array();
        foreach ($detalhesBaseExterna as $detalhe) {
            $captacaoDetalhesBaseExterna = new CaptacaoDetalheBaseExternaDTO();
            $captacaoDetalhesBaseExterna->codigoNaturezaReceita = $detalhe['codigoNaturezaReceita'];
            $captacaoDetalhesBaseExterna->codigoUnidadeRecolhedora = $detalhe['codigoUnidadeRecolhedora'];
            $captacaoDetalhesBaseExterna->subNatureza = $detalhe['subNatureza'];
            $captacaoDetalhesBaseExterna->justificativa = $detalhe['justificativa'];
            $captacaoDetalhesBaseExterna->metodologia = $detalhe['metodologia'];
            $captacaoDetalhesBaseExterna->memoriaDeCalculo = $detalhe['memoriaDeCalculo'];
            $captacaoDetalhesBaseExterna->valoresBaseExterna = $this->criarCaptacaoValorBaseExterna(
                $detalhe['valoresBaseExterna']
            );
            $retorno[] = $captacaoDetalhesBaseExterna;
        }

        if (empty($retorno)) {
            return null;
        }

        return $retorno;
    }

    protected function criarCaptacaoValorBaseExterna(array $valores)
    {
        $retorno = array();
        foreach ($valores as $valor) {
            $captacaoValorBaseExterna = new CaptacaoValorBaseExternaDTO();
            $captacaoValorBaseExterna->exercicio = $valor['exercicio'];
            $captacaoValorBaseExterna->valor = $valor['valor'];
            $retorno[] = $captacaoValorBaseExterna;
        }

        if (empty($retorno)) {
            return null;
        }
        return $retorno;
    }

    public function consultarDetalhesPorGrupo($codigoCaptacaoBaseExterna, $grupoNaturezaReceita)
    {
        $consultarDetalhesPorGrupo = new ConsultarDetalhesPorGrupo();
        $consultarDetalhesPorGrupo->credencial = $this->credenciais;
        $consultarDetalhesPorGrupo->codigoCaptacaoBaseExterna = $codigoCaptacaoBaseExterna;
        $consultarDetalhesPorGrupo->grupoNaturezaReceita = $grupoNaturezaReceita;

        return $this->soapClient->call('consultarDetalhesPorGrupo', array($consultarDetalhesPorGrupo));
    }

    public function consultarDisponibilidadeCaptacaoBaseExterna()
    {
        $consultarDisponibilidadeCaptacaoBaseExterna = new ConsultarDisponibilidadeCaptacaoBaseExterna();
        $consultarDisponibilidadeCaptacaoBaseExterna->credencial = $this->credenciais;

        return $this->soapClient->call(
            'consultarDisponibilidadeCaptacaoBaseExterna',
            array($consultarDisponibilidadeCaptacaoBaseExterna)
        );
    }
}
