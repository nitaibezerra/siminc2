<?php
	
class CtContratada extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.ctcontratada";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ctaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
										'ctaid' => null, 
										'ctanome' => null, 
										'ctacnpj' => null, 
										'ctacep' => null, 
										'ctalog' => null, 
										'ctanum' => null, 
										'ctacom' => null, 
										'ctabai' => null, 
										'muncod' => null, 
										'estuf' => null, 
										'ctafoneddd' => null, 
										'ctafonenum' => null, 
										'ctafaxddd' => null, 
										'ctafaxnum' => null, 
										'ctafonedddopc' => null, 
										'ctafonenumopc' => null, 
										'ctaemail' => null, 
										'ctastatus' => null,
    									  );
}