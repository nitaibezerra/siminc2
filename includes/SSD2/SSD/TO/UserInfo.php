<?php

	require_once( "City.php" );
	require_once( "Uf.php" );

	class UserInfo
	{
		/**
		 *
		 * @var string
		 */
		private $address ; 
		
		/**
		 *
		 * @var string
		 */
		private $alternativeEmail ;
		
		/**
		 *
		 * @var int
		 */
		private $birthDate ;
		
		/**
		 *
		 * @var string
		 */
		private $cellPhoneNumber ;
		
		/**
		 *
		 * @var City
		 */
		private $cityAddress ;
		
		/**
		 *
		 * @var string
		 */
		private $cnpj ;
		
		/**
		 *
		 * @var string
		 */
		private $cpf ;
		
		/**
		 *
		 * @var string
		 */
		private $dispatcherAgency;
		
		/**
		 *
		 * @var string
		 */
		private $email;
		
		/**
		 *
		 * @var string
		 */
		private $login ;
		
		/**
		 *
		 * @var string
		 */
		private $lotacao ;
		
		/**
		 *
		 * @var string
		 */
		private $name ;
		
		/**
		 *
		 * @var string
		 */
		private $nationality;
		
		/**
		 *
		 * @var City
		 */
		private $naturality;
		
		/**
		 *
		 * @var string
		 */
		private $nis;
		
		/**
		 *
		 * @var int
		 */
		private $postalCode;
		
		/**
		 *
		 * @var string
		 */
		private $responsibleCpf;
		
		/**
		 *
		 * @var string
		 */
		private $responsibleName;
		
		/**
		 *
		 * @var string
		 */
		private $rg ;
		
		/**
		 *
		 * @var string
		 */
		private $socialReason;
		
		/**
		 *
		 * @var Uf
		 */
		private $stateAddress;
		
		/**
		 *
		 * @var String
		 */
		private $telephoneNumber;
		
		/**
		 *
		 * @var string
		 */
		private $workInstitution;
		
		/**
		 * 
		 * @var UserInfoAndPermissionId
		 */
		private $userInfoAndPermissionId;
		
		/**
		 *
		 * @var array
		 */
		private $arrPermission = array();
	
		/**
		 * Enter description here...
		 *
		 * @param unknown_type $address
		 * @param unknown_type $alterantiveEmail
		 * @param unknown_type $birthDate
		 * @param unknown_type $cellPhoneNumber
		 * @param City $cityAddress
		 * @param unknown_type $cnpj
		 * @param unknown_type $cpf
		 * @param unknown_type $dispatcherAgency
		 * @param unknown_type $email
		 * @param unknown_type $login
		 * @param unknown_type $lotacao
		 * @param unknown_type $name
		 * @param unknown_type $nationality
		 * @param City $naturality
		 * @param unknown_type $nis
		 * @param unknown_type $postalCode
		 * @param unknown_type $responsibleCpf
		 * @param unknown_type $responsibleName
		 * @param unknown_type $rg
		 * @param unknown_type $socialReason
		 * @param Uf $stateAddress
		 * @param unknown_type $telephoneNumber
		 * @param unknown_type $workInsituition
		 * @return UsuarioTO
		 */
		public static function loadUserInfo(
			$address ,
			$alterantiveEmail ,
			$birthDate ,
			$cellPhoneNumber ,
			City $cityAddress ,
			$cnpj , 
			$cpf ,
			$dispatcherAgency ,
			$email , 
			$login ,
			$lotacao ,
			$name , 
			$nationality ,
			City $naturality ,
			$nis ,
			$postalCode ,
			$responsibleCpf , 
			$responsibleName ,
			$rg ,
			$socialReason ,
			Uf $stateAddress ,
			$telephoneNumber ,
			$workInsituition , 
			$userId
		)
		{
			$userInfo = new UsuarioTO() ;
			$userInfo->setAddress( $address );
			$userInfo->setAlternativeEmail( $alterantiveEmail );
			$userInfo->setBirthDate( $birthDate );
			$userInfo->setCellPhoneNumber( $cellPhoneNumber );
			$userInfo->setCityAddress( $cityAddress );
			$userInfo->setCnpj( $cnpj );
			$userInfo->setCpf( $cpf );
			$userInfo->setDispatcherAgency( $dispatcherAgency );
			$userInfo->setEmail( $email );
			$userInfo->setLogin( $login );
			$userInfo->setLotacao( $lotacao );
			$userInfo->setName( $name );
			$userInfo->setNationality( $nationality );
			$userInfo->setNaturality( $naturality );
			$userInfo->setNis( $nis );
			$userInfo->setPostalCode( $postalCode );
			$userInfo->setResponsibleCpf( $responsibleCpf );
			$userInfo->setResponsibleName( $responsibleName );
			$userInfo->setRg( $rg );
			$userInfo->setSocialReason( $socialReason );
			$userInfo->setStateAddress( $stateAddress );
			$userInfo->setTelephoneNumber( $telephoneNumber );
			$userInfo->setWorkInstitution( $workInsituition );
			$userInfo->setCoUsuarioSSD( $userId );
			return ( $userInfo );
		}
		
		
		
		/**
		 * @return string
		 */
		public function getAddress () {
			return $this->address ;
		}
		
		/**
		 * @return string
		 */
		public function getAlternativeEmail () {
			return $this->alternativeEmail ;
		}
		
		/**
		 * @return int
		 */
		public function getBirthDate () {
			return $this->birthDate ;
		}
		
		/**
		 * @return string
		 */
		public function getCellPhoneNumber () {
			return $this->cellPhoneNumber ;
		}
		
		/**
		 * @return City
		 */
		public function getCityAddress () {
			return $this->cityAddress ;
		}
		
		/**
		 * @return string
		 */
		public function getCnpj () {
			return $this->cnpj ;
		}
		
		/**
		 * @return string
		 */
		public function getCpf ( $isFormated = false ) {
			if( $isFormated )
			{
				return substr( $this->cpf , 0, 3 ) . '.' . substr( $this->cpf , 3, 3 ) . '.' . substr( $this->cpf , 6, 3 ) . '-' . substr( $this->cpf , 9, 2 );
			}
			else
			{
				return( $this->cpf );
			}
		}
		
		/**
		 * @return string
		 */
		public function getDispatcherAgency () {
			return $this->dispatcherAgency ;
		}
		
		/**
		 * @return string
		 */
		public function getEmail () {
			return $this->email ;
		}
		
		/**
		 * @return string
		 */
		public function getLogin () {
			return $this->login ;
		}
		
		/**
		 * @return string
		 */
		public function getLotacao () {
			return $this->lotacao ;
		}
		
		/**
		 * @return string
		 */
		public function getName () {
			return $this->name ;
		}
		
		/**
		 * @return string
		 */
		public function getNationality () {
			return $this->nationality ;
		}
		
		/**
		 * @return City
		 */
		public function getNaturality () {
			return $this->naturality ;
		}
		
		/**
		 * @return string
		 */
		public function getNis () {
			return $this->nis ;
		}
		
		/**
		 * @return int
		 */
		public function getPostalCode () {
			return $this->postalCode ;
		}
		
		/**
		 * @return string
		 */
		public function getResponsibleCpf () {
			return $this->responsibleCpf ;
		}
		
		/**
		 * @return string
		 */
		public function getResponsibleName () {
			return $this->responsibleName ;
		}
		
		/**
		 * @return string
		 */
		public function getRg () {
			return $this->rg ;
		}
		
		/**
		 * @return string
		 */
		public function getSocialReason () {
			return $this->socialReason ;
		}
		
		/**
		 * @return Uf
		 */
		public function getStateAddress () {
			return $this->stateAddress ;
		}
		
		/**
		 * @return String
		 */
		public function getTelephoneNumber () {
			return $this->telephoneNumber ;
		}
		
		/**
		 * @return string
		 */
		public function getWorkInstitution () {
			return $this->workInstitution ;
		}
		
		/**
		 * @param string $address
		 */
		public function setAddress ( $address ) {
			$this->address = $address ;
		}
		
		/**
		 * @param string $alternativeEmail
		 */
		public function setAlternativeEmail ( $alternativeEmail ) {
			$this->alternativeEmail = $alternativeEmail ;
		}
		
		/**
		 * @param int $birthDate
		 */
		public function setBirthDate ( $birthDate ) {
			$this->birthDate = $birthDate ;
		}
		
		/**
		 * @param string $cellPhoneNumber
		 */
		public function setCellPhoneNumber ( $cellPhoneNumber ) {
			$this->cellPhoneNumber = $cellPhoneNumber ;
		}
		
		/**
		 * @param City $cityAddress
		 */
		public function setCityAddress ( $cityAddress ) {
			$this->cityAddress = $cityAddress ;
		}
		
		/**
		 * @param string $cnpj
		 */
		public function setCnpj ( $cnpj ) {
			$this->cnpj = $cnpj ;
		}
		
		/**
		 * @param string $cpf
		 */
		public function setCpf ( $cpf ) {
			if( is_null( $cpf ) ){
				$this->cpf = NULL;
			}else{
				$this->cpf = str_replace( array('.', '-'), NULL, $cpf );
			}
		}
		
		/**
		 * @param string $dispatcherAgency
		 */
		public function setDispatcherAgency ( $dispatcherAgency ) {
			$this->dispatcherAgency = $dispatcherAgency ;
		}
		
		/**
		 * @param string $email
		 */
		public function setEmail ( $email ) {
			if( is_null( $email ) ){
				$this->email = NULL;
			}else{
				$this->email = $email;
			}
		}
		
		/**
		 * @param string $login
		 */
		public function setLogin ( $login ) {
			$this->login = $login ;
		}
		
		/**
		 * @param string $lotacao
		 */
		public function setLotacao ( $lotacao ) {
			$this->lotacao = $lotacao ;
		}
		
		/**
		 * @param string $name
		 */
		public function setName ( $name ) {
			if( is_null( $name ) ){
				$this->name = NULL;
			}else{
				$this->name = $name ;
			}
		}
		
		/**
		 * @param string $nationality
		 */
		public function setNationality ( $nationality ) {
			$this->nationality = $nationality ;
		}
		
		/**
		 * @param City $naturality
		 */
		public function setNaturality ( $naturality ) {
			$this->naturality = $naturality ;
		}
		
		/**
		 * @param string $nis
		 */
		public function setNis ( $nis ) {
			$this->nis = $nis ;
		}
		
		/**
		 * @param int $postalCode
		 */
		public function setPostalCode ( $postalCode ) {
			$this->postalCode = $postalCode ;
		}
		
		/**
		 * @param string $responsibleCpf
		 */
		public function setResponsibleCpf ( $responsibleCpf ) {
			$this->responsibleCpf = $responsibleCpf ;
		}
		
		/**
		 * @param string $responsibleName
		 */
		public function setResponsibleName ( $responsibleName ) {
			$this->responsibleName = $responsibleName ;
		}
		
		/**
		 * @param string $rg
		 */
		public function setRg ( $rg ) {
			$this->rg = $rg ;
		}
		
		/**
		 * @param string $socialReason
		 */
		public function setSocialReason ( $socialReason ) {
			$this->socialReason = $socialReason ;
		}
		
		/**
		 * @param Uf $stateAddress
		 */
		public function setStateAddress ( $stateAddress ) {
			$this->stateAddress = $stateAddress ;
		}
		
		/**
		 * @param String $telephoneNumber
		 */
		public function setTelephoneNumber ( $telephoneNumber ) {
			$this->telephoneNumber = $telephoneNumber ;
		}
		
		/**
		 * @param string $workInstitution
		 */
		public function setWorkInstitution ( $workInstitution ) {
			$this->workInstitution = $workInstitution ;
		}
		
		public function setArrUserInfoAndPermissionId( $array )
		{
			$this->userInfoAndPermissionId = $array;
		}
		
		public function getArrUserInfoAndPermissionId()
		{
			return( $this->userInfoAndPermissionId );
		}
		
		public function addPermission( $permission )
		{
			$this->arrPermission[] = $permission;
		}
		
		public function setArrPermission( $arrPermission )
		{
			$this->arrPermission = $arrPermission ;
		}
		
		public function getArrPermission()
		{
			return ( $this->arrPermission );
		}
	
	}

?>