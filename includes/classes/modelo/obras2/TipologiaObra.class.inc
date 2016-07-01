<?php

class TipologiaObra extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipologiaobra";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tpoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tpoid' => null,
        'cloid' => null,
        'tpodsc' => null,
        'tpodetalhe' => null,
        'tpomedida' => null,
        'tpodtinclusao' => null,
        'tpostatus' => null,
        'orgid' => null,
    );

    public function listaCombo(Array $param = array(), $query = false) {
        $where = array();

        if (!empty($param['orgid'])) {
            $where[] = "orgid = {$param['orgid']}";
        }

        if (!empty($param['tpodsc'])) {
            $where[] = "tpodsc LIKE '{$param['tpodsc']}%'";
        }

        if (!empty($param['IS(mi)'])) {
            $where[] = "tpoid IN (" . TPOID_MI_TIPO_B . ", " . TPOID_MI_TIPO_C . ")";
        }

        if (!empty($param['NOT(mi)'])) {
            $where[] = "tpoid NOT IN (" . TPOID_MI_TIPO_B . ", " . TPOID_MI_TIPO_C . ")";
        }

        if (!empty($param['tpoid'])) {
            $where[] = "tpoid IN (" . implode(', ', $param['tpoid']). ")";
        }

        $sql = "SELECT
					tpoid AS codigo,
					tpodsc AS descricao
				FROM
					obras2.tipologiaobra
				WHERE
					tpostatus = 'A'" .
                (count($where) ? " AND " . implode(" AND ", $where) : "");


        if ($query)
            return $sql;

        $dados = $this->carregar($sql);
        return (is_array($dados) ? $dados : array());
    }

    public function listaComboMarcados($param) {
        global $db;
        //ver($param,d);

        if (!empty($param[0])) {
            if (is_array($param)) {

                $sql = "SELECT
                
                        tpoid AS codigo,
                        tpodsc AS descricao
					 
					
				FROM
					obras2.tipologiaobra
				WHERE
					(tpostatus = 'A')" .
                        (count($param) ? " and (tpoid = " . implode(" or tpoid = ", $param) : "");
                $sql .= ")";

                return $sql;
            }
        }
    }

}
