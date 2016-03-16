<?php
	require_once(SSD_PATH."SSDConnector.class.php");
	require_once(SSD_PATH."TO/UserInfo.php");
	require_once(SSD_PATH."TO/UserPermissionInfo.php");
	require_once(SSD_PATH."TO/UserAndPermissionIds.php");
	require_once(SSD_PATH."TO/TicketUserInfo.php");
	require_once(SSD_PATH."TO/UrlInfo.php");
	
	require_once(SSD_PATH."TO/UserInfoAndPermissionId.php");
	require_once(SSD_PATH."TO/ConfigurationPermissionInfo.php");
	require_once(SSD_PATH."TO/UserAndPermissionInfo.php");
	
	class SSDWsUser {
		private $ssdConector;
		private $wsConector;
		public $wsdl;
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
				"userId" => $userId ,
				"permissionId" => $permissionId ,
				"justification" => $justification ,
				"userPermissionStatusDesired" => $userPermissionStatusDesired 
			);
			
			$retorno = $this->wsConnector->call(
				"changeUserPermissionStatus",
				$params,
				$options
			);	 
			
			//debug ($retorno);
			
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			debug( $retorno );
			
			/*
			echo "<pre>";
			var_dump ($retorno);
			echo "<pre>";
			*/
			
			if ( strcmp($retorno,"true") == 0 )
				return "Sim";
			else
				return "Nao";
			//return ( $retorno->out );
		}

		/*************************************************************************************************************/		 
		/* NOVO MÉTODO */
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
		public function changeUserPermissionStatusWithResponsible($userId, $permissionId, $justification, $userPermissionStatusDesired, $reponsibleForChangeId) {
			if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
				" para os ambiente de homologacao".
				" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			
			$params = array( 
				"userId" => $userId ,
				"permissionId" => $permissionId ,
				"justification" => $justification ,
				"userPermissionStatusDesired" => $userPermissionStatusDesired ,
				"responsibleForChangeId" => $reponsibleForChangeId
			);
			
			$retorno = $this->wsConnector->call(
				"changeUserPermissionStatusWithResponsible",
				$params,
				$options
			);	 
			
			debug ($retorno);
			
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			//debug( $retorno );
			
			/*
			echo "<pre>";
			var_dump ($retorno);
			echo "<pre>";
			*/
			
			if ( strcmp($retorno,"true") == 0 )
				return "Sim";
			else
				return "Nao";
			//return ( $retorno->out );
		}		 
		/* NOVO MÉTODO */
		/*************************************************************************************************************/

		/*************************************************************************************************************/		 
		/**
		 * Alterar status da permissao do usuario por CPF/CNPJ (com responsavel)
		 */
		public function changeUserPermissionStatusByCPFOrCNPJ($cpfOrCnpj, $permissionId, $justification, $userPermissionStatusDesired, $responsibleForChangeCpfOrCnpj) {
			if (!$this->wsConnector) {
			$msgConfig = "Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
				" para os ambientes de homologa&ccedil;&atilde;o".
				" ou produ&ccedil;&atilde;o.";
				throw new Exception($msgConfig);
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"cpfOrCnpj" => $cpfOrCnpj,
				"permissionId" => $permissionId,
				"justification" => $justification,
				"userPermissionStatusDesired" => $userPermissionStatusDesired,
				"responsibleForChangeCpfOrCnpj" => $responsibleForChangeCpfOrCnpj
			);
			$retorno = $this->wsConnector->call(
				"changeUserPermissionStatusByCPFOrCNPJ",
				$params,
				$options
			);	 
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Incluir permissao do usuario por CPF/CNPJ (com responsavel)
		 */
		public function includeUserPermissionByCPFOrCNPJ($cpfOrCnpj, $permissionId, $responsibleForChangeCpfOrCnpj) {
			if (!$this->wsConnector) {
				$msgConfig = "<pre>Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
					" para os ambientes de homologa&ccedil;&atilde;o".
					" ou produ&ccedil;&atilde;o.</pre>";
				print_r($msgConfig);
				exit;
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"cpfOrCnpj" => $cpfOrCnpj,
				"permissionId" => $permissionId,
				"responsibleForChangeCpfOrCnpj" => $responsibleForChangeCpfOrCnpj
			);
			$retorno = $this->wsConnector->call(
				"includeUserPermissionByCPFOrCNPJ",
				$params,
				$options
			);
			if ($retorno instanceof SOAP_Fault) {
				return array("erro" => "Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Recurar informacoes do usuario por CPF/CNPJ
		 */
		public function getUserInfoByCPFOrCNPJ($cpfOrCnpj) {
			if (!$this->wsConnector) {
				$msgConfig = "<pre>Chame algum m&eacute;todo de configura&ccedil;&atilde;o para os ambientes de homologa&ccedil;&atilde;o ou produ&ccedil;&atilde;o.</pre>";
				echo $msgConfig;
				exit;
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"cpfOrCnpj" => $cpfOrCnpj
			);
			$retorno = $this->wsConnector->call(
				"getUserInfoByCPFOrCNPJ",
				$params,
				$options
			);	 
			if($retorno instanceof SOAP_Fault) {
				return array("erro" => "Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Alterar senha do usuario por CPF/CNPJ
		 */
		public function changeUserPasswordByCPFOrCNPJ($cpfOrCnpj, $oldPassword, $newPassword) {
			if (!$this->wsConnector) {
			$msgConfig = "Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
				" para os ambientes de homologa&ccedil;&atilde;o".
				" ou produ&ccedil;&atilde;o.";
				throw new Exception($msgConfig);
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"cpfOrCnpj" => $cpfOrCnpj,
				"oldPassword" => $oldPassword,
				"newPassword" => $newPassword
			);
			$retorno = $this->wsConnector->call(
				"changeUserPasswordByCPFOrCNPJ",
				$params,
				$options
			);	 
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Cadastrar usuario
		 */
		public function signUpUser($userInfo) {
			if (!$this->wsConnector) {
				$msgConfig = "<pre>Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
					" para os ambientes de homologa&ccedil;&atilde;o".
					" ou produ&ccedil;&atilde;o.</pre>";
				print_r($msgConfig);
				exit;
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"userInfo" => $userInfo
			);
			$retorno = $this->wsConnector->call(
				"signUpUser",
				$params,
				$options
			);	 
			if($retorno instanceof SOAP_Fault) {
				return array("erro" => "Erro ao buscar informa&ccedil;&otilde;es do WebService: ".$retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Atualizar usuario
		 */
		public function updateUser($userInfo) {
			if (!$this->wsConnector) {
			$msgConfig = "Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
				" para os ambientes de homologa&ccedil;&atilde;o".
				" ou produ&ccedil;&atilde;o.";
				throw new Exception($msgConfig);
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"userInfo" => $userInfo
			);
			$retorno = $this->wsConnector->call(
				"updateUser",
				$params,
				$options
			);	 
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Recuprar senha do usuario por CPF/CNPJ
		 */
		public function recoveryUserPasswordByCPFOrCNPJ($cpfOrCnpj) {
			if (!$this->wsConnector) {
				$msgConfig = "Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
					" para os ambientes de homologa&ccedil;&atilde;o".
					" ou produ&ccedil;&atilde;o.";
				throw new Exception($msgConfig);
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"cpfOrCnpj" => $cpfOrCnpj
			);
			$retorno = $this->wsConnector->call(
				"recoveryUserPasswordByCPFOrCNPJ",
				$params,
				$options
			);
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
		}
		
		/**
		 * Recuprar senha do usuario por hash e codigo
		 */
		public function recoveryUserPasswordByHashAndCode($hash, $code, $password, $cpfOrCnpj) {
			if (!$this->wsConnector) {
				$msgConfig = "Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
					" para os ambientes de homologa&ccedil;&atilde;o".
					" ou produ&ccedil;&atilde;o.";
				throw new Exception($msgConfig);
			}
			$options = array("namespace" => $this->wsdl->namespaces["tns"]);
			$params = array( 
				"hash" => $hash,
				"code" => $code,
				"password" => $password,
				"cpfOrCnpj" => $cpfOrCnpj
			);
			$retorno = $this->wsConnector->call(
				"recoveryUserPasswordByHashAndCode",
				$params,
				$options
			);
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
			}
			return ($retorno);
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
			
			debug($retorno);
			
			return( "Sim" );
		}
		
		/******************************************************************************************************************/
		/* NOVO MÉTODO */		
		/**
		 * Inclui permissao para um usuario
		 *
		 * @param int $userId
		 * @param int $permissionId
		 * @return boolean
		 */
		public function includeUserPermissionWithResponsible($userId, $permissionId, $responsibleForChangeId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"userId" => $userId ,
				"permissionId" => $permissionId ,
				"responsibleForChangeId" => $responsibleForChangeId
			);
			
			$retorno = $this->wsConnector->call(
				"includeUserPermissionWithResponsible",
				$params,
				$options
			);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			debug($retorno);
			
			return( "Permissão incluida com sucesso." );
		}		
		/* NOVO MÉTODO */
		/******************************************************************************************************************/
		
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
			
			debug($retorno);

			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			return( TRUE );
		}
		
		/******************************************************************************************************************/		
		/* NOVO MÉTODO */		
		/**
		 * Altera permissao de um usuario
		 *
		 * @param int $userId
		 * @param int $oldPermissionId
		 * @param int $newPermissionId
		 * @param string $justification
		 * @return boolean
		 */
		public function changeUserPermissionWithResponsible( $userId , $oldPermissionId , $newPermissionId , $justification, $responsibleForChangeId ) {
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
				"justification" => $justification ,
				"responsibleForChangeId" => $responsibleForChangeId
			);
			
			$retorno = $this->wsConnector->call(
				"changeUserPermissionWithResponsible",
				$params,
				$options
			);	 
			
			debug ($retorno);
			
			if($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			return( TRUE );
		}
		/* NOVO MÉTODO */
		/******************************************************************************************************************/
		
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
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			debug($retorno);

			echo "retorno >";
			echo "<pre>";
			var_dump($retorno);
			echo "<pre>";

			//city Address
			$ufCityAddress = Uf::loadUf( $retorno->cityAddress->estado->descricao , $retorno->cityAddress->estado->sigla );
			$cityAddress = City::loadCity( $retorno->cityAddress->codigoIBGE , $retorno->cityAddress->descricao , $ufCityAddress , $retorno->cityAddress->siglaEstado );
			
			//naturality
			$ufCityNaturalidade = Uf::loadUf( $retorno->naturality->estado->descricao , $retorno->naturality->estado->sigla );
			$cityNaturalidade = City::loadCity( $retorno->naturality->codigoIBGE , $retorno->naturality->descricao , $ufCityNaturalidade , $retorno->naturality->siglaEstado );
			
			$ufUser = Uf::loadUf( $retorno->stateAddress->descricao , $retorno->stateAddress->sigla );
			
			$userInfo = UserInfo::loadUserInfo(
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
				$retorno->workInstitution
			);
			
			return( $userInfo );
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
			
			debug($retorno);
			
			//$retorno = array();
			/*
			echo "retorno >";
			echo "<pre>";
			var_dump($retorno);
			echo "<pre>";
			*/
			//return ($retorno);
			
			$contador = 0;
			
			$userInfoAndPermissionId = array();
			
			foreach ( $retorno as $eachUserInfo) {
				
				/*$userAndPermissionIds = $retorno->getUserAndPermissionIds();*/
				
				//$userAndPermission = UserAndPermissionIds::loadUserAndPermissionIds( $eachUserInfo->permissionId , $eachUserInfo->userInfo->userId );

				/*
				echo "eachUserInfo->userInfo >";
				echo "<pre>";
				var_dump($eachUserInfo->userInfo);
				echo "<pre>";
				*/

				//city Address
				@$ufCityAddress = Uf::loadUf( $eachUserInfo->userInfo->cityAddress->estado->descricao , $eachUserInfo->userInfo->cityAddress->estado->sigla );
				@$cityAddress = City::loadCity( $eachUserInfo->userInfo->cityAddress->codigoIBGE , $eachUserInfo->userInfo->cityAddress->descricao , $ufCityAddress , $eachUserInfo->userInfo->cityAddress->siglaEstado );
			
				//naturality
				@$ufCityNaturalidade = Uf::loadUf( $eachUserInfo->userInfo->naturality->estado->descricao , $eachUserInfo->userInfo->naturality->estado->sigla );
				@$cityNaturalidade = City::loadCity( $eachUserInfo->userInfo->naturality->codigoIBGE , $eachUserInfo->userInfo->naturality->descricao , $ufCityNaturalidade , $eachUserInfo->userInfo->naturality->siglaEstado );
			
				@$ufUser = Uf::loadUf( $eachUserInfo->userInfo->stateAddress->descricao , $eachUserInfo->userInfo->stateAddress->sigla );
				
				$userInfo = UserInfo::loadUserInfo(
					$eachUserInfo->userInfo->address , 
					$eachUserInfo->userInfo->alternativeEmail , 
					@$eachUserInfo->userInfo->birthDate ,
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
					$eachUserInfo->userInfo->workInstitution
				);
				
				/*
				echo "(each) userInfo - dentro do loop > ";
				echo "<pre>";
				var_dump($eachUserInfo->userInfo->);
				echo "<pre>";			
				*/			
				
				$userInfoAndPermissionId[$contador] = UserInfoAndPermissionId::loadUserInfoAndPermissionId( 
					$eachUserInfo->permissionId ,
					$eachUserInfo->userInfo->userId ,
					$userInfo
				);
				
				$contador++;
				
			}
			
			//}
			
			/*
			echo "arrUserAndPermission (retorno WsUser.class) > ";
			echo "<pre>";
			var_dump($arrUserAndPermission);
			echo "<pre>";			
			*/
			
			return( $userInfoAndPermissionId );
			
		}
		
		/**
		 * Consulta as permissoes de um usuario
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
			
			debug($retorno);
			
			//echo ($retorno->justificationOfStatusChange);
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			$contador = 0;
			
			$userPermissionInfo = array();
			
			foreach ($retorno as $eachUserPermissionInfo) {
			
				//debug($eachUserPermissionInfo->permissionId);
			
				$userPermissionInfo[$contador] = UserPermissionInfo::loadPermissionInfo( 
					@$eachUserPermissionInfo->justificationOfStatusChange ,
					$eachUserPermissionInfo->permissionId ,
					@$eachUserPermissionInfo->requiredDataStatus ,
					@$eachUserPermissionInfo->responsibleIdForStatusChange ,
					@$eachUserPermissionInfo->userPermissionStatus
				);
			
				$contador++;
			}
			
			//debug($userPermissionInfo);
			
			return($userPermissionInfo);
		}
		
		
		/************************************************* NOVO MÉTODO: CONSULTAR OS PERFIS DO SISTEMA *************************************************/
		/* \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
		
		/**
		 * Consulta as permissoes do sistema
		 *
		 * @return UserPermissionInfo
		 * Entidade contendo informacoes como o status da permissao do usuario,
		 * status dos dados obrigatorios, identificador do usuario responsavel pela alteracao
		 * do status da permissao do usuario, justificativa da alteracao
		 */
		public function getSystemPermissionsInfo() {
			if (!$this->wsConnector) {
				$msg_conf = "<pre>Chame algum metodo de configuracao para os ambiente de homologacao ou producao.</pre>";
				print_r($msg_conf);
				exit;
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);

			$params = array(/* 
				"userId" => $userId ,
				"permissionId" => $permissionId*/
			);
			
			$retorno = $this->wsConnector->call(
				"getSystemPermissionsInfo",
				$params,
				$options
			);	 
			
			//debug($retorno);
			
			//echo ($retorno->justificationOfStatusChange);
			
			if ($retorno instanceof SOAP_Fault) {
				return array("erro" => $retorno->message);
			}
			
			$contador = 0;
			
			$systemPermissionInfo = array();
			
			foreach ($retorno as $eachSystemPermissionInfo) {
			
				$systemPermissionInfo[$contador] = ConfigurationPermissionInfo::loadPermissionInfo( 
					@$eachSystemPermissionInfo->id ,
					@$eachSystemPermissionInfo->personType ,
					@$eachSystemPermissionInfo->registerStatus ,
					@$eachSystemPermissionInfo->changeJustify ,
					@$eachSystemPermissionInfo->defaultPermission ,
					@$eachSystemPermissionInfo->changeUserResponsible ,
					@$eachSystemPermissionInfo->description ,
					@$eachSystemPermissionInfo->sgProfile
				);
			
				$contador++;
			}
			
			//debug($userPermissionInfo);
			
			return($systemPermissionInfo);
		}		
		
		/*                                                            ACIMA                                                                              */
		/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		/************************************************* NOVO MÉTODO: CONSULTAR OS PERFIS DO SISTEMA *************************************************/		
		
		
		
		/************************************************* NOVO MÉTODO: CONSULTAR OS DADOS DO USUARIO ***************************************************/
		/* \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */		
		
		/**
		 * Consulta dados de um usuario
		 *
		 * @param string login
		 * @param string cpfOrCnpj
		 * @param string email
		 * @param string nis
		 * @return UserInfo
		 * Entidade contendo informacoes completas sobre um usuario
		 */
		public function getUserAndPermissionInfo($login, $cpfOrCnpj, $email, $nis) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"login" => $login ,
				"cpfOrCnpj" => $cpfOrCnpj ,
				"email" => $email ,
				"nis" => $nis ,
			);
			

			$retorno = $this->wsConnector->call(
				"getUserAndPermissionInfo",
				$params,
				$options
			);	 
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			
			debug($retorno);
						
			//city Address
			$ufCityAddress = Uf::loadUf( $retorno->userInfo->cityAddress->estado->descricao , $retorno->userInfo->cityAddress->estado->sigla );
			$cityAddress = City::loadCity( $retorno->userInfo->cityAddress->codigoIBGE , $retorno->userInfo->cityAddress->descricao , $ufCityAddress , $retorno->userInfo->cityAddress->siglaEstado );
			
			//naturality
			$ufCityNaturalidade = Uf::loadUf( $retorno->userInfo->naturality->estado->descricao , $retorno->userInfo->naturality->estado->sigla );
			$cityNaturalidade = City::loadCity( $retorno->userInfo->naturality->codigoIBGE , $retorno->userInfo->naturality->descricao , $ufCityNaturalidade , $retorno->userInfo->naturality->siglaEstado );
			
			$ufUser = Uf::loadUf( $retorno->userInfo->stateAddress->descricao , $retorno->userInfo->stateAddress->sigla );
			
			$userInfo = UserInfo::loadUserInfo(
				$retorno->userInfo->address , 
				$retorno->userInfo->alternativeEmail , 
				$retorno->userInfo->birthDate ,
				$retorno->userInfo->cellPhoneNumber ,
				$cityAddress , 
				$retorno->userInfo->cnpj ,
				$retorno->userInfo->cpf ,
				$retorno->userInfo->dispatcherAgency ,
				$retorno->userInfo->email , 
				$retorno->userInfo->login ,
				$retorno->userInfo->lotacao , 
				$retorno->userInfo->name , 
				$retorno->userInfo->nationality , 
				$cityNaturalidade ,
				$retorno->userInfo->nis, 
				$retorno->userInfo->postalCode ,
				$retorno->userInfo->responsibleCpf , 
				$retorno->userInfo->responsibleName ,
				$retorno->userInfo->rg ,
				$retorno->userInfo->socialReason ,
				$ufUser ,
				$retorno->userInfo->telephoneNumber ,
				$retorno->userInfo->workInstitution
			);
					
			//debug ($userInfo);					
						
			$contador = 0;
			
			$userPermissionInfo = array();
			
			foreach ($retorno->userPermissionsInfo as $eachUserPermissionInfo) {
			
				$userPermissionInfo[$contador] = UserPermissionInfo::loadPermissionInfo( 
					@$eachUserPermissionInfo->justificationOfStatusChange ,
					@$eachUserPermissionInfo->permissionId ,
					@$eachUserPermissionInfo->requiredDataStatus ,
					@$eachUserPermissionInfo->responsibleIdForStatusChange ,
					@$eachUserPermissionInfo->userPermissionStatus
				);
			
				$contador++;
			}			

			//debug ( $userPermissionInfo );
			
			$userAndPermissionInfo = UserAndPermissionInfo::loadUserAndPermissionInfo(
				$userInfo,
				$userPermissionInfo
			);
			
			//debug ( $userAndPermissionInfo );
			
			return $userAndPermissionInfo;
		}		
		
		/* 																ACIMA																			*/
		/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		/************************************************* NOVO MÉTODO: CONSULTAR OS DADOS DO USUARIO ***************************************************/		
		
				
		/*************************************** NOVO MÉTODO: CONSULTAR USUARIOS PELO STATUS DA PERMISSAO ************************************************/
		/* \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */		
		
		/**
		 * Recupera usuarios que possuem permissoes com status 
		 * 'Aguardando liberacao'
		 *
		 * @return array
		 * Array de UserAndPermissionIds contendo conjunto de pares usuario-permissao
		 * com status 'Aguardando liberacao'
		 */
		public function getUsersByPermissionStatus($status) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array(
				"status" => $status ,
			) ;
			
			$retorno = $this->wsConnector->call(
				"getUsersByPermissionStatus",
				$params,
				$options
			);	 
			
			if( $retorno instanceof SOAP_Fault ) {
				throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
			}
			
			debug($retorno);
			
			//$retorno = array();
			/*
			echo "retorno >";
			echo "<pre>";
			var_dump($retorno);
			echo "<pre>";
			*/
			//return ($retorno);
			
			$contador = 0;
			
			$userInfoAndPermissionId = array();
			
			foreach ( $retorno as $eachUserInfo) {
				
				/*$userAndPermissionIds = $retorno->getUserAndPermissionIds();*/
				
				//$userAndPermission = UserAndPermissionIds::loadUserAndPermissionIds( $eachUserInfo->permissionId , $eachUserInfo->userInfo->userId );

				/*
				echo "eachUserInfo->userInfo >";
				echo "<pre>";
				var_dump($eachUserInfo->userInfo);
				echo "<pre>";
				*/

				//city Address
				@$ufCityAddress = Uf::loadUf( $eachUserInfo->userInfo->cityAddress->estado->descricao , $eachUserInfo->userInfo->cityAddress->estado->sigla );
				@$cityAddress = City::loadCity( $eachUserInfo->userInfo->cityAddress->codigoIBGE , $eachUserInfo->userInfo->cityAddress->descricao , $ufCityAddress , $eachUserInfo->userInfo->cityAddress->siglaEstado );
			
				//naturality
				@$ufCityNaturalidade = Uf::loadUf( $eachUserInfo->userInfo->naturality->estado->descricao , $eachUserInfo->userInfo->naturality->estado->sigla );
				@$cityNaturalidade = City::loadCity( $eachUserInfo->userInfo->naturality->codigoIBGE , $eachUserInfo->userInfo->naturality->descricao , $ufCityNaturalidade , $eachUserInfo->userInfo->naturality->siglaEstado );
			
				@$ufUser = Uf::loadUf( $eachUserInfo->userInfo->stateAddress->descricao , $eachUserInfo->userInfo->stateAddress->sigla );
				
				$userInfo = UserInfo::loadUserInfo(
					$eachUserInfo->userInfo->address , 
					$eachUserInfo->userInfo->alternativeEmail , 
					@$eachUserInfo->userInfo->birthDate ,
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
					$eachUserInfo->userInfo->workInstitution
				);
				
				/*
				echo "(each) userInfo - dentro do loop > ";
				echo "<pre>";
				var_dump($eachUserInfo->userInfo->);
				echo "<pre>";			
				*/			
				
				$userInfoAndPermissionId[$contador] = UserInfoAndPermissionId::loadUserInfoAndPermissionId( 
					$eachUserInfo->permissionId ,
					$eachUserInfo->userInfo->userId ,
					$userInfo
				);
				
				$contador++;
				
			}
			
			//}
			
			/*
			echo "arrUserAndPermission (retorno WsUser.class) > ";
			echo "<pre>";
			var_dump($arrUserAndPermission);
			echo "<pre>";			
			*/
			
			return( $userInfoAndPermissionId );
			
		}

		/* 																ACIMA																			*/
		/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		/*************************************** NOVO MÉTODO: CONSULTAR USUARIOS PELO STATUS DA PERMISSAO ************************************************/
		
		
	/***************************************************** NOVO MÉTODO ***************************************************************************************/
	//*******************************************************************************************************************************************************//
		/**
	 * Retorna as informacoes basicas de autenticacao do usuario.
	 * Informacoes: id do usuario, nome, tipo de autenticacao, perfis 
	 * validos e invalidos, marcador, tipo do identificador utilizado 
	 * (login, cpf, email, rg...), e o valor fornecido pelo usuario 
	 * para este identificador.
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao
	 * do ambiente deve ser chamado.
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $ticketId 
	 * 	O identificador do ticket de autenticacao de usuario.
	 *
	 * @return UserAuthInfo
	 * 	As informacoes basicas da autenticacao do usuario.
	 */
	public function getUserMaintenanceTicket($userTicketInfoId){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);		
		$params = array( "userMaintanceTicketId" => $userTicketInfoId);
		$retorno = $this->wsConnector->call(
			"getUserMaintenanceTicket",
			$params,
			$options);

		if($retorno instanceof SOAP_Fault) {
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}

		
		debug($retorno);
		
		$authType = AuthTypeInfo::loadAuthTypeInfo( $retorno->authType->id , $retorno->authType->description );
		$loginIdentifier = LoginIdentifier::loadLoginIdentifier( $retorno->identifier->field , $retorno->identifier->value );

		$arrInvalidPermission = array();
		
		//debug($retorno->notValidPermission);
		
		/*		
		foreach ($retorno->notValidPermission as $invalidPermission) {
			
			debug($invalidPermission);
			
			$arrInvalidPermission[] = InvalidUserPermission::loadInvalidUserPermission(
				$invalidPermission->exceptionMsgs , 
				$invalidPermission->permission->configPermissionId
			);
		}*/
		
		foreach ($retorno->validPermission as $validPermission) {
			
			debug($validPermission);
			
			$arrValidPermission[] = validUserPermission::loadInvalidUserPermission(
				$invalidPermission->exceptionMsgs , 
				$invalidPermission->permission->configPermissionId
			);
		}		
		
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
	/***************************************************** NOVO MÉTODO ***************************************************************************************/
	//*******************************************************************************************************************************************************//		
	
	
	/***************************************************** NOVO MÉTODO ***************************************************************************************/
	//*******************************************************************************************************************************************************//
	/**
	 * Retorna as informacoes basicas de autenticacao do usuario.
	 * Informacoes: id do usuario, nome, tipo de autenticacao, perfis 
	 * validos e invalidos, marcador, tipo do identificador utilizado 
	 * (login, cpf, email, rg...), e o valor fornecido pelo usuario 
	 * para este identificador.
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao
	 * do ambiente deve ser chamado.
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $ticketId 
	 * 	O identificador do ticket de autenticacao de usuario.
	 *
	 * @return UserAuthInfo
	 * 	As informacoes basicas da autenticacao do usuario.
	 */
	public function getUserMaintenanceTicketInfo($userId, $systemId, $flag, $serviceId){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);		
		$params = array(
			"userId" => $userId ,
			"systemId" => $systemId ,
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
		
		$authType = AuthTypeInfo::loadAuthTypeInfo( $retorno->authType->id , $retorno->authType->description );
		$loginIdentifier = LoginIdentifier::loadLoginIdentifier( $retorno->identifier->field , $retorno->identifier->value );

		$arrInvalidPermission = array();
		
		//debug($retorno->notValidPermission);
		
		/*		
		foreach ($retorno->notValidPermission as $invalidPermission) {
			
			debug($invalidPermission);
			
			$arrInvalidPermission[] = InvalidUserPermission::loadInvalidUserPermission(
				$invalidPermission->exceptionMsgs , 
				$invalidPermission->permission->configPermissionId
			);
		}*/
		
		foreach ($retorno->validPermission as $validPermission) {
			
			debug($validPermission);
			
			$arrValidPermission[] = validUserPermission::loadInvalidUserPermission(
				$invalidPermission->exceptionMsgs , 
				$invalidPermission->permission->configPermissionId
			);
		}		
		
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
	/***************************************************** NOVO MÉTODO ***************************************************************************************/
	//*******************************************************************************************************************************************************//	
		
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
			
			debug($retorno);

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
		 * Requisita Url de Manutenção de Usuário
		 *
		 * @param string $flag
		 * @param string $serviceId
		 * @return TicketUserInfo
		 * Entidade contendo o identificador do usuario e a flag
		 */
		public function getUserMaintenanceUrl($flag, $serviceId) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"flag" => $flag,
				"serviceId" => $serviceId
			);
			
			$retorno = $this->wsConnector->call(
				"getUserMaintenanceUrl",
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

		public function getMaintenanceTicketInfo($flag){
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
						" para os ambiente de homologacao".
						" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array( "namespace" => $this->wsdl->namespaces['tns']);		
			$params = array( "flag" => $flag);
			$retorno = $this->wsConnector->call(
				"getMaintenanceTicketInfo",
				$params,
				$options);
	
			if($retorno instanceof SOAP_Fault) {
				throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
			}

			
			debug($retorno);
		}

		/* NOVO */
		public function getDateByTimestamp($timestampCreation, $timestampExpiration) {
			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
						" para os ambiente de homologacao".
						" ou producao.";
				throw new Exception($msg_conf);
			}
			$options = array( "namespace" => $this->wsdl->namespaces['tns']);		
			$params = array( 
				"timestampCreation" => $timestampCreation,
				"timestampExpiration" => $timestampExpiration
			);
			$retorno = $this->wsConnector->call(
				"getDateByTimestamp",
				$params,
				$options);
	
			if($retorno instanceof SOAP_Fault) {
				throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
			}

			
			debug($retorno);			
		}
		/* NOVO */

		/**
		 * Consulta dados de um usuario
		 *
		 * @param string login
		 * @param string cpfOrCnpj
		 * @param string email
		 * @param string nis
		 * @return UserInfo
		 * Entidade contendo informacoes completas sobre um usuario
		 */
		public function getUserInfoByOneOfTheIdentifiers($login, $cpfOrCnpj, $email, $nis) {

			if (!$this->wsConnector) {
				$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
				throw new Exception($msg_conf);
			}
			
			$options = array("namespace" => $this->wsdl->namespaces['tns']);
			$params = array( 
				"login" => $login ,
				"cpfOrCnpj" => $cpfOrCnpj ,
				"email" => $email ,
				"nis" => $nis ,
			);
			
			$retorno = $this->wsConnector->call(
				"getUserInfoByOneOfTheIdentifiers",
				$params,
				$options
			);	 
			
			if ($retorno instanceof SOAP_Fault) {
				throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
			}
			debug($retorno);
						
			//city Address
			$ufCityAddress = Uf::loadUf( $retorno->userInfo->cityAddress->estado->descricao , $retorno->userInfo->cityAddress->estado->sigla );
			$cityAddress = City::loadCity( $retorno->userInfo->cityAddress->codigoIBGE , $retorno->userInfo->cityAddress->descricao , $ufCityAddress , $retorno->userInfo->cityAddress->siglaEstado );
			
			//naturality
			$ufCityNaturalidade = Uf::loadUf( $retorno->userInfo->naturality->estado->descricao , $retorno->userInfo->naturality->estado->sigla );
			$cityNaturalidade = City::loadCity( $retorno->userInfo->naturality->codigoIBGE , $retorno->userInfo->naturality->descricao , $ufCityNaturalidade , $retorno->userInfo->naturality->siglaEstado );
			
			$ufUser = Uf::loadUf( $retorno->userInfo->stateAddress->descricao , $retorno->userInfo->stateAddress->sigla );
			
			$userInfo = UserInfo::loadUserInfo(
				$retorno->userInfo->address , 
				$retorno->userInfo->alternativeEmail , 
				$retorno->userInfo->birthDate ,
				$retorno->userInfo->cellPhoneNumber ,
				$cityAddress , 
				$retorno->userInfo->cnpj ,
				$retorno->userInfo->cpf ,
				$retorno->userInfo->dispatcherAgency ,
				$retorno->userInfo->email , 
				$retorno->userInfo->login ,
				$retorno->userInfo->lotacao , 
				$retorno->userInfo->name , 
				$retorno->userInfo->nationality , 
				$cityNaturalidade ,
				$retorno->userInfo->nis, 
				$retorno->userInfo->postalCode ,
				$retorno->userInfo->responsibleCpf , 
				$retorno->userInfo->responsibleName ,
				$retorno->userInfo->rg ,
				$retorno->userInfo->socialReason ,
				$ufUser ,
				$retorno->userInfo->telephoneNumber ,
				$retorno->userInfo->workInstitution
			);
					
			//debug ($userInfo);					
						
			$contador = 0;
			
			$userPermissionInfo = array();
				

		

			foreach ($retorno->userPermissionsInfo as $eachUserPermissionInfo) {
			
				$userPermissionInfo[$contador] = UserPermissionInfo::loadPermissionInfo( 
					@$eachUserPermissionInfo->justificationOfStatusChange ,
					@$eachUserPermissionInfo->permissionId ,
					@$eachUserPermissionInfo->requiredDataStatus ,
					@$eachUserPermissionInfo->responsibleIdForStatusChange ,
					@$eachUserPermissionInfo->userPermissionStatus
				);
			
				$contador++;
			}			

			//debug ( $userPermissionInfo );
			
			$userAndPermissionInfo = UserAndPermissionInfo::loadUserAndPermissionInfo(
				$userInfo,
				$userPermissionInfo
			);
			
			//debug ( $userAndPermissionInfo );
			
			return $userAndPermissionInfo;
		}		

		
		private function loadProductionUrls() {
			$this->fileUploadUrl = 
				"https://ssd.mec.gov.br/ssdserver/servlet/UploadTmpDoc";
			$this->wsdlUrl = 
				"https://ssd.mec.gov.br/ssdserver/services/UserMaintenance?wsdl";
			$this->downloadSignaturePackageUrl = 
				"https://ssd.mec.gov.br/ssdserver/servlet/DownloadSignaturePackage";
			$this->appletUrl = 
				"http://ssd.mec.gov.br/applet/SSDApplet.jar";
		}
		
		private function loadHomologationUrls() {
			$this->fileUploadUrl = 
				"https://ssdhmg.mec.gov.br/ssdserver/servlet/UploadTmpDoc";
			$this->wsdlUrl = 
				"https://ssdhmg.mec.gov.br/ssdserver/services/UserMaintenance?wsdl";
			$this->downloadSignaturePackageUrl = 
				"https://ssdhmg.mec.gov.br/ssdserver/servlet/DownloadSignaturePackage";
			$this->appletUrl = 
				"http://ssdhmg.mec.gov.br/applet/SSDApplet.jar";
		}
	}
?>
