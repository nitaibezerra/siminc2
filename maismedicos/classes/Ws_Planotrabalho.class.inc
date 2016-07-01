<?php
	
class Ws_Planotrabalho extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_planotrabalho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptrid' => null, 
									  	'wssdata' => null, 
									  	'ptrmes' => null, 
									  	'ptrano' => null, 
									  	'ptrinstituicao' => null, 
									  	'wssstatus' => null, 
									  );
}