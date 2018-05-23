<?php
	
class Ordembanchistorico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.ordembanchistorico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "obhid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'obhid' => null, 
									  	'orbid' => null, 
									  	'spgcodigo' => null, 
									  	'orbvalorpagamento' => null, 
									  	'usucpf' => null, 
									  	'obhdataalteracao' => null, 
									  );
}