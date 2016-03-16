<?php

class Controller_Etapa extends Abstract_Controller
{
    public function __construct()
    {
        parent::__construct();
        $this->view->perfilUsuario = new Model_PerfilUsuario();
        $this->view->titulo = 'Planos Táticos Operacionais';
        $this->view->acaoSolucao = new Model_Acaosolucao(false);
        $this->view->etapa = new Model_Etapa(false);
        $this->view->solucao = new Model_Solucao(false);
        $this->view->atividade = new Model_Atividade(false);
    }

    public function cadastrarAction()
    {
        $solid = (int)$this->getPost('solid');
        $etpid = (int)$this->getPost('id');

        if (!empty($solid) and $solid != 0) {
            $_SESSION['solid'] = $solid;
            $this->view->solucao->populateEntity(array('solid' => $solid));
            $this->view->solucao->treatEntityToUser();
            $this->view->tituloSolucao = $this->view->solucao->getTituloSolucao();
        }
        if (!empty($etpid) or $_SESSION['etpid']) {
            $_SESSION['etpid'] = (empty($etpid) ? $_SESSION['etpid'] : $etpid);
            $this->view->etapa->populateEntity(array('etpid' => $etpid));
            $this->view->etapa->treatEntityToUser();
            $this->view->acaoSolucao->setAttributeValue('acaid', $this->view->etapa->getAttributeValue('acaid'));
        }
        $this->view->data = $this->view->etapa->getDados($solid);
        $this->view->listingEtapa = $this->view->etapa->getListing();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction()
    {
        try {
            $this->view->etapa->salvarEtapa();
            $this->view->etapa->commit();
            $return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
        } catch (Exception $e) {
            $this->view->etapa->rollback();
            $error = $this->view->etapa->error;
            $return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $error);
        }
        echo simec_json_encode($return);
    }

    public function excluirAction()
    {
        $etpid = (int)$this->getPost('id');
        $solid = (int)$this->getPost('solid');

        if( $this->view->atividade->possuiAtividade($etpid) ){
            $return = array('status' => false, 'msg' => ( self::REGISTRO_POSSUI_VINCULO ));
        }else{
            try {
                $this->view->etapa = new Model_Etapa(false);
                $this->view->etapa->inativar($etpid);
                $this->view->etapa->commit();
                $return = array('status' => false, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'result' => '', 'type' => 'success', 'solid' => $solid);
            } catch (Exception $e) {
                $this->view->etapa->rollback();
                $return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $e->getMessage());
            }
        }
        echo simec_json_encode($return);
    }

    public function listarAction($solid = null)
    {
        $solid = (int)($solid ? $solid : $this->getPost('id'));
        $this->view->data = $this->view->etapa->getDados($solid);
        $this->view->listingEtapa = $this->view->etapa->getListing();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function limparAction()
    {
        $_SESSION['etpid'] = null;
    }

    public function editarAction()
    {
        $etpid = (int)$this->getPost('id');
        if (!empty($etpid)) {
            $_SESSION['etpid'] = $etpid;
            $this->view->etapa->populateEntity(array('etpid' => $etpid));
            $this->view->etapa->treatEntityToUser();
            $this->view->acaoSolucao->setAttributeValue('acaid', $this->view->etapa->getAttributeValue('acaid'));
        }
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function ordenarAction()
    {
        $novaOrdem = $this->getPost('novaOrdem');
        $solid = ($_SESSION['solid'] ? $_SESSION['solid'] : $this->getPost('solid'));
        $novaOrdem = array_filter($novaOrdem);

        $cont = 0;
        foreach ($novaOrdem as $idEtapa) {
            $idEtapaArray = explode('_', $idEtapa);
            if ($idEtapaArray[2] != 'atividade') {
                $cont++;
                $etpid = end($idEtapaArray);
                try {
                    $this->view->etapa = new Model_Etapa(false);
                    $this->view->etapa->alterarOrdem($etpid, $cont, $solid);
                    $this->view->etapa->commit();
                } catch (Exception $e) {
                    $this->view->etapa->rollback();
                }
            }
        }
        $this->listarAction($solid);
    }
}
