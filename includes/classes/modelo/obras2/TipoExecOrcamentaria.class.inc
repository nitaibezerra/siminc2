<?php
	
class TipoExecOrcamentaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoexecorcamentaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "teoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'teoid' => null, 
									  	'teodsc' => null, 
									  	'teodtinclusao' => null, 
									  	'teostatus' => null, 
									  );
}