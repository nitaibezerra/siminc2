<?php
	
class UnidadeMedida extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.unidademedida";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "undid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'undid' => null, 
									  	'unddsc' => null, 

									  );
									  
	public function carregaUnidadeMedidaPorUndtipo($retorno = 'array'){
		$sql = "SELECT undid as codigo, unddsc as descricao FROM {$this->stNomeTabela} WHERE undstatus = 'A' ORDER BY unddsc ASC";
		if($retorno == 'array'){
			return $this->carregar($sql);
		} else {
			return $sql;
		}
	}
}