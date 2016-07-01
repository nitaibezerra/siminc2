<?php

class Controller_ExecucaoAtividade extends Abstract_Controller
{
    public function __construct()
    {
        parent::__construct();
        $this->view->titulo = 'Planos Táticos Operacionais';
        $this->view->atividade = new Model_Atividade(false);
        $this->view->usuario = new Model_Usuario(false);
    }

    public function indexAction()
    {
        $_SESSION['solid'] = null;
        $_SESSION['etpid'] = null;
        $_SESSION['atvid'] = null;
        $_SESSION['acaids'] = null;

        $this->view->dataExecutor = $this->view->atividade->getAtividadesExecucao();
        $this->view->listingAtividade = $this->view->atividade->getListingExecucao();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction()
    {
        $this->view->dataExecutor = $this->view->atividade->getAtividadesExecucao();
        $this->view->listingAtividade = $this->view->atividade->getListingExecucao();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function selecionaratividadeAction()
    {
        $atvid = (int)$this->getPost('id');
        if (!empty($atvid)) {
            $_SESSION['atvid'] = $atvid;
            $this->view->atividade->populateEntity(array('atvid' => $atvid));
            $this->view->atividade->treatEntityToUser();

            $cpf = $this->view->atividade->getAttributeValue('usucpf');
            if (!empty($cpf)) {
                $result = $this->view->usuario->getUsuarioByCpf($cpf);
                if (is_array($result)){
                    $user = $result[0];
                }
                $this->view->executor = $this->view->usuario->mask($user['usucpf'], '###.###.###-##') . ' - ' . $user['usunome'];
            }
        }
        $this->render(__CLASS__, __FUNCTION__);
    }
}
