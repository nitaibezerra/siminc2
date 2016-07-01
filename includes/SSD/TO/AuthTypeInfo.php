<?php
	/**
	 * Classe para transferencia de informacoes do tipo de autenticacao
	 * do sistema
	 * 
	 * Essa classe contem as informacoes basicas geradas sobre como e feita
	 * a autenticacao do sistema cliente. O sistema pode ser autenticar utilizando
	 * diferentes tipos de campos, como email, cpf, etc.
	 * 
	 * @package TO
	 * 
	 */
	class AuthTypeInfo
	{
		private $id ;
		private $description ;
		
		/**
		 * Controi e alimenta um objeto AuthTypeInfo e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param int $id
		 * @param string $description
		 * @return AuthTypeInfo
		 */
		public static function loadAuthTypeInfo( $id , $description )
		{
			$authTypeInfo = new AuthTypeInfo() ;
			$authTypeInfo->setId( $id );
			$authTypeInfo->setDescription( $description );
			return( $authTypeInfo );
		}
		
		
		/**
		 * Retorna Id do tipo de autenticacao
		 *
		 * @return int
		 */
		public function getId()
		{
			return ( $this->id );
		}
		
		/**
		 * Retorna descricao do tipo de autenticacao
		 * 
		 * @return String
		 */
		public function getDescription()
		{
			return ( $this->description );
		}
		
		/**
		 * Recebe Id do tipo de autenticacao
		 *
		 * @param int $id
		 */
		public function setId( $id )
		{
			$this->id = $id ;
		}
		
		/**
		 * Recebe descricao do tipo de autenticacao
		 *
		 * @param string $description
		 */
		public function setDescription( $description )
		{
			$this->description = $description ;
		}
		
		
	}

?>