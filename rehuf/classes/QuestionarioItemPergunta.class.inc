<?php
	
class QuestionarioItemPergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.itempergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    //  perid = id da pergunta
    protected $arAtributos     = array(
									  	'itpid' => null,
									  	'perid' => null,
    									'itptitulo' => null
									  );
}