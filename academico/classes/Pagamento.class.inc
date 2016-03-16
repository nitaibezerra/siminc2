<?php
	
class Pagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.pagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pagid" );
    
    public $ndpcod;

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pagid' => null, 
									  	'ndpid' => null,  
									  	'prcid' => null,  
									  	'entid' => null, 
									  	'pagcredor' => null, 
									  	'pagnumerodoc' => null, 
									  	'pagdtdoc' => null,
    									'pagnumerotitulo' => null, 
									  	'pagdttitulo' => null, 
									  	'pagvalor' => null, 
									  );
	
	public function antesSalvar()
	{
		$obNaturezaDespesa = new NaturezaDespesa();
		if($ndpid = $obNaturezaDespesa->existeCodNatureza($this->ndpcod)){
			$this->ndpid = $ndpid;
		} else {
			echo "<script>
					alert('Não existe Natureza de código ".$this->ndpcod."');
					window.location.href = 'academico.php?modulo=principal/relacaoPagamentos&acao=A';	
				 </script>";
			
			return false;	
		}
			
		return true;	
	}
									  
}