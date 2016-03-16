<?php
	class UserBasicInfo
	{
		private $id;
		private $name;
		private $cpf;
		private $cnpj;
		
		public static function loadUserBasicInfo( $cpf , $cnpj , $id , $name )
		{
			$userBasicInfo = new UserBasicInfo() ;
			$userBasicInfo->setCpf( $cpf );
			$userBasicInfo->setCnpj( $cnpj );
			$userBasicInfo->setId( $id );
			$userBasicInfo->setName( $name );
			return ( $userBasicInfo );
		}
		
		public function getId()
		{
			return ( $this->id );
		}
		
		public function getCnpj()
		{
			return ( $this->cnpj );
		}
		
		public function getCpf()
		{
			return ( $this->cpf );
		}
		
		public function getName()
		{
			return ( $this->name ); 
		}
		
		public function setId( $id )
		{
			$this->id = $id ;
		}
		
		public function setCnpj( $cnpj )
		{
			$this->cnpj = $cnpj ;
		}
		
		public function setCpf( $cpf )
		{
			$this->cpf = $cpf ;
		}
		
		public function setName( $name )
		{
			$this->name = $name ;
		}
	}
?>