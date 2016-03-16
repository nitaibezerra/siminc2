<?php
	
class Pergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.pergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prgid' => null, 
									  	'dimid' => null, 
									  	'prgcod' => null, 
									  	'prgdsc' => null, 
									  	'prgstatus' => null, 
									  );
}