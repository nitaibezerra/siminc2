<?php
	require_once(SSD_PATH."TO/AuthTypeInfo.php");
	require_once(SSD_PATH."TO/InvalidUserPermission.php");
	require_once(SSD_PATH."TO/LoginIdentifier.php");
	
	class UserAuthInfo {
		private $objAuthType ; 
		private $flag ;
		private $userId ;
		private $objLoginIdentifier ;
		private $lastUpdateDate ;
		private $messages ; 
		private $field ;
		private $invalidPermission ;
		private $validPermission ;
		
		public static function loadUserAuthInfo( 
			AuthTypeInfo $objAuthInfo , 
			$flag , 
			$userId , 
			LoginIdentifier $objLoginIdentifier  , 
			$lastUpdateDate , 
			$arrMessages ,
			$field ,
			$arrInvalidPermission , 
			$arrValidPermission
		)
		{
			$userAuthInfo = new UserAuthInfo();
			$userAuthInfo->setObjAuthType($objAuthInfo);
			$userAuthInfo->setFlag($flag);
			$userAuthInfo->setUserId($userId);
			$userAuthInfo->setObjLoginIdentifier($objLoginIdentifier);
			$userAuthInfo->setLastUpdateDate($lastUpdateDate);
			$userAuthInfo->setMessages($arrMessages);
			$userAuthInfo->setField($field);
			$userAuthInfo->setInvalidPermission($arrInvalidPermission);
			$userAuthInfo->setValidPermission($arrValidPermission);
			return($userAuthInfo);
		}
		
		/**
		 *
		 * @return AuthTypeInfo
		 */
		public function getObjAuthType() {
			return($this->objAuthType);
		}
		
		/**
		 * 
		 * @return String
		 */
		public function getFlag() {
			return($this->flag);
		}
		
		/**
		 *
		 * @return integer
		 */
		public function getUserId() {
			return($this->userId);
		}
		
		/**
		 *
		 * @return LoginIdentifier
		 */
		public function getObjLoginIdentifier() {
			return ($this->objLoginIdentifier);
		}
		
		/**
		 *
		 * @return integer
		 */
		public function getLastUpdateDate() {
			return($this->lastUpdateDate);
		}
		
		/**
		 *
		 * @return array
		 */
		public function getMessages() {
			return($this->messages);
		}
		
		/**
		 *
		 * @return string
		 */
		public function getField() {
			return($this->field);
		}
		
		/**
		 *
		 * @return InvalidUserPermission[]
		 */
		public function getInvalidPermission() {
			return($this->invalidPermission);
		}
		
		/**
		 *
		 * @return int[]
		 */
		public function getValidPermission() {
			return($this->validPermission);
		}
		
		public function setObjAuthType($objAuthType) {
			$this->objAuthType = $objAuthType;
		}
		
		public function setFlag($flag) {
			$this->flag = $flag;
		}
		
		public function setUserId($userId) {
			$this->userId = $userId;
		}
		
		public function setObjLoginIdentifier($objLoginIdentifier) {
			$this->objLoginIdentifier = $objLoginIdentifier;
		}
		
		public function setLastUpdateDate($lastUpdateDate) {
			$this->lastUpdateDate = $lastUpdateDate;
		}
		
		public function setMessages($messages) {
			$this->messages = $messages;
		}
		
		public function setField($field) {
			$this->field = $field;
		}
		
		public function setInvalidPermission($invalidPermission) {
			$this->invalidPermission = $invalidPermission;
		}
		
		public function setValidPermission($validPermission) {
			$this->validPermission = $validPermission;
		}
	}
?>