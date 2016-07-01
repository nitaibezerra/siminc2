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
		private $userId;	
	
		/**
		 *
		 * @var int
		 */
		private $userInfo;
		
		public static function loadUserInfoAndPermissionId( $permissionId, $userId , $userInfo )
		{
			$userInfoAndPermissionId = new UserInfoAndPermissionId();
			$userInfoAndPermissionId->setUserPermissionId( $permissionId );
			$userInfoAndPermissionId->setUserId( $userId );
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
		public function getUserId () {
			return $this->userId ;
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
		public function setUserId ( $userId ) {
			$this->userId = $userId ;
		}
		
		/**
		 * @param int $userInfo
		 */
		public function setUserInfo ( $userInfo ) {
			$this->userInfo = $userInfo ;
		}
			
	}
?>