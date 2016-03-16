<?php
	
class DemandaClassificacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.demandaclassificacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dmcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dmcid' => null, 
									  	'dmdid' => null, 
									  	'claid' => null, 
									  	'usucpfinclusao' => null, 
									  	'dmcdtinclusao' => null, 
									  	'usucpfinativacao' => null, 
									  	'dmcdtinativacao' => null, 
									  	'dmcstatus' => null, 
									  );
}