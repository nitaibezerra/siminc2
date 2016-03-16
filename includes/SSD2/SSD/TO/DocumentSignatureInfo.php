<?php
	require_once("DocumentInfo.php");
	require_once("ExternalSystemInfo.php");
	require_once("UserBasicInfo.php");

	
	/**
	 * Classe para transferencia de informacoes da assinatura do documento
	 * 
	 * @package TO
	 * 
	 */
	class DocumentSignatureInfo
	{
		/**
		 * ID (Protocolo) da assinatura
		 *
		 * @var unknown_type
		 */
		private $protocol;
		
		/**
		 * ID do tíquete
		 *
		 * @var string
		 */
		private $ticket;
		
		/**
		 * Data do rebimento da assinatura
		 *
		 * @var int
		 */
		private $receivingDate;
		
		/**
		 * HASH da assinatura
		 *
		 * @var string
		 */
		private $signatureHash;
		
		/**
		 * Informacoes do documento
		 *
		 * @var DocumentInfo
		 */
		private $objDocumentInfo ;
		
		/**
		 * Informacoes do sistema externo que requisitou o serviço de assinatura
		 *
		 * @var ExternalSystemInfo
		 */
		private $objRequestSystemInfo;
		
		/**
		 * Informacoes basicas do usuario que assinou
		 * o documento
		 *
		 * @var UserBasicInfo
		 */
		private $objUserBasicInfo;
		
		/**
		 * Tipo da assinatura
		 *
		 * @var string
		 */
		private $signatureType ;
		
		/**
		 * Flag (variável utilizada pela aplicação externa)
		 *
		 * @var string
		 */
		private $flag ;
		
		/**
		 * Controi e alimenta um objeto DocumentSignatureInfo e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param int $protocol
		 * @param int $receivingDate
		 * @param string $signatureHash
		 * @param DocumentInfo $objDocumentInfo
		 * @param ExternalSystemInfo $objRequestSystemInfo
		 * @param UserBasicInfo $objUserBasicInfo
		 * @param string $signatureType
		 * @param string $flag
		 */
		public static function loadDocumentSignatureInfo(
			$protocol,
			$ticket,
			$receivingDate,
			$signatureHash,
			DocumentInfo $objDocumentInfo,
			ExternalSystemInfo $objRequestSystemInfo,
			UserBasicInfo $objUserBasicInfo,
			$signatureType,
			$flag
		)
		{
			$documentSignature = new DocumentSignatureInfo();
			$documentSignature->setObjDocumentInfo( $objDocumentInfo );
			$documentSignature->setObjExternalSystemInfo( $objRequestSystemInfo );
			$documentSignature->setHash( $signatureHash );
			$documentSignature->setProtocol( $protocol );
			$documentSignature->setTicketId( $ticket );
			$documentSignature->setReceivingDate( $receivingDate );
			$documentSignature->setObjUserInfo( $objUserBasicInfo );
			$documentSignature->setSignatureType( $signatureType );
			$documentSignature->setFlag( $flag );
			return ( $documentSignature );
		}
		
		/**
		 * Retorna um Objeto DocumentoInfo contendo informacoes do
		 * documento assinado
		 * 
		 * @return DocumentInfo
		 */
		public function getObjDocumentInfo()
		{
			return ( $this->objDocumentInfo );
		}
		
		/**
		 * Retorna um objeto ExternalSystemInfo contendo informacoes
		 * do sistema que enviou o documento
		 * 
		 * @return ExternalSystemInfo
		 */
		public function getObjExternalSystemInfo()
		{
			return ( $this->objRequestSystemInfo );
		}
		
		/**
		 * Retorna o HASH da assinatura
		 *
		 * @return string
		 */
		public function getHash()
		{
			return ( $this->signatureHash );
		}
		
		/**
		 * Retorna o ID (Protocolo) da assinatura
		 * 
		 * @return int
		 */
		public function getProtocol()
		{
			return ( $this->protocol );
		}
		
		/**
		 * Retorna o ID do tíquete
		 * 
		 * @return int
		 */
		public function getTicketId()
		{
			return ( $this->ticket );
		}
		
		/**
		 * Retorna o timestamp da data de recebimento da assinatura
		 * 
		 * @return int
		 */
		public function getReceivingDate()
		{
			return ( $this->receivingDate );
		}
		
		/**
		 * Retorna um objeto com as informacoes basicas do usuario
		 * 
		 * @return UserBasicInfo
		 */
		public function getObjUserBasicInfo()
		{
			return ( $this->objUserBasicInfo );
		}
		
		/**
		 * Retorna o tipo de assinatura
		 * 
		 * @return string
		 */
		public function getSignatureType()
		{
			return ( $this->signatureType );
		}
		
		/**
		 * Retorna a flag
		 * 
		 * @return string
		 */
		public function getFlag()
		{
			return ( $this->flag );
		}
		
		/**
		 * Recebe um objeto do tipo DocumentInfo contendo informacoes
		 * sobre o documento assinado
		 *
		 * @param DocumentInfo $objDocumentInfo
		 */
		public function setObjDocumentInfo( DocumentInfo $objDocumentInfo )
		{
			$this->objDocumentInfo = $objDocumentInfo ;
		}
		
		/**
		 * Recebe um objeto contendo as informacoes do Sistema Externo
		 *
		 * @param ExternalSystemInfo $objExternalSystemInfo
		 */
		public function setObjExternalSystemInfo( $objRequestSystemInfo )
		{
			$this->objRequestSystemInfo = $objRequestSystemInfo ;
		}
		
		/**
		 * Recebe o HASH da assinatura
		 *
		 * @param string $hash
		 */
		public function setHash( $hash )
		{
			$this->signatureHash = $hash ;
		}
		
		/**
		 * Recebe o ID (Protocolo) da assinatura
		 *
		 * @param int $protocol
		 */
		public function setProtocol( $protocol )
		{
			$this->protocol = $protocol ;
		}
		
		/**
		 * Recebe o ID do tíquete
		 *
		 * @param int $ticket
		 */
		public function setTicketId( $ticketId )
		{
			$this->ticket = $ticketId ;
		}
		
		/**
		 * Recebe o timestamp da data em que a assinatura foi enviada
		 *
		 * @param int $receivingDate
		 */
		public function setReceivingDate( $receivingDate )
		{
			$this->receivingDate = $receivingDate ;
		}
		
		/**
		 * Recebe objeto UserBasicInfo contendo informacoes basicas
		 * do usuario que assinou o documento
		 *
		 * @param UserBasicInfo $objUserInfo
		 */
		public function setObjUserInfo( $objUserInfo )
		{
			$this->objUserBasicInfo = $objUserInfo ;
		}
		
		/**
		 * Recebe o tipo de assinatura do documento
		 *
		 * @param string $signatureType
		 */
		public function setSignatureType( $signatureType )
		{
			$this->signatureType = $signatureType;
		}
		
		/**
		 * Recebe a flag de assinatura
		 *
		 * @param string $flag
		 */
		public function setFlag( $flag )
		{
			$this->flag = $flag;
		}	
	}
?>