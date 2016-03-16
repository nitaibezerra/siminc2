<?php
	
class PropostaSubacaoEstados extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostasubacaoestados";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pseid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppsid' => null, 
									  	'estuf' => null,
    									'pseid' => null,
									  );
									  
	public function excluirPorPpsid($ppsid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE ppsid = {$ppsid}";		
		$this->executar($sql);
	}
}