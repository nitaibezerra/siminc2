<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of QuestionarioRelatorioTCURespostas
 *
 * @author LindalbertoFilho
 */
class Acomporc_Model_QuestionarioRelatorioTCURespostas extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.questionariorelatoriotcurespostas";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qrrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qrrid' => null,
        'rldid' => null,
        'prfid' => null,
        'qprid' => null,
        'qrrresposta' => null
    );

    /**
     * Popular qprid, rldid antes de chamar função.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array
     */
    public function buscaResposta()
    {
        $query = <<<DML
            SELECT
                qrrid,
                qsrresposta
            FROM {$this->stNomeTabela}
            WHERE qprid = {$this->arAtributos['qprid']}
                AND rldid = {$this->arAtributos['rldid']}
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
            if($resposta['qrrid'] == null || trim($resposta['qrrid']) == ''){
                $this->inserir();
            }else{
                $this->alterar();
            }
        }

        return $this->commit();
    }

    public function apagar($rlgid)
    {
        $query = <<<DML
            DELETE FROM {$this->stNomeTabela} WHERE rldid IN (
                SELECT rldid FROM acomporc.relgdados WHERE rlgid = {$rlgid}
            )
DML;
        $this->executar($query);
    }

    
}
