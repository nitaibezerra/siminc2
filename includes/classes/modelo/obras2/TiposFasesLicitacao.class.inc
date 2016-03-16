<?php
	
class TiposFasesLicitacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tiposfaseslicitacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tflid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tflid' => null, 
									  	'tfldesc' => null, 
									  	'tflstatus' => null, 
									  	'tfldtinclusao' => null, 
									  	'tflordem' => null, 
									  	'tflexibefp' => null, 
									  	'tflexibefl' => null, 
									  );

	public function listaCombo($licid = null, $tflid = null, $arFlcid = null){
		if ( empty($licid) ){
			
			if ( is_array($arFlcid) && count($arFlcid) ){
				$where = "tf.tflid NOT IN (SELECT 
					        				tflid 
					        			  FROM 
					        				obras2.faselicitacao a 
					        			  WHERE 
					        				a.flcid IN (" . implode(", ", $arFlcid) . ") AND 
				        					" . ($tflid ? " a.tflid NOT IN ({$tflid}) AND " : "") . "
					        				a.flcstatus = 'A') AND ";	
			}
			
			$sql = "SELECT 
						tf.tflid AS codigo, 
						tf.tfldesc AS descricao 
					FROM 
						obras2.tiposfaseslicitacao tf
			        WHERE 
			        	{$where}
	 					tflexibefl = true AND 
	 					tflstatus = 'A' 
					ORDER BY 
						tf.tflordem";
		}else{
			$sql = "SELECT 
						tf.tflid AS codigo, 
						tf.tfldesc AS descricao 
					FROM 
						obras2.tiposfaseslicitacao tf
			        WHERE 
			        	tf.tflid NOT IN (SELECT 
			        						tflid 
			        					   FROM 
			        					   	obras2.faselicitacao a 
			        					   WHERE 
			        					   	a.licid = {$licid} AND 
		        					   		" . ($tflid ? " tflid NOT IN ({$tflid}) AND " : "") . "
			        					   	a.flcstatus = 'A') AND
	 					tflexibefl = true AND 
	 					tflstatus = 'A' 
					ORDER BY 
						tf.tflordem";
		}
		
		$dados = $this->carregar($sql);
		return ($dados ? $dados : array());
	}
}