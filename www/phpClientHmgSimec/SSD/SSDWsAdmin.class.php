<?php
	require_once("SSDConnector.class.php");
	require_once("TO/UserInfo.php");
	require_once("TO/UserPermissionInfo.php");
	require_once("TO/UserAndPermissionIds.php");
	require_once("TO/TicketUserInfo.php");
	require_once("TO/UrlInfo.php");
	
	require_once("TO/UserInfoAndPermissionId.php");
	require_once("TO/ConfigurationPermissionInfo.php");
	require_once("TO/UserAndPermissionInfo.php");
	
	class SSDWsAdmin {
		private $ssdConector;
		private $wsConector;
		private $wsdl;
		private $tmpDir;
		private $clientCert;
		private $privateKey;
		private $privateKeyPassword;
		private $trustedCaChain;
		private $appletUrl;
		private $wsdlUrl;
		
		/**
		 * Configura o acesso aos servicos de manutencao de usuario do SSD.
		 *
		 * @param $tmpDir
		 * 	O caminho do diretorio para a alocacao de arquivos temporarios.
		 *	Este diretorio deve ser limpo periodicamente, pois os arquivos 
		 * 	gerados nao serao removidos.
		 *	Exemplos de caminhos sao: para o Windows "C:/Windows/Temp", e 
		 *	para o Linux "/tmp".
		 * 
		 * @param $clientCert
		 * 	O arquivo no formato PEM para o certificado do sistema 
		 * 	autorizado a utilizar os servicos do SSD.
		 * 	Para obter este certificado, entre em contato com a equipe do 
		 *	SSD.
		 * 
		 * @param $privateKey
		 * 	O arquivo no formato PEM para a chave privada do certificado 
		 *	do sitema credenciado a utilizar o SSD.
		 *	Este arquivo deve ser armazenado com o maximo de sigilo 
		 *	possivel, para garantir a seguranca da autenticacao do sistema.
		 * 
		 * @param $privateKeyPassword
		 *	A senha para acessar a chave privada do certificado do sistema.
		 * 
		 * @param $trustedCaChain
		 * 	O arquivo contendo a cadeia de confianca (os certificados das 
		 *	autoridades certificadoras no formato PEM).
		 * 
		 */
		public function __construct(
			$tmpDir,
			$clientCert,
			$privateKey,
			$privateKeyPassword,
			$trustedCaChain
		)
		{
			$this->tmpDir = $tmpDir;
			$this->clientCert = $clientCert;
			$this->privateKey = $privateKey;
			$this->privateKeyPassword = $privateKeyPassword;
			$this->trustedCaChain = $trustedCaChain;
		}
		
		/**
		 * Configura o cliente dos servicos de manutencao de usuario do 
		 * SSD a utilizar os servicos de producao.
		 *
		 * Um dos metodos que configura a utilizacao do ambiente de 
		 * homologacao ou producao deve ser chamado obrigatoriamente 
		 * antes de fazer qualquer chamada aos servicos.
		 *
		 * @see useHomologationSSDServices
		 */
		public function useProductionSSDServices() {
			$this->loadProductionUrls();
			$this->ssdConnector = 
				new SSDConnector(
					$this->tmpDir,
					$this->clientCert,
					$this->privateKey,
					$this->privateKeyPassword,
					$this->trustedCaChain);
			$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl); 
			$this->wsdl = $this->ssdConnector->getWSDL();
		}
		
		/**
		 * Configura o cliente dos servicos de manutencao de usuario do SSD 
		 * a utilizar os servicos de homologacao.
		 *
		 * Um dos metodos que configura a utilizacao do ambiente de 
		 * homologacao ou producao deve ser chamado obrigatoriamente antes de
		 * fazer qualquer chamada aos servicos.
		 *
		 * @see useProductionSSDServices
		 */
		public function useHomologationSSDServices() {
			$this->loadHomologationUrls();
			$this->ssdConnector = 
				new SSDConnector(
					$this->tmpDir,
					$this->clientCert,
					$this->privateKey,
					$this->privateKeyPassword,
					$this->trustedCaChain);
			$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl);
			$this->wsdl = $this->ssdConnector->getWSDL();
		}
		
		public function getMaintenanceTicket($maintenanceTicketId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"maintenanceTicketId" => $maintenanceTicketId				
			);
			
			$retorno = $this->wsConnector->call(
				"getMaintenanceTicket",
				$params,
				$options
			);
			
			debug($retorno);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			$urlInfo = UrlInfo::loadUrlInfo(
				$retorno->url
			);
			
			return($urlInfo);
		}
	
		public function getUserMaintenanceTicketInfo($systemId, $userId, $flag, $serviceId){
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
						" para os ambiente de homologacao".
						" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array( "namespace" => $this->wsdl->namespaces['tns']);		
			$params = array(
				"systemId" => $systemId ,
				"userId" => $userId ,
				"flag" => $flag ,
				"serviceId" => $serviceId
			);
			$retorno = $this->wsConnector->call(
				"getUserMaintenanceTicketInfo",
				$params,
				$options);

			if($retorno instanceof SOAP_Fault) {
				throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
			}

		
			debug($retorno);
		
		
			$userTicketInfoId = UserAuthInfo::loadUserAuthInfo(
				$authType, 
				$retorno->flag, 
				$retorno->id, 
				$loginIdentifier, 
				$retorno->lastUpdateDate,
				$retorno->messages, 
				$retorno->field,
				$arrInvalidPermission, 
				$retorno->validPermission
			);
		
			return($userTicketInfoId);
		}	

		
		private function loadProductionUrls() {
			$this->fileUploadUrl = 
				"https://ssd.mec.gov.br/ssdserver/servlet/UploadTmpDoc";
			$this->wsdlUrl = 
				"https://ssd.mec.gov.br/ssdserver/services/Admin?wsdl";
			$this->downloadSignaturePackageUrl = 
				"https://ssd.mec.gov.br/ssdserver/servlet/DownloadSignaturePackage";
			$this->appletUrl = 
				"http://ssd.mec.gov.br/applet/SSDApplet.jar";
		}
		
		private function loadHomologationUrls() {
			$this->fileUploadUrl = 
				"https://ssdhmg.mec.gov.br/ssdserver/servlet/UploadTmpDoc";
			$this->wsdlUrl = 
				"https://ssdhmg.mec.gov.br/ssdserver/services/Admin?wsdl";
			$this->downloadSignaturePackageUrl = 
				"https://ssdhmg.mec.gov.br/ssdserver/servlet/DownloadSignaturePackage";
			$this->appletUrl = 
				"http://ssdhmg.mec.gov.br/applet/SSDApplet.jar";				
		}
	}
?>
