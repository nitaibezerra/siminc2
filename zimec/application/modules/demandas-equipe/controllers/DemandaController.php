<?php

include_once APPLICATION_PATH . '/../library/Simec/legacy/Listagem.php';

import('models.demandasequipe.Demanda');

class DemandasEquipe_DemandaController extends Simec_Controller_Action
{
    public function indexAction()
    {
        $model = new Model_DemandasEquipe_Demanda();
        ver(123, d);
    }

    public function formularioAction()
    {
        $model = new Model_DemandasEquipe_Demanda();

        $this->view->row = $model->getRow($this->_getParam('dmdid'));
    }

    public function gravarAction()
    {
        $dados = $this->_getAllParams();

        $dados['dmdprazo'] = Simec_Util::formatarData($dados['dmdprazo']);
        $dados['usucpfinclusao'] = $_SESSION['usucpforigem'];
        $dados['dmddtinclusao'] = date('Y-m-d H:i:s');

        $model = new Model_DemandasEquipe_Demanda();

        try {
            $model->beginTransaction();

            $id = $model->gravar($dados);

            $model->commit();
            
            $mensagem = 'Operação realizada com sucesso.';

            $this->_transport(MSG_SUCCESS, $mensagem, 'demandas-equipe/demanda/formulario/dmdid/' . $id);
        } catch (Exception $e) {
            $model->rollBack();

            $this->_transport(MSG_ERROR, $e->getMessage(), 'demandas-equipe/demanda/formulario');
        }
    }

    public function excluirAction()
    {
        ver(123, d);
        $dmtid = $this->_getParam('dmtid');
        $model = new Model_Par_Demandatipo();

        try {
            Zend_Db_Table::getDefaultAdapter()->beginTransaction();

            $dmtid = $model->excluir(array('dmtid = ?' => $dmtid));

            Zend_Db_Table::getDefaultAdapter()->commit();

            $this->_redirect('par/demanda-tipo/');

        } catch (Ev_Exception $e) {
            Zend_Db_Table::getDefaultAdapter()->rollBack();
        }
    }
}