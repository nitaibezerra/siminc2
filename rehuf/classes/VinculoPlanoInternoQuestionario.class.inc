<?php
	
class VinculoPlanoInternoQuestionario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.vinculointernoquestionario";	

    protected $arChavePrimaria = array( "vpqid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
    									'vpqid'     => null,
    									'piid' 		=> null, 
									  	'qrpid' 	=> null,
									  );

	public function buscarVinculoQuestionario($vpiid){
		$sql = "SELECT 
						*
				FROM 
					{$this->stNomeTabela}
			   	WHERE 
			   		vpiid = " .$vpiid;
		$arrDados = $this->carregar($sql);
		return $arrDados ? $arrDados : array();
	}

	public function excluirVinculoQuestionario($vpiid){
		$sql = "DELETE FROM 
					{$this->stNomeTabela}
			   	WHERE 
			   		vpiid = " .$vpiid;
		$this->executar($sql);
		$this->commit();
	}
									  
}