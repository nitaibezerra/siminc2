<?php

class Acomporc_Service_Questionario extends Spo_Service_Abstract
{

    /**
     * @see Acomporc_Model_Questionario
     */
    protected $_dbQuestionario;
    /**
     * @see QuestionarioPerguntas
     */
    protected $_dbQuestionarioPerguntas;
    /**
     * @see QuestionarioAcao
     */
    protected $_dbQuestionarioAcao;
    /**
     * @see QuestionarioSubacao
     */
    protected $_dbQuestionarioSubacao;
    /**
     * @see QuestionarioRelatorioTCU
     */
    protected $_dbQuestionarioRelatorioTCU;
    /**
     * @see QuestionarioSubacaoRespostas
     */
    protected $_dbQuestionarioSubacaoRespostas;
    /**
     * @see QuestionarioRelatorioTCURespostas
     */
    protected $_dbQuestionarioRelatorioTCURespostas;
    /**
     * @see Acomporc_Model_QuestionarioLocalizadorRespostas
     */
    protected $_dbAcompanhamentoQuestionarioLocalizadorRespostas;
    /**
     * Values: A, S, T;
     * @var char
     */
    protected $dados = array(
        'tipoAcesso' => null
    );

    public function __construct()
    {
    }

    public function listaJustificativas($data)
    {
        switch($this->dados['tipoAcesso'])
        {
            case 'A':
                $this->_dbQuestionarioAcao = new Acomporc_Model_QuestionarioAcao();
                $this->_dbQuestionarioAcao->__set('prfid', $data['prfid']);
                $this->_dbQuestionarioAcao->__set('unicod', $data['unicod']);
                $this->_dbQuestionarioAcao->__set('acacod', $data['acacod']);
                return $this->_dbQuestionarioAcao->buscaQuestionarioComPerguntas($data['aclid'],$data['loccod']);
            case 'S':
                $this->_dbQuestionarioSubacao = new Acomporc_Model_QuestionarioSubacao();
                $this->_dbQuestionarioSubacao->__set('prfid', $data['prfid']);
                $this->_dbQuestionarioSubacao->__set('sbacod', $data['sbacod']);
                return $this->_dbQuestionarioSubacao->buscaQuestionarioComPerguntas($data['asaid']);
            case 'T':
                $this->_dbQuestionarioRelatorioTCU = new Acomporc_Model_QuestionarioRelatorioTCU();
                $this->_dbQuestionarioRelatorioTCU->__set('prfid', $data['prfid']);
                $this->_dbQuestionarioRelatorioTCU->__set('acacod', $data['acacod']);
                return $this->_dbQuestionarioRelatorioTCU->capturaQuestionarioTCU($data['rldid']);
        }
    }

    public function removerQuestionario($id)
    {
        $this->_dbQuestionario = new Acomporc_Model_Questionario();
        $this->_dbQuestionario->__set('qstid',$id);
        return $this->_dbQuestionario->remover();
    }

    public function findAll($returnQuery = false)
    {
        $this->_dbQuestionario = new Acomporc_Model_Questionario();
        $this->_dbQuestionario->__set('qsttipo', $this->dados['tipoAcesso']);
        return $this->_dbQuestionario->findAll($returnQuery);
    }

