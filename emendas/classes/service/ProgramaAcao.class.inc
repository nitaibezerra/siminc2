<?php
	
class ProgramaAcao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.programaacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pacid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pacid' => null, 
									  	'proid' => null, 
									  	'acaid' => null, 
									  	'pacstatus' => null, 
									  );
}