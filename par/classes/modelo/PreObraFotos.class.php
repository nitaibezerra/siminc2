<?php
	
class PreObraFotos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preobrafotos";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pofid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pofid' => null, 
									  	'pofdescricao' => null, 
									  	'preid' => null,
    									'arqid' => null, 
									  );
									  
}