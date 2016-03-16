<?php
	
class DespesaOrcamentaria extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.despesaorcamentaria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dpoid" );
    
    public $ndpcod;
    
    public $boJson = false;

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dpoid' => null, 
									  	'prcid' => null, 
									  	'ndpid' => null, 
									  	'dpovalor' => null, 
									  	'dpodtinclusao' => null, 
									  	'dpostatus' => null, 
									  );
									  
	public function antesSalvar()
	{
		$obNaturezaDespesa = new NaturezaDespesa();
		if($ndpid = $obNaturezaDespesa->existeCodNatureza($this->ndpcod)){
			$this->ndpid = $ndpid;
		} else {
			if($this->boJson){
				$arRetorno['msg'] = 'erro';
				$arRetorno['alerta'] = utf8_encode('Não existe Natureza de código '.$this->ndpcod);
				echo simec_json_encode($arRetorno);
				exit;
			} else {
				echo "<script>
						alert('Não existe Natureza de código ".$this->ndpcod."');
					 </script>";
				return false;			
			}	
		}
			
		return true;	
	}

}