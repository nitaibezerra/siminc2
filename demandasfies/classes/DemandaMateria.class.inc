<?php
	
class DemandaMateria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.demandamateria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dmmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dmmid' => null, 
									  	'dmdid' => null, 
									  	'matid' => null, 
									  	'usucpfinclusao' => null, 
									  	'dmmdtinclusao' => null, 
									  	'usucpfinativacao' => null, 
									  	'dmmdtinativacao' => null, 
									  	'dmmstatus' => null, 
									  );
}