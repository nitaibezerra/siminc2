<?php
	
class PreObraPac extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preobrapac";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "muncod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'popquantidade' => null,
    									'popano' => null, 
									  	'poptipoobra' => null,
									  );
									  
	public function recuperarQuantidadePac($muncod)
	{
		$sql = "SELECT 
					popquantidade 
				FROM {$this->stNomeTabela} 
				WHERE muncod = '{$muncod}'";
		
		return $this->pegaUm($sql);
	}
									  
}									  