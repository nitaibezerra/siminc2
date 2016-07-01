<?php

class TipoDocumento extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.tipodocumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tpdcod");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tpdcod' => null,
        'tpddsc' => null,
        'tpdstatus' => null
    );
   
    public function carregarTipoDocumento($booSQL, $arrTpdCod = array()){
          $sql = "SELECT DISTINCT
                    td.tpdcod as codigo,
                    td.tpddsc as descricao
                FROM
                    {$this->stNomeTabela} td
                    INNER JOIN par.modelosdocumentos md ON md.tpdcod = td.tpdcod 
                WHERE
                    td.tpdstatus = 'A'
                    AND md.mdostatus = 'A'";
                    if (count($arrTpdCod)>0) {
                        $sql .= " and td.tpdcod in (".  implode(',', $arrTpdCod).")";
                    }
            $sql .= " ORDER BY descricao";
          return ($booSQL) ? $sql : $this->carregar($sql);
    }
}