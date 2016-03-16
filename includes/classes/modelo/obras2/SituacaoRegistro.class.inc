<?php
	
class SituacaoRegistro extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.situacao_registro";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "strid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'strid' => null,
									  	'strdsc' => null,
									  	'strstatus' => null
									  );

	public function listaCombo(){

		$sql = "SELECT
					strid AS codigo,
					strdsc AS descricao
				FROM
					obras2.situacao_registro
				WHERE
					strstatus = 'A'
				ORDER BY
					strdsc ASC";

		$dados = $this->carregar($sql);

		return (is_array($dados) ? $dados : array());
	}

    public function listaComboSql(){

        $sql = "SELECT
					strid AS codigo,
					strdsc AS descricao
				FROM
					obras2.situacao_registro
				WHERE
					strstatus = 'A'
				ORDER BY
					strdsc ASC";
        return $sql;
    }
									  
}