<?php
	
class InstrumentoUnidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.instrumentounidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "inuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'inuid' => null, 
									  	'itrid' => null, 
									  	'inudata' => null, 
									  	'estuf' => null, 
									  	'muncod' => null, 
									  	'mun_estuf' => null, 
									  	'usucpf' => null, 
									  );
									  
	public function verificaInuidMunicipio($muncod)
	{
		$sql = "select inuid from {$this->stNomeTabela} where muncod = '{$muncod}'";
		return $this->pegaUm($sql);
	}
	
	public function verificaInuidEstado($estuf)
	{
		$sql = "select inuid from {$this->stNomeTabela} where estuf = '{$estuf}'";		
		return $this->pegaUm($sql);
	}
									  
}