<?php
	
class EntidadeExecucaoSubacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.entidadeexecucaosubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eesid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eesid' => null, 
									  	'exeid' => null, 
									  	'eesnome' => null, 
									  	'eescpf' => null, 
									  	'polid' => null, 
									  	'eesentescola' => null, 
									  	'eesentqtd' => null, 
									  	'scuid' => null, 
									  );
}