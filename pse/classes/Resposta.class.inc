<?php
	
class Resposta extends Modelo{
	
	
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
    protected $stNomeTabela = "pse.resposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "resid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'resid' => null, 
									  	'perid' => null, 
									  	'empid' => null, 
									  	'resflagresposta' => null, 
									  );
									  
	function carregarResposta($idPergunta, $empid)
	{
		$sql = "SELECT respflagrespost FROM {$this->stNomeTabela} WHERE perid = {$this->perid}";
		//$rs 
	}								  
}