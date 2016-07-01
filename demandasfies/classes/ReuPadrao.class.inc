<?php
	
class ReuPadrao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.reupadrao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "repid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'repid' => null, 
									  	'repcnpj' => null, 
									  	'repnome' => null, 
									  	'repstatus' => null, 
									  );
}