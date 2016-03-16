<?php
	
class Pagamento_atividadeperiodo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.pagamento_atividadeperiodo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'atpid' => null, 
									  	'pflcod' => null, 
									  	'atpdsc' => null, 
									  	'atpobs' => null, 
									  	'atpstatus' => null, 
									  );
}