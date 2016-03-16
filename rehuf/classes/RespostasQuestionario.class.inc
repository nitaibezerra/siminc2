<?php
	
class RespostasQuestionario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.resposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "resid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    /*
     * perid = questionario.pergunta
     * qrpid = questionario.questionarioresposta
     * itpid = questionario.itempergunta      
	*/
    protected $arAtributos     = array(
									  	'resid'  => null,
									  	'perid'  => null,
    									'qrpid'  => null,
    									'usucpf' => null,
    									'itpid'  => null,
    									'resdsc' => null
									  );
}