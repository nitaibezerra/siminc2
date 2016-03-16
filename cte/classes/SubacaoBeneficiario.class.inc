<?php
	
class SubacaoBeneficiario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.subacaobeneficiario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( 'sbaid', 'benid', 'sabano' );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sbaid' => null, 
									  	'benid' => null, 
									  	'vlrurbano' => null, 
									  	'vlrrural' => null, 
									  	'sabano' => null, 
									  );
									  
									  
	public function excluirPorSubacao($sbaid, $parametro = NULL){
		$sql = "SELECT benid, sbaid, sabano FROM ".$this->stNomeTabela." WHERE sbaid =".$sbaid." ".$parametro;

		$beneficiarios = $this->carregar($sql);
		if(is_array($beneficiarios)){
			foreach($beneficiarios as $beneficiario){
				$sql = " DELETE FROM $this->stNomeTabela WHERE sbaid = ".$sbaid." and sabano = ".$beneficiario['sabano']." and benid = ".$beneficiario['benid']."; ";
				if($this->executar( $sql )){
					$this->commit();
				}else{
					return false;
				}
			}
			return true;
		}else{
			// Se não existe Beneficiarios.
			return false;
		}
	}
	
	public function carregarPorId( $sbaid, $benid, $sbano  ){
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE 	{$this->arChavePrimaria[0]} = '$sbaid' and 
															{$this->arChavePrimaria[1]} = '$benid' and  
															{$this->arChavePrimaria[2]} = '$sbano'; ";
		//dbg($sql,1);
		$arResultado = $this->pegaLinha( $sql );
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
		
	}
	
	
}