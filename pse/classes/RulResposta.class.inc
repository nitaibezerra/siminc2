<?php
	
class RulResposta extends Modelo{
	
	
	public function __construct($rulid,$idpergunta = null, $espid = null ){
			
			parent::__construct();
			
			if($espid){
				$espid = $espid;
			}else{
				$espid = '';
			}

			if( !$rulid && $idpergunta && $espid){
				$rulid = $this->pegaUm( "SELECT rulid FROM pse.rulresposta WHERE pulid = '{$idpergunta}' AND espid= '{$espid}'" );
			}
			
			if( !$rulid && $idpergunta && !$espid){
				$rulid = $this->pegaUm( "SELECT rulid FROM pse.rulresposta WHERE pulid = '{$idpergunta}'" );
			}
			
			if( $rulid ){

				$this->carregarPorId( $rulid );
			}
	}
		
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.rulresposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rulid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rulid' => null, 
									  	'pulid' => null, 
									  	'espid' => null, 
									  	'rulflagresposta' => null, 
									  );
									  
	function carregarResposta($idPergunta, $espid)
	{
		$sql = "SELECT rulflagrespost FROM {$this->stNomeTabela} WHERE pulid = {$this->pulid}";
		//$rs 
	}								  
}