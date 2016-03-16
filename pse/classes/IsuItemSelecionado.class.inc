<?php
	
class IsuItemSelecionado extends Modelo{
	
public function __construct($isuid = null, $rulid = null ){
				
			parent::__construct();
			
			if($isuid){
				$isuid = $isuid;
			}else{
				$isuid = '';
			}
			
			if($rulid){
				$rulid = $rulid;
			}else{
				$rulid = '';
			}
/*
			if($iteid && $resid){
				$iteid = $this->pegaUm( "SELECT iteid FROM pse.itemselecionado WHERE resid = {$resid} AND iteid = {$iteid} "  );
			}
	*/}
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.isuitemselecionado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "isuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'isuid' => null, 
									  	'iulid' => null, 
									  	'rulid' => null,
    									'sisid' => null, 
									  );
									  
	function deletar($rulid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE rulid = " . $rulid;
		
		if($this->executar($sql))
			$this->commit();
		else
			$this->rollback();	
	}								  
}