<?php
	
class Pagamento_Glosa_Supervisao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.pagamento_glosa_supervisao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pgsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pgsid' => null,
    									'pgsglosa' => null, 
									  	'pagid' => null,
									  	'tgpid' => null, 
    									'sueid' => null,
    									'usucpf' => null,
									  	'pgsobs' => null, 
    									'pgsvalor' => null,
                                        'pgsglosavalor' => null
									  );
									  
}