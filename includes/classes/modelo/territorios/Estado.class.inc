<?php
	
class Estado extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.estado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "estuf" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'estuf' => null, 
									  	'muncodcapital' => null, 
									  	'regcod' => null, 
									  	'estcod' => null, 
									  	'estdescricao' => null, 
									  );

	function buscaUFPorObra( $endid ){
		$sql = "SELECT 
					estuf
				FROM
					entidade.endereco e
					JOIN {$this->stNomeTabela} m USING(estuf)
				WHERE
					e.endid = {$endid}
				LIMIT 1";
		
		return $this->pegaUm( $sql );				
	}									  

	function listaCombo( Array $param = array() ){
		$sql = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				ORDER BY
					estuf";
		
    	$dados = $this->carregar($sql);
    	
    	return (is_array($dados) ? $dados : array());
	}
    function listaComboSql( Array $param = array() ){
        $sql = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				ORDER BY
					estuf";

        return $sql;
    }
}