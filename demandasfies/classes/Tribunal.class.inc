<?php
	
class Tribunal extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.tribunal";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "trbid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'trbid' => null, 
									  	'trbcompetencia' => null, 
									  	'trbnome' => null, 
									  	'trbstatus' => null, 
									  );
}