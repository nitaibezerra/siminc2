<?php
	
class SecretariaEstMun extends Modelo{
	
	public function __construct($semid){
		
		parent::__construct();
		
		if( $semid ){	
			$this->carregarPorId( $semid );
		}
	}	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.secretariaestmun";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "semid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'semid' => null, 
									  	'empid' => null, 
									  	'semsecretariaestadualmunicipal' => null, 
									  	'semnomesecretariosaude' => null, 
									  	'semendsecretariasaude' => null, 
									  	'semnomerepresecretariasaude' => null, 
									  	'sememailrepresecretariasaude' => null, 
									  	'semtelefonerepresecretariasaude' => null, 
									  	'semcargofuncaorepresecretariasa' => null, 
									  	'semnomesecretarioeducacao' => null, 
									  	'semendsecretariaeducacao' => null, 
									  	'semnomerepresecretariaeducacao' => null, 
									  	'sememailrepresecretariaeducacao' => null, 
									  	'semtelefonerepresecretariaeduca' => null, 
									  	'semcargofuncaorepresecretariaed' => null, 
									  );
									  
	function corrigeCharset()
	{

		foreach($this->arAtributos as $chave => $value)
		{
			$this->$chave = utf8_decode($value);
		}
	}								  

									  
}