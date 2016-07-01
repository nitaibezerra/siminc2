<?php
	
class SubacaoObra extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sobid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sobid' => null, 
									  	'sbaid' => null, 
									  	'preid' => null,
    									'sobano' => null
									  );
									  

	public function deletaPorPtoid($ptoid)
	{
		$sql = "SELECT preid FROM par.subacaoobra WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}))";
		$preids = $this->carregarColuna( $sql );

		$sql = "DELETE FROM par.subacaoobra WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}));"; 
		$this->executar( $sql );
		$this->commit();
		
		if( is_array( $preids ) ){
			foreach( $preids as $preid ){
				$obPreObra = new PreObra();
				$obPreObra->excluirPreObra( $preid );
				$obPreObra->commit();
			}
		}
	}
	
	public function deletaPorSbaid($sbaid, $ano = array())
	{
		$anos = implode( $ano, "," );

		$sql = "SELECT preid FROM par.subacaoobra WHERE sobano IN ({$anos}) AND sbaid = ".$sbaid;
		$preids = $this->carregarColuna( $sql );
		
		$sql = "DELETE FROM par.subacaoobra WHERE sobano IN ({$anos}) AND sbaid = ".$sbaid; 
		$this->executar( $sql );
		$this->commit();
		
		if( is_array( $preids ) ){
			foreach( $preids as $preid ){
				$obPreObra = new PreObra();
				$obPreObra->excluirPreObra( $preid );
				$obPreObra->commit();
			}
		}
	}
}									  