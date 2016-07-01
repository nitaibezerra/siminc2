<?php
	
class TipoResponsabilidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tiporesponsabilidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tprcod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tprcod' => null, 
									  	'tprdsc' => null, 
									  	'tprsnvisivelperfil' => null, 
									  	'tprsigla' => null, 
									  	'tprurl' => null, 
									  	'tprtabela' => null, 
									  	'tprcampo' => null, 
									  	'tprcampodsc' => null, 
									  );
}