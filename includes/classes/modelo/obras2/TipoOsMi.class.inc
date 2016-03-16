<?php
	
class TipoOsMi extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoosmi";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tomid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tomid' => null, 
									  	'tomdsc' => null, 
									  	'tomstatus' => null, 
									  );

	public function listaCombo( Array $param = array() ){
		$where   = array();
		$where[] = "tomstatus = 'A'";
						
		$sql = "SELECT
					tomid AS codigo,
					tomdsc AS descricao
				FROM
					obras2.tipoosmi
				WHERE " . 
				(count($where) ? implode(" AND ", $where) : "");
				
		$dados = $this->carregar($sql);
		return (is_array($dados) ? $dados : array());
	}

	public function resgataTipoOSMI( $tomid ){
		$sql = "
			SELECT tomdsc 
			FROM obras2.tipoosmi
			WHERE tomid = '". $tomid ."'
		";
		$tomdsc = $this->pegaUm( $sql );
		return $tomdsc;
	}
									  
}