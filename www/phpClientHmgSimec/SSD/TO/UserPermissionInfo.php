<?php
	class UserPermissionInfo
	{
		
		/**
		 *
		 * @var string
		 */
		private $justificationOfStatusChange ;
		
		/**
		 *
		 * @var int
		 */
		private $permissionId ;		
		
		/**
		 *
		 * @var boolean
		 */
		private $requiredDataStatus ;
		
		/**
		 *
		 * @var int
		 */
		private $responsibleIdForStatusChange ;
		
		/**
		 *
		 * @var string
		 */
		private $userPermissionStatus ;
		
	
		public static function loadPermissionInfo( 
			$justificationOfStatusChange ,
			$permissionId ,
			$requiredDataStatus , 
			$responsibleIdForStatusChange ,
			$userPermissionStatus
		)
		{
			$userPermission = new UserPermissionInfo() ;
			$userPermission->setJustificationOfStatusChange( $justificationOfStatusChange );
			$userPermission->setPermissionId( $permissionId );
			$userPermission->setRequiredDataStatus( $requiredDataStatus );
			$userPermission->setResponsibleIdForStatusChange( $responsibleIdForStatusChange );
			$userPermission->setUserPermissionStatus( $userPermissionStatus );
			
			return( $userPermission );
		}
		
		/**
		 * @return string
		 */
		public function getJustificationOfStatusChange () {
			return $this->justificationOfStatusChange ;
		}
		
		/**
		 * @return int
		 */
		public function getPermissionId () {			
			echo $this->permissionId . "<br>";			
		}		
		
		/**
		 * @return boolean
		 */
		public function getRequiredDataStatus () {
			return $this->requiredDataStatus ;
		}
		
		/**
		 * @return int
		 */
		public function getResponsibleIdForStatusChange () {
			return $this->responsibleIdForStatusChange ;
		}
		
		/**
		 * @return string
		 */
		public function getUserPermissionStatus () {
			return $this->userPermissionStatus ;
		}
		
		/**
		 * @param string $justificationOfStatusChange
		 */
		public function setJustificationOfStatusChange ( $justificationOfStatusChange ) {
			$this->justificationOfStatusChange = $justificationOfStatusChange ;
		}
		
		/**
		 * @param int $permissionId
		 */
		public function setPermissionId ( $permissionId ) {
			$this->permissionId = $permissionId ;
		}		
		
		/**
		 * @param boolean $requiredDataStatus
		 */
		public function setRequiredDataStatus ( $requiredDataStatus ) {
			$this->requiredDataStatus = $requiredDataStatus ;
		}
		
		/**
		 * @param int $responsibleIdForStatusChange
		 */
		public function setResponsibleIdForStatusChange ( $responsibleIdForStatusChange ) {
			$this->responsibleIdForStatusChange = $responsibleIdForStatusChange ;
		}
		
		/**
		 * @param string $userPermissionStatus
		 */
		public function setUserPermissionStatus ( $userPermissionStatus ) {
			$this->userPermissionStatus = $userPermissionStatus ;
		}
		
	}

?>