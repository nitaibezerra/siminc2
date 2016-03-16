<?php
require_once("TO/UploadedDoc.php");
require_once("TO/DocumentSignatureInfo.php");
/**
 * Classe de acesso aos servicos de assinatura do SSD.
 *
 */
class SSDWSSignDocs {
	
	private $ssdConector;
	private $wsConector;
	private $wsdl;
	private $tmpDir;
	private $clientCert;
	private $privateKey;
	private $privateKeyPassword;
	private $trustedCaChain;
	private $appletUrl;
	private $fileUploadUrl;
	private $downloadSignaturePackageUrl;
	private $wsdlUrl;
	private $codebase;
	private $appletClass;
	
	/**
	 * Configura o acesso aos servicos de assinatura do SSD.
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
		$this->privateKeyPassword = $privateKeyPassword;
		$this->trustedCaChain = $trustedCaChain;
	}

	/**
	 * Configura o cliente dos servicos de assinatura do SSD a utilizar 
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
		$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl, 'SIGN'); 
		$this->wsdl = $this->ssdConnector->getWSDL();
	}

	/**
	 * Configura o cliente dos servicos de assinatura do SSD a utilizar 
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
		$this->ssdConnector = 
			new SSDConnector(
				$this->tmpDir,
				$this->clientCert,
				$this->privateKey,
				$this->privateKeyPassword,
				$this->trustedCaChain);
		$this->wsConnector = $this->ssdConnector->getWsClient($this->wsdlUrl, 'SIGN');
		$this->wsdl = $this->ssdConnector->getWSDL();
	}
	

	/**
	 * Carrega para o SSD o documento a ser assinado pelo usuario.
	 *
	 * Utilizado para enviar um documento gerado pelo sistema ao SSD que 
	 * o usuario deve assinar. O sistema envia tal documento para o SSD 
	 * e recebe um ticket para abertura da applet
	 *
	 * @param $aboluteFilePath
	 *	O caminho absoluto do documento a ser carregado para o SSD.
	 *	 
	 * @param string $cpfOrCnpj
	 * CPF ou CNPJ do usuario que tera direito de assinar o documento
	 *
	 * @param string $marker 
	 * O marcador sera retornado nas informacoes de assinatura 
	 * exatamente como foi fornecido. 
	 * Podera ser utilizado pela aplicacao para identificar de onde
	 * partiu a solicitacao do servico (processo ou modulo).
	 * O marcador pode ser fornecido como vazio, porem nunca deve
	 * ser fornecido como nulo.
	 *
	 * @return UploadedDoc 
	 * XML com ticket para abertura da applet de assinatura de documentos gerados pela
	 * aplicacao e hash do documento carregado	
	 *
	 */
	public function loadDocumentForUserSigning($aboluteFilePath , $cpfOrCnpj = null , $mark = null ) {
		
		//verifica se arquivo existe
		if(is_file($aboluteFilePath)){
			//do nothing
		} else {
			throw new Exception('O arquivo nao existe: ' . $aboluteFilePath);
		}
		
		
		//conecta ao servidor requisitando o upload-id
		$uploadId = $this->retrieveUploadId( $cpfOrCnpj , $mark );
		//cabecalhos adicionais para o POST
		$headerData = array( 
			"Content-Upload-Id: " . $uploadId 
		);
		
		
		
		//Posta documento a ser enviado
		$postData = array(
			'documentToSign' => "@$aboluteFilePath"
		);

		//envia requisicao
		$response = $this->ssdConnector->post( $this->fileUploadUrl, $postData , $headerData , TRUE );
		if ($response) {
			//do nothing
		} else {
			throw new Exception('Impossivel ler a resposta.');
		}
		
		$xml = @simplexml_load_string($response);
		if ($xml) {
			//do nothing
		} else {
			throw new Exception('Formato da resposta invalido.');
		}
		
		$uploadDoc = UploadedDoc::loadUploadedDoc( (string) $xml->{"applet-ticket-id"} , (string) $xml->{"file-hash-sha256-base16"} );
		return ( $uploadDoc );
	}
	
