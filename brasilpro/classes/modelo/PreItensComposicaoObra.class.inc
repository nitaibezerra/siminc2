<?php
	
class PreItensComposicaoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preitenscomposicaoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "icoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'icoid' => null, 
									  	'preid' => null, 
    									'icopercprojperiodo' => null,
    									'icopercexecutado' => null,
    									'icoordem' => null,
    									'icodtinicioitem' => null,
    									'icodterminoitem' => null,
    									'icostatus' => null,
    									'icodtinclusao' => null,
    									'itcid' => null,  	 
									  );
									  
}									  