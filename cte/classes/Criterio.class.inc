<?php
	
class Criterio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.criterio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "crtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'crtid' => null, 
									  	'indid' => null, 
									  	'tpcid' => null, 
									  	'crtdsc' => null, 
									  	'ctrpontuacao' => null, 
									  	'ctrord' => null, 
									  	'ctrindpla' => null, 
									  );
}