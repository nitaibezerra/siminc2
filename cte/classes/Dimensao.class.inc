<?php
	
class Dimensao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.dimensao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dimid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dimid' => null, 
									  	'itrid' => null, 
									  	'dimcod' => null, 
									  	'dimdsc' => null, 
									  	'dimstatus' => null, 
									  );
}