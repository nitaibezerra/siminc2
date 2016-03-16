<?php
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

 /*
 * @method: pdeEscolaWs
 * @author: Pedro Dantas
 * @date: 06/04/2009
 * @params: method, $ano, $coEscola  
 * @return: no caso de chamar o método "valorEstimadoEscola" o retorno é uma string
 * 		    no caso dos outros métodos o retorno é boleano. 
 */
 		public function pdeEscolaWs( $method, $ano, $coEscola, $coProgramaFNDE ){
 			
 			$this->wsdl = "http://www.fnde.gov.br/pddeWebService/services/PddeDelegate?wsdl";
 			//$this->wsdl = "http://172.20.65.115/pddeWebService/services/PddeDelegate?wsdl";
// 			$retorno = curl_init( "http://172.20.65.115/pddeWebService/services/PddeDelegate?method=valorEstimadoEscola&anoExercicio=2008&coEscola=24009377");
// 			pre( $retorno,1 );
 			$this->config = array(
						"encoding" => "ISO-8859-1",
						"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
						"trace" => true
					); 
					
 			$conexao = new SoapClient($this->wsdl, $this->config);
 
 			$this->method				 		= $method;
			$this->parametros 					= array();
			$this->parametros["anoExercicio"]	= $ano;
			$this->parametros["coEscola"]	    = $coEscola;
			$this->parametros["coProgramaFNDE"]	= $coProgramaFNDE;
			$this->objRetorno 					= $conexao->__call( $this->method, $this->parametros );
	 		
			return $this->objRetorno; 
 		}
} 

if( $entcodent = pegarEntCodEnt( $_SESSION['entid'] ) ){	
	$ws = new PdeWs(); 
	$ws->pdeEscolaWs( 'valorEstimadoEscola',2008, $entcodent );	
} 
?>