<?php

class TipoArquivo extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoarquivo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tpaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'tpaid' => null,
                                    'tpadesc' => null,
                                    'tpastatus' => null,
                                    'tpadtinclusao' => null,
                                  );

    public function listaCombo(Array $param = array()) {
//        $where = array();
        $where = $param;

        $sql = "SELECT
                        tpaid AS codigo,
                        tpadesc AS descricao
                FROM
                        obras2.tipoarquivo
                WHERE
                        tpastatus = 'A' ".(count($where) ? " AND " . implode(" AND ", $where) : "")."
                ORDER BY
                        tpaid;";
                
        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }
}
