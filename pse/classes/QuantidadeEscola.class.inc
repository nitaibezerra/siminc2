<?php
	
class QuantidadeEscola extends Modelo{
	
/*
	public function __construct($resid,$idpergunta = null, $empid = null ){
			
			parent::__construct();
			
			if($empid){
				$empid = $empid;
			}else{
				$empid = '';
			}

			if( !$resid && $idpergunta && $empid){
				$resid = $this->pegaUm( "SELECT resid FROM pse.resposta WHERE perid = '{$idpergunta}' AND empid= '{$empid}'" );
			}
			
			if( !$resid && $idpergunta && !$empid){
				$resid = $this->pegaUm( "SELECT resid FROM pse.resposta WHERE perid = '{$idpergunta}'" );
			}
			
			if( $resid ){

				$this->carregarPorId( $resid );
			}
	}
		
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.quantidadeescola";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qtdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qtdid' => null, 
									  	'atvid' => null, 
									  	'empid' => null, 
									  	'qtdescola' => null, 
									  );
									  
	function carregarResposta($idPergunta, $empid)
	{
		$sql = "SELECT respflagrespost FROM {$this->stNomeTabela} WHERE perid = {$this->perid}";
		//$rs 
	}
	function deletar($qtdid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE qtdid = " . $qtdid;
		
		if($this->executar($sql))
			$this->commit();
		else
			$this->rollback();	
	}									  
}