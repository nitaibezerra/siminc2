<?php
class Fies_Service_Abatimento_sent_object {
	
	private $strUsuario;
	private $strSenha;
	private $strNuCpf;
	private $strDtNascimento;
	private $strNoCliente;
	
	function __construct($dados = false){
		foreach( $dados as $k=>$dado ){
			$this->$k = $dado;
		}
	}
}
/**
 * Fies_Service_Abatimento_WebServiceService class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class Fies_Service_Abatimento_WebServiceService extends SoapClient {
	
	

  private static $classmap = array(
                                   );

  public function Fies_Service_Abatimento_WebServiceService($wsdl = "http://sisfiesdev.mec.gov.br/service/abatimento?wsdl", $options = 	Array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   * Metodo resnponsavel para validar e consultar o agente financeiro 
   *
   * @param string $strNuCpf
   * @param anyType $strDtNascimento
   * @param anyType $strNoCliente
   * @return string
   */
  public function verificarEstudanteFIESAtivo($sendObject) {
    return $this->__soapCall('verificarEstudanteFIESAtivo', array($sendObject), array(
            'http://sisfiesdev.mec.gov.br/service/abatimento?wsdl',
            'soapaction' => ''
           )
      );
  }
  

  
  
  
  

  /**
   *  
   *
   * @param string $strNuCpf
   * @param string $strDtNascimento
   * @param string $strQtMeseTrabalhado
   * @param string $strStSuspenderCobranca
   * @param string $strDtInicioSuspensao
   * @param string $strNoCliente
   * @return int
   */
  public function enviarSuspensaoCobranca($strNuCpf, $strDtNascimento, $strQtMeseTrabalhado, $strStSuspenderCobranca, $strDtInicioSuspensao, $strNoCliente) {
    return $this->__soapCall('enviarSuspensaoCobranca', array($strNuCpf, $strDtNascimento, $strQtMeseTrabalhado, $strStSuspenderCobranca, $strDtInicioSuspensao, $strNoCliente),       array(
            'uri' => 'http://sisfiesdev.mec.gov.br/service/abatimento?wsdl',
            'soapaction' => ''
           )
      );
  }

  /**
   * enviarEncerramentoSuspensao 
   *
   * @param  
   * @return void
   */
  public function enviarEncerramentoSuspensao() {
    return $this->__soapCall('enviarEncerramentoSuspensao', array(),       array(
            'uri' => 'http://sisfiesdev.mec.gov.br/service/abatimento?wsdl',
            'soapaction' => ''
           )
      );
  }

}












































//class verificarEstudanteFIESAtivoIn {
//  public $strNuCpf; // string
//  public $strDtNascimento; // string
//  public $strNoCliente; // string
//}
//
//class verificarEstudanteFIESAtivoOut {
//  public $return; // verificarEstudanteFIESAtivoOut
//}
//
///**
// * WSQuantitativo class
// * 
// *  
// * 
// * @author    {author}
// * @copyright {copyright}
// * @package   {package}
// */
//class WSFinanceiro extends SoapClient {
// 
//  	private static $classmap = array(
//                                    'verificarEstudanteFIESAtivoIn' => 'verificarEstudanteFIESAtivoIn',
//                                    'verificarEstudanteFIESAtivoOut' => 'verificarEstudanteFIESAtivoOut'
//                                   );
//
//	public function WSFinanceiro($wsdl = "https://10.220.8.56/sisfies_dev/public/teste/wsdl?wsdl", $options = array()) {
//    	foreach(self::$classmap as $key => $value) {
//      		if(!isset($options['classmap'][$key])) {
//        	$options['classmap'][$key] = $value;
//      	}
//    }
//    parent::__construct($wsdl, $options);
//  	}
//
//
//  /**
//   *  
//   *
//   * @param verificarEstudanteFIESAtivoIn $verificarEstudanteFIESAtivoIn
//   * @return verificarEstudanteFIESAtivoOut
//   */
//	public function verificarEstudanteFIESAtivoIn(verificarEstudanteFIESAtivoIn $verificarEstudanteFIESAtivoIn) {
//  	//ver($verificarEstudanteFIESAtivoIn,d);
//  	/*
//  	 * $verificarEstudanteFIESAtivoIn[strNuCpf]
//  	 * $verificarEstudanteFIESAtivoIn[strDtNascimento]
//  	 * $verificarEstudanteFIESAtivoIn[strNoCliente]
//  	 * */
//    return $this->__soapCall('verificarEstudanteFIESAtivo', 
//    						 Array((array)$verificarEstudanteFIESAtivoIn) , 
//    						 Array('uri' => 'http://http://10.220.8.56', 
//					               'soapaction' => ''
//					              )
//    	);
//  }
//}

?>
