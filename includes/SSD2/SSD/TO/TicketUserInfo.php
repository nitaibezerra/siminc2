<?php
	class TicketUserInfo
	{
		/**
		 *
		 * @var string
		 */
		private $flag ;
		
		/**
		 *
		 * @var int
		 */
		private $userId ;
	
		public static function loadTicketUserInfo( $flag , $userId )
		{
			$ticket = new TicketUserInfo();
			$ticket->setFlag( $flag );
			$ticket->setUserId( $userId );
			return ( $ticket );
		}
		
		/**
		 * @return string
		 */
		public function getFlag () {
			return $this->flag ;
		}
		
		/**
		 * @return int
		 */
		public function getUserId () {
			return $this->userId ;
		}
		
		/**
		 * @param string $flag
		 */
		public function setFlag ( $flag ) {
			$this->flag = $flag ;
		}
		
		/**
		 * @param int $userId
		 */
		public function setUserId ( $userId ) {
			$this->userId = $userId ;
		}

	}
?>