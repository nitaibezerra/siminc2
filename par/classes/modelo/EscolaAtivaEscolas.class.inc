<?php
	
class Escolaativaescolas extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.escolaativaescolas";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eaeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eaeid' => null, 
									  	'esaid' => null, 
									  	'entid' => null, 
									  	'eaeqtdpredios' => null, 
									  	'eaeqtdturmas' => null, 
									  	'eaeqtdalunos1' => null, 
									  	'eaeqtdalunos2' => null, 
									  	'eaeqtdalunos3' => null, 
									  	'eaeqtdalunos4' => null, 
									  	'eaeqtdalunos5' => null, 
									  );
}