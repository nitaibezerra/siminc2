<?php

class SubacaoDetalheLiberacaoPagamento extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaodetalhe_liberacaopagamento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sdpid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sdpid' => null,
        'sbdid' => null,
        'sdpvalortotal' => null,
        'sdpvalorliberado' => null,
        'sdpdata' => null,
        'usucpf' => null,
        'sdpstatus' => null
    );

    public function salvarValorLiberacao($arrParam) {
        $sql = "INSERT INTO {$this->stNomeTabela}(
                    sbdid, sdpvalortotal, sdpvalorliberado, sdpdata, usucpf, 
                    sdpstatus)
                VALUES ({$arrParam['sbdid']}, {$arrParam['sdpvalortotal']}, {$arrParam['sdpvalorliberado']}, '{$arrParam['sdpdata']}', '{$arrParam['usucpf']}', '{$arrParam['sdpstatus']}')";
        return $this->executar($sql);
    }
    
    public function alterarStatusValorLiberacaoSbdid($sbdid, $sdpstatus) {
        $sql = "UPDATE {$this->stNomeTabela} SET sdpstatus = '{$sdpstatus}' WHERE sbdid = {$sbdid}";
        return $this->executar($sql);
    }
    
    public function findBy($arrAtributos, $arrWhere, $arrParam) {
        $strAtributos = "";
        $sql = "SELECT ";
        foreach($arrAtributos as $atributo){
            $strAtributos .= "{$atributo},";
        }
        $sql .= substr($strAtributos, 0, -1);
        $sql .= " FROM {$this->stNomeTabela} ";
        $sql .= "WHERE 1=1";
        foreach($arrWhere as $where){
            $sql .= " AND {$where}";
        }
        foreach($arrParam as $param){
            $sql .= " {$param}";
        }
        return $this->carregar($sql);
    }
}
