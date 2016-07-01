<?php
	
class Analise extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.analise";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "anaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'anaid' => null, 
									  	'ptrid' => null, 
									  	'uniid' => null, 
									  	'resid' => null, 
									  	'anatipo' => null, 
									  	'analote' => null, 
									  	'anasituacaoparecer' => null, 
									  	'anaparecer' => null, 
									  	'usucpf' => null, 
									  	'anadataconclusao' => null, 
									  );
}