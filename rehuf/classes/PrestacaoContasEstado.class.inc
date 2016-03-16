<?php
	
class prestacaocontasestado extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.prestacaocontasestado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pceid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pceid' => null,
									  	'pcedescricao' => null,
									  	'prcid' => null
									  );
}