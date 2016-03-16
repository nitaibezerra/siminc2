<?php
// include_once APPRAIZ."includes/classes/WsCurlConnect.class.inc";
require_once APPRAIZ.'includes/nusoap/lib/nusoap.php';
class enderecoCEP {
	
	public $co_logradouro 				= NULL;
	public $co_municipio 				= NULL;
	public $sg_uf 						= NULL;
	public $no_bairro 					= NULL;
	public $tipo_logradouro 			= NULL;
	public $no_logradouro 				= NULL;
	public $no_logradouro_abrev 		= NULL;
	public $no_complemento_logradouro	= NULL;
	public $nu_cep 						= NULL;
	public $co_ibge 					= NULL;
	public $erro 						= NULL;
	
	public function __construct( $cep ){
		$this->consultaEnderecoCEP( $cep );
	}
	
	public function __destruct(){
	}
	
	public function consultaEnderecoCEP( $cep ){
		
		// Array a ser enviado com as informações do serviço
		$proxyhost 		= isset($_POST['proxyhost']) 		? $_POST['proxyhost'] 		: '';
		$proxyport 		= isset($_POST['proxyport']) 		? $_POST['proxyport'] 		: '';
		$proxyusername 	= isset($_POST['proxyusername']) 	? $_POST['proxyusername'] 	: '';
		$proxypassword 	= isset($_POST['proxypassword']) 	? $_POST['proxypassword'] 	: '';
		
		$client = new nusoap_client('http://ws.mec.gov.br/Dne/wsdl', 'wsdl', $proxyhost, $proxyport, $proxyusername, $proxypassword);
		
		$err = $client->getError();
		if ($err) {
			echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
		}
		// Doc/lit parameters get wrapped
		$param = array('cep' => $cep);
		$result = $client->call('lerDNE', array('cep' => $cep), '', '', false, true);
		// Check for a fault
		if ($client->fault) {
			echo 'Aqui1<h2>Fault</h2><pre>';
			print_r($result);
			echo '</pre>';
		} else {
			// Check for errors
			$err = $client->getError();
			if ($err) {
				// Display the error
				echo 'Aqui2<h2>Error</h2><pre>' . $err . '</pre>';
			} else {
				$xml = substr($client->response, strpos($client->response, '<'));
				
// 				if( substr_count ( $xml, 'co_logradouro' ) > 2 ){
// 					$this->erro = 'CEP possui mais de 1 endereço.';
// 				}else{
					$arrCampos = Array( 'co_logradouro', 
										'co_municipio', 
										'sg_uf', 
										'no_bairro', 
										'tipo_logradouro', 
										'no_logradouro', 
										'no_logradouro_abrev', 
										'no_complemento_logradouro',
										'nu_cep',
										'co_ibge');
					foreach($arrCampos as $campo){
						//preg_match('/&lt;'.$campo.'&gt;&lt;!\[CDATA\[(([a-zA-Z0-9 ])*)\]\]&gt;&lt;\/'.$campo.'&gt;/', $client->response, $a);
 						//preg_match('/&lt;'.$campo.'&gt;&lt;!\[CDATA\[(.*)\]\]&gt;&lt;\/'.$campo.'&gt;/', $client->response, $a);
 						preg_match('/&lt;'.$campo.'&gt;&lt;!\[CDATA\[([[:alnum:][:space:]#&;\)\(]*)\]\]&gt;&lt;\/'.$campo.'&gt;/', $client->response, $a);
						$this->$campo = html_entity_decode($a[1]);
// 					}
				}
			}
		}
	}
}
?>