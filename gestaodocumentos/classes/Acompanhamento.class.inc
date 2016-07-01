<?php
	
class Acompanhamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.acompanhamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "solid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'acoid' => null, 
									  	'usucpf' => null, 
									  	'tarid' => null, 
									  	'acodsc' => null, 
									  	'acodata' => null, 
									  	'acostatus' => null, 
									  );
									  
	/*public function carregarPorTarId($tarid){
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = $id; ";

		$arResultado = $this->pegaLinha( $sql );
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
	}*/
}