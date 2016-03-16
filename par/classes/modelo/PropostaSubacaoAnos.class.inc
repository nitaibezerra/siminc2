<?php
class PropostaSubacaoAnos extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostasubacaoanos";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "psaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
    									'psaid' => null, 
									  	'ppsid' => null, 
									  	'praid' => null
									  );
									  
									  
	public function recuperaDadosAnosPorAno($ano)
	{
		
		$sql = "SELECT 	praid, praanos		
				FROM par.propostaanos
				WHERE praanos = {$ano}					
				ORDER BY praid";
		
		return $this->pegaLinha($sql);
	}
	
	public function recuperaAnos()
	{
		
		$sql = "SELECT 	praid, praanos		
				FROM par.propostaanos
				WHERE prastatus = 'A'					
				ORDER BY praid";
		
		return $this->carregar($sql);
	}
	
	public function recuperarAnosPorSubacao($ppsid)
	{
		$sql = "SELECT  pps.psaid, pps.ppsid, pps.praid	
				FROM par.propostasubacaoanos pps
				INNER JOIN par.propostaanos pra ON pps.praid = pra.praid and prastatus = 'A'
				WHERE pps.ppsid = {$ppsid} order by pra.praid";

		return $this->carregar($sql);
	}	

	public function excluirPorPpsid($ppsid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE ppsid = {$ppsid}";		
		$this->executar($sql);
	}
	
}									  