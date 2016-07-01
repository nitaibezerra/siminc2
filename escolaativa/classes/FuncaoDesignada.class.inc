<?php
	
class FuncaoDesignada extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.funcaodesignada";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fudid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fudid' => null, 
									  	'fuddsc' => null, 
									  	'fudvlrbolsa' => null, 
									  	'fuddtvigencia' => null, 
									  	'fudativo' => null, 
									  );
									  
}