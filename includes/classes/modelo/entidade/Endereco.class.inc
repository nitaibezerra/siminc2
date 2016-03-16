<?php
	
class Endereco extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.endereco";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "endid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'endid' => null, 
									  	'entid' => null, 
									  	'tpeid' => null, 
									  	'endcep' => null, 
									  	'endlog' => null, 
									  	'endcom' => null, 
									  	'endbai' => null, 
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'endnum' => null, 
									  	'endstatus' => null, 
									  	'medlatitude' => null, 
									  	'medlongitude' => null, 
									  	'endcomunidade' => null, 
									  	'endzoom' => null, 
									  	'endpc' => null, 
									  );
}