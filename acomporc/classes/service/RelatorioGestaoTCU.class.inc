<?php

/**
 * Description of RelatorioGestaoTCU
 * @see Acomporc_Service_RelatorioGestaoTCU
 * @author LindalbertoFilho
 */
class Acomporc_Service_RelatorioGestaoTCU extends Spo_Service_Abstract
{
    /**
     * @see Acomporc_Model_RelGestao
     */
    protected $_dbRelGestao;
    /*
     * @see Acomporc_Model_RelgDados
     */
    protected $_dbRelgDados;
    /*
     * @see Acomporc_Service_Questionario
     */
    protected $_serviceQuestionario;
    /*
     * @see Acomporc_Model_UsuarioResponsabilidade
     */
    protected $_usuarioResponsabilidade;

    public function __construct()
    {
    }

    public function cadastrarRelatorioTCU($dados)
    {
        $validacao = true;
        $resultado['msg'] = 'Falha ao cadastrar Relatório TCU.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        if (empty($dados['unicod']) || (empty($dados['acacod']) && empty($dados['acaofloa'])) || empty($dados['prfid'])) {
            $resultado['msg'] = 'Para criar um novo relatório, você primeiro deve selecionar um Período, uma Unidade Orçamentária e uma Ação.';
            $validacao = false;
        }
        if($validacao){
            $resultado = $this->cadastro($dados);
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
        return $resultado;

    }

