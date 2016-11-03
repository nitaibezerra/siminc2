<?php
/**
 * Implementa a classe de acesso ao webservice WSReceita da SOF.
 * $Id: Receita.php 101880 2015-08-31 19:50:33Z maykelbraz $
 */

/**
 * Classe base com as configurações comuns para os webservices da SOF.
 * @see Spo_Ws_Sof
 */
require_once(dirname(__FILE__) . '/../Sof.php');

/**
 * Classes de mapeamento de dados para o webservice WSReceita da SOF.
 * @see WSQualitativoMap.php
 */
require_once(dirname(__FILE__) . '/ReceitaMap.php');

/**
 * Classe de acesso ao Webserviceo WSReceita da SOF.
 */
class Spo_Ws_Sof_Receita extends Spo_Ws_Sof
{
    /**
     * {@inheritdoc}
     */
    protected function loadURL()
    {
        $this->urlWSDL = WEB_SERVICE_SIOP_URL. 'WSReceita?wsdl';
        return $this;
    }

    /**
     * {@inheritdoc}
     * @todo Atualizar o ReceitaMap para usar com o loadClassMap do Sof.
     */
    protected function loadClassMap()
    {
        $classMap = new Simec_SoapClient_ClassMap();
        $classMapClass = new ReflectionClass(get_class($this) . "Map");

        foreach ($classMapClass->getStaticPropertyValue('classmap') as $tipo => $classe) {
            $classMap->add($tipo, $classe);
        }

        return $classMap;
    }

	/**
	 * Checks if an argument list matches against a valid argument type list
	 * @param array $arguments The argument list to check
	 * @param array $validParameters A list of valid argument types
	 * @return boolean true if arguments match against validParameters
	 * @throws Exception invalid function signature message
	 */
	public function _checkArguments($arguments, $validParameters) {
		$variables = "";
		foreach ($arguments as $arg) {
		    $type = gettype($arg);
		    if ($type == "object") {
		        $type = get_class($arg);
		    }
		    $variables .= "(".$type.")";
		}
		if (!in_array($variables, $validParameters)) {
		    throw new Exception("Invalid parameter types: ".str_replace(")(", ", ", $variables));
		}
		return true;
	}

    public function captarBaseExterna($codigoCaptacaoBaseExterna, $descricao, array $detalhesBaseExterna, $ignorarSubnatureza = false)
    {
        $captacaoBaseExterna = new CaptacaoBaseExternaDTO();
        $captacaoBaseExterna->codigoCaptacaoBaseExterna = $codigoCaptacaoBaseExterna;
        $captacaoBaseExterna->descricao = $descricao;
        $captacaoBaseExterna->detalhesBaseExterna = $this->criarDetalhesBaseExterna($detalhesBaseExterna, $ignorarSubnatureza);

        $captarBaseExterna = new CaptarBaseExterna();
        $captarBaseExterna->credencial = $this->credenciais;
        $captarBaseExterna->captacaoBaseExterna = $captacaoBaseExterna;

        return $this->getSoapClient()->call('captarBaseExterna', array($captarBaseExterna));
    }

    protected function criarDetalhesBaseExterna(array $detalhesBaseExterna, $ignorarSubnatureza = false)
    {
        $retorno = array();
        foreach ($detalhesBaseExterna as $detalhe) {
            $captacaoDetalhesBaseExterna = new CaptacaoDetalheBaseExternaDTO();
            $captacaoDetalhesBaseExterna->codigoNaturezaReceita = $detalhe['codigoNaturezaReceita'];
            $captacaoDetalhesBaseExterna->codigoUnidadeRecolhedora = $detalhe['codigoUnidadeRecolhedora'];
            $captacaoDetalhesBaseExterna->subNatureza = $ignorarSubnatureza?null:$detalhe['subNatureza'];
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

    public function consultarDisponibilidadeCaptacaoBaseExterna(ConsultarDisponibilidadeCaptacaoBaseExterna $obj = null)
    {
		$validParameters = array(
			'(ConsultarDisponibilidadeCaptacaoBaseExterna)',
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$obj->credencial = $this->credenciais;

        return $this->getSoapClient()->call('consultarDisponibilidadeCaptacaoBaseExterna', $args);
    }

    public function consultarDetalhesPorGrupo(ConsultarDetalhesPorGrupo $obj = null)
    {
		$validParameters = array(
			'(ConsultarDetalhesPorGrupo)',
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$obj->credencial = $this->credenciais;

        return $this->getSoapClient()->call('consultarDetalhesPorGrupo', $args);
    }
}
