<?php

class NotasFiscais extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.notasfiscais";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ntfid");

    /**
     * Atributos
     * @var array
     * @access protected
     */

    protected $arAtributos = array(
        'ntfid' => null,
        'ntfdescricao' => null,
        'ntfstatus' => null,
        'ntfdata' => null,
        'ntfnumeronotafiscal' => null,
        'usucpf' => null,
        'arqid' => null,
        'ntfvlrnota' => null,
        'ntfvalidado' => null
    );

    public function alterarValidacaoNotasFiscais($ntfid, $ntfvalidado) {
        $sql = "UPDATE {$this->stNomeTabela} SET ntfvalidado = '{$ntfvalidado}' WHERE ntfid = {$ntfid}";
        return $this->executar($sql);
    }

    public function findById($nftid){
        $sql = "SELECT
                  ntfid,
                  ntfdescricao,
                  ntfstatus,
                  TO_CHAR(ntfdata, 'DD/MM/YYYY') as ntfdata,
                  ntfnumeronotafiscal,
                  usucpf,
                  arqid,
                  ntfvlrnota,
                  ntfvalidado
                FROM {$this->stNomeTabela}
                WHERE {$this->arChavePrimaria[0]} = {$nftid}";
        return $this->pegaLinha($sql);
    }

}
