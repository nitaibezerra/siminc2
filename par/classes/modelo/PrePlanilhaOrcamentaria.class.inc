<?php
	
class PrePlanilhaOrcamentaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preplanilhaorcamentaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ppoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppoid' => null, 
									  	'preid' => null, 
									  	'itcid' => null,
    									'ppovalorunitario' => null,									  	 
									  );	

	public function excluiItensPlanilhaOrcamentaria($preid, $itcid)
	{
		
		if($preid){
			$sql = "delete from obras.preplanilhaorcamentaria where preid = {$preid} AND itcid = {$itcid};";
			if($this->executar($sql)){
				return true;
			}else{
				return false;
			}
		}
		return false;
	}
								  
}									  