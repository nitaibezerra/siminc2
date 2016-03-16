<?php
	
class QuestionarioResposta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.questionarioresposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qrpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qrpid' => null,
									  	'queid' => null,
    									'qrptitulo' => null, 
    									'qrpdata' => null
									  );
}