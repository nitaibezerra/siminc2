<?php
	
class PlanoTrabalho extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.planotrabalho";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptrid' => null, 
									  	'entid' => null, 
									  	'ptrexercicio' => null, 
									  	'ptrjustificativa' => null, 
									  	'ptrtipodirigente' => null, 
									  	'entiddirigente' => null, 
									  	'ptrstatus' => null, 
									  	'ptrvalorproponente' => null, 
									  	'bcoid' => null, 
									  	'ptragenciabancaria' => null, 
									  	'ptrcontrapartida' => null, 
									  	'docid' => null, 
									  	'ptrnumdocumenta' => null, 
									  	'ptrnumconvenio' => null, 
									  	'ptranoconvenio' => null, 
    
									  );
}