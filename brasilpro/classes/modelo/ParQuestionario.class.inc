<?php
	
class ParQuestionario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.parquestionario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qtpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qtpid' => null, 
									  	'inuid' => null, 
									  	'qrpid' => null, 
    									'pqtanoreferencia' => null
									  );
}