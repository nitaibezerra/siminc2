<?php
	
class TipoPrioridade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.tipoprioridade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tppid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tppid' => null, 
									  	'tppdsc' => null, 
									  );
									  
	public function listaCombo(){
		
		$sql = "SELECT
					tppid AS codigo,
					tppdsc AS descricao
				FROM
					agenda.tipoprioridade"; 
		
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}