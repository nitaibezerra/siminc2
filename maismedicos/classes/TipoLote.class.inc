<?php
	
class TipoLote extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.tipolote";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tplid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tplid' => null, 
									  	'tpldsc' => null,
    									'tplcod' => null,    
    									'tplstatus' => null,);    
    
}