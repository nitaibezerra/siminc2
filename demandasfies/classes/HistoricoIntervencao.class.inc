<?php
	
class HistoricoIntervencao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.historicointervencao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("hiiid");

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hiiid' => null, 
									  	'dmdid' => null, 
									  	'docid' => null, 
									  	'hstid' => null, 
									  	'hiistatus' => null, 
									  	'pflcod' => null, 
									  	'esdidorigem' => null, 
									  );
}