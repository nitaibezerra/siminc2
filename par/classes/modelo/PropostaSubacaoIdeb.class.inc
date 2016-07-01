<?php
	
class PropostaSubacaoIdeb extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostasubacaoideb";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "psiid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
										'psiid' => null,								  	
									    'ppsid' => null, 
									  	'tpmid' => null
									  );
									  
	public function excluirPorPpsid($ppsid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE ppsid = {$ppsid}";		
		$this->executar($sql);
	}
}