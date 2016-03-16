<?php

class Contratos extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.contratos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("conid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'conid' => null,
        'condescricao' => null,
        'constatus' => null,
        'condata' => null,
        'connumerocontrato' => null,
        'usucpf' => null,
        'arqid' => null,
        'convlrcontrato' => null,
        'concnpj' => null,
        'convalidado' => null
    );

    public function alterarValidacaoContrato($conid, $convalidado) {
        $sql = "UPDATE {$this->stNomeTabela} SET convalidado = '{$convalidado}' WHERE conid = {$conid}";
        return $this->executar($sql);
    }

    public function findById($conid){
        $sql = "SELECT
                  conid,
                  condescricao,
                  constatus,
                  TO_CHAR(condata, 'DD/MM/YYYY') as condata,
                  connumerocontrato,
                  usucpf,
                  arqid,
                  convlrcontrato,
                  concnpj,
                  convalidado
                FROM {$this->stNomeTabela}
                WHERE {$this->arChavePrimaria[0]} = {$conid}";
        return $this->pegaLinha($sql);
    }



}
