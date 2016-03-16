<?php
	/**
	 * Classe para transferencia de informacoes do identificado do login
	 * 
	 * @package TO
	 * 
	 */
	class LoginIdentifier
	{
		private $field ;
		private $value ;
		
		/**
		 * Controi e alimenta um objeto LoginIdentifier e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param string $field
		 * @param string $value
		 * @return LoginIdentifier
		 */
		public static function loadLoginIdentifier( $field , $value )
		{
			$loginIdentifier = new LoginIdentifier();
			$loginIdentifier->setField( $field );
			$loginIdentifier->setValue( $value );
			return $loginIdentifier;
		}
		
		/**
		 * Retorna o nome do identificado do login
		 *
		 * @return string
		 */
		public function getField()
		{
			return( $this->field );
		}
		
		/**
		 * Retorna o valor do identificador do login
		 *
		 * @return string
		 */
		public function getValue()
		{
			return ( $this->value );
		}
		
		/**
		 * Recebe o nome do identificador do login
		 *
		 * @param string $field
		 */
		public function setField( $field )
		{
			$this->field = $field ; 
		}
		
		/**
		 * Recebe o valor do identificador do login
		 *
		 * @param string $value
		 */
		public function setValue( $value )
		{
			$this->value =  $value ;
		}
	}
?>