    public function deletarRelatorioGestao($rlgid)
    {
        $resultado['msg'] = 'Falha ao deletar Relatório.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        $this->_serviceQuestionario = new Acomporc_Service_Questionario();
        $this->_serviceQuestionario->__set('tipoAcesso', 'T');
        $this->_serviceQuestionario->apagarRespostasQuestionario($rlgid);

        $this->_dbRelGestao = new Acomporc_Model_RelGestao();
        $this->_dbRelgDados = new Acomporc_Model_RelgDados();

        $this->_dbRelgDados->__set('rlgid',$rlgid);
        $this->_dbRelgDados->apagar();

        $this->_dbRelGestao->__set('rlgid',$rlgid);
        $this->_dbRelGestao->excluir();

        if($this->_dbRelGestao->commit()){
            $resultado['msg'] = 'Relatório excluido com sucesso!';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }

    public function cadastro($data)
    {
        $this->_dbRelGestao = new Acomporc_Model_RelGestao();
        $this->_dbRelGestao->__set('unicod', $data['unicod']);
        $this->_dbRelGestao->__set('prfid', $data['prfid']);
        $this->_dbRelGestao->__set('acacod', ($data['acatipo'] == 'N' ? $data['acacod'] : $data['acaofloa']));

        if (!($rlgid = $this->_dbRelGestao->verificaExistencia())) {
            $this->_dbRelGestao->__set('usucpf', $_SESSION['usucpf']);
            $this->_dbRelGestao->__set('acatipo', $data['acatipo']);

            $rlgid = $this->_dbRelGestao->novo();

            $mensagem = 'Registro <u>criado</u> com sucesso.';
            $tipo = Simec_Helper_FlashMessage::SUCESSO;
        } else {
            $mensagem = 'Registro <u>aberto</u> com sucesso.';
            $tipo = Simec_Helper_FlashMessage::INFO;
        }

        return array('sucesso' => true, 'msg' => $mensagem, 'data' => array('rlgid' => $rlgid), 'type' => $tipo);
    }

    public function buscaRelatorio($rlgid)
    {
        $this->_dbRelGestao = new Acomporc_Model_RelGestao();
        if($rlgid == ''){
            return false;
        }
        $this->_dbRelGestao->__set('rlgid',$rlgid);
        return $this->_dbRelGestao->buscaRelatorio();
    }

    /**
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param string $aba
     * @param int $rlgid
     * @param char $acatipo
     * @param int $rldid
     * @return array/boolean
     */
    public function buscaRelatorioAba($aba,$rlgid,$acatipo,$rldid)
    {
        $this->_dbRelgDados = new Acomporc_Model_RelgDados();
        switch($aba)
        {
            case 'detalhamentopo':
            case 'justificativapo':
                $this->_dbRelgDados->__set('rldid', $rldid);
                $this->_dbRelgDados->__set('rlgid', $rlgid);
                return $this->_dbRelgDados->buscaRelatorioPOJustificaRelatorio();
            case 'po':
                return false;
            default:
                $this->_dbRelgDados->__set('rlgid', $rlgid);
                if ('N' == $acatipo) {
                    return $this->_dbRelgDados->buscaRelatorioNormal();
                }else{
                    return $this->_dbRelgDados->buscaRelatorioRAP();
                }
        }
    }

    public function salvarAcao($data)
    {
        $data['rldcod'] = strtoupper($data['rldcod']);
        $data['rldacaoprioritaria'] = $data['rldacaoprioritaria'] == '' ? 'f' : $data['rldacaoprioritaria'];
        $data = $this->_formataCamposNumeric($data);
        if (empty($data['rldid'])) {
            $resultado = $this->_novaAcao($data);
        } else {
            $resultado = $this->_atualizaAcao($data);
        }
        $resultado['data'] = array('rlgid' => $_REQUEST['rlgid']);
        // -- Redirecionamento contextual
        if ('detalhamentopo' == $_REQUEST['aba']) {
            $resultado['data'] = array('rlgid' => $_REQUEST['rlgid'], 'aba' => 'po');
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
        return $resultado;
    }

    protected function _formataCamposNumeric($data){
        $data['rlddotacaoinicial'] = $this->_formataNumeric($data['rlddotacaoinicial']);
        $data['rlddotacaofinal'] = $this->_formataNumeric($data['rlddotacaofinal']);
        $data['rlddespempenhada'] = $this->_formataNumeric($data['rlddespempenhada']);
        $data['rlddespliquidada'] = $this->_formataNumeric($data['rlddespliquidada']);
        $data['rlddesppaga'] = $this->_formataNumeric($data['rlddesppaga']);
        $data['rldrapinscprocessado'] = $this->_formataNumeric($data['rldrapinscprocessado']);
        $data['rldrapinscnaoprocessado'] = $this->_formataNumeric($data['rldrapinscnaoprocessado']);
        $data['rldrapeaem0101'] = $this->_formataNumeric($data['rldrapeaem0101']);
        $data['rldrapeavalorliquidado'] = $this->_formataNumeric($data['rldrapeavalorliquidado']);
        $data['rldrapeavalorcancelado'] = $this->_formataNumeric($data['rldrapeavalorcancelado']);

        $data['rldrapearealizado'] = $this->_formataNumeric($data['rldrapearealizado']);
        $data['rldmontanteprevisto'] = $this->_formataNumeric($data['rldmontanteprevisto']);
        $data['rldmontantereprogramado'] = $this->_formataNumeric($data['rldmontantereprogramado']);
        $data['rldmontanterealizado'] = $this->_formataNumeric($data['rldmontanterealizado']);

        return $data;
    }

    protected function _formataNumeric($value){
        $value = str_replace(',','.',str_replace('.', '', $value));
        return trim($value) == '' ? 0 : $value;
    }

    protected function _novaAcao($data)
    {
        $this->_dbRelgDados = new Acomporc_Model_RelgDados();
        $this->_dbRelgDados->popularDadosObjeto($data);
        // -- insert
        $rldid = $this->_dbRelgDados->inserir();
        // -- Executando o insert
        $resultado['msg'] = 'Não foi possível executar sua solicitação.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        if ($rldid) {
            $resultado['msg'] = 'Sua solicitação foi executada com sucesso.';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
            incluirDocid('rldid', 'relgdados', $rldid, TPDID_RELATORIO_TCU, 'Relatório TCU', 'acomporc');
        }
        return $resultado;
    }

    protected function _atualizaAcao($data)
    {
        $this->_dbRelgDados = new Acomporc_Model_RelgDados();
        $resultado['msg'] = 'Não foi possível executar sua solicitação.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
        $this->_dbRelgDados->popularDadosObjeto($data);
        if($this->_dbRelgDados->alterar()){
            $resultado['msg'] = 'Sua solicitação foi executada com sucesso.';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
        }
        return $resultado;
    }

    /**
    * Consulta os dados de um relatório completo, ou de um item do relatório e retorna um array com estes dados.
    *
    * @global cls_banco $db DBAL do banco de dados.
    * @param int $rlgid Id do relatório.
    * @param int $rldid Id do item do relatório.
    * @return array|bool
    * @throws Exception Lançado quando não são encontrados dados.
    */
    public function consultarDadosTCU($rlgid, $rldid = null)
    {
        $this->_dbRelGestao = new Acomporc_Model_RelGestao();
        $this->_dbRelGestao->__set('rlgid',$rlgid);
        $dados = $this->_dbRelGestao->consultarDadosTCU($rldid);

        return $dados;
    }

    public function atribuirResponsavel($rldid, $usucpf, $rlgid, $prfid)
    {
        $resultado['msg'] = 'Não foi possível executar a associação do responsável.';
        $resultado['data'] = array('rlgid' => $rlgid);
        $resultado['type'] = Simec_Helper_FlashMessage::FALSE;
        $this->_usuarioResponsabilidade = new Acomporc_Model_UsuarioResponsabilidade();
        $this->_usuarioResponsabilidade->__set('rldid',$rldid);
        $this->_usuarioResponsabilidade->__set('usucpf',$usucpf);
        $this->_usuarioResponsabilidade->__set('prfid',$prfid);

        if($this->_usuarioResponsabilidade->atribuirResponsavel()){
            $resultado['msg'] = 'Associação do responsável executada com sucesso.';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
        return $resultado;
    }
}
