<?php
	
class Pi_SubacaoHistorico extends Modelo {
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_subacaohistorico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sahid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sahid' => null, 
									  	'sbaid' => null, 
									  	'sahobs' => null, 
									  	'sahdaa' => null, 
									  	'usucpf' => null, 
									  	'sahsituacao' => null, 
									  );
}