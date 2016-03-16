<?php
	require_once("Uf.php");
	
	class City
	{
		/**
		 *
		 * @var string
		 */
		private $codigoIBGE ;
		
		/**
		 *
		 * @var string
		 */
		private $descricao ;
		
		/**
		 *
		 * @var Uf
		 */
		private $estado ;
		
		/**
		 *
		 * @var string
		 */
		private $siglaEstado ;
	
		public static function loadCity( $codigoIBGE , $descricao , Uf $estado , $siglaEstado )
		{
			$city = new City();
			$city->setCodigoIBGE( $codigoIBGE );
			$city->setDescricao( $descricao );
			$city->setEstado( $estado );
			$city->setSiglaEstado( $siglaEstado );
			return( $city );
		}
		
		
		/**
		 * @return string
		 */
		public function getCodigoIBGE () {
			return $this->codigoIBGE ;
		}
		
		/**
		 * @return string
		 */
		public function getDescricao () {
			return $this->descricao ;
		}
		
		/**
		 * @return Uf
		 */
		public function getEstado () {
			return $this->estado ;
		}
		
		/**
		 * @return string
		 */
		public function getSiglaEstado () {
			return $this->siglaEstado ;
		}
		
		/**
		 * @param string $codigoIBGE
		 */
		public function setCodigoIBGE ( $codigoIBGE ) {
			$this->codigoIBGE = $codigoIBGE ;
		}
		
		/**
		 * @param string $descricao
		 */
		public function setDescricao ( $descricao ) {
			$this->descricao = $descricao ;
		}
		
		/**
		 * @param Uf $estado
		 */
		public function setEstado ( $estado ) {
			$this->estado = $estado ;
		}
		
		/**
		 * @param string $siglaEstado
		 */
		public function setSiglaEstado ( $siglaEstado ) {
			$this->siglaEstado = $siglaEstado ;
		}

	}

?>