    function salvarQuestionario($data)
    {
        $this->_dbQuestionario = new Acomporc_Model_Questionario();
        $resultado['msg'] = 'Falha ao cadastrar Questionário.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        $this->_dbQuestionario->populardadosObjeto($data);
        if($this->_dbQuestionario->__get('qstid') != ''){
            if($this->_dbQuestionario->alterar()){
                $this->_dbQuestionario->commit();
                $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
                $resultado['msg'] = 'Questionário atualizado com sucesso!';
            }
        }else{
            if($this->_dbQuestionario->inserir()){
                $this->_dbQuestionario->commit();
                $resultado['msg'] = 'Questionário cadastrado com sucesso!';
                $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
            }
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }

    public function findById($id)
    {
        $this->_dbQuestionario = new Acomporc_Model_Questionario();
        $this->_dbQuestionario->__set('qstid',$id);
        return $this->_dbQuestionario->buscaPorId();
    }

    /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $id
    * @return string
    */
    function getName($id)
    {
        $this->_dbQuestionario = new Acomporc_Model_Questionario();
        $this->_dbQuestionario->__set('qstid',$id);
        return $this->_dbQuestionario->buscaNomeQuestionario();
    }

    function salvarResposta($data)
    {
        if(!$data || !is_array($data)){
            return false;
        }
        switch($this->dados['tipoAcesso'])
        {
            case 'A':
                $this->_dbAcompanhamentoQuestionarioLocalizadorRespostas = new Acomporc_Model_QuestionarioLocalizadorRespostas();
                return $this->_dbAcompanhamentoQuestionarioLocalizadorRespostas->salvarQuestao($data);
            case 'S':
                $this->_dbQuestionarioSubacaoRespostas = new Acomporc_Model_QuestionarioSubacaoRespostas();
                return $this->_dbQuestionarioSubacaoRespostas->salvarQuestao($data);
            case 'T':
                $this->_dbQuestionarioRelatorioTCURespostas = new Acomporc_Model_QuestionarioRelatorioTCURespostas();
                $resultado['msg'] = 'Falha ao salvar Justificativa.';
                $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
                $resultado['data'] = array('rlgid' => $data['rlgid'],'aba' => $data['aba']);
                if($this->_dbQuestionarioRelatorioTCURespostas->salvarQuestao($this->_organizaCamposRespostaTCU($data))){
                    $resultado['msg'] = 'Justificativa salva com sucesso!';
                    $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
                }
                $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
                return $resultado;
        }
    }

    /**
    * Organiza o array de respostas para envio do acompanhamento.
    * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param array $dados
    * @return array
    */
    protected function _organizaCamposRespostaTCU($dados)
    {
        if(!$dados['qprid']){
            return false;
        }
        $chavesQuestao = array_keys($dados['qprid']);
        foreach($chavesQuestao as $chave){
            $camposResposta[] = array(
                'qrrid' => $dados['qpridHidden'][$chave],
                'rldid' => $dados['rldid'],
                'prfid' => $dados['prfid'],
                'qprid' => $chave,
                'qrrresposta' => $dados['qprid'][$chave]);
        }
       return $camposResposta;
    }

    /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $dados
    * @return array()
    */
   function associarPeriodoAcoes($dados)
   {
       $data = array();
       $resultado['msg'] = 'Falha ao inserir Associação.';
       $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
       if($dados['acacod']){
           $this->_dbQuestionarioAcao = new Acomporc_Model_QuestionarioAcao();
           foreach($dados['acacod'] as $acao){
               $res = explode('.',$acao);
               $data[] = array('prfid' => $dados['prfid'],'unicod' => "'".$res[0]."'", 'acacod' => "'".$res[1]."'", 'qstid' => $dados['qstid']);
           }
           //Limpa os dados antes de inserir.
           $this->_dbQuestionarioAcao->__set('prfid', $dados['prfid']);
           $this->_dbQuestionarioAcao->__set('qstid', $dados['qstid']);
           $this->_dbQuestionarioAcao->excluir();
           if($this->_dbQuestionarioAcao->novo($data)){
               $resultado['msg'] = 'Associação inclusa com sucesso!';
               $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
           }
       }
       $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
   }

   /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $dados
    * @return array()
    */
   function associarPeriodoSubacoes($dados)
   {
       $data = array();
       $resultado['msg'] = 'Falha ao inserir Associação.';
       $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
       if($dados['sbacod']){
           $this->_dbQuestionarioSubacao = new Acomporc_Model_QuestionarioSubacao();
           foreach($dados['sbacod'] as $sbacod){
               $data[] = array('prfid' => $dados['prfid'],'sbacod' => "'".$sbacod."'", 'qstid' => $dados['qstid']);
           }
           //Limpa os dados antes de inserir.
           $this->_dbQuestionarioSubacao->__set('prfid', $dados['prfid']);
           $this->_dbQuestionarioSubacao->__set('qstid', $dados['qstid']);
           $this->_dbQuestionarioSubacao->excluir();
           if($this->_dbQuestionarioSubacao->novo($data)){
               $resultado['msg'] = 'Associação inclusa com sucesso!';
               $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
           }
       }
       $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
   }

   /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $dados
    * @return array()
    */
   function associarPeriodoTCU($dados)
   {
       $data = array();
       $resultado['msg'] = 'Falha ao inserir Associação.';
       $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
       if($dados['acacod']){
           $this->_dbQuestionarioRelatorioTCU = new Acomporc_Model_QuestionarioRelatorioTCU();
           foreach($dados['acacod'] as $acacod){
               $data[] = array('prfid' => $dados['prfid'], 'acacod' => "'".$acacod."'", 'qstid' => $dados['qstid']);
           }
           //Limpa os dados antes de inserir.
           $this->_dbQuestionarioRelatorioTCU->__set('prfid', $dados['prfid']);
           $this->_dbQuestionarioRelatorioTCU->__set('qstid', $dados['qstid']);
           $this->_dbQuestionarioRelatorioTCU->excluir();
           if($this->_dbQuestionarioRelatorioTCU->novo($data)){
               $resultado['msg'] = 'Associação inclusa com sucesso!';
               $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
           }
       }
       $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
   }

    function apagarRespostasQuestionario($rlgid)
    {
        switch($this->dados['tipoAcesso'])
        {
            case 'A':
                break;
            case 'S':
                break;
            case 'T':
                $this->_dbQuestionarioRelatorioTCURespostas = new Acomporc_Model_QuestionarioRelatorioTCURespostas();
                $this->_dbQuestionarioRelatorioTCURespostas->apagar($rlgid);
                break;
        }
    }

    function relatorioQuestionario($rldid,$prfid,$qsttipo)
    {
        $this->_dbQuestionario = new Acomporc_Model_Questionario();
        $this->_dbQuestionario->__set('prfid',$prfid);
        $this->_dbQuestionario->__set('qsttipo',$qsttipo);
        return $this->_dbQuestionario->capturaRelatorio($rldid);
    }

    function capturaTCUPorAssociacao($prfid,$qstid)
    {
        $this->_dbQuestionarioRelatorioTCU = new Acomporc_Model_QuestionarioRelatorioTCU();
        $this->_dbQuestionarioRelatorioTCU->__set('prfid', $prfid);
        $this->_dbQuestionarioRelatorioTCU->__set('qstid', $qstid);
        $acoes = $this->_dbQuestionarioRelatorioTCU->capturaTCUAssociadas();

        if($acoes){
            $retorno = array();
            foreach($acoes as $dado){
                $retorno[] = $dado['codigo'];
            }
            return $retorno;
        }
        return null;
    }

    /**
    * Retorna array com os códigos de todas as associações entre Subações e Questionário.
    * @user Lindalberto <lindalbertorvcf@gmail.com>
    * @param int $prfid
    * @param int $qstid
    * @return array/null
    */
   function capturaSubacoesPorAssociacao($prfid,$qstid)
   {
       $this->_dbQuestionarioSubacao = new Acomporc_Model_QuestionarioSubacao();
       $this->_dbQuestionarioSubacao->__set('prfid',$prfid);
       $this->_dbQuestionarioSubacao->__set('qstid',$qstid);

       $subacoes = $this->_dbQuestionarioSubacao->capturaSubacoesAssociadas();
       if($subacoes){
           $retorno = array();
           foreach($subacoes as $dado){
               $retorno[] = $dado['codigo'];
           }
           return $retorno;
       }
       return null;
   }

    /**
    * Retorna array com os códigos de todas as associações entre Ações e Questionário.
    * @user Lindalberto <lindalbertorvcf@gmail.com>
    * @param int $prfid
    * @param int $qstid
    * @return array/null
    */
   function capturaAcoesPorAssociacao($prfid,$qstid)
   {
       $this->_dbQuestionarioAcao = new Acomporc_Model_QuestionarioAcao();
       $this->_dbQuestionarioAcao->__set('prfid', $prfid);
       $this->_dbQuestionarioAcao->__set('qstid', $qstid);
       $acoes = $this->_dbQuestionarioAcao->capturaAcoesAssociadas();
       if($acoes){
           $retorno = array();
           foreach($acoes as $dado){
               $retorno[] = $dado['codigo'];
           }
           return $retorno;
       }
       return null;
   }

    /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param int $id
    * @param string $tipo (A,S,T)
    * @param int $qstid
    * @return string
    */
   function apresentaComboPorPeriodo($id, $tipo, $qstid)
   {
       if($id && $tipo == 'A'){
           $retorno = array('acao' => inputCombo('acacod[]', sprintf(retornaQueryComboAcao(),$id), $this->capturaAcoesPorAssociacao($id, $qstid), 'acoes', array('multiple' => true,'return'=> true)));
       }else if($id && $tipo == 'S'){
           $retorno = array('subacao' => inputCombo('sbacod[]', sprintf(retornaQueryComboSubacao(),$id), $this->capturaSubacoesPorAssociacao($id,$qstid), 'subacoes', array('multiple' => true, 'return' => true)));
       }else if($id && $tipo == 'T'){
           $retorno = array('acao' => inputCombo('acacod[]', sprintf(retornaQueryComboRelatorioTCU(),$id), $this->capturaTCUPorAssociacao($id,$qstid), 'acoes', array('multiple' => true, 'return' => true)));
       }
       return simec_json_encode($retorno);
   }

}