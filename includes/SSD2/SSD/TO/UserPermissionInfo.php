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
		private $permissionId;
		
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
		
		/**
		 *
		 * @var string
		 */
		private $sgProfile;
		
		private $description;
	
		public static function loadPermissionInfo( 
			$permissionId ,
			$justificationOfStatusChange ,
			$requiredDataStatus , 
			$responsibleIdForStatusChange ,
			$userPermissionStatus ,
			$sgProfile ,
			$description = null
		)
		{
 			$userPermission = new UserPermissionInfo() ;
			$userPermission->setPermissionId( $permissionId );
			$userPermission->setJustificationOfStatusChange( $justificationOfStatusChange );
			$userPermission->setRequiredDataStatus( $requiredDataStatus );
			$userPermission->setResponsibleIdForStatusChange( $responsibleIdForStatusChange );
			$userPermission->setUserPermissionStatus( $userPermissionStatus );
			$userPermission->setSgProfile( $sgProfile );
			$userPermission->setDescription( $description );
			return( $userPermission );
		}
		
		/**
		 * @return string
		 */
		public function getJustificationOfStatusChange () {
			return $this->justificationOfStatusChange ;
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
		
		public function setPermissionId( $permissionId )
		{
			$this->permissionId = $permissionId ;
		}
		
		public function getPermissionId()
		{
			return ( $this->permissionId );
		}
		
		public function setSgProfile( $sgProfile )
		{
			$this->sgProfile = trim( $sgProfile ) ;
		}
		
		public function getSgProfile()
		{
			return ( $this->sgProfile );
		}
		
		public function setDescription( $value )
		{
			$this->description = $value;
		}
		
		public function getDescription()
		{
			return( $this->description );
		}
		
	}

?>