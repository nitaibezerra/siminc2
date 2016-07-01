<?php

class Acomporc_Model_Questionario extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qstid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qstid' => null,
        'qsttipo' => null,//A - Ação, S - Subação, T - Tcu
        'qstnome' => null,
        'prfid' => null,
        'dataultimaalteracao' => null,
        'qststatus' => null,
    );

    public function buscaPorId(){
        $query = <<<DML
            SELECT
                qstid,
                qstnome,
                prfid
            FROM {$this->stNomeTabela}
            WHERE qstid = {$this->arAtributos['qstid']}
DML;
        return $this->pegaLinha($query);
    }

    public function findAll($returnQuery = false)
    {
        $query = <<<DML
            SELECT
                q.qstid,
                q.qstnome,
                p.prftitulo
            FROM {$this->stNomeTabela} q
            JOIN acomporc.periodoreferencia p ON q.prfid = p.prfid
            WHERE q.qsttipo = '{$this->arAtributos['qsttipo']}' AND p.prsano = '{$_SESSION['exercicio']}' AND qststatus = 'A'
            ORDER BY q.qstnome ASC
DML;
        if($returnQuery){
            return $query;
        }
        return $this->carregar($query);
    }

    public function buscaNomeQuestionario(){
        $query = <<<DML
            SELECT
                qstnome
            FROM {$this->stNomeTabela}
            WHERE qstid = {$this->arAtributos['qstid']}
DML;
        return $this->pegaUm($query);
    }

    public function capturaRelatorio($rldid)
    {
        $query = <<<DML
            SELECT
                q.qstid,
                pr.prftitulo,
                q.qstnome AS questionario,
                qp.qprid,
                qp.qprpergunta AS pergunta,
                qrr.qrrid,
                qrr.qrrresposta AS resposta
            FROM {$this->stNomeTabela} q
            INNER JOIN acomporc.periodoreferencia pr ON (q.prfid = pr.prfid)
            INNER JOIN acomporc.questionarioperguntas qp ON (q.qstid = qp.qstid)
            LEFT JOIN acomporc.questionariorelatoriotcurespostas qrr ON (qp.qprid = qrr.qprid AND rldid = {$rldid})
            WHERE
                q.prfid = '{$this->arAtributos['prfid']}'
                AND q.qsttipo = '{$this->arAtributos['qsttipo']}'
DML;
        $questionario = $this->carregar($query);
        if(!$questionario){
            return false;
        }
        return $questionario;
    }

    public function remover()
    {
        $query = <<<DML
            UPDATE {$this->stNomeTabela} SET qststatus = 'I' WHERE qstid = {$this->arAtributos['qstid']};
DML;
        if($this->executar($query)){
            return $this->commit();
        }
    }

}