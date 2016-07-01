<?php
	
class DiretrizIndicador extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.diretrizindicador";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dinid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dinid' => null, 
									  	'dirid' => null, 
									  	'indid' => null, 
									  	'dinpeso' => null, 
									  );
}