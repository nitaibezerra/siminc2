<?php
	
class EnemDiretor extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pde.enemdiretor";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ediid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ediid' => null, 
									  	'usucpf' => null, 
									  	'usudescricao' => null, 
									  	'edistatus' => null, 
									  	'edidatacadastro' => null, 
									  );
}