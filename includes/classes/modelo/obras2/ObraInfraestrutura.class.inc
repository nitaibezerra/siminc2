<?php
	
class ObraInfraestrutura extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.obrainfraestrutura";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "obrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'obrid' => null, 
									  	'orgid' => null, 
									  	'endid' => null, 
									  	'entidunidade' => null, 
									  	'obrdesc' => null, 
									  	'obrdescundimplantada' => null, 
									  	'obrdtinicio' => null, 
									  	'obrdttermino' => null, 
									  	'usucpf' => null, 
									  	'entidcampus' => null, 
									  	'obrdtinauguracao' => null, 
									  	'obrdtprevinauguracao' => null, 
									  	'obrstatusinauguracao' => null, 
									  	'obrdtvistoria' => null, 
									  	'obrvalorprevisto' => null, 
									  	'obrprazoexec' => null, 
									  	'docid' => null, 
									  	'obrtipoesfera' => null, 
									  	'tobid' => null, 
									  	'cloid' => null, 
									  	'obrstatus' => null, 
									  );
}