<?php
	
class TipoRestricao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tiporestricao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tprid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tprid' => null, 
									  	'tprdsc' => null, 
									  	'tprdtinclusao' => null, 
									  	'liberaanexo' => null,
									  	'tprstatus' => null,
									  );

	public function listaCombo(){
		$sql = "SELECT
					tprid AS codigo,
					tprdsc AS descricao
				FROM
					obras2.tiporestricao
				WHERE
					tprstatus = 'A'
				ORDER BY
					tprdsc";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}

    public function listaComboFlag(){
        $sql = "SELECT
					tprid AS codigo,
					tprdsc AS descricao,
					liberaanexo
				FROM
					obras2.tiporestricao
				WHERE
					tprstatus = 'A'
				ORDER BY
					tprdsc";

        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }
									  
}