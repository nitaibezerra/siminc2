<?php
include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');
class HistoricoMonitoramentoConvSubac extends Regrasfinanceiro{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicomonitoramentoconvsubac";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hmsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hmsid' => null, 
									  	'hmcid' => null, 
									  	'sbaid' => null, 
									  	'ssuid' => null,
    									'hmsjustificativa' => null,
    									'hmsano' => null
									  );
									  
	public function recuperaHmsid($prsid, $sbaid, $ano){
		$sql = "SELECT hmsid 
				FROM cte.historicomonitoramentoconvsubac 
				WHERE sbaid =".$sbaid."
				and hmsano = '".$ano."'
				AND hmcid = ".$_SESSION['hmc'][$prsid];
		
		return $this->pegaUm($sql);
		
	}
	
	public function salvarItens($hmsid, $hmsidAnterior, $ano){
			$sql = "SELECT  hmsquantidadeempenhada, hmsquantidadeliquidada, hmsquantidadepaga, hmsvalorunitario, 
							hmsvalortotalempenhado, hmsvalortotalliquidado, hmsvalortotalpago, scsid, micid, hmsobs, cosid 
					FROM cte.historicoconvitemcomposicao  i
					INNER JOIN cte.historicomonitoramentoconvsubac s ON i.hmsid = s.hmsid
					WHERE s.hmsid = ".$hmsidAnterior;
			$dadosAnteriores = $this->carregar($sql);
			if(is_array($dadosAnteriores) ){
				foreach($dadosAnteriores as $dados){
					if($dados['scsid'] != NAOINICIADAITEMCOMP){
						$obHistItem	= new HistoricoConvItemComposicao();
						$obHistItem->hmsid = $hmsid;
						$obHistItem->scsid = $dados['scsid'];
						$obHistItem->micid = $dados['micid'];
						$obHistItem->cosid = $dados['cosid'];
						$obHistItem->hmsano = $ano;
						if($dados['scsid'] == EXECUTADOITEMCOMP){
							$obHistItem->hmsquantidadeempenhada = $dados['hmsquantidadeempenhada'];
							$obHistItem->hmsquantidadeliquidada = $dados['hmsquantidadeliquidada'];
							$obHistItem->hmsquantidadepaga 		= $dados['hmsquantidadepaga'];
							$obHistItem->hmsvalorunitario 		= $dados['hmsvalorunitario'];
							$obHistItem->hmsvalortotalempenhado = $dados['hmsvalortotalempenhado'];
							$obHistItem->hmsvalortotalliquidado = $dados['hmsvalortotalliquidado'];
							$obHistItem->hmsvalortotalpago 		= $dados['hmsvalortotalpago'];
						}else if($dados['scsid'] == CANCELADAITEMCOMP){
							$obHistItem->hmsobs 				= $dados['hmsobs'];
						}
						if($obHistItem->salvar()){
							$obHistItem->commit();
						}else{
							$obHistItem->rollback();
						}
					}
				}	
			}else{
				return true;	
			}
	}
}