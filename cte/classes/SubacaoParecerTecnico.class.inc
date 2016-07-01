<?php
	
class SubacaoParecerTecnico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.subacaoparecertecnico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sptid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sptid' => null, 
									  	'sbaid' => null, 
									  	'sptparecer' => null, 
									  	'sptunt' => null, 
									  	'sptuntdsc' => null, 
									  	'sptano' => null, 
									  	'sptinicio' => null, 
									  	'sptfim' => null, 
									  	'tppid' => null, 
									  	'ssuid' => null, 
									  	'sptanoterminocurso' => null, 
									  	'sptplanointerno' => null, 
									  	'sptnumprocesso' => null, 
									  );
}