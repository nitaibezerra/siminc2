<?php
	
class ItemSelecionado extends Modelo{
	
/*public function __construct($iteid = null, $resid = null ){
				
			parent::__construct();
			
			if($iteid){
				$iteid = $iteid;
			}else{
				$iteid = '';
			}
			
			if($resid){
				$resid = $resid;
			}else{
				$resid = '';
			}

			if($iteid && $resid){
				$iteid = $this->pegaUm( "SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$iteid} "  );
			}
	}
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.itemselecionado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "iseid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'iseid' => null, 
									  	'iteid' => null, 
									  	'resid' => null, 
									  );
									  
	function deletar($resid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE resid = " . $resid;
		
		if($this->executar($sql))
			$this->commit();
		else
			$this->rollback();	
	}								  
}