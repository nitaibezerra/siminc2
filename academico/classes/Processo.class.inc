<?php
	
class Processo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.processo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prcid' => null, 
									  	'unicodexecutor' => null, 
									  	'unitpocod' => null, 
									  	'ungcodgestor' => null, 
									  	'prtid' => null,
    									'prcnumero' => null, 
									  	'prndtinicio' => null, 
									  	'prndtfim' => null, 
									  	'tmcid' => null
									  );
}