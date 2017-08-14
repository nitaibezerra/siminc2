<?php
/**
 * Implementa a classe de acesso ao webservice WSQualitativo da SOF.
 * $Id: Qualitativo.php 101880 2015-08-31 19:50:33Z maykelbraz $
 */

/**
 * Classe base com as configurações comuns para os webservices da SOF.
 * @see Spo_Ws_Sof
 */
require_once(dirname(__FILE__) . '/../Sof.php');

/**
 * Classes de mapeamento de dados para o webservice WSQualitativo da SOF.
 * @see WSQualitativoMap.php
 */
require_once(dirname(__FILE__) . '/QualitativoMap.php');

/**
 * Classe de acesso ao Webserviceo WSQualitativo da SOF.
 */
class Spo_Ws_Sof_Qualitativo extends Spo_Ws_Sof
{
    /**
     * Número de registros retornados por página.
     */
    const REGISTROS_POR_PAGINA = 2000;

    /**
     * {@inheritdoc}
     */
    protected function loadURL()
    {
        $this->urlWSDL = WEB_SERVICE_SIOP_URL. 'WSQualitativo?wsdl';
        return $this;
    }

    /**
     * {@inheritdoc}
     * @todo Atualizar o QuantitativoMap para usar com o loadClassMap do Sof.
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

	/**
	 * Service Call: obterAcaoPorIdentificadorUnico
	 * Parameter options:
	 * (obterAcaoPorIdentificadorUnico) obterAcaoPorIdentificadorUnico
	 * @param mixed,... See function description for parameter options
	 * @return obterAcaoPorIdentificadorUnicoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterAcaoPorIdentificadorUnico($mixed = null) {
		$validParameters = array(
			"(obterAcaoPorIdentificadorUnico)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterAcaoPorIdentificadorUnico", $args);
	}


	/**
	 * Service Call: obterAcoesPorIniciativa
	 * Parameter options:
	 * (obterAcoesPorIniciativa) obterAcoesPorIniciativa
	 * @param mixed,... See function description for parameter options
	 * @return obterAcoesPorIniciativaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterAcoesPorIniciativa($mixed = null) {
		$validParameters = array(
			"(obterAcoesPorIniciativa)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterAcoesPorIniciativa", $args);
	}


	/**
	 * Service Call: obterAcoesPorOrgao
	 * Parameter options:
	 * (obterAcoesPorOrgao) obterAcoesPorOrgao
	 * @param mixed,... See function description for parameter options
	 * @return obterAcoesPorOrgaoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterAcoesPorOrgao($mixed = null) {
		$validParameters = array(
			"(obterAcoesPorOrgao)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterAcoesPorOrgao", $args);
	}


	/**
	 * Service Call: obterAcoesPorPrograma
	 * Parameter options:
	 * (obterAcoesPorPrograma) obterAcoesPorPrograma
	 * @param mixed,... See function description for parameter options
	 * @return obterAcoesPorProgramaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterAcoesPorPrograma($mixed = null) {
		$validParameters = array(
			"(obterAcoesPorPrograma)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterAcoesPorPrograma", $args);
	}


	/**
	 * Service Call: obterFinanciamentoExtraOrcamentarioPorIniciativa
	 * Parameter options:
	 * (obterFinanciamentoExtraOrcamentarioPorIniciativa) obterFinanciamentoExtraOrcamentarioPorIniciativa
	 * @param mixed,... See function description for parameter options
	 * @return obterFinanciamentoExtraOrcamentarioPorIniciativaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterFinanciamentoExtraOrcamentarioPorIniciativa($mixed = null) {
		$validParameters = array(
			"(obterFinanciamentoExtraOrcamentarioPorIniciativa)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterFinanciamentoExtraOrcamentarioPorIniciativa", $args);
	}


	/**
	 * Service Call: obterIndicadoresPorPrograma
	 * Parameter options:
	 * (obterIndicadoresPorPrograma) obterIndicadoresPorPrograma
	 * @param mixed,... See function description for parameter options
	 * @return obterIndicadoresPorProgramaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterIndicadoresPorPrograma($mixed = null) {
		$validParameters = array(
			"(obterIndicadoresPorPrograma)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterIndicadoresPorPrograma", $args);
	}


	/**
	 * Service Call: obterIniciativasPorObjetivo
	 * Parameter options:
	 * (obterIniciativasPorObjetivo) obterIniciativasPorObjetivo
	 * @param mixed,... See function description for parameter options
	 * @return obterIniciativasPorObjetivoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterIniciativasPorObjetivo($obterIniciativasPorObjetivo) {
		$validParameters = array(
			"(ObterIniciativasPorObjetivo)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
        $obterIniciativasPorObjetivo->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterIniciativasPorObjetivo", $args);
	}


	/**
	 * Service Call: obterLocalizadoresPorAcao
	 * Parameter options:
	 * (obterLocalizadoresPorAcao) obterLocalizadoresPorAcao
	 * @param mixed,... See function description for parameter options
	 * @return obterLocalizadoresPorAcaoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterLocalizadoresPorAcao($mixed = null) {
		$validParameters = array(
			"(obterLocalizadoresPorAcao)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterLocalizadoresPorAcao", $args);
	}


	/**
	 * Service Call: obterMedidaInstitucionalPorIniciativa
	 * Parameter options:
	 * (obterMedidaInstitucionalPorIniciativa) obterMedidaInstitucionalPorIniciativa
	 * @param mixed,... See function description for parameter options
	 * @return obterMedidaInstitucionalPorIniciativaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterMedidaInstitucionalPorIniciativa($mixed = null) {
		$validParameters = array(
			"(obterMedidaInstitucionalPorIniciativa)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterMedidaInstitucionalPorIniciativa", $args);
	}


	/**
	 * Service Call: obterMetasPorObjetivo
	 * Parameter options:
	 * (obterMetasPorObjetivo) obterMetasPorObjetivo
	 * @param mixed,... See function description for parameter options
	 * @return obterMetasPorObjetivoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterMetasPorObjetivo($obterMetasPorObjetivo = null) {
		$validParameters = array(
			"(ObterMetasPorObjetivo)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
        $obterMetasPorObjetivo->credencial = $this->credenciais;

		return $this->getSoapClient()->call("obterMetasPorObjetivo", $args);
	}


	/**
	 * Service Call: obterMomentoCarga
	 * Parameter options:
	 * (obterMomentoCarga) obterMomentoCarga
	 * @logger true
	 * @name Momento carga
	 * @param mixed,... See function description for parameter options
	 * @return obterMomentoCargaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterMomentoCarga(ObterMomentoCarga $mixed = null) {
		$validParameters = array(
			"(ObterMomentoCarga)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$mixed->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterMomentoCarga", $args);
	}


	/**
	 * Service Call: obterObjetivosPorPrograma
	 * Parameter options:
	 * (obterObjetivosPorPrograma) obterObjetivosPorPrograma
	 * @param mixed,... See function description for parameter options
	 * @return obterObjetivosPorProgramaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterObjetivosPorPrograma(ObterObjetivosPorPrograma $obterObjetivosPorPrograma) {
		$validParameters = array(
			"(ObterObjetivosPorPrograma)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
        $obterObjetivosPorPrograma->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterObjetivosPorPrograma", $args);
	}


	/**
	 * Service Call: obterOrgao
	 * Parameter options:
	 * (obterOrgao) obterOrgao
	 * @param mixed,... See function description for parameter options
	 * @return obterOrgaoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterOrgao($mixed = null) {
		$validParameters = array(
			"(obterOrgao)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterOrgao", $args);
	}


	/**
	 * Service Call: obterOrgaoPorCodigoSiorg
	 * Parameter options:
	 * (obterOrgaoPorCodigoSiorg) obterOrgaoPorCodigoSiorg
	 * @param mixed,... See function description for parameter options
	 * @return obterOrgaoPorCodigoSiorgResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterOrgaoPorCodigoSiorg($mixed = null) {
		$validParameters = array(
			"(obterOrgaoPorCodigoSiorg)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterOrgaoPorCodigoSiorg", $args);
	}


	/**
	 * Service Call: obterPlanosOrcamentariosPorAcao
	 * Parameter options:
	 * (obterPlanosOrcamentariosPorAcao) obterPlanosOrcamentariosPorAcao
	 * @param mixed,... See function description for parameter options
	 * @return obterPlanosOrcamentariosPorAcaoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterPlanosOrcamentariosPorAcao($mixed = null) {
		$validParameters = array(
			"(obterPlanosOrcamentariosPorAcao)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterPlanosOrcamentariosPorAcao", $args);
	}


	/**
	 * Service Call: obterProgramacaoCompleta
	 * Parameter options:
	 * (obterProgramacaoCompleta) obterProgramacaoCompleta
	 * @logger true
	 * @name Programação completa
	 * @param mixed,... See function description for parameter options
	 * @return obterProgramacaoCompletaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterProgramacaoCompleta(ObterProgramacaoCompleta $mixed = null) {
		$validParameters = array(
			"(ObterProgramacaoCompleta)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$mixed->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterProgramacaoCompleta", $args);
	}


	/**
	 * Service Call: obterProgramasPorOrgao
	 * Parameter options:
	 * (obterProgramasPorOrgao) obterProgramasPorOrgao
	 * @param mixed,... See function description for parameter options
	 * @return obterProgramasPorOrgaoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterProgramasPorOrgao(ObterProgramasPorOrgao $obterProgramasPorOrgao = null) {
		$validParameters = array(
			"(ObterProgramasPorOrgao)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);

        $obterProgramasPorOrgao->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterProgramasPorOrgao", $args);
	}


	/**
	 * Service Call: obterRegionalizacoesPorMeta
	 * Parameter options:
	 * (obterRegionalizacoesPorMeta) obterRegionalizacoesPorMeta
	 * @param mixed,... See function description for parameter options
	 * @return obterRegionalizacoesPorMetaResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterRegionalizacoesPorMeta($mixed = null) {
		$validParameters = array(
			"(obterRegionalizacoesPorMeta)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterRegionalizacoesPorMeta", $args);
	}


	/**
	 * Service Call: obterTabelasApoio
	 * Parameter options:
	 * (ObterTabelasApoio) ObterTabelasApoio
	 * @logger true
	 * @name Tabelas de apoio
	 * @param $obterTabelasApoio
	 * @return obterTabelasApoioResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterTabelasApoio(ObterTabelasApoio $obterTabelasApoio = null) {
		$validParameters = array(
			"(ObterTabelasApoio)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		$obterTabelasApoio->credencial = $this->credenciais;
		return $this->getSoapClient()->call("obterTabelasApoio", $args);
	}


	/**
	 * Service Call: obterUnidadesOrcamentariasOrgao
	 * Parameter options:
	 * (obterUnidadesOrcamentariasOrgao) obterUnidadesOrcamentariasOrgao
	 * @param mixed,... See function description for parameter options
	 * @return obterUnidadesOrcamentariasOrgaoResponse
	 * @throws Exception invalid function signature message
	 */
	public function obterUnidadesOrcamentariasOrgao($mixed = null) {
		$validParameters = array(
			"(obterUnidadesOrcamentariasOrgao)",
		);
		$args = func_get_args();
		$this->_checkArguments($args, $validParameters);
		return $this->getSoapClient()->call("obterUnidadesOrcamentariasOrgao", $args);
	}
}
