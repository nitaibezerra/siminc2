<?php
	
class MensagemCampoTipo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.mensagemcampotipo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mctid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mctid' => null, 
									  	'mctdsc' => null, 
									  	'mctfk' => null, 
									  	'mcttipo' => null, 
									  );
}