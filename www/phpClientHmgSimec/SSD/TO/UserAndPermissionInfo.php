<?php
	class UserAndPermissionInfo {

		private $userInfo;
		private $userPermissionInfo;
	
		public static function loadUserAndPermissionInfo( $userInfo , $userPermissionInfo )
		{
			$userAndPermissionInfo = new UserAndPermissionInfo();
			$userAndPermissionInfo->setUserInfo( $userInfo );
			$userAndPermissionInfo->setUserPermissionInfo( $userPermissionInfo );
			return( $userAndPermissionInfo );
		}	
	
		public function getUserInfo() {
			return $this->userInfo;
		}
		
		public function setUserInfo($userInfo) {
			$this->userInfo = $userInfo;
		}
		
		public function getUserPermissionInfo() {
			return $this->userPermissionInfo;
		}
		
		public function setUserPermissionInfo($userPermissionInfo) {
			$this->userPermissionInfo = $userPermissionInfo;
		}
}
?>