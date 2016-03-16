<?php
	
class Pi_PlanoInternoHistorico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_planointernohistorico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pihid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pihid' => null, 
									  	'pliid' => null, 
									  	'pihobs' => null, 
									  	'pihdata' => null, 
									  	'usucpf' => null, 
									  	'pihsituacao' => null, 
									  	'plicod' => null, 
									  	'plicodorigem' => null, 
									  );
}