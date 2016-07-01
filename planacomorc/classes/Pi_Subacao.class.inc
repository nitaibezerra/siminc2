<?php
	
class Pi_Subacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_subacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sbaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
        'sbaid' => null, 
        'sbadsc' => null, 
        'sbastatus' => null, 
        'sbadata' => null, 
        'sbasigla' => null, 
        'sbatitulo' => null, 
        'sbacod' => null, 
        'usucpf' => null, 
        'sbasituacao' => null, 
        'sbaobras' => null, 
        'sbaplanotrabalho' => null, 
        'pigid' => null,
        'pieid' => null,
        'sbaano' => null
    );
									  
	public function deletaSubacaoDotacao($sbaid){
		$sql = "DELETE FROM monitora.pi_subacaodotacao WHERE sbaid = '$sbaid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
	
	public function deletaSubacaoUnidade($sbaid){
		$sql = "DELETE FROM monitora.pi_subacaounidade WHERE sbaid = '$sbaid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
	
	public function deletaSubacaoAtividade($sbaid){
		$sql = "DELETE FROM monitora.pi_subacaoatividade WHERE sbaid = '$sbaid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
	
	public function deletaSubacaoHistorico($sbaid){
		$sql = "DELETE FROM monitora.pi_subacaohistorico WHERE sbaid = '$sbaid'";
		if ( !$this->executar($sql)){
			return false;
		}
	}
}