<?php

class Acomporc_Model_QuestionarioAcao extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionarioacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qsaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qsaid' => null,
        'prfid' => null,
        'unicod' => null,
        'acacod' => null,
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

    public function novo($arValores, $arCampos = array('prfid','unicod','acacod','qstid')){

        $campos = implode( ', ',$arCampos);
        $valores;
        foreach($arValores as $val){
            if($val['unicod'] == '' || $val['acacod'] == ''){
                continue;
            }
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

    public function excluir($id = NULL, $retorno = NULL)
    {
        $query = <<<DML
            DELETE FROM {$this->stNomeTabela} WHERE qstid = '{$this->arAtributos['qstid']}' AND prfid = '{$this->arAtributos['prfid']}'
DML;
        $this->executar($query);
        return $this->commit();
    }

    public function capturaAcoesAssociadas()
    {
        $query = <<<DML
            SELECT DISTINCT
                unicod ||'.'||acacod AS codigo
            FROM {$this->stNomeTabela}
            WHERE qstid = {$this->arAtributos['qstid']}
                AND prfid = {$this->arAtributos['prfid']}
DML;
        return $this->carregar($query);
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array/boolean
     */
    public function buscaQuestionarioComPerguntas($aclid,$loccod)
    {
        $leftJoin = "";
        $columns = array(
            'q.qstid',
            'q.qstnome',
            'qp.qprid',
            'qp.qprpergunta',
            'qp.qprnumcaracteres',
            'qp.qprobrigatorio',
            'qp.qprrespostapadrao',
            'qp.qprordem'
            );
        if(!is_null($aclid)){
            $leftJoin = "LEFT JOIN acomporc.questionariolocalizadorrespostas aqlr ON (qp.qprid = aqlr.qprid AND aqlr.aclid = {$aclid} AND aqlr.loccod = '{$loccod}' AND aqlr.unicod = qa.unicod AND aqlr.acacod = qa.acacod)";
            $columns[] = 'aqlr.qlrid';
            $columns[] = 'aqlr.qlrresposta AS resposta';
        }
        $columns = implode(',',$columns);
        $query = <<<DML
            SELECT
                {$columns}
            FROM acomporc.questionario q
            JOIN acomporc.questionarioperguntas qp ON (q.qstid = qp.qstid)
            JOIN {$this->stNomeTabela} qa ON (q.qstid = qa.qstid AND qa.unicod = '{$this->arAtributos['unicod']}' AND qa.acacod = '{$this->arAtributos['acacod']}' AND qa.prfid = q.prfid)
            {$leftJoin}
            WHERE q.prfid = {$this->arAtributos['prfid']}
                AND q.qsttipo = 'A'
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
                'qlrid' => $dado['qlrid'],
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