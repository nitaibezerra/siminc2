<?php
	
class Monitoramento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.monitoramento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "monid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'monid' => null, 
									  	'docid' => null, 
									  	'espid' => null, 
									  	'tecid' => null, 
									  	'tpqid' => null, 
									  	'qrpid' => null, 
									  	'montipo' => null, 
									  	'monreferencia' => null, 
									  );
}