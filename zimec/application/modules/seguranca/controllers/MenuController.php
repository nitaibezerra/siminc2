<?php

include_once APPLICATION_PATH . '/../library/Simec/legacy/Listagem.php';

class Seguranca_MenuController extends Simec_Controller_Action
{
    public function indexAction()
    {
    }

    public function formularioAction()
    {
        $sisid = 23;

        $model = new Model_Seguranca_Menu();
        $this->view->row = $model->getRow($this->_getParam('mnuid'));

        // Carregando tipos
        $this->view->tipos = array('1'=>'1', '2'=>'2', '3'=>'3', '4'=>'4');

        // Carregando Menu Pai
        $this->view->menusPai = $model->getMenuPai($sisid, 2);

        // Carregando Abas
        $modelAba = new Model_Seguranca_Aba();
        $this->view->abas = $modelAba->getPreparedArray();

        // Carregando Perfis
        $modelPerfil = new Model_Seguranca_Perfil();
        $this->view->perfis = $modelPerfil->getPreparedArray(null, array("pflstatus='A'", 'sisid = ?'=>$sisid));

        // Carregando Resouces
        $modelPerfil = new Model_Seguranca_Resource();
        $this->view->resources = $modelPerfil->getPreparedArray('rsccontroller', array('sisid = ?'=>$sisid));
        $this->view->actions = $modelPerfil->getPreparedArray(array('rscdsc'=>'rscdsc'), array('sisid = ?'=>$sisid));

        $this->view->camposComErro = Simec_Util::getSession('form_validation_error');
        Simec_Util::clear('form_validation_error');
    }

    public function gravarAction()
    {
        $dados = $this->_getAllParams();
        $model = new Model_Seguranca_Menu();

        try {
            Zend_Db_Table::getDefaultAdapter()->beginTransaction();

            $id = $model->gravar($dados);

            Zend_Db_Table::getDefaultAdapter()->commit();

            // -- Redirecionando
            $this->_redirect('seguranca/menu/formulario/mnuid/' . $id, 'Operação realizada com sucesso.', 'success');

        } catch (Simec_Db_Exception $e) {
            Zend_Db_Table::getDefaultAdapter()->rollBack();
            $this->_redirect('seguranca/menu/formulario/mnuid/' . $id, $e->getDetalhe(), 'error');
        }
    }

    public function excluirAction()
    {
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