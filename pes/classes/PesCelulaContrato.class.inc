<?php
	
class PesCelulaContrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pes.pescelulacontrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ceccodigo" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ceccodigo' => null, 
									  	'concodigo' => null, 
									  	'ccdcodigo' => null, 
									  	'cecvalor' => null, 
									  );
}