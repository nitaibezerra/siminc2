<?php
	
class Pontuacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.pontuacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptoid' => null, 
									  	'crtid' => null, 
									  	'ptojustificativa' => null, 
									  	'ptodemandamunicipal' => null, 
									  	'ptodemandaestadual' => null, 
									  	'ptodata' => null, 
									  	'usucpf' => null, 
									  	'inuid' => null, 
									  	'indid' => null, 
									  	'ptostatus' => null, 
									  	'ptoparecertecnico' => null, 
									  );
}