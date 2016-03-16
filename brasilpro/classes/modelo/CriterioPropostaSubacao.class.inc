<?php 

class CriterioPropostaSubacao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.criteriopropostasubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( 'cpsid' );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
    									'cpsid' => null,
									  	'crtid' => null, 
									  	'ppsid' => null, 
									  );
									  
	public function carregarPorPpsid($ppsid)
	{
		$sql = "SELECT crtid FROM {$this->stNomeTabela} WHERE ppsid = {$ppsid}";
		
		return $this->carregar($sql);
	}
	
	public function excluirPorPpsid($ppsid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE ppsid = {$ppsid}";
		
		$this->executar($sql);
	}	
									  
}