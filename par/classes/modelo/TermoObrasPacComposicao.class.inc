<?php

class TermoObrasPacComposicao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.termoobraspaccomposicao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("terid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'terid' => null,
        'preid' => null
    );

    public function excluir($terid) {
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE terid = {$terid}";
        return $this->executar($sql);
    }
    
    public function inserir($terid, $preid) {
        $sql = "INSERT INTO {$this->stNomeTabela} (terid, preid) VALUES ({$terid}, {$preid})";
        return $this->executar($sql);
    }
}
