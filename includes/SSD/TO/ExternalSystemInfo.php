<?php
	/**
	 * Classe para transferencia de informacoes do Sistema Externo
	 * 
	 * Sistema externo e o sistema que envia o documento e a assinatura
	 * para o webservice
	 * 
	 * @package TO
	 * 
	 */
	class ExternalSystemInfo
	{
		private $id ;
		private $name ;
		
		/**
		 * Controi e alimenta um objeto ExternalSystemInfo e retorna
		 * para o metodo solicitante o objeto solicitado
		 *
		 * @param int $id
		 * @param string $name
		 * @return ExternalSystemInfo
		 */
		public static function loadExternalSystemInfo( $id , $name )
		{
			$objExternalSystemInfo = new ExternalSystemInfo() ;
			$objExternalSystemInfo->setId( $id );
			$objExternalSystemInfo->setName(  $name );
			return ( $objExternalSystemInfo );
		}
		
		/**
		 * Retorna o id do sistema externo
		 *
		 * @return int
		 */
		public function getId()
		{
			return ( $this->id );
		}
		
		/**
		 * Retorna o nome do sistema externo
		 *
		 * @return string
		 */
		public function getName()
		{
			return ( $this->name );
		}
		
		/**
		 * Recebe o id do sistema externo
		 *
		 * @param int $id
		 */
		public function setId( $id )
		{
			$this->id = $id;
		}
		
		/**
		 * Recebe o nome do sistema externo
		 *
		 * @param string $name
		 */
		public function setName( $name )
		{
			$this->name = $name ;
		}
	}
?>