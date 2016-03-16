<?php
	require_once("TO/UserInfo.php");
	require_once("TO/UserPermissionInfo.php");
	require_once("TO/UserAndPermissionIds.php");
	require_once("TO/TicketUserInfo.php");
	require_once("TO/UrlInfo.php");
	require_once("TO/UserInfoAndPermissionId.php");

	class SSDWsUser {
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
		private $codebase;
		private $appletClass;
		
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
			$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl, 'USER'); 
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
			$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl, 'USER');
			$this->wsdl = $this->ssdConnector->getWSDL();
		}
		
		/**
		 * Monta um codigo html basico para a abertura da applet.
		 *
		 * O codigo retornado por este metodo pode ser subistituido por outro 
		 * nos casos necessarios, tais como a compatibilidade com navegadores.
		 *
		 * Antes da invocacacao deste metodo, um dos metodos de configuracao 
		 * do ambiente deve ser chamado.
		 *
		 * @see useProductionSSDServices
		 * @see useHomologationSSDServices
		 *
		 * @param $ticketId 
		 * 	O identificador do ticket da applet.
		 *
		 * @return string
		 * 	O codigo html basico para abrir a applet.
		 */
		public function getAppletHtmlSampleCode($ticketId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
						" para os ambiente de homologacao".
						" ou producao.";
				throw new Exception($msg_conf);
			}
			return $this->ssdConnector->getAppletHtmlSampleCode(
				$ticketId, $this->appletUrl);
		}
		
		/**
		* Altera o status da permissao do usuario
		* 
		* Este metodo pode ser utilizado diretamente para a alteracao do 
		* status da permisssao de um usuario
		* 
		* @param int $userId
		* @param int $permissionId
		* @param string $justification
		* @param string $userPermissionStatusDesired
		* 
		* @return boolean
		* Status da permissao do usuario alterado corretamente
		*/
		public function changeUserPermissionStatus($userId, $permissionId, $justification, $userPermissionStatusDesired) {
			if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
				" para os ambiente de homologacao".
				" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			
			$params = array( 
				"userId" => (integer) $userId ,
				"permissionId" => (integer) $permissionId ,
				"justification" => $justification ,
				"userPermissionStatusDesired" => $userPermissionStatusDesired 
			);
			
			$retorno = $this->wsConnector->call(
				"changeUserPermissionStatus",
				$params,
				$options
			);	 
			
			if( $retorno == "true" )
			{
				return( TRUE );
			}
			else 
			{
				return( FALSE );
			}
		}
		 
		/**
		 * Inclui permissao para um usuario
		 *
		 * @param int $userId
		 * @param int $permissionId
		 * @return boolean
		 */
		public function includeUserPermission($userId, $permissionId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userId" => $userId ,
				"permissionId" => $permissionId 
			);
			
			$retorno = $this->wsConnector->call(
				"includeUserPermission",
				$params,
				$options
			);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			return( TRUE );
		}
		
		/**
		 * Altera permissao de um usuario
		 *
		 * @param int $userId
		 * @param int $oldPermissionId
		 * @param int $newPermissionId
		 * @param string $justification
		 * @return boolean
		 */
		public function changeUserPermission( $userId , $oldPermissionId , $newPermissionId , $justification ) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userId" => $userId ,
				"oldPermissionId" => $oldPermissionId ,
				"newPermissionId" => $newPermissionId ,
				"justification" => $justification 
			);
			
			$retorno = $this->wsConnector->call(
				"changeUserPermission",
				$params,
				$options
			);	 
			
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			return( TRUE );
		}
		
		/**
		 * Consulta dados de um usuario
		 *
		 * @param int $userId
		 * @return UserInfo
		 * Entidade contendo informacoes completas sobre um usuario
		 */
		public function getUserInfo($userId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userId" => $userId
			);
			
			$retorno = $this->wsConnector->call(
				"getUserInfo",
				$params,
				$options
			);

			if ($retorno instanceof SOAP_Fault) {
                $message = trim(str_to_upper(ereg_replace("[^a-zA-Z0-9_]", "", strtr($retorno->message, "áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ ", "aaaaeeiooouucAAAAEEIOOOUUC_"))));
                $message2 = "O Usuário Não possui permissões ativas no sistema.";
                $message2 = trim(str_to_upper(ereg_replace("[^a-zA-Z0-9_]", "", strtr($message2, "áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ ", "aaaaeeiooouucAAAAEEIOOOUUC_"))));
                
                if($message == $message2) return $retorno->message;
                else throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
                
			}
			
			return $retorno;

			/*
			//city Address
			$ufCityAddress = Uf::loadUf( $retorno->naturality->estado->descricao , $retorno->cityAddress->estado->sigla );
			$cityAddress = City::loadCity( $retorno->naturality->codigoIBGE , $retorno->address , $ufCityAddress , $retorno->siglaUFCEP );
			
			
			//naturality
			$ufCityNaturalidade = Uf::loadUf( $retorno->naturality->estado->descricao , $retorno->naturality->estado->sigla );
			$cityNaturalidade = City::loadCity( $retorno->naturality->codigoIBGE , $retorno->naturality->descricao , $ufCityNaturalidade , $retorno->naturality->siglaEstado );
			
			$ufUser = Uf::loadUf( $retorno->address , $retorno->siglaUFCEP );

			
			$userInfo = array(
				$retorno->address , 
				$retorno->alternativeEmail , 
				$retorno->birthDate ,
				$retorno->cellPhoneNumber ,
				$cityAddress , 
				$retorno->cnpj ,
				$retorno->cpf ,
				$retorno->dispatcherAgency ,
				$retorno->email , 
				$retorno->login ,
				$retorno->lotacao , 
				$retorno->name , 
				$retorno->nationality , 
				$cityNaturalidade ,
				$retorno->nis, 
				$retorno->postalCode ,
				$retorno->responsibleCpf , 
				$retorno->responsibleName ,
				$retorno->rg ,
				$retorno->socialReason ,
				$ufUser ,
				$retorno->telephoneNumber ,
				$retorno->workInstitution ,
				$userId
			);
			
			return( $userInfo ); */
		}
		
		/**
		 * Recupera usuarios que possuem permissoes com status 
		 * 'Aguardando liberacao'
		 *
		 * @return array
		 * Array de UserAndPermissionIds contendo conjunto de pares usuario-permissao
		 * com status 'Aguardando liberacao'
		 */
		public function getUsersWaitingForPermissionRelease() {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array() ;
			
			$retorno = $this->wsConnector->call(
				"getUsersWaitingForPermissionRelease",
				$params,
				$options
			);	 
			
			if( $retorno instanceof SOAP_Fault ) {
				throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
			}
			
			Zend_Debug::dump($retorno);exit;

			if( ! is_array( $retorno ) )
			{
				return( FALSE );
			}
			$arrObjUsuario = array();
			foreach ($retorno as $eachUserInfo) {

				if( key_exists( $eachUserInfo->userInfo->userId , $arrObjUsuario ) )
				{
					$arrObjUsuario[ $eachUserInfo->userInfo->userId ]->addPermission( $eachUserInfo->permissionId );						
					continue;	
				}
				
				//city Address
				$ufCityAddress = Uf::loadUf( $eachUserInfo->cityAddress->estado->descricao , $eachUserInfo->cityAddress->estado->sigla );
				$cityAddress = City::loadCity( $eachUserInfo->cityAddress->codigoIBGE , $eachUserInfo->cityAddress->descricao , $ufCityAddress , $eachUserInfo->cityAddress->siglaEstado );
			
				//naturality
				$ufCityNaturalidade = Uf::loadUf( $eachUserInfo->naturality->estado->descricao , $eachUserInfo->naturality->estado->sigla );
				$cityNaturalidade = City::loadCity( $eachUserInfo->naturality->codigoIBGE , $eachUserInfo->naturality->descricao , $ufCityNaturalidade , $eachUserInfo->naturality->siglaEstado );
			
				$ufUser = Uf::loadUf( $eachUserInfo->stateAddress->descricao , $eachUserInfo->stateAddress->sigla );
				$userInfo = UsuarioTO::loadUserInfo(
					$eachUserInfo->userInfo->address , 
					$eachUserInfo->userInfo->alternativeEmail , 
					null ,
					$eachUserInfo->userInfo->cellPhoneNumber ,
					$cityAddress , 
					$eachUserInfo->userInfo->cnpj ,
					$eachUserInfo->userInfo->cpf ,
					$eachUserInfo->userInfo->dispatcherAgency ,
					$eachUserInfo->userInfo->email , 
					$eachUserInfo->userInfo->login ,
					$eachUserInfo->userInfo->lotacao , 
					$eachUserInfo->userInfo->name , 
					$eachUserInfo->userInfo->nationality , 
					$cityNaturalidade ,
					$eachUserInfo->userInfo->nis, 
					$eachUserInfo->userInfo->postalCode ,
					$eachUserInfo->userInfo->responsibleCpf , 
					$eachUserInfo->userInfo->responsibleName ,
					$eachUserInfo->userInfo->rg ,
					$eachUserInfo->userInfo->socialReason ,
					$ufUser ,
					$eachUserInfo->userInfo->telephoneNumber ,
					$eachUserInfo->userInfo->workInstitution ,
					$eachUserInfo->userInfo->userId
				);
				
				$arrObjUsuario[ $userInfo->getCoUsuarioSSD() ] = $userInfo ;
				//$arrObjUsuario[ $eachUserInfo->userInfo->userId ]->addPermission( $eachUserInfo->permissionId );
			}
			
			return( $arrObjUsuario );
			
		}
		
		/**
		 * Consulta uma permissao de um usuario
		 *
		 * @param int $userId
		 * 
		 * @return UserPermissionInfo
		 * Entidade contendo informacoes como o status da permissao do usuario,
		 * status dos dados obrigatorios, identificador do usuario responsavel pela alteracao
		 * do status da permissao do usuario, justificativa da alteracao
		 */
		public function getUserPermissionsInfo( $userId /*, $permissionId */) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userId" => $userId/* ,
				"permissionId" => $permissionId */
			);
			
			$retorno = $this->wsConnector->call(
				"getUserPermissionsInfo",
				$params,
				$options
			);	 
			
			//echo ($retorno->justificationOfStatusChange);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			if( $retorno instanceof stdClass )
			{
				$retorno = array( $retorno->UserPermissionInfo );
			}
			
			$userPermissionInfo = array();
			
			if( is_array( $retorno ) )
			{
				foreach ($retorno as $eachUserPermissionInfo) {
				
					$userPermissionInfo[] = UserPermissionInfo::loadPermissionInfo( 
						$eachUserPermissionInfo->permissionId ,
						$eachUserPermissionInfo->justificationOfStatusChange ,
						$eachUserPermissionInfo->requiredDataStatus ,
						$eachUserPermissionInfo->responsibleIdForStatusChange ,
						$eachUserPermissionInfo->userPermissionStatus,
						$eachUserPermissionInfo->profileSg
					);
				}	
			}
			
			
			return($userPermissionInfo);
		}

		
		public function getUserMaintenanceTicket( $ticket )
		{
			//retorno
			/*$resposta = unserialize( 'O:8:"stdClass":5:{s:19:"expirationTimestamp";s:13:"1213135536774";s:4:"flag";s:0:"";s:7:"service";s:2:"50";s:8:"systemId";s:1:"9";s:6:"userId";s:5:"14502";}' );
			return ( $resposta );*/
			
			
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userMaintenanceTicketId" => $ticket
			);
			
			$retorno = $this->wsConnector->call(
				"getUserMaintenanceTicket",
				$params,
				$options
			);	 
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			return ( $retorno );
			
		}
		
		/**
		 * Consulta identificador de um usuario pelo ticket
		 * de autenticacao deste usuario
		 *
		 * @param string $userTicketId
		 * @return TicketUserInfo
		 * Entidade contendo o identificador do usuario e a flag
		 */
		public function getTicketUserInfoByUserTicketId($userTicketId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userTicketId" => $userTicketId
			);
			
			$retorno = $this->wsConnector->call(
				"getTicketUserInfoByUserTicketId",
				$params,
				$options
			);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			$ticketUserInfo = TicketUserInfo::loadTicketUserInfo(
				$retorno->flag,
				$retorno->userId
			);
			
			return($ticketUserInfo);
		}
		
		/**
		 * Requisita Url de ManutenÃ§Ã£o de UsuÃ¡rio
		 *
		 * @param string $flag
		 * @param string $serviceId
		 * @return UrlInfo
		 * Entidade contendo o identificador do usuario e a flag
		 */
		public function getUserMaintenanceUrl($flag, $serviceId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => @$this->wsdl->namespaces['tns']);
			$params = array( 
				"flag" => $flag,
				"serviceId" => $serviceId
			);

			$retorno = $this->wsConnector->call(
				"getUserMaintenanceUrl",
				$params,
				$options
			);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			$urlInfo = UrlInfo::loadUrlInfo(
				$retorno->url
			);
			
			return($urlInfo);
		}
		
		/**
		 * Busca perfis de usuï¿½rio cadastrado no SSD
		 *
		 * @return stdClass
		 */
		public function getSystemPermissionsInfo() {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
			);
			
			$retorno = $this->wsConnector->call(
				"getSystemPermissionsInfo",
				$params,
				$options
			);
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			$userPermissionInfo = array();
			
			if( is_array( $retorno ) )
			{
				foreach ($retorno as $eachUserPermissionInfo) {
			
					$userPermissionInfo[] = UserPermissionInfo::loadPermissionInfo( 
						$eachUserPermissionInfo->id ,
						$eachUserPermissionInfo->changeJustify ,
						$eachUserPermissionInfo->requiredDataStatus ,
						$eachUserPermissionInfo->changeUserResponsibleConf ,
						$eachUserPermissionInfo->userPermissionStatus,
						$eachUserPermissionInfo->sgProfile ,
						$eachUserPermissionInfo->description
					);
				}	
			}
			
			return($userPermissionInfo);
		}
		
		private function loadProductionUrls() {
			$this->fileUploadUrl = 
				"https://ssd.mec.gov.br/ssd-server/servlet/UploadTmpDoc";
			$this->wsdlUrl = 
				"https://ssd.mec.gov.br/ssd-server/services/UserMaintenance?wsdl";
			$this->downloadSignaturePackageUrl = 
				"https://ssd.mec.gov.br/ssd-server/servlet/DownloadSignaturePackage";
			$this->appletUrl = 
				"http://ssd.mec.gov.br/applet/ssd-applet.jar";
		}
		
		private function loadHomologationUrls() {
			$this->fileUploadUrl = 
				"https://ssdhmg.mec.gov.br/ssd-server/servlet/UploadTmpDoc";
			$this->wsdlUrl = 
				"https://ssdhmg.mec.gov.br/ssd-server/services/UserMaintenance?wsdl";
			$this->downloadSignaturePackageUrl = 
				"https://ssdhmg.mec.gov.br/ssd-server/servlet/DownloadSignaturePackage";
			$this->appletUrl = 
				"http://ssdhmg.mec.gov.br/applet/ssd-applet.jar";
		}
	}
?>
