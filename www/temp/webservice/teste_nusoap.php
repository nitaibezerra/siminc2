<?php
	require_once( 'lib_blaine/nusoap.php' );
	class SIGtoINFRAClient
	{
		protected $wsdlURI;
		protected $client;
		private $host;
		private $port;
		protected $proxy_user;
		protected $proxy_pass;
		/**
		 * Parтmetros que serуo enviados via SOAP.
		 */
		public $usuario;
		public $senha;
		public $PRGAno;
		public $PRGCod;
		public $ACACod;
		public $UNICod;
		public $LOCCod;
		public $ORGCod;
		
		/**
		 * Parтmetros adicionados em 09/10/2006
		 * TODO: Solicitar nova documentaчуo do webservice para verificar especificaчѕes desses parтmetros
		 */
		public $ReceberPrograma;
		public $ReceberIndicador;
		public $ReceberRestricaoPrograma;
		public $ReceberAcao;
		public $ReceberDadoFisico;
		public $ReceberDadoFinanceiro;
		public $ReceberDadoFisicoRAP;
		public $ReceberDadoFinanceiroRAP;
		public $ReceberRestricaoAcao;
		public $ReceberVAT;
		
		 
		public function SIGtoINFRAClient( $wsdlURI, $host, $port = "", $proxy_user = "", $proxy_pass = "" )
		{			
			$this->wsdlURI = $wsdlURI;
			$this->host = $host;
			$this->port = $port;
			$this->proxy_user = $proxy_user;
			$this->proxy_pass = $proxy_pass;
			$this->client = new soapclient( $this->wsdlURI, true, $this->host, $this->port, $this->proxy_user, $this->proxy_pass );
			set_time_limit( 0 );
			ini_set( "memory_limit", "128M" );
		}
		
		public function callService( $serviceName )
		{
			/**
			 * Array de parтmetros a serem enviados via SOAP
			 * 
			 * @var array $param
			 */
			$param = array(
				'usuario' 					 => $this->usuario,
				'senha'   					 => $this->senha,
				'PRGAno'  					 => $this->PRGAno,
				'PRGCod'  					 => $this->PRGCod,
				'ACACod'  					 => $this->ACACod,
				'UNICod'  					 => $this->UNICod,
				'LOCCod'  					 => $this->LOCCod,
				'ORGCod'  					 => $this->ORGCod,
				'ReceberAcao' 			     => $this->ReceberAcao,
				'ReceberDadoFinanceiro' 	 => $this->ReceberDadoFinanceiro,
				'ReceberDadoFinanceiroRAP'   => $this->ReceberDadoFinanceiroRAP,
				'ReceberDadoFisico' 		 => $this->ReceberDadoFisico,
				'ReceberDadoFisicoRAP' 		 => $this->ReceberDadoFisicoRAP,
				'ReceberIndicador' 			 => $this->ReceberIndicador,
				'ReceberPrograma' 			 => $this->ReceberPrograma,
				'ReceberRestricaoAcao' 		 => $this->ReceberRestricaoAcao,
				'ReceberRestricaoPrograma'   => $this->ReceberRestricaoPrograma,
				'ReceberVAT' 				 => $this->ReceberVAT
			);
			$response = $this->client->call( $serviceName, array( 'parameters' => $param ) );
			if( !isset( $this->client->fault ) || $this->client->fault == false ) 
			{				
				$this->generateArray( $serviceName, $response );
			}
			
			return $response;
			
		}
		
		private function generateArray( $serviceName, &$serviceResponse )
		{
			$resultIndice = $serviceName."Result";
			foreach( $serviceResponse[ $resultIndice ] as $type => $data )
			{
				if( $data )
				{
					foreach( $data as $key => $value )
					{
						if( !$value[ 0 ] )
						{
							$serviceResponse[ $resultIndice ][ $type ] = array();
							$serviceResponse[ $resultIndice ][ $type ][ 0 ] = $value;
						}
						else
						{							
							$serviceResponse[ $resultIndice ][ $type ] = $value;
						}
						break;
					}
				}
			}
			$serviceResponse = $serviceResponse[ $resultIndice ];
		}
	}
	
	/**
	 * Exemplo de uso da classe SIGtoINFRAClient
	 */
	
	$wsdlURI = "https://homsigplan.serpro.gov.br/infrasig/sigtoinfra.asmx?wsdl";
	
	$objSIGToInfraClient = new SIGtoINFRAClient( $wsdlURI, "homsigplan.serpro.gov.br", "", "", "" );
	
	$objSIGToInfraClient->ReceberAcao = '';
	$objSIGToInfraClient->ReceberDadoFinanceiro = '';
	$objSIGToInfraClient->ReceberDadoFinanceiroRAP = '';
	$objSIGToInfraClient->ReceberDadoFisico = '';
	$objSIGToInfraClient->ReceberDadoFisicoRAP = '';
	$objSIGToInfraClient->ReceberIndicador = '';
	$objSIGToInfraClient->ReceberPrograma = '';
	$objSIGToInfraClient->ReceberRestricaoAcao = '';
	$objSIGToInfraClient->ReceberRestricaoPrograma = '';
	$objSIGToInfraClient->ReceberVAT = '';
	
	$objSIGToInfraClient->usuario = "leokessel";
	$objSIGToInfraClient->senha = "nova01";
	$objSIGToInfraClient->PRGAno = "2006";
	$objSIGToInfraClient->PRGCod = "1073";	
	$responsePorPrograma = $objSIGToInfraClient->callService( "geracaoPorPrograma" );
	
	
	$objSIGToInfraClient->UNICod = "26101";
	$objSIGToInfraClient->ACACod = "6373";	
	//$responsePorUOAcao = $objSIGToInfraClient->callService( "geracaoPorUOAcao" );
	
	var_dump( $responsePorPrograma );
	
?>