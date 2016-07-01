<?php

/**
 * Description of Questao
 * Modelo responsável por gerenciar requisições da controller de perguntas.
 * @author LindalbertoFilho
 */
class Acomporc_Service_Questao extends Spo_Service_Abstract
{

    protected $_dbQuestionarioPerguntas;
    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param type $arDados
     * @return array()
     */
    function salvarQuestao($arDados)
    {
        $this->_dbQuestionarioPerguntas = new Acomporc_Model_QuestionarioPerguntas();
        $this->_dbQuestionarioPerguntas->popularDadosObjeto($arDados);
        if($arDados['qprnumcaracteres'] == ''){
            $arDados['qprnumcaracteres'] = 2000;
        }
        if($arDados['qprordem'] == ''){
           $arDados['qprordem'] = $this->_dbQuestionarioPerguntas->pegaUltimoOrdem();
        }
        $this->_dbQuestionarioPerguntas->popularDadosObjeto($arDados);
        $resultado['msg'] = 'Falha ao cadastrar Questão.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        if ($this->_dbQuestionarioPerguntas->inserir()){
            $this->_dbQuestionarioPerguntas->commit();
            $resultado['msg'] = 'Questão cadastrada com sucesso!';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param type $id
     * @return array()
     */
    function buscarQuestao($id)
    {
        $this->_dbQuestionarioPerguntas = new Acomporc_Model_QuestionarioPerguntas();
        $this->_dbQuestionarioPerguntas->__set('qprid',$id);
        return $this->_dbQuestionarioPerguntas->buscaPorId();
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param type $arDados
     * @return array()
     */
    function atualizarQuestao($arDados)
    {
        $this->_dbQuestionarioPerguntas = new Acomporc_Model_QuestionarioPerguntas();
        $this->_dbQuestionarioPerguntas->popularDadosObjeto($arDados);
        if($arDados['qprnumcaracteres'] == ''){
            $arDados['qprnumcaracteres'] = 2000;
        }
        if($arDados['qprordem'] == ''){
           $arDados['qprordem'] = $this->_dbQuestionarioPerguntas->pegaUltimoOrdem();
        }
        $this->_dbQuestionarioPerguntas->popularDadosObjeto($arDados);
        $resultado['msg'] = 'Falha ao atualizar Questão.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        if ($this->_dbQuestionarioPerguntas->alterar()){
            $this->_dbQuestionarioPerguntas->commit();
            $resultado['msg'] = 'Questão atualizada com sucesso!';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }
}
