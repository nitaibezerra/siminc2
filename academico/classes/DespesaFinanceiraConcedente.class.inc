<?php
	
class DespesaFinanceiraConcedente extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.despesafinanceiraconcedente";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dfcid" );
    
    public $ndpcod;
    
    public $boJson = false;

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dfcid' => null, 
									  	'prcid' => null, 
									  	'ndpid' => null, 
									  	'dfcvalor' => null, 
									  	'dfcdtinclusao' => null, 
									  	'dfcstatus' => null, 
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