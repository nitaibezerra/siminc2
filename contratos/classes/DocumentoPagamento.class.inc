<?php
	
class DocumentoPagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.documentopagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dpaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dpaid' => null, 
									  	'eveid' => null, 
									  	'tdpid' => null, 
									  	'dpadataemissao' => null, 
									  	'dpavalor' => null, 
									  	'dpanumero' => null, 
									  	'dpaobs' => null, 
									  );
}