<?php

class ModelosDocumentos extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.modelosdocumentos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("mdoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'mdoid' => null,
        'tpdcod' => null,
        'mdonome' => null,
        'mdoconteudo' => null,
        'mdostatus' => null,
        'usucpf' => null,
        'mdodatainclusao' => null,
        'mdovalidacaogestor' => null,
        'mdovalidacaofnde' => null,
        'mdoqtdvalidacao' => null,
        'mdotipovinculado' => null,
        'mdovisivel' => null,
        'iutid' => null,
        'mdodocumentoex' => null
    );

    public function carregarModeloDocumento($tpdcod, $booSQL, $strObra = '', $arrCondicao = array()){
          $sql = "SELECT 
                    md.mdoid as codigo, 
                    md.mdonome as descricao
                FROM 
                    {$this->stNomeTabela} md 
                WHERE md.tpdcod = {$tpdcod}
                    AND lower(md.mdonome) {$strObra} ILIKE LOWER('%obra%')
                    AND md.mdostatus = 'A'";
                    if (count($arrCondicao)>0) {
                        foreach ($arrCondicao as $key => $value) {
                            $sql .= " AND {$key} = '{$value}'";  
                        }
                    }
            $sql .= " ORDER BY md.mdonome ASC";
          return ($booSQL) ? $sql : $this->carregar($sql);
    }
}