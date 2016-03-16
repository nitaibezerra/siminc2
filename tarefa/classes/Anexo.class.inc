<?php
	
class Anexo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "tarefa.Anexo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "anxid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'anxid' => null, 
									  	'arqid' => null, 
									  	'tarid' => null, 
									  	'anxdesc' => null, 
									  	'anxdtinclusao' => null, 
									  	'tpdid' => null, 
									  	'anxassunto' => null, 
									  	'anxnumdoc' => null, 
									  	'anxnumsidoc' => null, 
									  	'estuf' => null, 
									  	'anxprocessosidoc' => null, 
									  );
}