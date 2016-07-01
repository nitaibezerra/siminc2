<?php
// error handler function
function myErrorHandler($errno, $errstr, $errfile, $errline){
    /* Don't execute PHP internal error handler */
    return true;
}
//include_once APPRAIZ . 'includes/nusoap/lib/nusoap.php';

/**
public String valorEstimadoEscola(String anoExercicio, String coEscola)
public boolean isEscolaPaga(String anoExercicio, String coEscola)
public boolean atualizaAnaliseEscola(String anoExercicio, String coEscola)
**/
class PdeWs {	 
	
 	private $entcodent;
 	private $entid; 
 	private $wsdl;
 	private $parametros;
 	private $registrosWebService;
 	private $objRetorno;
 	private $method;  
 	private $config;
	private $link;
	
		 /*
		 * @method: pdeEscolaWs
		 * @author: Pedro Dantas
		 * @date: 06/04/2009
		 * @params: method, $ano, $coEscola ,$coProgramaFNDE, 
		 * 			$tipoConexao ('P' -> Produção, 'D' -> Desenvolvimento)
		 * @return: no caso de chamar o método "valorEstimadoEscola" o retorno é uma string
		 * 		    no caso dos outros métodos o retorno é boleano. 
		 */
	


 		public function pdeEscolaWs( $method, $ano, $coEscola , $coProgramaFNDE, $tipoConexao = 'P') {
 			//dbg($method .'<br>'. $ano .'<br>'. $coEscola .'<br>'. $coProgramaFNDE .'<br>'. $tipoConexao,1);
 			$config = array(
 						"proxy_host"     => "",
                        "proxy_port"     => "",
 			 			"encoding" => "ISO-8859-1",
						"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
						"trace" => true,
 						"exceptions" => true
					);
					 	
			// set to the user defined error handler
			$old_error_handler = set_error_handler("myErrorHandler");
					
			// Conexão de Produção
			if($tipoConexao ==  'P') {
				try {
					//$link = new SoapClient('http://www.fnde.gov.br/pddeWebService/services/PddeDelegate?wsdl', $config );
					
					//$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
					
					$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl');
				}
				catch (SoapFault  $e) {echo "<pre>Conexâo produção<br>";print_r($e);}
			}
			
			// Conexão de Desenvolvimento 
			else {
				try {
					//$link = new SoapClient('http://www.fnde.gov.br/pddeWebService/services/PddeDelegate?wsdl', $config ); 				
					$link = new SoapClient('http://www.fnde.gov.br/pddewebservice/server.php?wsdl', $config );
				} catch (SoapFault  $e) {echo "<pre>Conexâo desenvolvimento<br>";print_r($e);}
			}
						
			$param = array(
							$method=>array(
	                          	'anoExercicio'=>$ano,
	                        	'coEscola'=>$coEscola,
	                       		'coProgramaFnde'=>$coProgramaFNDE,
							    'senha'=>'simec',
								'tipo'=> 3
			                   )			
			);

				if($link) {
					try {
						$result = $link->__call($method,$param);
						//$result = $link->__soapCall($method,$param);
					} catch (SoapFault  $e) {echo "<pre>Chamando o método<br>";print_r($e);}
					
					if($result)
						return $result;
					else
						return "errowebservice";
					
				} else {
					return "errowebservice";
				}
 		}
 		

}  
?>