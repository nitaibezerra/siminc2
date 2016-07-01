<?php
	
class FolhaPagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.folhapagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fpgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fpgid' => null, 
									  	'tfpid' => null,
    									'fpgdsc' => null,    
    									'fpgmes' => null,    
    									'fpgstatus' => null,);    
    
}