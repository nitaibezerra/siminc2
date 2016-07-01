<?php 
	class ConfigurationPermissionInfo 
	{
	
	private $id;												// co_permissao_configuracao
	private $personType;										// tp_pessoa
	private $registerStatus;									// tp_registro
	private $changeJustify;										// ds_justificativa_alteracao
	private $defaultPermission;									// st_permissao_defaul
	
	//Atributos do Profile.
	private $changeUserResponsibleConf;							// co_usuario_responsavel_operacao
	private $description;										// ds_perfil
	private $sgProfile; 		 								// sg_perfil	
	
	public static function loadPermissionInfo( 
		$id ,
		$personType ,
		$registerStatus , 
		$changeJustify ,
		$defaultPermission ,
		$changeUserResponsibleConf , 
		$description ,
		$sgProfile
		)	
	{
		$userPermission = new ConfigurationPermissionInfo();
		$userPermission->setId( $id );
		$userPermission->setPersonType( $personType );
		$userPermission->setRegisterStatus( $registerStatus );
		$userPermission->setChangeJustify( $changeJustify );
		$userPermission->setDefaultPermission( $defaultPermission );
		$userPermission->setChangeUserResponsibleConf( $changeUserResponsibleConf );
		$userPermission->setDescription( $description );
		$userPermission->setSgProfile( $sgProfile );			
		return( $userPermission );
	}	
	
	public function getDefaultPermission() {
		return $this->defaultPermission ;
	}
	
	public function getChangeUserResponsibleConf() {
		return $this->changeUserResponsibleConf ;
	}
	
	public function setChangeUserResponsibleConf($changeUserResponsibleConf) {
		$this->changeUserResponsibleConf = $changeUserResponsibleConf ;
	}
	
	public function getDescription() {
		return $this->description ;
	}
	
	public function setDescription($description) {
		$this->description = $description ;
	}
	
	public function getSgProfile() {
		return $this->sgProfile ;
	}
	
	public function setSgProfile($sgProfile) {
		$this->sgProfile = $sgProfile ;
	}
	
	public function setDefaultPermission($defaultPermission) {
		$this->defaultPermission = $defaultPermission ;
	}
	
	/*
	public function setDefaultPermission($defaultPermission) {
		try {
			$this->defaultPermission = Integer.parseInt(defaultPermission) ;
		} catch (Throwable t) {
			$this->defaultPermission = null ;
		}
	}
	*/
	
	public function getId() {
		return $this->id ;
	}
	
	public function setId($id) {
		$this->id = $id ;
	}
	
	/*
	public function setId($id) {
		$this->id = $Long.parseLong(id) ;
	}
	*/
	
	public function getPersonType() {
		return $this->personType ;
	}
	
	public function setPersonType($personType) {
		$this->personType = $personType ;
	}	
	
	/*
	public function setPersonType($personType) {
		$this->personType = $personType.charAt(0) ;
	}	
	*/
	
	public function getRegisterStatus() {
		return $this->registerStatus ;
	}
	
	public function setRegisterStatus($registerStatus) {
		$this->registerStatus = $registerStatus ;
	}

	/*
	public function setRegisterStatus($registerStatus) {
		$this->registerStatus = $registerStatus.charAt(0) ;
	}
	*/
	
	public function getChangeJustify() {
		return $this->changeJustify ;
	}
	
	public function setChangeJustify($changeJustify) {
		$this->changeJustify = $changeJustify ;
	}
}
?>