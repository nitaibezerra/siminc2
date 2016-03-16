<?php
	class UserAndPermissionIds {
		/**
		 *
		 * @var int
		 */
		private $permissionId;
	
		/**
		 *
		 * @var int
		 */
		private $userId;
		
		public static function loadUserAndPermissionIds( $permissionId , $userId )
		{
			$userAndPermission = new UserAndPermissionIds();
			$userAndPermission->setPermissionId( $permissionId );
			$userAndPermission->setUserId( $userId );
			return( $userAndPermission );
		}
		
		/**
		 * @return int
		 */
		public function getPermissionId () {
			return $this->permissionId ;
		}
		
		/**
		 * @return int
		 */
		public function getUserId () {
			return $this->userId ;
		}
		
		/**
		 * @param int $permissionId
		 */
		public function setPermissionId ( $permissionId ) {
			$this->permissionId = $permissionId ;
		}
		
		/**
		 * @param int $userId
		 */
		public function setUserId ( $userId ) {
			$this->userId = $userId ;
		}
			
		}

?>