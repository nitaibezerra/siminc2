<?php
	
class Ws_Planotrabalho_Itens extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_planotrabalho_itens";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptiid' => null, 
									  	'wssdata' => null, 
									  	'ptrid' => null, 
									  	'cpfProfissional' => null, 
									  	'nomeProfissional' => null, 
									  	'situacao' => null, 
									  	'ibge' => null, 
									  	'municipio' => null, 
									  	'uf' => null, 
									  	'cpfTutor' => null, 
									  	'nomeTutor' => null, 
									  	'cpfSupervisor' => null, 
									  	'nomeSupervisor' => null, 
									  	'justificativaProfissional' => null, 
									  	'wssstatus' => null, 
    									'naolocalizado' => null,
									  );
}