	/**
	 * Conecta-se ao servidor e busca upload-id. Esse numero e necessario para
	 * o upload do documento
	 *
	 * @param string $cpfOrCnpj
	 * CPF ou CNPJ do usuario que tera direito de assinar o documento
	 * 
	 * @param string $marker 
	 * O marcador sera retornado nas informacoes de assinatura 
	 * exatamente como foi fornecido. 
	 * Podera ser utilizado pela aplicacao para identificar de onde
	 * partiu a solicitacao do servico (processo ou modulo).
	 * O marcador pode ser fornecido como vazio, porem nunca deve
	 * ser fornecido como nulo.
	 * 
	 * @return string
	 * Retorna string Upload-ID
	 */
	private function retrieveUploadId( $cpfOrCnpj = null , $mark = null ) {
		$postData['cpfOrCnpj'] = "";
		$postData['flag'] 	= 	"";
		
		if( $cpfOrCnpj ) {
			$postData['cpfOrCnpj'] = "$cpfOrCnpj";		
		}
		if( $mark ) {
			$postData['flag'] = "$mark";		
		}
		
		
		$response = $this->ssdConnector->post(
			$this->fileUploadUrl, $postData);
			
		if ($response) {
			//do nothing
		} else {
			throw new Exception('Impossivel ler a resposta.');
		}
		$xml = @simplexml_load_string($response);
		
		if ($xml) {
			//do nothing
		} else {
			throw new Exception('Formato da resposta invalido.');
		}
		
		$uploadId = (string) $xml->{"upload-id"};
		
		if( $uploadId == "" )
		{
			throw new Exception( "Impossivel receber autorizacao para upload de arquivo" );
		}
		else 
		{
			return( $uploadId );
		}
	}
	
	/**
	 * Recupera o pacote de assinatura atraves numero de protocolo gerado 
	 * para o usuario final no momento da assinatura. 
	 *
	 * E utilizado quando o sistema quiser recuperar o pacote de assinatura 
	 * referente a um determinado protocolo de assinatura
	 * 
	 * @param String $signatureProtocol
	 * Numero de protocolo gerado para o usuario final no momento
	 * da assinatura
	 * 
	 * @param array $headerReturn
	 * Parametro passado por referencia contendo o header de retorno do post. Contem dados
	 * do arquivo recebido
	 * 
	 * @see downloadSignaturePackage
	 * 
	 * @return binary
	 * Pacote ZIP contendo documento assinado, a assinatura e o recibo da
	 * da assinatura de um determinado protocolo de assinatura
	 */
	 public function downloadSignaturePackageByProtocol( $signatureProtocol , & $headerReturn ) {
		if( $signatureProtocol == "" ) {
			throw new Exception( "Numero de protocolo nao pode ser vazio" );
		}
		
		$postData = array(
			//'documentToSign' => "$aboluteFilePath"
			'signatureProtocol' => "$signatureProtocol"
		);
		
		return( $this->downloadSignaturePackage( $postData , $headerReturn ) );
	 }
	 
	 /**
	 * Recupera o pacote de assinatura atraves ticket gerado para o sistema 
	 * cliente no momento da assinatura. 
	 * 
	 * E utilizado quando o sistema quiser recuperar o pacote de assinatura 
	 * referente a um determinado ticket de assinatura
	 *
	 * @param String $ticketId
	 * Identificador do documento assinado gerado para o sistema cliente no
	 * momento da assinatura
	 * 
	 * @param array $headerReturn
	 * Parametro passado por referencia contendo o header de retorno do post. Contem dados
	 * do arquivo recebido
	 * 
	 * @see downloadSignaturePackage
	 *
	 * @return binary
	 * Pacote ZIP contendo documento assinado, a assinatura e o recibo da
	 * da assinatura de um determinado protocolo de assinatura
	 */
	 public function downloadSignaturePackageByTicket( $ticketId , & $headerReturn ) {
		if( $ticketId == "" ) {
			throw new Exception( "O ticket nao pode ser vazio" );
		}
		
		$postData = array(
			//'documentToSign' => "$aboluteFilePath"
			'ticketId' => "$ticketId"
		);
							 
		return( $this->downloadSignaturePackage( $postData , $headerReturn ) ) ;
	 }
	 
