<?php
	
class AcaoJudicial extends Modelo{

    const K_MANDADO_SEGURANCA   = 1;
    const K_ACAO_ORDINARIA      = 2;
    const K_ACAO_CIVIL_PUBLICA  = 3;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.acaojudicial";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "acjid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'acjid' => null, 
									  	'acjnome' => null, 
									  	'acjstatus' => null, 
									  	'acjcriticidade' => null, 
									  );
}