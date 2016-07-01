<?php

/**
 * Controle responsavel pelos documentos.
 *
 * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
 * @since  23/10/2014
 *
 * @name       Documentoarquivo
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Documentoarquivo extends Abstract_Controller {

    protected $_model;

    public function __construct() {
        parent::__construct();
        $this->_model = new Model_Demandaarquivo();
    }

    public function indexAction() {
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction() {
        $modelTipoDocumento = new Model_Tipodocumento();
        $modelProcedencia = new Model_Procedencia();

        $this->view->tipoDocumento = $modelTipoDocumento->getAllByValues(array('tpdstatus' => 'A'));
        $this->view->procedencias = $modelProcedencia->getAllByValues(array('prcstatus' => 'A'));

        $id = $this->getPost('id');
        if ($this->getPost('editar')) {
            $this->_model->populateEntity(array('dmaid' => $id));
        } else {
            $this->_model->populateEntity(array('dmdid' => $id));
        }

        $this->view->entity = $this->_model->entity;

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction() {
        $dmdid = $this->getPost('id');

        $listing = new Listing();

        $listing->enableCount(true);
        $listing->setActions(array('edit' => 'editarArquivo', 'delete' => 'excluirArquivo', 'download-alt' => 'downloadArquivo'));
        $listing->setHead(array('Descrição'));
        $data = "SELECT dmaid , dmadsc
                    FROM demandasse.demandaarquivo dma
                    WHERE dmastatus = 'A'
                    AND dmdid = {$dmdid}";

        $this->view->exibirTitulo = true;
        $this->view->data = $data;
        $this->view->listing = $listing;

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction() {
        if (!$_POST['dmaid']) {
            $_POST['usucpfinclusao'] = "{$_SESSION['usucpf']}";
            $_POST['dmadtinclusao'] = 'now()';
            $_POST['dmastatus'] = 'A';
        } else {
            $_POST['dmddtalteracao'] = 'now()';
            $_POST['usucpfalteracao'] = "{$_SESSION['usucpf']}";
        }

        $this->_model->populateEntity($_POST);

        if (empty($this->_model->entity['arqid']['value'])) {
            $this->_model->setArqId($this->getPost('dmadsc'));
        }

        $id = $this->_model->save();

        if ($this->_model->error) {
            $return = array('status' => false, 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $this->_model->error);
        } else {
            $return = array('status' => true, 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function deletarAction() {
        $id = $this->getPost('id');
        $this->_model->entity['dmadsc'] = array('is_null' => 'YES');
        $this->_model->entity['usucpfinclusao'] = array('is_null' => 'YES');
        $this->_model->entity['dmadtinclusao'] = array('is_null' => 'YES');
        $this->_model->entity['arqid'] = array('is_null' => 'YES');

        $dataForm = array();
        $dataForm['dmaid'] = $id;
        $dataForm['dmastatus'] = 'I';
        $dataForm['usucpfinativacao'] = $_SESSION['usucpf'];
        $dataForm['dmadtinativacao'] = 'now()';

        $this->_model->populateEntity($dataForm);

        $result = $this->_model->update();
        if ($result) {
            $return = array('status' => true, 'msg' => ('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false, 'msg' => ('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }

//    public function downloadAction() {
//        $this->view->fatorAvaliado = new Model_FatoravaliadoExecucao();
//        $this->view->fatorAvaliado->getArquivo((int) $_GET['arqid']);
//    }
}
