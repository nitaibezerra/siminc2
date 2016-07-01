<?php
	
class EmailItemMonitoramento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.emailitemmonitoramento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eimid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eimid' => null,
									  	'emlid' => null,
									  	'itmid' => null,
									  	'eimdtinclusao' => null,
									  );
}