	 /**
	  * Recebe dados a serem enviados para a Servlet e retorna
	  * pacote de assinatura.
	  * 
	  * @param array $postData
	  * Array com parametros a serem enviados para a servlet. 
	  * Pode conter tanto o protocolo quanto o ticket
	  * 
	  * @param array $headerReturn
	  * Parametro passado por referencia contendo o header de retorno do post. Contem dados
	  * do arquivo recebido
	  *
	  * @return binary
	  * Pacote ZIP contendo documento assinado, a assinatura e o recibo da
	  * da assinatura de um determinado protocolo de assinatura. Caso algum
	  * erro ocorra, um xml e retornado ao inves do binario 
	  *
	  * @see downloadSignaturePackageByProtocol
	  * @see downloadSignaturePackageByTicket
	  */
	 private function downloadSignaturePackage( $postData , & $headerReturn ) {

	 	$response = $this->ssdConnector->post(
			$this->downloadSignaturePackageUrl, $postData , null , FALSE , $headerReturn );
			
		if ($response) {
			//do nothing
		} else {
			throw new Exception('Impossivel ler a resposta.');
		}
		
		/*$xml = @simplexml_load_string( $response );
		if( $xml )
		{
			throw new Exception( (string) $xml[0] );
		}*/

		return( $response );
		
	 }
	 
