<?php

class Acomporc_Model_QuestionarioSubacaoRespostas extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionariosubacaorespostas";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qsrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qsrid' => null,
        'asaid' => null,
        'prfid' => null,
        'qprid' => null,
        'sbacod' => null,
        'qsrresposta' => null
    );

    /**
     * Popular qprid, asaid antes de chamar função.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array
     */
    public function buscaResposta()
    {
        $query = <<<DML
            SELECT
                qsrid,
                qsrresposta
            FROM acomporc.questionariosubacaorespostas
            WHERE qprid = {$this->arAtributos['qprid']}
                AND asaid = {$this->arAtributos['asaid']}
DML;
        return $this->pegaLinha($query);
    }

    public function salvarQuestao($camposResposta)
    {
        if(!$camposResposta || !is_array($camposResposta)){
            return false;
        }
        foreach($camposResposta as $resposta){
            $this->popularDadosObjeto($resposta);
            if($resposta['qsrid'] == null || trim($resposta['qsrid']) == ''){
                $this->inserir();
            }else{
                $this->alterar();
            }
        }

        return $this->commit();
    }
}