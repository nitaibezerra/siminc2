<?php
	
class Portaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.portaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prtid' => null,
    									'prtnumero' => null, 
    									'prtdata' => null, 
    									'prtdtpublicacao' => null, 
    									'prtnumerodou' => null, 
									  );
									  

}