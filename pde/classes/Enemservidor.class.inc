<?php
	
class EnemServidor extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pde.enemservidor";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eseid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eseid' => null, 
									  	'ediid' => null, 
									  	'usucpf' => null, 
									  	'eseistatus' => null, 
									  	'esedatacadastro' => null, 
									  	'entid' => null, 
									  );
}