	 /**
	  * Recupera o ticket da applet para assinatura de documentos
	  * gerados pelo usuario
	  * 
	  * @see loadDocumentForUserSigning
	  *
	  * @param $marker 
	  * O marcador sera retornado nas informacoes de assinatura 
	  * exatamente como foi fornecido. 
	  * Podera ser utilizado pela aplicacao para identificar de onde
	  * partiu a solicitacao do servico (processo ou modulo).
	  * O marcador pode ser fornecido como vazio, porem nunca deve
	  * ser fornecido como nulo.
	  * 
	  * @return XML
	  * O ticket para a abertura da applet de assinatura, o timestamp de 
	  * criacao e o timestamp de expiracao do ticket.	  
	  */
	 public function getAppletTicketInfoForUserDocSigning( $marker ) {
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker );
		
		
		$retorno = $this->wsConnector->call("getAppletTicketInfoForUserDocSigning",$params,$options);	 

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
	  * Recupera o ticket da applet para assinatura de documentos 
	  * gerados pelo usuario.
	  * 
	  * Este metodo e utilizado quando deseja-se assinar um documento 
	  * gerado pelo usuario. Entretanto, apenas o usuario detento do cpf
	  * ou cnpj recebido por parametro pode assinar tal documento
	  *
	  * @see loadDocumentForUserSigning
	  *
	  * @param string $cpfOrCnpj
	  * CPF ou CNPJ do usuario que tera direito de assinar o documento
	  *
	  * @param $marker 
	  * O marcador sera retornado nas informacoes de assinatura 
	  * exatamente como foi fornecido. 
	  * Podera ser utilizado pela aplicacao para identificar de onde
	  * partiu a solicitacao do servico (processo ou modulo).
	  * O marcador pode ser fornecido como vazio, porem nunca deve
	  * ser fornecido como nulo.
	  *
	  * @return XML
	  * O ticket para a abertura da applet de assinatura, o timestamp de 
	  * criacao e o timestamp de expiracao do ticket.	
	  */
	 public function getAppletTicketInfoForUserDocSigningWithRestriction( $cpfOrCnpj , $marker ) {
	 	if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( 
			"flag" => "$marker" ,
			"cpfOrCnpj" => "$cpfOrCnpj" 
		);
		$retorno =  $this->wsConnector->call(
			"getAppletTicketInfoForUserDocSigningWithRestriction",
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
	 * Retorna o ticket para a assinatura pelo usuario do documento 
	 * originado pelo sistema.
	 *
	 * Este metodo e utilizado quando deseja-se assinar um documento gerado
	 * pela aplicacao externa. Atualmente nao e utilizado pois ja esta 
	 * encapsulado no processo de upload do documento atraves de uma servlet. 
	 * Caso seja necessario restringir a assinatura a um determinado usuario
	 * utilize o metodo {@link getAppletTicketInfoForAppDocSigningWithRestriction()} 
	 *
	 * @see loadDocumentForUserSigning
	 * 
	 * @param $documentId
	 *	O identificador do documento carregado no SSD para ser assinado pelo
	 * 	usuario, obtido chamando-se o metodo "loadDocumentForUserSigning".
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de assinatura 
	 * 	exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return XML
	 * O ticket para a abertura da applet de assinatura, o timestamp de 
	 * criacao e o timestamp de expiracao do ticket.
	 * 
	 * @deprecated	
	 */
	public function getAppletTicketInfoForAppDocSigning($documentId, $marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( "flag" => $marker, "documentId" => $documentId);
		return $this->wsConnector->call(
			"getAppletTicketInfoForAppDocSigning",
			$params,
			$options);
	}
	
	/**
	 * Retorna o ticket para a assinatura pelo usuario do documento 
	 * originado pelo sistema.
	 *
	 * Este metodo e utilizado quando deseja-se assinar um documento gerado
	 * pela aplicacao externa. Entretanto, apenas o usuario detentor do CPF
	 * ou CNPJ recebido por parametro pode assinar tal documento. 
	 * Caso nao seja necessario restringir a assinatura a um determinado usuario
	 * utilize o metodo {@link getAppletTicketInfoForAppDocSigning()} 
	 * Atualmente nao e utilizado pois ja esta encapsulado no processo de 
	 * upload do documento atraves de uma servlet.
	 *
	 * @see loadDocumentForUserSigning
	 * @see getAppletTicketInfoForAppDocSigning
	 *
	 * @param $documentId
	 *	O identificador do documento carregado no SSD para ser assinado pelo
	 * 	usuario, obtido chamando-se o metodo "loadDocumentForUserSigning".
	 *
	 * @param string $cpfOrCnpj
	 * CPF ou CNPJ do usuario que tera direito de assinar o documento
	 *
	 * @param $marker 
	 * 	O marcador sera retornado nas informacoes de assinatura 
	 * 	exatamente como foi fornecido. 
	 *	Podera ser utilizado pela aplicacao para identificar de onde
	 *	partiu a solicitacao do servico (processo ou modulo).
	 * 	O marcador pode ser fornecido como vazio, porem nunca deve
	 * 	ser fornecido como nulo.
	 *
	 * @return XML
	 * O ticket para a abertura da applet de assinatura, o timestamp de 
	 * criacao e o timestamp de expiracao do ticket.	
	 *
	 * @deprecated
	 */
	public function getAppletTicketInfoForAppDocSigningWithRestriction($documentId, $cpfOrCnpj, $marker){
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array( 
			"flag" => "$marker", 
			"documentId" => "$documentId" , 
			"cpfOrCnpj" => "$cpfOrCnpj"
			);
		return $this->wsConnector->call(
			"getAppletTicketInfoForAppDocSigningWithRestriction",
			$params,
			$options);
	}
	
	/**
	 * Recupera informacoes de uma assinatura
	 *
	 * Este metodo e utilizado quando deseja-se recuperar informacoes de uma
	 * determinada assinatura a partir de um determinado protocolo de assinatura.
	 *
	 * @param String $signatureProtocol
	 * Numero de protocolo gerado para o usuario final no momento
	 * da assinatura	 
	 *
	 * @return DocumentSignatureInfo
	 * Identificador da assinatura, a data de recebmento desta, o valor do hash,
	 * informacoes sobre o documento assinado, o sistema e o usuario que assinou
	 * o documento	
	 *
	 */
	public function getDocumentSignatureInfoByProtocol( $signatureProtocol ) {
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		
		$params = array(
			"signatureProtocol" => "$signatureProtocol" 
			);
		$retorno = $this->wsConnector->call(
			"getDocumentSignatureInfoByProtocol",
			$params,
			$options);
			
		if( $retorno instanceof SOAP_Fault )
		{
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		$externalInfoDocument = ExternalSystemInfo::loadExternalSystemInfo( $retorno->documentInfo->sourceSystemInfo->id , $retorno->documentInfo->sourceSystemInfo->name );

		$objDocumentInfo = DocumentInfo::loadDocumentInfo(
			$externalInfoDocument ,
			$retorno->documentInfo->documentHash ,
			$retorno->documentInfo->mimeType ,
			$retorno->documentInfo->originalName ,
			$retorno->documentInfo->receivingDate,
			$retorno->documentInfo->size
		);
		$externalInfoSignature = ExternalSystemInfo::loadExternalSystemInfo( $retorno->requestSystemInfo->id , $retorno->requestSystemInfo->name );

		$userBasicInfo = UserBasicInfo::loadUserBasicInfo( 
			$retorno->userInfo->cpf ,
			$retorno->userInfo->cnpj ,
			$retorno->userInfo->id ,
			$retorno->userInfo->name
		);
		
		$documentSignature = DocumentSignatureInfo::loadDocumentSignatureInfo(
			$retorno->protocol ,
			$retorno->ticket ,
			$retorno->receivingDate ,
			$retorno->signatureHash ,
			$objDocumentInfo ,
			$externalInfoSignature ,
			$userBasicInfo,
			$retorno->signatureType ,
			$retorno->flag
		);
		return ( $documentSignature );
	}
	
	/**
	 * Recupera informacoes de uma assinatura
	 *
	 * Este metodo e utilizado quando deseja-se recuperar informacoes de uma
	 * determinada assinatura a partir de um determinado ticket de assinatura.
	 *
	 * @param String $ticketId
	 * Identificador do documento assinado gerado para o sistema cliente no
	 * momento da assinatura
	 *
	 * @return DocumentSignatureInfo
	 * Identificador da assinatura, a data de recebmento desta, o valor do hash,
	 * informacoes sobre o documento assinado, o sistema e o usuario que assinou
	 * o documento	
	 *
	 */
	public function getDocumentSignatureInfoByTicket( $signatureTicket ) {
		if (!$this->wsConnector) {
			$msg_conf = "Chame algum metodo de configuracao".
					" para os ambiente de homologacao".
					" ou producao.";
			throw new Exception($msg_conf);
		}
		$options = array( "namespace" => $this->wsdl->namespaces['tns']);
		$params = array(  
			"signatureTicket" => "$signatureTicket" 
			);
		$retorno = $this->wsConnector->call(
			"getDocumentSignatureInfoByTicket",
			$params,
			$options);	
			
		if( $retorno instanceof SOAP_Fault ) {
			throw new Exception( "Erro ao buscar informacoes do webservice: " . $retorno->message );
		}
		
		$externalInfoDocument = ExternalSystemInfo::loadExternalSystemInfo( $retorno->documentInfo->sourceSystemInfo->id , $retorno->documentInfo->sourceSystemInfo->name );

		$objDocumentInfo = DocumentInfo::loadDocumentInfo(
			$externalInfoDocument ,
			$retorno->documentInfo->documentHash ,
			$retorno->documentInfo->mimeType ,
			$retorno->documentInfo->originalName ,
			$retorno->documentInfo->receivingDate,
			$retorno->documentInfo->size
		);
		$externalInfoSignature = ExternalSystemInfo::loadExternalSystemInfo( $retorno->requestSystemInfo->id , $retorno->requestSystemInfo->name );

		$userBasicInfo = UserBasicInfo::loadUserBasicInfo( 
			$retorno->userInfo->cpf ,
			$retorno->userInfo->cnpj ,
			$retorno->userInfo->id ,
			$retorno->userInfo->name
		);
		
		$documentSignature = DocumentSignatureInfo::loadDocumentSignatureInfo(
			$retorno->protocol ,
			$retorno->ticket ,
			$retorno->receivingDate ,
			$retorno->signatureHash ,
			$objDocumentInfo ,
			$externalInfoSignature ,
			$userBasicInfo,
			$retorno->signatureType ,
			$retorno->flag
		);
		return ( $documentSignature );
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

	private function loadProductionUrls() {
                $this->codebase = "https://ssd.mec.gov.br";
		$this->fileUploadUrl = 
			"https://ssd.mec.gov.br/ssd-server/servlet/UploadTmpDoc";
		$this->downloadSignaturePackageUrl = 
			"https://ssd.mec.gov.br/ssd-server/servlet/DownloadSignaturePackage";
		$this->wsdlUrl = 
			"https://ssd.mec.gov.br/ssd-server/services/Signature?wsdl";
		$this->appletUrl = 
			"http://ssd.mec.gov.br/applet/ssd-applet.jar";
	}

	private function loadHomologationUrls() {
		$this->codebase = "https://ssdhmg.mec.gov.br";
		$this->fileUploadUrl = 
			"https://ssdhmg.mec.gov.br/ssd-server/servlet/UploadTmpDoc";
		$this->downloadSignaturePackageUrl = 
			"https://ssdhmg.mec.gov.br/ssd-server/servlet/DownloadSignaturePackage";
		$this->wsdlUrl = 
			"https://ssdhmg.mec.gov.br/ssd-server/services/Signature?wsdl";
		$this->appletUrl = 
			"http://ssdhmg.mec.gov.br/applet/ssd-applet.jar";
	}
}
?>
