<?php
	
class PropostaSubacaoMunicipio extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostasubacaomunicipio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "psmid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppsid' => null, 
									  	'muncod' => null,
    									'psmid' => null,
									  );
									  
	public function excluirPorPpsid($ppsid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE ppsid = {$ppsid}";		
		$this->executar($sql);
	}
}