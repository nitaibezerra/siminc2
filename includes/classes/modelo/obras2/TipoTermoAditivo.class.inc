<?php
	
class TipoTermoAditivo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipotermoaditivo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ttaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ttaid' => null, 
									  	'ttadsc' => null, 
									  );

	public function listaCombo(){
		
		$sql = "SELECT 
					ttaid AS codigo, 
					ttadsc AS descricao 
				FROM 
					obras2.tipotermoaditivo
				ORDER BY 
					ttadsc";
		
		$dados = $this->carregar($sql);
		return ($dados ? $dados : array());
	}
									  
}