<?php
	
class Tema extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "callcenter.tema";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "temid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'temid' => null, 
									  	'sisid' => null, 
									  	'temdatainicio' => null, 
									  	'temdatafim' => null, 
									  	'temaimagem' => null, 
									  	'temalink' => null, 
									  	'temadatainclusao' => null, 
									  	'temastatus' => null
									  );
}