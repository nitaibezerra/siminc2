<?php
	
class CoendereCoentrega extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.coenderecoentrega";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "coeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'coeid' => null, 
									  	'entidcampus' => null, 
									  	'estuf' => null, 
									  	'coenddsc' => null, 
									  	'muncod' => null, 
									  	'coaid' => null, 
									  	'coendcep' => null, 
									  	'coendlog' => null, 
									  	'coendcom' => null, 
									  	'coendbai' => null, 
									  	'coendnum' => null, 
									  	'coendhrini' => null, 
									  	'coendhrfim' => null, 
									  	'coendrespalmx' => null, 
									  	'coendstatus' => null, 
									  	'medlatitude' => null, 
									  	'medlongitude' => null, 
									  	'endzoom' => null, 
									  );
}