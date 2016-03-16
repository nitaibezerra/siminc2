<?php
	
class DadosUnidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.dadosunidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dunid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dunid' => null, 
									  	'duncpf' => null, 
									  	'duncnpj' => null, 
									  	'dunnome' => null, 
									  	'duntelddd' => null, 
									  	'duntel' => null, 
									  	'duncelddd' => null, 
									  	'duncel' => null, 
									  	'dunemail' => null, 
									  	'dunemail2' => null, 
									  	'dunrg' => null, 
									  	'dunorgaorg' => null, 
									  	'dunufrg' => null, 
									  	'dunfuncao' => null, 
									  	'muncod' => null, 
									  	'dunvinculo' => null, 
									  	'dunsegmento' => null, 
									  	'tduid' => null, 
									  	'estuf' => null, 
									  	'dunendereco' => null, 
									  	'duncep' => null, 
									  	'sgmid' => null, 
									  	'tvpid' => null, 
									  	'inuid' => null
									  );
									  
	public function recuperarSegmentos(){
		return  $this->carregar( "select sgmid as codigo, sgmdsc as descricao from cte.segmento" );
	}
}