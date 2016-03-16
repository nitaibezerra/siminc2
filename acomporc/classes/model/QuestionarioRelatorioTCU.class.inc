<?php

class Acomporc_Model_QuestionarioRelatorioTCU extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionariorelatoriotcu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qrtid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qrtid' => null,
        'prfid' => null,
        'acacod' => null,
        'qstid' => null
    );


    public function novo($arValores, $arCampos = array('prfid','acacod','qstid')){
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

    public function capturaTCUAssociadas()
    {
        $query = <<<DML
            SELECT
                acacod as codigo
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
    public function capturaQuestionarioTCU($rldid)
    {
        $query = <<<DML
            SELECT
                q.qstid,
                q.qstnome as questionario,
                qp.qprid,
                qp.qprpergunta as pergunta,
                qp.qprnumcaracteres as caracteres,
                qp.qprobrigatorio as obrigatorio,
                qp.qprrespostapadrao as respostapadrao,
                qp.qprordem as ordem,
                aqrr.qrrid,
                aqrr.qrrresposta AS resposta
            FROM acomporc.questionario q
            JOIN acomporc.questionarioperguntas qp ON (q.qstid = qp.qstid)
            JOIN acomporc.questionariorelatoriotcu aqr ON (q.qstid = aqr.qstid AND aqr.acacod = '{$this->arAtributos['acacod']}' AND q.prfid = aqr.prfid)
            LEFT JOIN acomporc.questionariorelatoriotcurespostas aqrr ON (qp.qprid = aqrr.qprid AND aqrr.rldid = {$rldid})
            WHERE q.prfid = {$this->arAtributos['prfid']}
                AND q.qsttipo = 'T'
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
                'pergunta' => $dado['pergunta'],
                'caracteres' => $dado['caracteres'],
                'obrigatorio' => $dado['obrigatorio'],
                'respostapadrao' => $dado['respostapadrao'],
                'resposta' => $dado['resposta'],
                'qrrid' => $dado['qrrid'],
                'ordem' => $dado['ordem'],
            );
        }
        $retorno[] = array(
            'qstid' => $lista[0]['qstid'],
            'questionario' => $lista[0]['questionario'],
            'questoes' => $questoes
        );
        return $retorno;
    }


}