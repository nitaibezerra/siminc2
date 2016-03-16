<?php
	
class Restricao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.restricao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "resid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'resid' => null,
									  	'resdata' => null, 
									  	'resdescricao' => null, 
									  	'resstatus' => null, 
									  	'tarid' => null, 
									  	'usucpf' => null, 
									  	'ressolucao' => null, 
									  	'resdatasolucao' => null, 
									  	'usucpfsolucao' => null, 
									  	'resmedida' => null, 
									  );
}