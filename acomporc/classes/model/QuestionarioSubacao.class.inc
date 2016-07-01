<?php

class Acomporc_Model_QuestionarioSubacao extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionariosubacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qsbid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qsbid' => null,
        'prfid' => null,
        'sbacod' => null,
        'qstid' => null
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

    public function novo($arValores, $arCampos = array('prfid','sbacod','qstid')){
        $campos = implode( ', ',$arCampos);
        $valores;
        foreach($arValores as $val){
            $valores .= "(".implode(', ',$val)."),";
        }
        $valores = substr($valores,0,-1);
        $sql = <<<DML
            INSERT INTO {$this->stNomeTabela} ({$campos}) VALUES {$valores}
DML;
        if($this->executar($sql)){
            return $this->commit();
        }else{
            return false;
        }
    }

    public function capturaSubacoesAssociadas()
    {
        $query = <<<DML
            SELECT
                sbacod as codigo
            FROM {$this->stNomeTabela}
            WHERE qstid = {$this->arAtributos['qstid']}
                AND prfid = {$this->arAtributos['prfid']}
DML;
        return $this->carregar($query);
    }

    public function excluir($id = NULL, $retorno = NULL)
    {
        $query = <<<DML
            DELETE FROM {$this->stNomeTabela} WHERE qstid = '{$this->arAtributos['qstid']}' AND prfid = '{$this->arAtributos['prfid']}'
DML;
        $this->executar($query);
        return $this->commit();
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array/boolean
     */
    public function buscaQuestionarioComPerguntas($asaid)
    {
        $columns = array('q.qstid',
            'q.qstnome',
            'qp.qprid',
            'qp.qprpergunta',
            'qp.qprnumcaracteres',
            'qp.qprobrigatorio',
            'qp.qprrespostapadrao',
            'qp.qprordem');
        if($asaid){
            $join = "LEFT JOIN acomporc.questionariosubacaorespostas aqsr ON (qp.qprid = aqsr.qprid AND aqsr.asaid = {$asaid})";
            $columns[] = 'aqsr.qsrid';
            $columns[] = 'aqsr.qsrresposta AS resposta';
        }
        $colunas = implode(',',$columns);
        $query = <<<DML
            SELECT
                {$colunas}
            FROM acomporc.questionario q
            JOIN acomporc.questionarioperguntas qp ON (q.qstid = qp.qstid)
            JOIN acomporc.questionariosubacao qs ON (q.qstid = qs.qstid AND qs.sbacod = '{$this->arAtributos['sbacod']}' AND q.prfid = qs.prfid)
            {$join}
            WHERE q.prfid = {$this->arAtributos['prfid']}
                AND q.qsttipo = 'S'
                AND q.qststatus = 'A'
            ORDER BY qp.qprordem;
DML;
        $lista = $this->carregar($query);
        if(!$lista){
            return array();
        }
        return $this->_formataLista($lista);
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $lista
     * @return array Retorna o array formatado para inserção no Simec_Listagem
     */
    protected function _formataLista($lista = array())
    {
        $retorno = array();
        $questoes = array();
        foreach($lista as $dado){

            $questoes[] = array(
                'qprid' => $dado['qprid'],
                'qprpergunta' => $dado['qprpergunta'],
                'qprnumcaracteres' => $dado['qprnumcaracteres'],
                'qprobrigatorio' => $dado['qprobrigatorio'],
                'qprrespostapadrao' => $dado['qprrespostapadrao'],
                'qprordem' => $dado['qprordem'],
                'qsrid' => $dado['qsrid'],
                'resposta' => $dado['resposta'],
            );
        }
        $retorno[] = array(
            'qstid' => $lista[0]['qstid'],
            'qstnome' => $lista[0]['qstnome'],
            'questoes' => $questoes
        );
        return $retorno;
    }
}