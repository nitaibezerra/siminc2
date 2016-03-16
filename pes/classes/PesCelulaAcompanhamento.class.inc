<?php
	
class PesCelulaAcompanhamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pes.pescelulaacompanhamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ceacodigo" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ceacodigo' => null, 
									  	'concodigo' => null, 
									  	'ccdcodigo' => null, 
									  	'ceaano' => null, 
									  	'ceavalor' => null, 
									  );
}