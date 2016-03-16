<?php
require_once("SSDConnector.class.php");
require_once("TO/AppletTicketInfo.php");
require_once("TO/UserAuthInfo.php");

/**
 * Classe de acesso aos servicos de autenticao do SSD.
 *
 * Estao disponiveis metodos para os tres modos de autenticacao da applet e um
 * metodo para a recuperacao das informacoes da autenticacao do usuario.
 *
 * O codigo desta classe deve estar no mesmo local que o da classe 
 * "SSDConnector", que se encontra no arquivo "SSDConnector.class.php".
 *
 * O processo de autenticacao divide-se em dois passos: A) o primeiro onde a 
 * aplicacao requisita o ticket para abrir a applet no navegador do usuario; 
 * B) e o segundo onde a aplicacao recebe a respota do processo de autenticacao
 * feito pela applet na forma de um outro ticket (o ticket de autenticacao de
 * usuario) e recupera a informacoes da autenticacao do usuario.
 * 
 * Seguem-se exemplos para estes dois momentos do processo.
 * 
 * No primeiro (A) momento, para abrir a applet de autenticacao no modo de 
 * identificador e senha usando o ambiente de homologacao:
 * <pre>
 *	<?php
 *		//configura o cliente
 *		$tmpDir = '/tmp';
 *		$clientCert = '/caminho/para/certificado.pem';
 *		$privateKey = '/caminho/para/chave_privada.pem';
 *		$privateKeyPassword = 'senha_da_chave_privada';
 *		$trustedCaChain = '/caminho/para/autoridades_confiadas.pem';
 *
 *		//instanciacao do cliente de autenticacao do SSD
 *		$ssdWsAutn = 
 *			new SSDWsAuth(
 *				$tmpDir,$clientCert,$privateKey,
 *				$privateKeyPassword,$trustedCaChain);
 *
 *		//configura a utilizacao do ambiente de homologacao
 *		$ssdWsAutn->useHomologationSSDServices(); 
 *
 *		//indica o modulo de onde esta sendo chamada a autenticacao
 *		$appletTicketId = 
 *			$ssdWsAutn->getIdentifierAppletTicket('modulo_alfa');
 *
 *		//imprime o codigo de carga da applet
 *		echo $ssdWsAutn->getAppletHtmlSampleCode($appletTicketId); 
 *	?>
 * </pre>
 *
 * No segundo (B) momento, para, como o ticket de autenticacao do usuario,
 * recuperar as informacoes basicas da autenticacao deste:
 * <pre>
 *	<?php
 *		//recupera o ticket de autenticacao do usuario
 *		$userAuthTicketId = $_REQUEST['userAuthTicketId'];
 *
 *		//configurar o cliente para o servico de autenticacao do SSD
 *		//vide exemplo do primeiro passo
 *		$userAuthInfo = 
 *			$ssdWsAuth->getUserAuthInfoByUserTicketInfo(
 *				$userAuthTicketId);
 *
 *		//testa se a autenticacao foi chamada para o modulo especifico
 *		if ( $userAuthInfo->flag == 'modulo_alfa') {
 *			//o identificador do usuario
 *			echo $userAuthInfo->id;
 *			//o nome do usuario
 *			echo $userAuthInfo->name; 
 *			//a data da ultima atualizacao do cadastro do usuario
 *			echo $userAuthInfo->lastUpdateDate;
 *			//e assim por diante
 *		}
 *	?>
 * </pre>
 */
class SSDWsAuth {
	
	private $ssdConnector;
	private $wsConnector;
	private $wsdl;
	private $tmpDir;
	private $clientCert;
	private $privateKey;
	private $privateKeyPassword;
	private $trustedCaChain;
	private $appletUrl;
	private $fileUploadUrl;
	private $wsdlUrl;
	
	/**
	 * Configura o acesso aos servicos de autenticacao do SSD.
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
			$tmpDir,$clientCert,$privateKey,
			$privateKeyPassword,$trustedCaChain){
		$this->tmpDir = $tmpDir;
		$this->clientCert = $clientCert;
		$this->privateKey = $privateKey;
		$this->privateKeyPassword = base64_decode($privateKeyPassword);
		$this->trustedCaChain = $trustedCaChain;
	}

	/**
	 * Configura o cliente dos servicos de autenticacao do SSD a utilizar 
	 * os servicos de producao.
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
	 * Configura o cliente dos servicos de autenticacao do SSD a utilizar 
	 * os servicos de homologacao.
	 *
	 * Um dos metodos que configura a utilizacao do ambiente de 
	 * homologacao ou producao deve ser chamado obrigatoriamente antes de
	 * fazer qualquer chamada aos servicos.
	 *
	 * @see useProductionSSDServices
	 */
	public function useHomologationSSDServices() {
		$this->loadHomologationUrls();
		
		/*
		echo	"tmpdir".			$this->tmpDir . "<br>"; 
		echo	"cert".		$this->clientCert . "<br>";
		echo	"privatekey".	$this->privateKey . "<br>";
		echo	"pass".	$this->privateKeyPassword . "<br>";
		echo	"chain".		$this->trustedCaChain . "<br>";
		*/
		
		$this->ssdConnector = 
			new SSDConnector(
				$this->tmpDir,
				$this->clientCert,
				$this->privateKey,
				$this->privateKeyPassword,
				$this->trustedCaChain);
		$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl);
		$this->wsdl = $this->ssdConnector->getWSDL();
		
