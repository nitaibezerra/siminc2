<?php
class credencialDTO {
  public $perfil; // int
  public $senha; // string
  public $usuario; // string
}

class obterAnalisesEmendas {
  public $CredencialDTO; // credencialDTO
  public $FiltroAnaliseEmendaDTO; // filtroAnaliseEmendaDTO
}

class cadastrarAnalisesEmendas {
  public $CredencialDTO; // credencialDTO
  public $Integer; // int
  public $AnaliseEmendaDTO; // analiseEmendaDTO
}

class analiseEmendaDTO {
  public $identificadorUnicoLocalizador; // int
  public $esfera; // string
  public $codigoUO; // string
  public $codigoPrograma; // string
  public $codigoFuncao; // string
  public $codigoSubFuncao; // string
  public $codigoAcao; // string
  public $codigoLocalizador; // string
  public $naturezaDespesa; // string
  public $resultadoPrimario; // string
  public $fonte; // string
  public $idUso; // string
  public $codigoParlamentar; // int
  public $numeroEmenda; // int
  public $siglaPartido; // string
  public $ufParlamentar; // string
  public $valorAtual; // long
  public $codigoMomento; // int
  public $indicadorImpedimento; // string
  public $snValidado; // boolean
  public $snAtual; // boolean
  public $valorImpedimento; // long
  public $codigosImpedimento; // codigosImpedimento
  public $justificativa; // string
}

class filtroAnaliseEmendaDTO {
  public $exercicio; // int
  public $codigoOrgao; // string
  public $codigosUO; // codigosUO
  public $codigosParlamentar; // codigosParlamentar
  public $codigoMomento; // int
  public $indicadorImpedimento; // string
  public $snAtual; // boolean
  public $snValidado; // boolean
}

class codigosUO {
  public $codigoUO; // string
}

class codigosParlamentar {
  public $codigoParlamentar; // string
}

class obterAnalisesEmendasResponse {
  public $return; // retornoAnaliseEmendaDTO
}

class retornoAnaliseEmendaDTO {
  public $analisesEmenda; // analisesEmenda
}

class analisesEmenda {
  public $analiseEmenda; // analiseEmendaDTO
}

class codigosImpedimento {
  public $codigoImpedimento; // int
}

class cadastrarAnalisesEmendasResponse {
  public $return; // retornoCadastrarAnaliseEmendaDTO
}

class retornoCadastrarAnaliseEmendaDTO {
  public $analiseEmendaDTO; // analiseEmendaDTO
  public $pendencias; // pendencias
}

class pendencias {
  public $pendencia; // string
}

/**
 * WSEmendaImpositivoSIOP class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class WSEmendaImpositivoSIOP extends SoapClient {
  protected $soapClient;
  private $tipolog;
  public $emdid;
  public $emecod;
  private $logid;
  private $dbws; 

  private static $classmap = array(
                                    
                                    'credencialDTO' => 'credencialDTO',
                                    'obterAnalisesEmendas' => 'obterAnalisesEmendas',
                                    'filtroAnaliseEmendaDTO' => 'filtroAnaliseEmendaDTO',
                                    'codigosUO' => 'codigosUO',
                                    'codigosParlamentar' => 'codigosParlamentar',
                                    'obterAnalisesEmendasResponse' => 'obterAnalisesEmendasResponse',
                                    'retornoAnaliseEmendaDTO' => 'retornoAnaliseEmendaDTO',
                                    'analisesEmenda' => 'analisesEmenda',
  									'pendencias' => 'pendencias',
                                    'analiseEmendaDTO' => 'analiseEmendaDTO',
                                    'codigosImpedimento' => 'codigosImpedimento',
                                    'cadastrarAnalisesEmendas' => 'cadastrarAnalisesEmendas',
                                    'cadastrarAnalisesEmendasResponse' => 'cadastrarAnalisesEmendasResponse',
                                    'retornoCadastrarAnaliseEmendaDTO' => 'retornoCadastrarAnaliseEmendaDTO',
                                   );

  public function WSEmendaImpositivoSIOP($wsdl = "https://testews.siop.gov.br/services/WSAlteracoesOrcamentarias?wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    
    $this->tipolog = '';
    $this->logid = '';
    $this->soapClient = $client;
    
    parent::__construct($wsdl, $options);
  }
  
  /**
   *  
   *
   * @param obterAnalisesEmendas $obterAnalisesEmendas
   * @return obterAnalisesEmendasResponse
   */
  public function obterAnalisesEmendas(obterAnalisesEmendas $obterAnalisesEmendas) {
  	$this->tipolog = 'obterAnalisesEmendas';
  	
    return $this->__soapCall('obterAnalisesEmendas', array($obterAnalisesEmendas),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param cadastrarAnalisesEmendas $cadastrarAnalisesEmendas
   * @return cadastrarAnalisesEmendasResponse
   */
  public function cadastrarAnalisesEmendas(cadastrarAnalisesEmendas $cadastrarAnalisesEmendas) {
  	$this->tipolog = 'cadastrarAnalisesEmendas';
  	
    return $this->__soapCall('cadastrarAnalisesEmendas', array($cadastrarAnalisesEmendas),       array(
            'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
            'soapaction' => ''
           )
      );
  }

  
  	function __doRequest($request, $location, $action, $version, $one_way = 0){
  		
    	$this->log($request, $location, $action, $version, 'request');
         
    	$response = SoapClient::__doRequest($request, $location, $action, $version, $one_way);
         
    	$this->log($response, $location, $action, $version, 'response');
        
    	return $response;
  	}
	
	public function log($request, $location, $action, $version, $tipo){
		global $db;
	       
		//ver( simec_htmlentities($request),d );
		$request = str_ireplace( "'", '"', $request );
		
		if( $this->tipolog ){
	    	if( $tipo == 'request' ){
	        	$sql = "INSERT INTO emenda.logerroswsdl(logtipo, logrequest, logresponse, usucpf, logdatasolicitacao, emecod, emdid) 
						VALUES ('{$this->tipolog}', '".utf8_decode($request)."', null, '{$_SESSION['usucpf']}', now(), '{$this->emecod}', {$this->emdid}) returning logid";
				$this->logid = $db->pegaUm( $sql );
				$_SESSION['siop']['logid'] = $this->logid; 
	         } else {
	         	$sql = "UPDATE emenda.logerroswsdl SET logresponse = '".utf8_decode($request)."' WHERE logid = ".$this->logid;
				$db->executar($sql);
	         }
         }
         $db->commit();
	}
	
	// route all other method calls directly to soapClient
	public function __call($method, $args){
        // you could also add method_exists check here
        return call_user_func_array(array($this->soapClient, $method), $args);
	}
}

?>
