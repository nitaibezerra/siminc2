<?php
	
class EmpresaMi_Uf extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.empresami_uf";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eufid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eufid' => null, 
									  	'emiid' => null, 
									  	'estuf' => null, 
									  	'tmiid' => null, 
									  	'eufstatus' => null, 
									  );
}