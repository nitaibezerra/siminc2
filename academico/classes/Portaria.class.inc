<?php
	
class Portaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.portaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prtid' => null, 
									  	'prtdsc' => null, 
									  	'prtdata' => null, 
									  	'prtstatus' => null 
									  );
}