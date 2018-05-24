<?php
	
class Especificacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.especificacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'espid' => null, 
									  	'espnome' => null, 
									  	'espunidademedida' => null, 
									  	'espkit' => null, 
									  	'espstatus' => null, 
									  );
}