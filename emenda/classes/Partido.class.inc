<?php
	
class Partido extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.partido";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "parid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'parid' => null, 
									  	'parcodigo' => null, 
									  	'parnome' => null, 
									  	'parsigla' => null, 
									  	'parstatus' => null, 
									  );
}
?>