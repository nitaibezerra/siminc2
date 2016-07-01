<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class Proporc_Model_PrelimitesPessoal extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "proporc.prelimites_pessoal";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("prpid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prpid' => null,
        'unicod' => null,
        'usucpf' => null,
        'usucpfresponsavel' => null,
        'dataultimaatualizacao' => null,
        'docid' => null
    );

    public function listar()
    {
        $query = <<<DML

DML;
    }

    public function pegaId()
    {
        $query = <<<DML
            SELECT prpid FROM {$this->stNomeTabela} WHERE unicod = '{$this->arAtributos['unicod']}'
DML;
        return $this->pegaUm($query);
    }

    public function verificaDocid()
    {
        $query = <<<DML
            SELECT docid FROM {$this->stNomeTabela} WHERE prpid = {$this->arAtributos['prpid']}
DML;
        return $this->pegaUm($query);

    }

    public function carregarPorId($id){
        $query = <<<DML
            SELECT
                pp.prpid,
                pp.unicod,
                pp.usucpfresponsavel,
                usu.usucpf ||' - '|| SPLIT_PART(usu.usunome, ' ', 1)||' '|| CASE WHEN LENGTH(SPLIT_PART(usu.usunome, ' ', 2)) < 3 THEN SPLIT_PART(usu.usunome, ' ', 2)||' '|| SPLIT_PART(usu.usunome, ' ', 3) ELSE SPLIT_PART(usu.usunome, ' ', 2) END AS usunome,
                usu.usuemail,
                usu.usufoneddd,
                usu.usufonenum,
                to_char(dataultimaatualizacao,'DD/MM/YYYY') as dataultimaatualizacao
            FROM proporc.prelimites_pessoal pp
            LEFT JOIN seguranca.usuario usu ON (pp.usucpfresponsavel = usu.usucpf)
            WHERE pp.prpid = '$id';
DML;
        return $this->pegaLinha($query);
    }

    public function carregaArqid($prpid,$tipo,$exercicio){
        $query = <<<DML
            SELECT arqid FROM proporc.anexogeral WHERE prpid = $prpid AND tipo = '$tipo' AND angano = '$exercicio'
DML;
          return $this->pegaUm($query);
    }

    public function deletaArqid($arqid){
        $query = <<<DML
            DELETE FROM proporc.anexogeral WHERE arqid = $arqid
DML;
        if($this->executar($query)){
            return $this->commit();
        }
    }

    public function alterarResponsavel()
    {
        $query = <<<DML
            UPDATE $this->stNomeTabela SET usucpfresponsavel = '{$this->arAtributos['usucpfresponsavel']}' WHERE {$this->arChavePrimaria[0]} = {$this->arAtributos['prpid']}
DML;
        if($this->executar($query)){
            return $this->commit();
        }
    }


}