<?php
	
class ProcessoObjeto extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.processoobjeto";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "objid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'objid' => null, 
									  	'prcid' => null, 
									  	'objdescricao' => null, 
									  	'objbeneficio' => null, 
									  	'objjustificativa' => null, 
									  	'objplanotrabalho' => null, 
									  	'objtam' => null, 
									  	'objdata' => null,
    									'objstatus' => null, 
									  	'objdtinclusao' => null, 
									  );
									  
}