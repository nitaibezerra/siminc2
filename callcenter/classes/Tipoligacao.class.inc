<?php
	
class Tipoligacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "callcenter.tipoligacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tlgdescricao" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tlgdescricao' => null, 
									  	'tlgid' => null, 
									  	'tlgstatus' => null
									  );
}