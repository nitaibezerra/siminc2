<?php

include_once APPRAIZ . 'maismedicos/classes/Ws_Territorios.class.inc';
	
class Ws_Territorios_Responsavel extends Ws_Territorios{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_territorios_responsavel";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "wsrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'wsrid' => null, 
									  	'wssdata' => null, 
									  	'idibge' => null, 
									  	'municipio' => null, 
									  	'regiao' => null, 
									  	'uf' => null, 
									  	'wstid' => null, 
									  );
 
}