<?php
	/**
	 * Contem informacoes do retorno do upload de documento
	 *
	 */
	class UploadedDoc
	{
		/**
		 * Ticket para abertura de applet para assinatura do
		 * documento
		 *
		 * @var string
		 */
		private $appletTicketId;
		
		/**
		 * Hash sha256-base16 do documento carregado
		 *
		 * @var string
		 */
		private $fileHash ;
		
		/**
		 * Controi e alimenta um objeto UploadedDoc e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param string $appletTicketId
		 * @param string $fileHash
		 * @return UploadedDoc
		 */
		public function loadUploadedDoc( $appletTicketId , $fileHash )
		{
			$uploadedDoc = new UploadedDoc();
			$uploadedDoc->setAppletTicketId( $appletTicketId );
			$uploadedDoc->setFileHash( $fileHash );
			return ( $uploadedDoc );
		}
		
		/**
		 * Retorna id do ticket para abertura de applet de assinatura
		 *
		 * @return string
		 */
		public function getAppletTicketId()
		{
			return ( $this->appletTicketId );
		}
		
		/**
		 * Retorna Hash sha256-base16 do documento carregado
		 *
		 * @return string
		 */
		public function getFileHash()
		{
			return ( $this->fileHash );
		}
		
		/**
		 * Recebe id do ticket para abertura de applet de assinatura
		 *
		 * @param string $appletTicketId
		 */
		public function setAppletTicketId( $appletTicketId )
		{
			$this->appletTicketId = $appletTicketId ;
		}
		
		/**
		 * Recebe Hash sha256-base16 do documento carregado
		 *
		 * @param string $fileHash
		 */
		public function setFileHash( $fileHash ) 
		{
			$this->fileHash = $fileHash;
		}
		
	}

?>