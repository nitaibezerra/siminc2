<?php
	
class DivisaoQuestao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.divisaoquestao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dvqid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dvqid' => null, 
									  	'dvqdsc' => null, 
									  	'dvqnumero' => null, 
									  	'dvqordem' => null, 
									  	'dvqstatus' => null, 
									  );
}