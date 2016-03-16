<?php
	
class SituacaoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.situacaoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sobid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sobid' => null, 
									  	'sobdsc' => null, 
									  	'sobordem' => null, 
									  	'sobstatus' => null 
									  );
									  
	public function listaCombo( Array $param = array() ){
		$where = array();
		
		$sql = "SELECT
					sobid AS codigo,
					sobdsc AS descricao
				FROM
					obras2.situacaoobra
				WHERE
					sobstatus = 'A'
				ORDER BY
					sobordem" . 
				(count($where) ? " AND " . implode(" AND ", $where) : "");
				
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}