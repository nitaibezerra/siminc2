<?php
include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');
class HistoricoConvItemComposicao extends Regrasfinanceiro{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicoconvitemcomposicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hciid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hciid' => null, 
									  	'hmsid' => null, 
									  	'hmsquantidadeempenhada' => null, 
									  	'hmsquantidadeliquidada' => null, 
									  	'hmsquantidadepaga' => null, 
									  	'hmsvalorunitario' => null, 
									  	'hmsvalortotalempenhado' => null, 
									  	'hmsvalortotalliquidado' => null, 
									  	'hmsvalortotalpago' => null, 
									  	'scsid' => null, 
									  	'micid' => null, 
									  	'hmsobs' => null, 
									  	'cosid' => null, 
    									'hmsano' => null
									  );
									  
	public function recuperaDadosItensComposicao($hmsid){
		$sql = "SELECT hciid, scsid, hmsobs, hmsano 
				FROM cte.historicoconvitemcomposicao 
				WHERE  hmsid = ".$hmsid; 
		
		return $this->carregar($sql);
	}
}