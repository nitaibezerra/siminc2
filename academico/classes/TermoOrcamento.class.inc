<?php
	
class TermoOrcamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.termoorcamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tmoid" );
    
    public $ndpcod;
    
    public $boJson = false;

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tmoid' => null, 
									  	'ndpid' => null, 
									  	'tmcid' => null, 
									  	'prgcod' => null, 
									  	'acacod' => null, 
									  	'tmovalor' => null, 
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