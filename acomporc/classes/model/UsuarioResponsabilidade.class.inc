<?php

class Acomporc_Model_UsuarioResponsabilidade extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acomporc.usuarioresponsabilidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('rpuid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rpuid' => null,
        'pflcod' => null,
    	'prfid' => null,
        'usucpf' => null,
    	'acaid' => null,
    	'acacod' => null,
    	'unicod' => null,
        'rpustatus' => null,
        'rpudata_inc' => null,
        'rldid' => null,
        'sbacod' => null
    );

    public function atribuirResponsavel()
    {
        $retorno = false;
        if ($this->arAtributos['rldid'] != null) {
            $retorno = $this->relatorioTCU();
        } else if($this->arAtributos['sbacod'] != null) {
            $retorno = $this->acompanhamentoSubacao();
        } else if($this->arAtributos['acacod'] != null && $this->arAtributos['unicod'] != null) {
            $retorno = $this->coordenadorValidadorAcao();
        } else if ($this->arAtributos['unicod'] != null && $this->arAtributos['acacod'] == null) {
            $retorno = $this->monitorInterno();
        }

        return $retorno;
    }

    public function acompanhamentoSubacao()
    {
        // -- Limpando responsabilidades anteriores
        $query = <<<DML
            UPDATE {$this->stNomeTabela}
            SET rpustatus = 'I'
            WHERE sbacod = '{$this->arAtributos['sbacod']}'
DML;
        if(!$this->executar($query)){
            return false;
        }

        if (!empty($this->arAtributos['usucpf'])) {
            // -- Atribuíndo a nova responsabilidade
            $query = <<<DML
                INSERT INTO {$this->stNomeTabela}(pflcod, usucpf, sbacod, prfid)
                VALUES(%d, '%s', '%s', %d)
DML;
            $stmt = sprintf($query, PFL_COORDENADORSUBACAO, $this->arAtributos['usucpf'], $this->arAtributos['sbacod'], $this->arAtributos['prfid']);

            if (!$this->executar($stmt)) {
                return false;
            }
        }
        return $this->commit();
    }

    public function relatorioTCU()
    {
        // -- Limpando responsabilidades anteriores
        $query = <<<DML
            UPDATE {$this->stNomeTabela}
            SET rpustatus = 'I'
            WHERE rldid = {$this->arAtributos['rldid']}
DML;
        if(!$this->executar($query)){
            return false;
        }

        if (!empty($this->arAtributos['usucpf'])) {
            // -- Atribuíndo a nova responsabilidade
            $query = <<<DML
                INSERT INTO {$this->stNomeTabela}(pflcod, usucpf, rldid, prfid)
                VALUES(%d, '%s', %d, %d)
DML;
            $stmt = sprintf($query, PFL_RELATORIO_TCU, $this->arAtributos['usucpf'], $this->arAtributos['rldid'], $this->arAtributos['prfid']);

            if (!$this->executar($stmt)) {
                return false;
            }
        }
        return $this->commit();
    }

    public function monitorInterno()
    {
        // -- Limpando responsabilidades anteriores
        $query = <<<DML
            UPDATE {$this->stNomeTabela}
            SET rpustatus = 'I'
            WHERE unicod = '{$this->arAtributos['unicod']}'
            AND prfid = '{$this->arAtributos['prfid']}' AND pflcod  = '%s'
            AND acacod IS NULL
DML;
        if(!$this->executar(sprintf($query, PFL_MONITOR_INTERNO))){
            return false;
        }

        if (!empty($this->arAtributos['usucpf'])) {
            // -- Atribuíndo a nova responsabilidade
            $query = <<<DML
                INSERT INTO {$this->stNomeTabela}(pflcod, usucpf, unicod, prfid)
                VALUES(%d, '%s', '%s', %d)
DML;
            $stmt = sprintf($query, PFL_MONITOR_INTERNO, $this->arAtributos['usucpf'], $this->arAtributos['unicod'], $this->arAtributos['prfid']);

            if (!$this->executar($stmt)) {
                return false;
            }
        }
        return $this->commit();
    }

    public function coordenadorValidadorAcao()
    {
        // -- Limpando responsabilidades anteriores
        $query = <<<DML
            UPDATE {$this->stNomeTabela}
            SET rpustatus = 'I'
            WHERE acacod = '{$this->arAtributos['acacod']}' AND prfid = '{$this->arAtributos['prfid']}' AND pflcod = '{$this->arAtributos['pflcod']}' AND unicod = '{$this->arAtributos['unicod']}'
DML;
        $this->executar($query);
        $this->commit();

        if (!empty($this->arAtributos['usucpf'])) {
            // -- Atribuíndo a nova responsabilidade
            $query = <<<DML
                INSERT INTO {$this->stNomeTabela}(pflcod, usucpf, prfid, unicod, acacod)
                VALUES(%d, '%s', '%s', '%s', '%s')
DML;
            $stmt = sprintf($query, $this->arAtributos['pflcod'], $this->arAtributos['usucpf'], $this->arAtributos['prfid'], $this->arAtributos['unicod'], $this->arAtributos['acacod']);
            //ver($stmt, d);

            $this->executar($stmt);
            $this->commit();
        }

        return true;
    }

    /**
     * Popular: acaid, prfid;
     * Verifica a existencia de um cpf para determinada ação e período.
     */
    public function verificaIncidenciaAcao()
    {
        $query = <<<DML
            SELECT 1 FROM {$this->stNomeTabela} WHERE acacod = {$this->arAtributos['acacod']}
                AND prfid = {$this->arAtributos['prfid']}
                AND rpustatus = 'A'
                AND usucpf = '{$_SESSION['usucpf']}'
DML;
        return $this->pegaUm($query);
    }
}