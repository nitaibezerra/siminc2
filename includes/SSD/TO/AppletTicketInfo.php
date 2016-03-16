<?php
	/**
	 * Classe para transferencia de informacoes do Ticket para abertura
	 * do applet
	 * 
	 * Essa classe contem as informacoes basicas geradas sobre o ticket de abertura
	 * da applet gerado pelo webservice. 
	 * 
	 * @package TO
	 * 
	 */
	class AppletTicketInfo
	{
		
		private $creationTimestamp	;
		private $expirationTimestamp ;
		private $ticketId ;
		
		/**
		 * Controi e alimenta um objeto AppletTicketInfo e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param int $creationTimestamp
		 * @param int $expirationTimestamp
		 * @param string $ticketId
		 * @return AppletTicketInfo
		 */
		public static function loadAppletTicketInfo( $creationTimestamp , $expirationTimestamp , $ticketId )
		{
			$appletTicketInfo = new AppletTicketInfo();
			$appletTicketInfo->setCreationTimestamp( $creationTimestamp );
			$appletTicketInfo->setExpirationTimestamp( $expirationTimestamp );
			$appletTicketInfo->setTicketId( $ticketId );
			return ( $appletTicketInfo );
		}
		
		/**
		 * Retorna um timestamp com a data de criacao do ticket
		 *
		 * @return int
		 */
		public function getCreationTimestamp()
		{
			return ( $this->creationTimestamp );
		}
		
		/**
		 * Rertorna timestamp com a data de expiracao do ticket
		 *
		 * @return int
		 */
		public function getExpirationTimestamp()
		{
			return ( $this->expirationTimestamp );
		}
		
		/**
		 * Retorna o ticket para abertura da applet
		 *
		 * @return string
		 */
		public function getTicketId()
		{
			return ( $this->ticketId );
		}
		
		/**
		 * Recebe um timestamp contendo a data de criacao do ticket
		 *
		 * @param int $creationTimestamp
		 */
		public function setCreationTimestamp( $creationTimestamp )
		{
			$this->creationTimestamp = round( $creationTimestamp / 1000 , 0 );
		}
		
		/**
		 * Recebe um timestamp contendo a data de expiracao do ticket
		 *
		 * @param int $expirationTimestamp
		 */
		public function setExpirationTimestamp( $expirationTimestamp )
		{
			$this->expirationTimestamp = round( $expirationTimestamp / 1000 , 0 ) ;
		}
		
		/**
		 * Recebe o ticket para abertura da applet
		 *
		 * @param int $ticketId
		 */
		public function setTicketId( $ticketId )
		{
			$this->ticketId = $ticketId;
		}
	}

?>