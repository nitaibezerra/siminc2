<?php
	/**
	 * Classe para transferencia de informacoes de permissoes invalidas do usuario
	 * 
	 * @package TO
	 * 
	 */
	class InvalidUserPermission
	{
		private $arrExceptionMessages ;
		private $permissionId ;

		/**
		 * Controi e alimenta um objeto InvalidUserPermission e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param array $arrException
		 * @param int $permissionId
		 */
		public static function loadInvalidUserPermission( $arrException , $permissionId )
		{
			$invalidUserPermission = new InvalidUserPermission() ;
			$invalidUserPermission->setArrExceptionMessages( $arrException );
			$invalidUserPermission->setPermissionId( $permissionId );
			return ( $invalidUserPermission );
		}
		
		/**
		 * Retorna array contendo as mensagens de excecao do usuario
		 *
		 * @return array
		 */
		public function getArrExceptionMessages()
		{
			return( $this->arrExceptionMessages ) ; 
		}
		
		/**
		 * Retorna o ID da permissao invalida
		 *
		 * @return int
		 */
		public function getPermissionId()
		{
			return( $this->permissionId );
		}
		
		/**
		 * Recebe um array de mensagens de excecao
		 *
		 * @param array $arrExceptionMessages
		 */
		public function setArrExceptionMessages( $arrExceptionMessages )
		{
			$this->arrExceptionMessages = $arrExceptionMessages ; 
		}
		
		/**
		 * Recebe o id da permissao
		 *
		 * @param int $permissionId
		 */
		public function setPermissionId( $permissionId )
		{
			$this->permissionId = $permissionId ;
		}
		
	}
?>