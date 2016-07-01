<?php
	
class EtapaQuestao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.etapaquestao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "etqid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'etqid' => null, 
									  	'etqdsc' => null, 
									  	'etqnumero' => null, 
									  	'etqordem' => null, 
									  	'etqstatus' => null, 
									  );
}