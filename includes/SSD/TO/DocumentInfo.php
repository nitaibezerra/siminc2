<?php
	/**
	 * Classe para transferencia de informacoes do Documento assinado
	 * 
	 * @package TO
	 * 
	 */
	class DocumentInfo
	{
		/**
		 * Informacoes do sistema que enviou o documento (sistema de origem)
		 *
		 * @var ExternalSystemInfo
		 */
		private $sourceSystemInfo ;
		
		/**
		 * Tamanho do documento
		 *
		 * @var int
		 */
		private $size ;
		
		/**
		 * Nome original do documento
		 *
		 * @var string
		 */
		private $originalName ;
		
		/**
		 * Tipo do documento
		 *
		 * @var string
		 */
		private $mimeType ;
		
		/**
		 * Data de recebimento do documento
		 *
		 * @var int
		 */
		private $receivingDate ;
		
		/**
		 * HASH do documento
		 *
		 * @var string
		 */
		private $documetHash ;
		
		/**
		 * Controi e alimenta um objeto DocumentInfo e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param ExternalSystemInfo $sourceSystemInfo
		 * @param string $documetHash
		 * @param string $mimeType
		 * @param string $originalName
		 * @param int $receivingDate
		 * @param int $size
		 * @return DocumentInfo
		 */
		public function loadDocumentInfo( 
			ExternalSystemInfo $sourceSystemInfo , 
			$documetHash , 
			$mimeType , 
			$originalName , 
			$receivingDate ,
			$size
		)
		{
			$documentInfo = new DocumentInfo() ;
			$documentInfo->setObjExternalSystemInfo( $sourceSystemInfo );
			$documentInfo->setHash( $documetHash );
			$documentInfo->setMimeType( $mimeType );
			$documentInfo->setOriginalName( $originalName );
			$documentInfo->setReceivingDate( $receivingDate );
			$documentInfo->setSize( $size );
			return ( $documentInfo );
		}
		
		/**
		 * Retorna Informacoes do sistema que enviou o documento
		 * 
		 * @return ExternalSystemInfo
		 */
		public function getObjExternalSystemInfo()
		{
			return ( $this->objExternalSystemInfo );
		}
		
		/**
		 * Retorna o HASH do documento
		 * 
		 * @return string
		 */
		public function getHash()
		{
			return ( $this->documetHash );
		}
		
		/**
		 * Retorna o tipo do documento
		 * 
		 * @return string
		 */
		public function getMimeType()
		{
			return ( $this->mimeType );
		}
		
		/**
		 * Retorna o nome original do documento
		 * 
		 * @return string
		 */
		public function getOriginalName()
		{
			return ( $this->originalName );
		}
		
		/**
		 * Retorna a data de recebimento do documento pelo servidor
		 * 
		 * @return int
		 */
		public function getReceivingDate()
		{
			return ( $this->receivingDate );
		}
		
		/**
		 * Retorna o tamanho do arquivo enviado
		 * 
		 * @return int
		 */
		public function getSize()
		{
			return ( $this->size );
		}
		
		/**
		 * Recebe as informacoes do Sistema Externo
		 *
		 * @param ExternalSystemInfo $objExternalSystemInfo
		 */
		public function setObjExternalSystemInfo( $objExternalSystemInfo )
		{
			$this->objExternalSystemInfo = $objExternalSystemInfo ;
		}
		
		/**
		 * Recebe o HASH do documento
		 *
		 * @param string $hash
		 */
		public function setHash( $documetHash )
		{
			$this->documetHash = $documetHash ;
		}
		
		/**
		 * Recebe o tipo do arquivo
		 *
		 * @param string $mimeType
		 */
		public function setMimeType( $mimeType )
		{
			$this->mimeType = $mimeType ;
		}
		
		/**
		 * Recebe o nome original do documento
		 * 
		 * @param string $originalName
		 */
		public function setOriginalName( $originalName )
		{
			$this->originalName = $originalName;
		}
		
		/**
		 * Recebe um timestamp contendo a data em que o 
		 * documento foi recebido pelo servidor
		 *
		 * @param unknown_type $receivingDate
		 */
		public function setReceivingDate( $receivingDate )
		{
			$this->receivingDate = $receivingDate ;
		}
		
		/**
		 * Recebe o tamanho do arquivo em bytes
		 *
		 * @param int $size
		 */
		public function setSize( $size )
		{
			$this->size = $size ;
		}
		
	}
?>