<?php
	
class MeParceiro extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pdeescola.meparceiro";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mepid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mepid' => null, 
									  	'memid' => null, 
									  	'mepquantidadealuno' => null, 
									  	'entid' => null, 
									  );
}