<?php
	
class SituacaoProjeto extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.situacaoprojeto";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'stpid' => null, 
									  	'stpdesc' => null, 
									  	'stpstatus' => null, 
									  );
}