<?php
	
class TipoOrigemObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoorigemobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tooid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tooid' => null, 
									  	'toodescricao' => null, 
									  	'toostatus' => null, 
									  );

	public function listaCombo($permissao = false, Array $param = array(), $query = false){
        $where = array();

        if (!empty($param['tooid'])) {
            $where[] = "tooid IN (" . implode(', ', $param['tooid']). ")";
        }

		$sql = "SELECT
					tooid AS codigo,
					toodescricao AS descricao
				FROM
					obras2.tipoorigemobra
				WHERE
					toostatus = 'A'
                    " . ( $permissao==false ? 'AND tooid <> 1' : '' ) ."
                    " . (count($where) ? (" AND " . implode(" AND ", $where)) : "") . "
				ORDER BY
					toodescricao";
        if($query)
            return $sql;
        
		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}

    public function listaComboRecursos(){
        $sql = "SELECT
					tooid AS codigo,
					toodescricao AS descricao
				FROM
					obras2.tipoorigemobra
				WHERE
					toostatus = 'A' AND tooid NOT IN(5, 7, 8)
				ORDER BY
					toodescricao";

        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }

}