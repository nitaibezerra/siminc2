<?php
	
class EmailAtividadeMonitoramento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.emailatividademonitoramento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eamid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eamid' => null,
									  	'emlid' => null,
									  	'atmid' => null,
									  	'eamdtinclusao' => null,
									  );
}