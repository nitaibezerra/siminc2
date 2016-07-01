<?php

class Controller_Colegiado extends Abstract_Controller
{
    public function __construct()
    {
        parent::__construct();
        $this->view->colegiado = new Model_Colegiado();
        $this->view->identificacaoGrupo = new Model_Identificacaogrupo();
    }

    public function editarAction()
    {
        $this->view->idGrupo = (int)$_POST['idGrupo'];
        $this->view->identificacaoGrupo->getIdentificacaoGrupoPorIdGrupo($this->view->idGrupo);

        if ($this->getPost('id')) {
            $colegiado = $this->view->colegiado->getByValues(array('colid' => (int)$this->getPost('id')));
            $this->view->colegiado->populateEntity($colegiado);
            $this->view->colegiado->treatEntityToUser();
        }
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function excluirAction()
    {
        $colegiado = new Model_Colegiado();
//        if ($this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) === false) {
//            $return = array('status' => false, 'msg' => utf8_encode(self::ERRO_SEM_PERMISAO), 'type' => 'danger');
//        } else {
        try {
            $colegiado->excluir((int)$this->getPost('id'));
            $return = array('status' => true, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'type' => 'success');
        } catch (Exception $exc) {
            if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
                echo $exc->getTraceAsString();
            }
            $return = array('status' => false, 'msg' => (self::ERRO_AO_EXCLUIR), 'type' => 'danger');
        }
//        }
        $return['msg'] = '<div class="alert alert-' . $return['type'] . '" role="alert">' . $return['msg'] . '</div>';
        echo simec_json_encode($return);
    }

    public function salvarAction()
    {
        $this->view->colegiado->populateEntity($_POST);
        $this->view->colegiado->setAttributeValue('colstatus', 'A');

//        if ($this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) === false) {
//            $return = array('status' => false, 'msg' => utf8_encode(self::ERRO_SEM_PERMISAO), 'type' => 'success');
//        } else {
        if ($this->view->colegiado->save()) {
            $return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
        } else {
            $return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $this->view->colegiado->error);
        }
//        }
        echo simec_json_encode($return);
    }

    public function listarAction()
    {
        $this->view->idGrupo = (int)$_POST['id'];
        $this->render(__CLASS__, __FUNCTION__);
    }


}