		//echo $this->wsdl;
	}

	/**
	 * Retorna o ticket para a abertura da applet no modo de autenticacao
	 * por idetificador e senha.
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao do
	 * ambiente deve ser chamado. Esse metodo invoca o metodo getAppletTicketInfoForAuthById
	 * no Webservice
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de autenticacao do 
	 * 	usuario exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return AppletTicketInfo
	 * 	O ticket para a abertura da applet de autenticacao por
	 *	identificador e senha, data de expiracao e data de criacao.
	 */
	public function getIdentifierAppletTicket($marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker);
		$retorno = $this->wsConnector->call(
			"getAppletTicketInfoForAuthById",
			$params,
			$options);
		
		if( $retorno instanceof SOAP_Fault )
		{
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		$appletTicketInfo = AppletTicketInfo::loadAppletTicketInfo( 
			$retorno->creationTimestamp ,
			$retorno->expirationTimestamp , 
			$retorno->id
		);
		
		return ( $appletTicketInfo );
	}




	/* ************************************************************************************************************************************ */
																// NOVO MÉTODO //

	
	/**
	 * Recupera o ticket da tela de autenticação web recupera o ticket da tela de autenticação web, 
	 * por identificador e senha, e o redireciona junto com a  URL da tela 		     
	 * de autenticação web
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao do
	 * ambiente deve ser chamado. Esse metodo invoca o metodo getAppletTicketInfoForAuthById
	 * no Webservice
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de autenticacao do 
	 * 	usuario exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return UrlInfo
	 * 	Entidade contendo a url da tela de autenticação web.
	 */
	 
	public function getAuthenticationByIdServletUrl($marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker);
		$retorno = $this->wsConnector->call(
			"getUrlInfoForAuthByIdServlet",
			$params,
			$options);
		
		if( $retorno instanceof SOAP_Fault ) {
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		//var_dump($retorno);
		
		$urlInfo = UrlInfo::loadUrlInfo(
			$retorno->url);
		
		return ( $urlInfo );
	}


	
	/* *********************************************************************************************************************************************** */	



	/* ************************************************************************************************************************************ */
																// NOVO MÉTODO //
	/**
	 * Recupera o ticket da tela de autenticação web recupera o ticket da tela de autenticação web, 
	 * por identificador e senha, e o redireciona junto com a  URL da tela 		     
	 * de autenticação web
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao do
	 * ambiente deve ser chamado. Esse metodo invoca o metodo getAppletTicketInfoForAuthById
	 * no Webservice
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de autenticacao do 
	 * 	usuario exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return UrlInfo
	 * 	Entidade contendo a url da tela de autenticação web.
	 */
	 
	public function getExternalUserAuthInfo($marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker);
		$retorno = $this->wsConnector->call(
			"getExternalUserAuthInfo",
			$params,
			$options);
		
		if( $retorno instanceof SOAP_Fault ) {
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		debug($retorno);
		
		/*
		$urlInfo = UrlInfo::loadUrlInfo(
			$retorno->url);
		*/
		
		return ( $urlInfo );
	}
	/* *********************************************************************************************************************************************** */	


	/* ************************************************************************************************************************************ */
																// NOVO MÉTODO //
	/**
	 * Recupera o ticket da tela de autenticação web recupera o ticket da tela de autenticação web, 
	 * por identificador e senha, e o redireciona junto com a  URL da tela 		     
	 * de autenticação web
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao do
	 * ambiente deve ser chamado. Esse metodo invoca o metodo getAppletTicketInfoForAuthById
	 * no Webservice
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de autenticacao do 
	 * 	usuario exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return UrlInfo
	 * 	Entidade contendo a url da tela de autenticação web.
	 */
	 
	public function getExternalUserInfo($ticket){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "ticket" => $ticket);
		$retorno = $this->wsConnector->call(
			"getExternalUserInfo",
			$params,
			$options);
		
		if( $retorno instanceof SOAP_Fault ) {
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		debug($retorno);
		
		/*
		$urlInfo = UrlInfo::loadUrlInfo(
			$retorno->url);
		*/
		
		return ( $urlInfo );
	}
	/* *********************************************************************************************************************************************** */	

	/**
	 * Retorna o ticket para a abertura da applet no modo de autenticacao 
	 * por certificacao digital e por idetificador e senha, sendo a opcao
	 * a criterio do usuario.
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao 
	 * do ambiente deve ser chamado.
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de autenticacao do 
	 * 	usuario exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return AppletTicketInfo
	 * 	O ticket para a abertura da applet de autenticacao por 
	 *	certificado digital e identificador e senha.
	 */
	public function getIdentifierAppletTicketByIdOrCert($marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker);
		$retorno = $this->wsConnector->call(
			"getAppletTicketInfoForAuthByIdOrCert",
			$params,
			$options);
			
		if( $retorno instanceof SOAP_Fault )
		{
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		$appletTicketInfo = AppletTicketInfo::loadAppletTicketInfo( 
			$retorno->creationTimestamp ,
			$retorno->expirationTimestamp , 
			$retorno->id
		);
		
		return ( $appletTicketInfo );
	}

	/**
	 * Retorna o ticket para a abertura da applet no modo de autenticacao
	 * por certificacao digital.
	 *
	 * Antes da invocacacao deste metodo, um dos metodos de configuracao 
	 * do ambiente deve ser chamado.
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de autenticacao do 
	 * 	usuario exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return AppletTicketInfo
	 * 	O ticket para a abertura da applet de autenticacao por
	 *	identificador e senha, data de expiracao e data de criacao.
	 */
	public function getIdentifierAppletTicketByCert($marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker);
		$retorno = $this->wsConnector->call(
			"getAppletTicketInfoForAuthByCert",
			$params,
			$options);
			
		if($retorno instanceof SOAP_Fault) {
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		$appletTicketInfo = AppletTicketInfo::loadAppletTicketInfo( 
			$retorno->creationTimestamp ,
			$retorno->expirationTimestamp , 
			$retorno->id
		);
		
		return ( $appletTicketInfo );
	}

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
	public function getUserAuthInfo($userTicketInfoId){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);		
		$params = array( "userTicketInfoId" => $userTicketInfoId);
		$retorno = $this->wsConnector->call(
			"getUserAuthInfo",
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
	 * @return 
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
	 * Realiza o processo de autenticação por login (base64)  e senha (base64). 
	 *
	 * @see useProductionSSDServices
	 * @see useHomologationSSDServices
	 *
	 * @return UrlInfo
	 * 	Um objeto com a url de redirecionamento.
	 */
	public function userAuthByIdentifierAndPassword($login, $senha, $marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array("namespace" => $this->wsdl->namespaces['tns']);
		$params = array(
					"login" => $login,
					"password" => $password,
					"flag" => $flag
					);
		$retorno = $this->wsConnector->call(
			"userAuthByIdentifierAndPassword",
			$params,
			$options);
		
		if ($retorno instanceof SOAP_Fault) {
			throw new Exception("Erro ao buscar informacoes do webservice: " . $retorno->message);
		}
		
		return ($urlInfo);
	}
	
	
	/**
	 * Logar usuario no sistema por CPF/CNPJ (BASE64) e senha (BASE64). 
	 */
	public function loginUserIntoSystemByCPFOrCNPJAndPassword($cpfOrCnpj, $password) {
		if (!$this->wsConnector) {
			$msgConfig = "Chame algum m&eacute;todo de configura&ccedil;&atilde;o".
				" para os ambientes de homologa&ccedil;&atilde;o".
				" ou produ&ccedil;&atilde;o.";
			throw new Exception($msgConfig);
		}
		$options = array("namespace" => $this->wsdl->namespaces["tns"]);
		$params = array(
			"cpfOrCnpj" => $cpfOrCnpj,
			"password" => $password
		);
		$retorno = $this->wsConnector->call(
			"loginUserIntoSystemByCPFOrCNPJAndPassword",
			$params,
			$options
		);
		if ($retorno instanceof SOAP_Fault) {
			throw new Exception("Erro ao buscar informa&ccedil;&otilde;es do WebService: " . $retorno->message);
		}
		return ($retorno);
	}
	
	
	private function loadProductionUrls() {
		$this->wsdlUrl = 
			"https://ssd.mec.gov.br/ssdserver/services/Authentication?wsdl";
		$this->appletUrl = 
			"http://ssd.mec.gov.br/applet/SSDApplet.jar";

	}
	
	private function loadHomologationUrls() {
		$this->wsdlUrl = 
			"https://ssdhmg.mec.gov.br/ssdserver/services/Authentication?wsdl";
		$this->appletUrl = 
			"http://ssdhmg.mec.gov.br/applet/SSDApplet.jar";			
	}
}
?>
