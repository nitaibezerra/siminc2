<?php
	class UserInfoAndPermissionId {
		/**
		 *
		 * @var int
		 */
		private $permissionId;
	
		/**
		 *
		 * @var int
		 */
		private $userInfo;
		
		public static function loadUserInfoAndPermissionId( $permissionId , $userInfo )
		{
			$userInfoAndPermissionId = new UserInfoAndPermissionId();
			$userInfoAndPermissionId->setUserPermissionId( $permissionId );
			$userInfoAndPermissionId->setUserInfo( $userInfo );
			return( $userInfoAndPermissionId );
		}
		
		/**
		 * @return int
		 */
		public function getUserPermissionId () {
			return $this->permissionId ;
		}
		
		/**
		 * @return int
		 */
		public function getUserInfo () {
			return $this->userInfo ;
		}
		
		/**
		 * @param int $permissionId
		 */
		public function setUserPermissionId ( $permissionId ) {
			$this->permissionId = $permissionId ;
		}
		
		/**
		 * @param int $userInfo
		 */
		public function setUserInfo ( $userInfo ) {
			$this->userInfo = $userInfo ;
		}
			
	}
?>