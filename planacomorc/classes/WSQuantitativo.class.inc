<?php
/**
 * PLANACOMORC
 * base: Programação Orçamentária
 * $Id: WSQuantitativo.class.inc 94320 2015-02-23 21:09:34Z OrionMesquita $
 */

require_once('ClassesDTO.class.inc');

/**
 * WSQuantitativo class
 */
class WSQuantitativo extends SoapClient {
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
      'acompanhamentoPlanoOrcamentarioDTO' => 'acompanhamentoPlanoOrcamentarioDTO',
      'paginacaoDTO' => 'paginacaoDTO');

  public function WSQuantitativo($wsdl = "\wsdl2php\mec\WSQuantitativo.xml", $options = array() ) {

    if (('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin'])) {
//        $options['proxy_host'] = 'proxy3.mec.gov.br';
//        $options['proxy_port'] = '8080';
    }

    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    $this->arrProposta = '';
    $this->logid = '';
    $this->soapClient = $client;
    $this->tipolog = '';
    $this->db = '';
//ver($wsdl, $options, d);
    parent::__construct($wsdl, $options);
  }

  /**
   *
   * @param consultarExecucaoOrcamentaria $consultarExecucaoOrcamentaria
   * @return consultarExecucaoOrcamentariaResponse
   */
  public function consultarExecucaoOrcamentaria(consultarExecucaoOrcamentaria $consultarExecucaoOrcamentaria) {
    return $this->__soapCall('consultarExecucaoOrcamentaria', array($consultarExecucaoOrcamentaria), array(
        'uri' => WEB_SERVICE_SIOP_URL,
        'soapaction' => ''));
  }
  /**
   *
   * @param consultarExecucaoOrcamentaria $consultarExecucaoOrcamentaria
   * @return consultarExecucaoOrcamentariaResponse
   */
  public function consultarExecucaoOrcamentariaMensal(consultarExecucaoOrcamentariaMensal $consultarExecucaoOrcamentariaMensal) {
    return $this->__soapCall('consultarExecucaoOrcamentariaMensal', array($consultarExecucaoOrcamentariaMensal), array(
        'uri' => WEB_SERVICE_SIOP_URL,
        'soapaction' => ''));
  }

  /**
   *
   * @param consultarAcompanhamentoOrcamentario $consultarAcompanhamentoOrcamentario
   * @return consultarAcompanhamentoOrcamentarioResponse
   */
  public function consultarAcompanhamentoOrcamentario(consultarAcompanhamentoOrcamentario $consultarAcompanhamentoOrcamentario) {
    return $this->__soapCall('consultarAcompanhamentoOrcamentario', array($consultarAcompanhamentoOrcamentario), array(
        'uri' => WEB_SERVICE_SIOP_URL,
        'soapaction' => ''));
  }

    /**
     * @param cadastrarAcompanhamentoOrcamentario $cadastrarAcompanhamentoOrcamentario
     * @return cadastrarAcompanhamentoOrcamentarioResponse
     */
    public function cadastrarAcompanhamentoOrcamentario(
        cadastrarAcompanhamentoOrcamentario $cadastrarAcompanhamentoOrcamentario,
        $arrProposta
    ) {
        $this->arrProposta = $arrProposta;
        $this->tipolog = 'cadastrarAcompanhamentoOrcamentario';

        return $this->__soapCall(
            'cadastrarAcompanhamentoOrcamentario',
            array($cadastrarAcompanhamentoOrcamentario),
            array(
                'uri' => WEB_SERVICE_SIOP_URL,
                'soapaction' => '',
                'compression' => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP
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

    return $this->__soapCall('cadastrarProposta', array($cadastrarProposta), array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => ''
           )
      );
  }

  public function obterAcoesDisponiveisAcompanhamentoOrcamentario(obterAcoesDisponiveisAcompanhamentoOrcamentario $obter)
  {
    return $this->__soapCall('obterAcoesDisponiveisAcompanhamentoOrcamentario', array($obter), array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
    return $this->__soapCall('consultarExecucaoOrcamentariaEstataisMensal', array($consultarExecucaoOrcamentariaEstataisMensal), array(
            'uri' => WEB_SERVICE_SIOP_URL,
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

    return $this->__soapCall('consultarProposta', array($consultarProposta), array(
            'uri' => WEB_SERVICE_SIOP_URL,
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

    return $this->__soapCall('excluirProposta', array($excluirProposta), array(
            'uri' => WEB_SERVICE_SIOP_URL,
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
    return $this->__soapCall('obterExecucaoOrcamentariaSam', array($obterExecucaoOrcamentariaSam), array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => '')
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
    return $this->__soapCall('obterProgramacaoCompletaQuantitativo', array($obterProgramacaoCompletaQuantitativo), array(
            'uri' => WEB_SERVICE_SIOP_URL,
            'soapaction' => '')
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
    return $this->__soapCall('obterTabelasApoioQuantitativo', array($obterTabelasApoioQuantitativo), array(
        'uri' => WEB_SERVICE_SIOP_URL,
        'soapaction' => '')
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
	         if( $this->tipolog != 'naograva' && is_array($this->arrProposta)){
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
