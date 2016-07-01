<?php

class Aspar_ProposicaoController extends Simec_Controller_Action
{
    public function indexAction() {
        try{
            $model = new Model_Aspar_Proposicao();
            $modelTipoProposicao = new Model_Aspar_TipoProposicao();
            $modelPrioridade = new Model_Aspar_Prioridade();
            
            $filtros = $this->getRequest()->getParam('filtro');
            
            $this->view->rowSet = $model->lista($filtros);
            $this->view->rowSetFilters = $filtros;
            $this->view->tipoProposicao = $modelTipoProposicao->getPreparedArray(array('tprid'=>array('tprsigla', 'tprdsc')));
            $this->view->tipoPrioridade = $modelPrioridade->getPreparedArray();
        }catch (Exception $e){
    		$this->_message(MSG_ERROR, $e->getMessage());
    	}
    }
    
    public function formularioAction()
    {
        $model = new Model_Aspar_Proposicao();
        $modelTipoImpacto = new Model_Aspar_TipoImpacto();
        $modelTipoProposicao = new Model_Aspar_TipoProposicao();
        $modelPrioridade = new Model_Aspar_Prioridade();

        $this->view->tipoPrioridade = $modelPrioridade->getPreparedArray();
        $this->view->tipoImpacto = $modelTipoImpacto->getPreparedArray();
        $this->view->tipoProposicao = $modelTipoProposicao->getPreparedArray(array('tprid'=>array('tprsigla', 'tprdsc')));
        $this->view->row = $model->getRow(Simec_Util::decode($this->_getParam('prpid')));
    }

    public function gravarAction()
    {
        $dados = $this->getRequest()->getPost();
        $model = new Model_Aspar_Proposicao();
        try {
            $model->salvar($dados);
            $mensagem = 'Operação realizada com sucesso.';
            $this->_transport(MSG_SUCCESS, $mensagem, 'aspar/proposicao');
        } catch (Exception $e) {
            if ($dados['prpid']) {
	            $this->_transport(MSG_ERROR, $e->getMessage(), 'aspar/proposicao/formulario/prpid/' . Simec_Util::encode($dados['prpid']));
            } else {
            	$this->_transport(MSG_ERROR, $e->getMessage(), 'aspar/proposicao/formulario');
            }
        }
    }
    
    public function excluirAction()
    {
        $prpid = Simec_Util::decode($this->_getParam('prpid'));
        $model = new Model_Aspar_Proposicao();
        try {
        	$model->remover($prpid);
            $mensagem = 'Registro removido com sucesso.';
            $this->_transport(MSG_SUCCESS, $mensagem, 'aspar/proposicao');
        } catch (Exception $e) {
            $this->_transport(MSG_ERROR, $e->getMessage(), 'aspar/proposicao');
            #$this->_message(MSG_ERROR, $e->getMessage());
        }
    }
}